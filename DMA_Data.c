#include "DMA_Data.h"
int FFT_cnt = 0;

static AXI_DMA_Descriptor *FFT_Desc_Ring;// = NULL;
static uint8_t *FFT_Data_Buf;// = NULL;

extern pthread_mutex_t radar_data_mutex; // Khai báo mutex toàn cục
// --- ĐỊA CHỈ DMA (Thay đổi theo thiết kế thực tế của bạn) ---
#define DMA_MM2S_SR 0x04 // TX Status Register
#define DMA_S2MM_SR 0x34 // RX Status Register

// 65536 * 16 * 4 = 4,194,304 bytes (4MB)
#define ADC_PACKET_SIZE 4194304
#define FFT_PACKET_SIZE 1048576

int DMA_ADC_Read_Once(uint64_t phys_addr, uint32_t length) {
  if (phys_addr % 64 != 0)
    return -4;

  // 1. Reset DMA
  Xil_Out32(ADDR_DMA_ADC + 0x30, 0x4);

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
      printf("[ADC] Timeout! SR: 0x%X, Recv: %u/%u\n", sr, actual_bytes,
             length);
      return -3;
    }
  }
  return 0;
}

void *ADC_Worker_Thread(void *arg) {
  static int adc_count = 0;
  // Lấy con trỏ ảo để kiểm tra dữ liệu
  uint32_t *data_ptr = (uint32_t *)BRAM_Get_Virt_Addr(IDX_ADC_BUF);

  printf("[ADC] Thread started for 4MB packets.\n");

  while (1) {
    if (trigger_adc) {
      // Đọc đúng 4,194,304 bytes
      if (DMA_ADC_Read_Once(PHYS_ADC_BUF, ADC_PACKET_SIZE) == 0) {
        uint32_t *data = (uint32_t *)BRAM_Get_Virt_Addr(IDX_ADC_BUF);
        // In 4 word đầu tiên để xem nó còn là 0xABCDE000 (pattern cũ) hay đã
        // đổi
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


// Cache Line trên dòng Zynq UltraScale+ (Cortex-A53) là 64 bytes
#define CACHE_LINE_SIZE 64

/**
 * @brief Đẩy dữ liệu từ Cache xuống RAM (Clean)
 * Dùng sau khi CPU ghi xong Descriptor để DMA thấy được dữ liệu mới.
 */
void hal_dcache_flush(void *addr, size_t len) {
    if (addr == NULL || len == 0) return;

    uintptr_t start = (uintptr_t)addr;
    uintptr_t end = start + len;
    
    // Căn chỉnh địa chỉ về đầu dòng Cache
    start &= ~(CACHE_LINE_SIZE - 1);
    
    for (; start < end; start += CACHE_LINE_SIZE) {
        // "dc cvac" - Clean data cache by VA to Point of Coherency
        asm volatile("dc cvac, %0" : : "r" (start) : "memory");
    }
    
    // DSB (Data Synchronization Barrier) đảm bảo các lệnh lưu trữ hoàn tất trước khi tiếp tục
    asm volatile("dsb sy" : : : "memory");
}

/**
 * @brief Vô hiệu hóa Cache (Invalidate)
 * Dùng sau khi DMA ghi xong dữ liệu vào RAM để CPU buộc phải nạp lại từ RAM.
 */
void hal_dcache_invalidate(void *addr, size_t len) {
    if (addr == NULL || len == 0) return;

    uintptr_t start = (uintptr_t)addr;
    uintptr_t end = start + len;
    
    // Luôn luôn căn chỉnh lùi lại đầu cache line
    uintptr_t align_start = start & ~(CACHE_LINE_SIZE - 1);
    
    for (; align_start < end; align_start += CACHE_LINE_SIZE) {
        // Sử dụng IVAC để ép CPU đọc lại từ DDR
        asm volatile("dc ivac, %0" : : "r" (align_start) : "memory");
    }
    
    asm volatile("dsb sy" : : : "memory");
    asm volatile("isb"    : : : "memory"); // Thêm ISB để đảm bảo pipeline CPU được cập nhật
}

/**
 * @brief Khởi tạo vòng Descriptor (Descriptor Ring) cho AXI DMA SG S2MM
 */
int FFT_Setup_SG() {
    uint32_t buf_offset = 0;
    
    // Đảm bảo con trỏ đã được gán (trong hal_init)
    //FFT_DMA_Reg = BRAM_Get_Virt_Addr(IDX_DMA_FFT);
    FFT_Desc_Ring = BRAM_Get_Virt_Addr(IDX_DMA_FFT_DESC); // Non-Cached
    FFT_Data_Buf = BRAM_Get_Virt_Addr(IDX_FFT_BUF);      // Cached
    
    uint32_t phys_desc_base = PHYS_FFT_DESC;
    uint32_t phys_data_base = PHYS_FFT_BUF;
    
    // --- 1. Tạo vòng Descriptor ---
    for (int i = 0; i < FFT_NUM_DESCRIPTORS; i++) {
        AXI_DMA_Descriptor *current = &FFT_Desc_Ring[i];
        
        // Địa chỉ vật lý tiếp theo cách 64 byte
        uint32_t next_phys = (i == FFT_NUM_DESCRIPTORS - 1) 
                             ? phys_desc_base 
                             : phys_desc_base + (i + 1) * 64; // Dùng số cứng 64 để an toàn
        
        current->next_desc_ptr_low = next_phys;
        current->buf_addr_low = phys_data_base + (i * FFT_FRAME_LEN_BYTES);
        current->ctrl = FFT_FRAME_LEN_BYTES;
        current->status = 0;
        
        // Cập nhật offset cho Buffer tiếp theo
        buf_offset += FFT_FRAME_LEN_BYTES; 
        
      printf("[DESC %d] VirtAddr: %p\n", i, (void*)current);
      printf("  -> Next Desc Phys: 0x%08X\n", current->next_desc_ptr_low);
      printf("  -> Buff Addr Phys: 0x%08X\n", current->buf_addr_low);
      printf("  -> Control (Len):  %u bytes\n", current->ctrl);
      printf("  -> Initial Status: 0x%08X\n", current->status);
      printf("------------------------------------------\n");

        // Bắt buộc: Flush cache cho Descriptor vì nó được map NON-CACHED
        // Kỹ thuật này giúp DMA thấy ngay sự thay đổi của cấu trúc Descriptor
        hal_dcache_flush(current, 64);
    }

    // Đảm bảo mọi việc ghi RAM hoàn tất trước khi báo cho DMA
    asm volatile("dsb sy" : : : "memory");

    // a. Reset DMA
    Xil_Out32(ADDR_DMA_FFT + S2MM_DMACR, 0x00000004); 
    // CHỜ RESET XONG (Cực kỳ quan trọng)
    while (Xil_In32(ADDR_DMA_FFT + S2MM_DMACR) & 0x00000004);

    // b. Thiết lập CURDESC TRƯỚC khi chạy DMA
    Xil_Out32(ADDR_DMA_FFT + S2MM_CURDESC, phys_desc_base);

    // c. Bật DMA (Run/Stop, Interrupt...)
    // Lưu ý: Đảm bảo bit RS (bit 0) được lên 1
    Xil_Out32(ADDR_DMA_FFT + S2MM_DMACR, 0x00013001); 

    // Cách tính đúng cho 16 descriptor (0 đến 15), mỗi cái 64 byte:
    uint32_t tail_desc_phys = phys_desc_base + (15 * 64); // = 0x7E0003C0
    Xil_Out32(ADDR_DMA_FFT + S2MM_TAILDESC, tail_desc_phys);

    printf("[FFT] DMA SG S2MM setup complete. Start monitoring.\n");
    return 0;
}

void FFT_Dump_DMA_Status() {
    uint32_t cr = Xil_In32(ADDR_DMA_FFT + S2MM_DMACR);
    uint32_t sr = Xil_In32(ADDR_DMA_FFT + S2MM_DMASR);
    uint32_t cur = Xil_In32(ADDR_DMA_FFT + S2MM_CURDESC);
    uint32_t tail = Xil_In32(ADDR_DMA_FFT + S2MM_TAILDESC);

    printf("\n--- [FFT DMA DEBUG STATUS] ---\n");
    printf("S2MM_DMACR: 0x%08X (Control)\n", cr);
    printf("S2MM_DMASR: 0x%08X (Status)\n", sr);
    printf("S2MM_CURDESC:  0x%08X\n", cur);
    printf("S2MM_TAILDESC: 0x%08X\n", tail);

    if (sr & 0x00000001) printf("  >> DMA is HALTED\n");
    if (sr & 0x00000002) printf("  >> DMA is IDLE\n");
    if (sr & 0x00000100) printf("  >> SG Internal Error\n");
    if (sr & 0x00000200) printf("  >> SG Slave Error\n");
    if (sr & 0x00000400) printf("  >> SG Decode Error\n");
    if (sr & 0x00001000) printf("  >> DMA Internal Error\n");
    if (sr & 0x00002000) printf("  >> DMA Slave Error\n");
    if (sr & 0x00004000) printf("  >> DMA Decode Error\n");
    printf("------------------------------\n\n");
}

/**
 * @brief Thread chính để giám sát và nhận dữ liệu FFT từ AXI DMA
 */
void *FFT_Monitor_SG_S2MM_Thread(void *arg) {
    long long fft_count = 0;
    int current_desc_idx = 0; // Khởi tạo index cục bộ

    printf("[FFT] Initializing virtual pointers and setting up DMA...\n");
    if (FFT_Setup_SG() < 0) return NULL;

    FFT_Dump_DMA_Status();

    while (1) {
        AXI_DMA_Descriptor *current_desc = &FFT_Desc_Ring[current_desc_idx];

        // Nếu vùng nhớ là CACHED, bạn bị Illegal Instruction ở đây.
        // Nếu vùng nhớ là UNCACHED (mmap O_SYNC), bỏ dòng dưới đi:
        // hal_dcache_invalidate(current_desc, 64); 

        // Kiểm tra bit 31 của status (Completed)
        // Xilinx DMA đặt bit 0x80000000 khi truyền xong một Descriptor
        uint32_t status = current_desc->status; 

        if (status & 0x80000000) { 
            // Xác định dữ liệu
            uint8_t *data_ptr = FFT_Data_Buf + (current_desc_idx * 1048576);
            
            // Xử lý dữ liệu tại data_ptr
            fft_count++;
            
            // RESET Descriptor để quay vòng
            current_desc->status = 0; 
            current_desc->ctrl = 1048576; // Cấp lại độ dài cho lần sau

            // QUAN TRỌNG: Với Scatter-Gather, bạn cần đẩy TAILDESC đi tới
            // để "mở đường" cho DMA chạy tiếp vòng tròn.
            uintptr_t phys_addr = PHYS_FFT_DESC + (current_desc_idx * 64);
            Xil_Out32(ADDR_DMA_FFT + S2MM_TAILDESC, phys_addr);

            current_desc_idx = (current_desc_idx + 1) % FFT_NUM_DESCRIPTORS;
            
            if(fft_count % 10 == 0) // Giảm bớt log để tránh rớt mạng CAN
                printf("[FFT] Received 10 frames. Total: %lld\n", fft_count);
        } else {
            // Kiểm tra lỗi DMA trong khi chờ
            uint32_t dma_sr = Xil_In32(ADDR_DMA_FFT + S2MM_DMASR);
            if (dma_sr & 0x00004000) { // Decode Error
                printf("[FATAL] DMA Decode Error! SR: 0x%08X\n", dma_sr);
                FFT_Dump_DMA_Status();
                break;
            }
            usleep(1000); // 1ms là đủ nhanh cho gói 1MB
        }
    }
    return NULL;
}