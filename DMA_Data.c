#include "DMA_Data.h"
int FFT_cnt = 0;

// --- ĐỊA CHỈ DMA (Thay đổi theo thiết kế thực tế của bạn) ---
#define DMA_MM2S_SR             0x04    // TX Status Register
#define DMA_S2MM_SR             0x34    // RX Status Register

// 65536 * 16 * 4 = 4,194,304 bytes (4MB)
#define ADC_PACKET_SIZE     4194304 
#define FFT_PACKET_SIZE     1048576

int DMA_ADC_Read_Once(uint64_t phys_addr, uint32_t length) {
    if (phys_addr % 64 != 0) return -4;

    // 1. Reset DMA
    Xil_Out32(ADDR_DMA_ADC + 0x30, 0x4); 
    while (Xil_In32(ADDR_DMA_ADC + 0x30) & 0x4);

    // 2. Clear các bit Interrupt cũ (W1C - Write 1 to Clear)
    // Ghi 0xF000 để xóa sạch các bit lỗi và bit Done cũ
    Xil_Out32(ADDR_DMA_ADC + 0x34, 0x0000F000); 

    // 3. Khởi chạy DMA
    Xil_Out32(ADDR_DMA_ADC + 0x30, 0x1); // Run
    Xil_Out32(ADDR_DMA_ADC + 0x48, (uint32_t)phys_addr);
    Xil_Out32(ADDR_DMA_ADC + 0x4C, (uint32_t)(phys_addr >> 32));
    Xil_Out32(ADDR_DMA_ADC + 0x58, length); // Kích hoạt

    // 4. Polling chờ Xong (0x02) hoặc Đã nhận đủ gói (0x1000)
    int timeout = 10000;
    while (1) {
        uint32_t sr = Xil_In32(ADDR_DMA_ADC + 0x34);
        
        // SỬA Ở ĐÂY: Chấp nhận cả bit 1 (Idle) và bit 12 (IOC)
        if (sr & 0x1002) {
            // Xóa bit IOC ngay sau khi nhận xong để chuẩn bị cho gói sau
            Xil_Out32(ADDR_DMA_ADC + 0x34, 0x00001000); 
            break; 
        }

        if (sr & 0x70) {
            printf("[ADC] DMA Error! SR: 0x%08X\n", sr);
            return -2;
        }

        usleep(10);
        if (--timeout == 0) {
            // Nếu timeout, in ra số byte thực tế đã nhận được
            // Trong S2MM, đọc 0x58 sẽ cho biết số byte đã nhận
            uint32_t actual_bytes = Xil_In32(ADDR_DMA_ADC + 0x58);
            printf("[ADC] Timeout! SR: 0x%X, Recv: %u/%u\n", sr, actual_bytes, length);
            return -3;
        }
    }
    return 0;
}

void* ADC_Worker_Thread(void* arg) {
    static int adc_count = 0;
    // Lấy con trỏ ảo để kiểm tra dữ liệu
    uint32_t *data_ptr = (uint32_t*)BRAM_Get_Virt_Addr(IDX_ADC_BUF);

    printf("[ADC] Thread started for 4MB packets.\n");

    while(1) {
        if (trigger_adc) {
            // Đọc đúng 4,194,304 bytes
            if (DMA_ADC_Read_Once(PHYS_ADC_BUF, ADC_PACKET_SIZE) == 0) {
                uint32_t *data = (uint32_t*)BRAM_Get_Virt_Addr(IDX_ADC_BUF);
                // In 4 word đầu tiên để xem nó còn là 0xABCDE000 (pattern cũ) hay đã đổi
                printf("[ADC] SUCCESS! Data[0..3]: 0x%08X 0x%08X 0x%08X 0x%08X\n", 
                data[0], data[1], data[2], data[3]);
                adc_count++;
                
                // Mỗi 100 gói in kiểm tra 1 lần
                if (adc_count % 100 == 0) {
                    printf("[ADC] 100 Frames OK. Data[0]=0x%08X\n", data_ptr[0]);
                }

                pthread_mutex_lock(&radar_data_mutex);
                trigger_adc = 0; 
                pthread_mutex_unlock(&radar_data_mutex);
            } else {
                // Nếu lỗi, in ra số byte thực tế nhận được
                uint32_t bytes_rec = Xil_In32(ADDR_DMA_ADC + 0x58);
                printf("[ADC] Error! Bytes received before TLAST: %u\n", bytes_rec);
            }
        }
        usleep(50); // Tốc độ cao, chỉ nên nghỉ rất ngắn
    }
    return NULL;
}

/**
 * @brief Thread chính để giám sát và nhận dữ liệu FFT từ AXI DMA
 */
void* FFT_Monitor_SG_S2MM_Thread(void* arg) {
    long long fft_count = 0;

    // --- BƯỚC 1: LẤY CÁC CON TRỎ ĐỊA CHỈ ẢO TỪ HAL ---
    printf("[FFT] Initializing virtual pointers...\n");
    
    // 1. Thanh ghi điều khiển DMA (Map Non-Cached)
    volatile uint32_t* dma_fft_regs = (volatile uint32_t*)BRAM_Get_Virt_Addr(IDX_DMA_FFT);
    // 2. Descriptor trong RAM (Map Non-Cached)
    volatile DMA_Descriptor *desc_virt = (volatile DMA_Descriptor*)BRAM_Get_Virt_Addr(IDX_DMA_FFT_DESC);
    // 3. Buffer dữ liệu (Map Cached - để truy xuất nhanh)
    volatile uint8_t *buffer_virt_base = (volatile uint8_t*)BRAM_Get_Virt_Addr(IDX_FFT_BUF);

    // --- IN THÔNG TIN CHẨN ĐOÁN MAPPING ---
    printf("------------------------------------------------------------\n");
    printf("[FFT] Virtual Memory Map Diagnostics:\n");
    printf("  - DMA Registers (IDX_DMA_FFT): Phys=0x%08X, Virt=%p\n", 
            BRAM_Get_Phys_Addr(IDX_DMA_FFT), dma_fft_regs);
    printf("  - Descriptors   (IDX_DMA_FFT_DESC): Phys=0x%08X, Virt=%p\n", 
            BRAM_Get_Phys_Addr(IDX_DMA_FFT_DESC), desc_virt);
    printf("  - Data Buffer   (IDX_FFT_BUF): Phys=0x%08X, Virt=%p\n", 
            BRAM_Get_Phys_Addr(IDX_FFT_BUF), buffer_virt_base);
    printf("------------------------------------------------------------\n");

    if (!dma_fft_regs || !desc_virt || !buffer_virt_base) {
        printf("[FFT] FATAL ERROR: Invalid virtual pointers. Check mmap.\n");
        return NULL;
    }

    // --- BƯỚC 2: CẤU HÌNH DESCRIPTOR ---
    // Lấy địa chỉ vật lý để nạp cho DMA
    uint64_t desc0_phys = BRAM_Get_Phys_Addr(IDX_DMA_FFT_DESC);
    // Descriptor 1 nằm ngay sau Descriptor 0 (64 bytes = sizeof(DMA_Descriptor))
    uint64_t desc1_phys = desc0_phys + sizeof(DMA_Descriptor);

    // CẤU HÌNH KÍCH THƯỚC:
    // Gói tin thực tế: 16384 mẫu * 16 kênh * 32 bit = 1,048,576 bytes
    const uint32_t ACTUAL_PACKET_SIZE = 1048576; 
    // Buffer thử nghiệm: Lớn hơn gói tin để đảm bảo DMA dừng bằng TLAST
    const uint32_t EXPERIMENTAL_BUFFER_SIZE = ACTUAL_PACKET_SIZE + 256; 

    printf("[FFT] Config: Packet Size=%u, Buffer Size=%u\n", ACTUAL_PACKET_SIZE, EXPERIMENTAL_BUFFER_SIZE);

    // Xóa sạch vùng nhớ Descriptor
    memset((void*)desc_virt, 0, 2 * sizeof(DMA_Descriptor));

    // -- Descriptor 0 --
    desc_virt[0].nxtdesc         = (uint32_t)desc1_phys;
    desc_virt[0].nxtdesc_msb     = (uint32_t)(desc1_phys >> 32);
    desc_virt[0].buffer_addr     = (uint32_t)BRAM_Get_Phys_Addr(IDX_FFT_BUF); // Buffer 0
    desc_virt[0].buffer_addr_msb = (uint32_t)(BRAM_Get_Phys_Addr(IDX_FFT_BUF) >> 32);
    desc_virt[0].control         = EXPERIMENTAL_BUFFER_SIZE;
    desc_virt[0].status          = 0;

    // -- Descriptor 1 --
    uint64_t buffer1_phys = BRAM_Get_Phys_Addr(IDX_FFT_BUF) + EXPERIMENTAL_BUFFER_SIZE;
    desc_virt[1].nxtdesc         = (uint32_t)desc0_phys; // Quay vòng về 0
    desc_virt[1].nxtdesc_msb     = (uint32_t)(desc0_phys >> 32);
    desc_virt[1].buffer_addr     = (uint32_t)buffer1_phys; // Buffer 1
    desc_virt[1].buffer_addr_msb = (uint32_t)(buffer1_phys >> 32);
    desc_virt[1].control         = EXPERIMENTAL_BUFFER_SIZE;
    desc_virt[1].status          = 0;

    // =================================================================
    // PHẦN BỔ SUNG: IN THÔNG TIN CẤU HÌNH DESCRIPTOR TỪ VIRTUAL ADDRESS
    // =================================================================
    
    printf("\n[FFT DEBUG] Descriptor 0 (Virt: %p, Phys: 0x%llX) Cấu hình:\n", 
           (void*)&desc_virt[0], (long long)desc0_phys);
    printf("  - Buffer Phys: 0x%08X (MSB: 0x%X)\n", 
           desc_virt[0].buffer_addr, desc_virt[0].buffer_addr_msb);
    printf("  - Next Desc Phys: 0x%08X (MSB: 0x%X) -> Expected 0x%llX\n", 
           desc_virt[0].nxtdesc, desc_virt[0].nxtdesc_msb, (long long)desc1_phys);
    printf("  - Control (Size): %u\n", desc_virt[0].control);
    printf("  - Status: 0x%X\n", desc_virt[0].status);
    
    printf("\n[FFT DEBUG] Descriptor 1 (Virt: %p, Phys: 0x%llX) Cấu hình:\n", 
           (void*)&desc_virt[1], (long long)desc1_phys);
    printf("  - Buffer Phys: 0x%08X (MSB: 0x%X) -> Expected 0x%llX\n", 
           desc_virt[1].buffer_addr, desc_virt[1].buffer_addr_msb, (long long)buffer1_phys);
    printf("  - Next Desc Phys: 0x%08X (MSB: 0x%X) -> Expected 0x%llX (Desc 0)\n", 
           desc_virt[1].nxtdesc, desc_virt[1].nxtdesc_msb, (long long)desc0_phys);
    printf("  - Control (Size): %u\n", desc_virt[1].control);
    printf("  - Status: 0x%X\n", desc_virt[1].status);


    // // =================================================================
    // // ==> BƯỚC BẮT BUỘC: FLUSH CACHE CHO DESCRIPTOR <==
    // // Đảm bảo Descriptor được đẩy từ cache của CPU ra RAM cho DMA đọc
    // cache_flush_range((void*)desc_virt, 2 * sizeof(DMA_Descriptor));
    // // =================================================================    


    // --- BƯỚC 3: KHỞI ĐỘNG PHẦN CỨNG DMA (S2MM) ---
    // Định nghĩa offset thanh ghi (tính theo word 32-bit)
    const int S2MM_DMACR_OFF    = 0x30 / 4;
    const int S2MM_DMASR_OFF    = 0x34 / 4;
    const int S2MM_CURDESC_OFF  = 0x38 / 4;
    const int S2MM_TAILDESC_OFF = 0x40 / 4;
    // Thêm 4 dòng này vào BƯỚC 3
    const int S2MM_CURDESC_MSB_OFF = 0x3C / 4;
    const int S2MM_TAILDESC_MSB_OFF = 0x44 / 4;

    // 3.1 Reset
    printf("[FFT] Resetting DMA channel...\n");
    dma_fft_regs[S2MM_DMACR_OFF] = 0x4; 
    while (dma_fft_regs[S2MM_DMACR_OFF] & 0x4); 
    printf("[FFT] DMA Reset complete.\n");

    printf("[FFT] Setting initial descriptors...\n");
    // 3.3 Nạp Descriptor đầu tiên
    dma_fft_regs[S2MM_CURDESC_MSB_OFF] = 0; // Zero MSB
    dma_fft_regs[S2MM_CURDESC_OFF] = (uint32_t)desc0_phys;

    // 3.4 KÍCH HOẠT: Nạp Descriptor cuối cùng vào Tail
    dma_fft_regs[S2MM_TAILDESC_MSB_OFF] = 0; // Zero MSB
    dma_fft_regs[S2MM_TAILDESC_OFF] = (uint32_t)desc1_phys;    

    printf("[FFT] Starting DMA Engine...\n");
    // 3.2 Bật DMA (Run=1)
    // Lưu ý: Không bật ngắt (IOC_IrqEn, Err_IrqEn) vì ta dùng Polling
    dma_fft_regs[S2MM_DMACR_OFF] = 0x3001; 
    


    // =================================================================
    // PHẦN BỔ SUNG: ĐỌC LẠI THANH GHI DMA SAU KHI GHI LỆNH CẤU HÌNH
    // =================================================================
    uint32_t readback_curdesc = dma_fft_regs[S2MM_CURDESC_OFF];
    uint32_t readback_taildesc = dma_fft_regs[S2MM_TAILDESC_OFF];
    uint32_t readback_dmasr = dma_fft_regs[S2MM_DMASR_OFF];

    printf("\n[FFT DEBUG READBACK] Sau khi Ghi Lệnh Cấu hình:\n");
    printf("  - DMACR: 0x%X (Readback)\n", dma_fft_regs[S2MM_DMACR_OFF]);
    printf("  - DMASR: 0x%08X\n", readback_dmasr);
    printf("  - CURDESC: 0x%08X (Exp: 0x%08X)\n", readback_curdesc, (uint32_t)desc0_phys);
    printf("  - TAILDESC: 0x%08X (Exp: 0x%08X)\n", readback_taildesc, (uint32_t)desc1_phys);
    printf("  - Status Idle/Halted: Idle=%d, Halted=%d, ErrIrq=%d\n", 
           (readback_dmasr & 0x2) >> 1, (readback_dmasr & 0x1), (readback_dmasr & 0x2000) >> 13);
    
    // =================================================================    

    printf("[FFT] Ping-Pong SG Mode Started. Polling loop active...\n");

    int active_idx = 0; 
    
    uint32_t hw_status          = 0;//dma_fft_regs[S2MM_DMASR_OFF];
    uint32_t cur_desc_phys      = 0;//dma_fft_regs[S2MM_CURDESC_OFF];
    uint32_t tail_desc_phys     = 0;//dma_fft_regs[S2MM_TAILDESC_OFF];
    uint32_t last_hw_status     = 0x12345;//dma_fft_regs[S2MM_DMASR_OFF];
    uint32_t last_cur_desc_phys = 0x12345;//dma_fft_regs[S2MM_CURDESC_OFF];
    uint32_t last_tail_desc_phys= 0x12345;//dma_fft_regs[S2MM_TAILDESC_OFF];

    while(1) {
        // --- 1. KIỂM TRA TRẠNG THÁI TỪ RAM (Descriptor Status) ---
        // Bit 31 = Completed. Bit này do DMA ghi vào RAM khi xong gói tin.
        if (desc_virt[active_idx].status & 0x80000000) {
            
            // Lấy thông tin kết quả
            uint32_t final_status = desc_virt[active_idx].status;
            uint32_t bytes_received = final_status & 0x007FFFFF; 

            // LOG THÀNH CÔNG
            if (fft_count % 100 == 0) { // Log mỗi 100 gói để đỡ rối
                printf("[FFT] SUCCESS! Pkt %lld. Bytes: %u. Status: 0x%08X\n", 
                       fft_count, bytes_received, final_status);
            }

            // Xử lý Cache (Rất quan trọng)
            void* current_buffer_virt = (void*)(buffer_virt_base + (active_idx * EXPERIMENTAL_BUFFER_SIZE));
            cache_invalidate_range(current_buffer_virt, bytes_received);

            // ==> XỬ LÝ DỮ LIỆU TẠI ĐÂY <==
            // process_data(current_buffer_virt, bytes_received);

            // Dọn dẹp cho vòng lặp sau
            desc_virt[active_idx].status = 0;

            // Cập nhật TAILDESC để báo cho DMA biết descriptor này đã trống và có thể dùng lại
            // (Trong mô hình vòng tròn 2 desc, điều này đảm bảo DMA không bao giờ dừng)
            uint64_t next_tail_phys = (active_idx == 0) ? desc1_phys : desc0_phys;
            dma_fft_regs[S2MM_TAILDESC_OFF] = (uint32_t)next_tail_phys;

            // Đổi buffer
            active_idx = 1 - active_idx;
            fft_count++;

        } 
        else 
        {
            // --- 2. CHẨN ĐOÁN LỖI TỪ PHẦN CỨNG (Register Status) ---
            // Nếu chưa xong, hãy xem thanh ghi phần cứng đang báo gì
            hw_status = dma_fft_regs[S2MM_DMASR_OFF];
            cur_desc_phys = dma_fft_regs[S2MM_CURDESC_OFF];
            tail_desc_phys = dma_fft_regs[S2MM_TAILDESC_OFF];

            if ((hw_status != last_hw_status)||
                (cur_desc_phys != last_cur_desc_phys)||
                (tail_desc_phys != last_tail_desc_phys))
            {
                // Chỉ in khi trạng thái thay đổi để tránh spam log
                printf("[FFT DIAG] Hardware Status Changed: 0x%08X\n", hw_status);
                printf("[FFT DIAG] Pointers -> hw_status: 0x%08X CURDESC: 0x%08X | TAILDESC: 0x%08X\n", hw_status, cur_desc_phys, tail_desc_phys);

                // Phân giải các bit lỗi quan trọng
                if (hw_status & 0x1)    printf("   -> Halted (DMA Stopped)\n");
                if (hw_status & 0x10)   printf("   -> DMA Internal Error\n");
                if (hw_status & 0x20)   printf("   -> DMA Slave Error (Check Burst Size!)\n");
                if (hw_status & 0x40)   printf("   -> DMA Decode Error (Check Address Map)\n");
                if (hw_status & 0x100)  printf("   -> SG Internal Error\n");
                
                last_hw_status      = hw_status;
                last_cur_desc_phys  = cur_desc_phys;
                last_tail_desc_phys = tail_desc_phys;
            }

            // Ngủ ngắn để giảm tải CPU
            usleep(100); 
        }
    }
    return NULL;
}

int DMA_FFT_Read_Once(uint64_t phys_addr, uint32_t length) {
    // Offset của thanh ghi S2MM (Stream to Memory-Mapped)
    const uint32_t DMACR_OFF    = 0x30; // Control Register
    const uint32_t DMASR_OFF    = 0x34; // Status Register
    const uint32_t DEST_ADDR_OFF= 0x48; // Destination Address Register
    const uint32_t LENGTH_OFF   = 0x58; // Length Register

    // 1. Reset DMA
    Xil_Out32(ADDR_DMA_FFT + DMACR_OFF, 0x4); 
    while (Xil_In32(ADDR_DMA_FFT + DMACR_OFF) & 0x4);

    // 2. Clear các bit Interrupt cũ (W1C - Write 1 to Clear)
    Xil_Out32(ADDR_DMA_FFT + DMASR_OFF, 0x0000F000); 

    // 3. Khởi chạy DMA
    // Chế độ Direct Register: Cấu hình địa chỉ đích (Destination Address)
    Xil_Out32(ADDR_DMA_FFT + DEST_ADDR_OFF, (uint32_t)phys_addr);
    Xil_Out32(ADDR_DMA_FFT + DEST_ADDR_OFF + 0x4, (uint32_t)(phys_addr >> 32)); // MSB
    
    // Kích hoạt DMA (Run = 1)
    Xil_Out32(ADDR_DMA_FFT + DMACR_OFF, 0x1); 
    
    // Nạp độ dài để KÍCH HOẠT DMA
    Xil_Out32(ADDR_DMA_FFT + LENGTH_OFF, length); 

    // 4. Polling chờ Xong (0x1000 = IOC)
    int timeout = 10000; // 10000 * 10us = 100ms
    while (1) {
        uint32_t sr = Xil_In32(ADDR_DMA_FFT + DMASR_OFF);
        
        // Kiểm tra bit 12 (0x1000 = IOC) - Hoàn thành giao dịch
        if (sr & 0x1000) { 
            // Xóa bit IOC ngay sau khi nhận xong (W1C)
            Xil_Out32(ADDR_DMA_FFT + DMASR_OFF, 0x00001000); 
            break; 
        }

        // Kiểm tra các bit lỗi (DMA Int Err, DMA Slave Err, DMA Decode Err)
        if (sr & 0x70) { 
            printf("[FFT] DMA Direct Error! SR: 0x%08X\n", sr);
            return -2;
        }

        usleep(10);
        if (--timeout == 0) {
            uint32_t actual_bytes = Xil_In32(ADDR_DMA_FFT + LENGTH_OFF);
            printf("[FFT] Timeout! SR: 0x%X, Recv: %u/%u\n", sr, actual_bytes, length);
            return -3;
        }
    }
    return 0;
}

void* FFT_Worker_Thread(void* arg) {
    static int fft_count = 0;
    // Lấy con trỏ ảo để kiểm tra dữ liệu
    uint32_t *data_ptr = (uint32_t*)BRAM_Get_Virt_Addr(IDX_FFT_BUF);

    printf("[FFT] Worker Thread started for 1MB packets (Direct Mode).\n");

    while(1) {
        // Đợi tín hiệu trigger từ bên ngoài (ví dụ: một GPIO)
        if (trigger_FFT == 1) {
            
            // Lấy địa chỉ vật lý của Buffer
            uint64_t phys_buf_addr = BRAM_Get_Phys_Addr(IDX_FFT_BUF);
            
            if (DMA_FFT_Read_Once(phys_buf_addr, FFT_PACKET_SIZE) == 0) {
                
                // Xử lý Cache (Rất quan trọng vì Data Buffer được map Cached)
                void* current_buffer_virt = (void*)data_ptr;
                cache_invalidate_range(current_buffer_virt, FFT_PACKET_SIZE);

                // In debug để xem dữ liệu có hợp lệ hay không
                printf("[FFT] SUCCESS! Frame %d. Data[0..3]: 0x%08X 0x%08X 0x%08X 0x%08X\n", 
                       fft_count, data_ptr[0], data_ptr[1], data_ptr[2], data_ptr[3]);
                
                fft_count++;
                
                // Tắt trigger
                pthread_mutex_lock(&radar_data_mutex);
                trigger_FFT = 0; 
                pthread_mutex_unlock(&radar_data_mutex);
            } else {
                uint32_t bytes_rec = Xil_In32(ADDR_DMA_FFT + 0x58);
                printf("[FFT] Error! Bytes received before TLAST: %u\n", bytes_rec);
            }
        }
        usleep(10); // Nghỉ ngắn chờ trigger
    }
    return NULL;
}