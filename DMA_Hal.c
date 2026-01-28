#include "DMA_Hal.h"


// Biến điều khiển thread
volatile int stop_cmac_tx = 0;

void cmac_setup_for_dma_loopback() {
    // 1. Đảm bảo TX Enable và KHÔNG phát IDLE (Để nhận dữ liệu từ AXI-Stream)
    Xil_Out32(ADDR_DMA_CMAC + 0x000C, 0x00000001); 

    // 2. Cấu hình RX để nhận mọi loại gói tin (Promiscuous Mode)
    // Thanh ghi 0x0014: Bit 4 (ctl_rx_check_mcast), Bit 5 (ctl_rx_check_ucast)
    // Hãy đảm bảo các bit check này bằng 0 (mặc định là 0, nhưng nên ghi đè cho chắc)
    uint32_t rx_cfg = Xil_In32(ADDR_DMA_CMAC + 0x0014);
    rx_cfg &= ~((1 << 4) | (1 << 5)); // Tắt kiểm tra Multicast và Unicast MAC
    Xil_Out32(ADDR_DMA_CMAC + 0x0014, rx_cfg);
    
    printf("[DMA_LOOPBACK] CMAC configured to accept all loopback packets.\n");
}

void run_full_dma_loopback_test(uint64_t tx_phys_addr, uint64_t rx_phys_addr, uint32_t pkt_size) {
    // 1. Chuẩn bị CMAC
    cmac_setup_for_dma_loopback();

    // 2. Khởi động chiều NHẬN (S2MM) trước
    // Luôn khởi động bộ nhận trước để khi dữ liệu vừa quay về là có chỗ chứa ngay
    Xil_Out32(ADDR_CMAC_REG + 0x30, 0x00000001); // Run S2MM DMA
    Xil_Out32(ADDR_CMAC_REG + 0x48, (uint32_t)rx_phys_addr);
    Xil_Out32(ADDR_CMAC_REG + 0x4C, (uint32_t)(rx_phys_addr >> 32));
    Xil_Out32(ADDR_CMAC_REG + 0x58, pkt_size); // Báo DMA chuẩn bị nhận pkt_size bytes

    // 3. Khởi động chiều TRUYỀN (MM2S)
    Xil_Out32(ADDR_CMAC_REG + 0x00, 0x00000001); // Run MM2S DMA
    Xil_Out32(ADDR_CMAC_REG + 0x18, (uint32_t)tx_phys_addr);
    Xil_Out32(ADDR_CMAC_REG + 0x1C, (uint32_t)(tx_phys_addr >> 32));
    Xil_Out32(ADDR_CMAC_REG + 0x28, pkt_size); // Lệnh bắt đầu đẩy dữ liệu đi

    printf("[DMA] Dang truyen %u bytes...\n", pkt_size);

    // 4. Đợi hoàn tất
    int timeout = 1000000;
    while(timeout--) {
        uint32_t tx_status = Xil_In32(ADDR_CMAC_REG + 0x04);
        uint32_t rx_status = Xil_In32(ADDR_CMAC_REG + 0x34);
        
        if ((tx_status & 0x02) && (rx_status & 0x02)) {
            printf(">>>> [SUCCESS] Dữ liệu đã đi hết vòng loopback!\n");
            break;
        }
        if (timeout == 0) printf(">>>> [FAILED] DMA Timeout! Check TREADY/TVALID trên ILA.\n");
    }
}

void test_external_loopback_cable() {
    printf("\n--- KIEM TRA VOI DAY LOOPBACK NGOAI ---\n");
    
    // 1. Chế độ External, Enable TX/RX, Tắt IDLE
    Xil_Out32(ADDR_CMAC_REG + 0x0090, 0x00000000); 
    Xil_Out32(ADDR_CMAC_REG + 0x000C, 0x00000001); 
    
    // 2. Reset nhanh để bắt nhịp mới
    Xil_Out32(ADDR_CMAC_REG + 0x0004, 0xC0000000); 
    usleep(10000);
    Xil_Out32(ADDR_CMAC_REG + 0x0004, 0x00000000);
    
    usleep(1000000); // Đợi 1s cho ổn định

    uint32_t status = Xil_In32(ADDR_CMAC_REG + 0x0204);
    if (status == 0x3) {
        printf(">>>> SUCCESS: FPGA DA NHAN DIEN DUOC CHINH NO QUA DAY QUANG!\n");
        printf("     (Loi truoc do chac chan do Card Broadcom hoac PC).\n");
    } else {
        printf(">>>> FAIL: Ngay ca Loopback ngoai cung khong len (Status: 0x%X).\n", status);
        printf("     => Kiem tra ngay: Invert TX Polarity trong Vivado.\n");
    }
}

void* CMAC_Monitor_Thread(void* arg) {
    uint32_t rx_stat, block_lock, tx_pkts, rx_pkts;

    while(1) {
        // Đọc 2 lần để xóa bit chốt
        Xil_In32(ADDR_CMAC_REG + 0x0204);
        rx_stat = Xil_In32(ADDR_CMAC_REG + 0x0204);
        block_lock = Xil_In32(ADDR_CMAC_REG + 0x020C);

        // Chốt số liệu thống kê
        Xil_Out32(ADDR_CMAC_REG + 0x02B0, 0x00000001);
        tx_pkts = Xil_In32(ADDR_CMAC_REG + 0x0500);
        rx_pkts = Xil_In32(ADDR_CMAC_REG + 0x0608);

        if ((rx_stat & 0x03) == 0x03) {
            printf("[LOOPBACK_OK] Status: 0x3 | TX_Pkts: %u | RX_Pkts: %u\n", tx_pkts, rx_pkts);
            
            // Neu ban dang chay DMA o thread khac, rx_pkts phai tang len deu dan
            if (tx_pkts > 0 && rx_pkts == 0) {
                printf(" |- [!] CANH BAO: Co phat nhung khong co nhan. Check Destination MAC!\n");
            }
        } else {
            printf("[LOOPBACK_FAIL] Status: 0x%X | Block_Lock: 0x%X\n", rx_stat, block_lock);
        }

        sleep(2); // Giam sat moi 2 giay
    }
    return NULL;
}

int cmac_dma_send_external(uint64_t phys_addr, uint32_t size) {
    // 1. Kiểm tra trạng thái DMA
    
    uint32_t status = Xil_In32(ADDR_DMA_CMAC + 0x04);
    if (status & 0x01) { // Nếu DMA đang bị Halted (do lỗi trước đó)
        Xil_Out32(ADDR_DMA_CMAC + 0x00, 0x01); // Thử Run lại
        usleep(100);
    }

    // 2. Thiết lập địa chỉ vùng nhớ (Hỗ trợ 64-bit)
    Xil_Out32(ADDR_DMA_CMAC + 0x18, (uint32_t)phys_addr);
    Xil_Out32(ADDR_DMA_CMAC + 0x1C, (uint32_t)(phys_addr >> 32));

    // 3. Kích hoạt truyền bằng cách ghi độ dài
    Xil_Out32(ADDR_DMA_CMAC + 0x28, size); 

    // 4. Chờ hoàn tất (với Timeout)
    int wait_count = 50000; // Đợi tối đa ~500ms cho gói tin lớn
    while (wait_count--) {
        status = Xil_In32(ADDR_DMA_CMAC + 0x04);
        
        if (status & 0x02) return 0;  // SUCCESS (Idle/Done)
        
        if (status & 0x70) { // Lỗi Internal/Slave/Decode Error
            printf("[DMA_ERR] Fatal Error! SR: 0x%X\n", status);
            return -1;
        }
        usleep(10); 
    }
    
    // 5. NẾU TIMEOUT - PHÂN TÍCH NGUYÊN NHÂN TẠI CMAC
    uint32_t cmac_l1 = Xil_In32(ADDR_CMAC_REG + 0x0204);
    uint32_t fec_l1  = Xil_In32(ADDR_CMAC_REG + 0x1004);
    
    printf("[DMA_TIMEOUT] Dữ liệu kẹt tại FIFO/CMAC!\n");
    printf(" |- CMAC Status: 0x%X (%s)\n", cmac_l1, (cmac_l1 & 0x3) == 0x3 ? "Link UP" : "Link DOWN");
    printf(" |- FEC Status : 0x%X (%s)\n", fec_l1, (fec_l1 & 0x4000) ? "Locked" : "Unlocked");
    
    if (!(cmac_l1 & 0x2)) {
        printf(" |- Gợi ý: Link Down nên CMAC không kéo TREADY, làm đầy FIFO.\n");
    }
    
    return -2; // Timeout
}

void* CMAC_TX_External_Worker_Thread(void* arg) {
    uint32_t *src_virt = (uint32_t*)BRAM_Get_Virt_Addr(IDX_CMAC_TX_BUF);
    uint32_t test_size = 4096; // 4KB (Đảm bảo >= 64 bytes)
    uint64_t phys_addr = 0x60000000;  
    uint32_t loop_count = 0;
    int error_count = 0;

    printf("\n==================================================\n");
    printf("  STARTING CMAC EXTERNAL TRANSMISSION (SFP+MODE)  \n");
    printf("==================================================\n");

    // BƯỚC 1: CẤU HÌNH CMAC CHẾ ĐỘ EXTERNAL (TẮT LOOPBACK)
    Xil_Out32(ADDR_CMAC_REG + 0x0090, 0x00000000); // 0 = External
    
    // BƯỚC 2: CẤU HÌNH RS-FEC (Bắt buộc cho module SR4)
    Xil_Out32(ADDR_CMAC_REG + 0x107C, 0x00000003); // Enable TX/RX FEC
    Xil_Out32(ADDR_CMAC_REG + 0x1000, 0x00000007); // IEEE Mode + Correction

    // BƯỚC 3: ENABLE TẦNG PHÁT (TX)
    Xil_Out32(ADDR_CMAC_REG + 0x000C, 0x00000001); 

    // BƯỚC 4: RESET CMAC CORE & DMA ĐỂ BẮT ĐẦU SẠCH
    Xil_Out32(ADDR_CMAC_REG + 0x0004, 0xC0000000); // Reset PCS
    Xil_Out32(ADDR_DMA_CMAC + 0x00, 0x04);         // Reset DMA
    usleep(10000);
    Xil_Out32(ADDR_CMAC_REG + 0x0004, 0x00000000);
    Xil_Out32(ADDR_DMA_CMAC + 0x00, 0x01);         // Run DMA
    usleep(50000);

    // Khởi tạo dữ liệu mẫu
    for(int i = 0; i < (test_size / 4); i++) src_virt[i] = 0x55AA0000 + i;

    while (!stop_cmac_tx) {
        // Cập nhật nhãn gói tin để soi ILA
        src_virt[0] = loop_count;

        // THỰC HIỆN GỬI RA ĐƯỜNG QUANG
        int result = cmac_dma_send_external(phys_addr, test_size);

        if (result == 0) {
            error_count = 0;
            if (loop_count % 1000 == 0) {
                printf("[TX_OK] Sent %u packets to PC.\n", loop_count);
            }
        } else {
            error_count++;
            // Nếu lỗi quá nhiều, dừng lại 1 giây để tránh tràn Log
            if (error_count > 5) {
                printf("[TX_STALL] Link issue detected. Retrying in 1s...\n");
                sleep(1);
                error_count = 0;
            }
        }

        loop_count++;
        usleep(1000); // Giãn cách giữa các lần gửi 1ms
    }

    printf("[TX] Thread exiting...\n");
    return NULL;
}

void cmac_dma_test_send_and_read() {
   // 4. CHUẨN BỊ DỮ LIỆU MẪU (Pattern)
    uint32_t *src_ptr = (uint32_t*)BRAM_Get_Virt_Addr(IDX_CMAC_TX_BUF);
    uint32_t *dst_ptr = (uint32_t*)BRAM_Get_Virt_Addr(IDX_CMAC_RX_BUF);
    uint32_t test_size = 8192 * 1024; // Phải chia hết cho 64

    // Ghi pattern vào nguồn và XÓA sạch vùng đích
    for(int i = 0; i < (test_size / 4); i++) {
        src_ptr[i] = 0xABCDE000 + i; 
        dst_ptr[i] = 0x00000000; // Xóa sạch dữ liệu cũ
    }

    // 5. CHẠY TEST LOOPBACK NGOÀI
    // Lưu ý: Hàm cmac_dma_external_loopback_test cần được cập nhật 
    // để bật CMAC TX/RX Enable như tôi đã hướng dẫn ở câu trước.
    cmac_dma_run_transfer(0x60000000, 0x68000000, test_size);

    // 6. KIỂM TRA DỮ LIỆU NHẬN VỀ
    int error_count = 0;
    for(int i = 0; i < (test_size / 4); i++) {
        if (dst_ptr[i] != src_ptr[i]) {
            if (error_count < 5) { // Chỉ in 5 lỗi đầu tiên
                printf("[ERROR] Sai lech tai index %d: Sent 0x%08X, Got 0x%08X\n", 
                        i, src_ptr[i], dst_ptr[i]);
            }
            error_count++;
        }
    }

    if (error_count == 0) {
        printf("[MAIN] Data Integrity Check: PASSED! (Verified %u bytes)\n", test_size);
    } else {
        printf("[MAIN] Data Integrity Check: FAILED! Total errors: %d\n", error_count);
    }
}    

void* CMAC_TX_Worker_Thread(void* arg) {
    uint32_t *src_ptr = (uint32_t*)BRAM_Get_Virt_Addr(IDX_CMAC_TX_BUF);
    uint32_t test_size = 1024 * 1024; // 8MB
    uint64_t phys_addr = 0x60000000;  // Địa chỉ vật lý TX
    uint32_t pkt_count = 0;

    printf("[CMAC_TX] Thread started. Interval: 10ms, Size: %u bytes\n", test_size);

    // 1. CHUẨN BỊ DỮ LIỆU MẪU MỘT LẦN (Hoặc cập nhật trong vòng lặp nếu muốn data thay đổi)
    for(int i = 0; i < (test_size / 4); i++) {
        src_ptr[i] = 0xABCDE000 + i;
    }

    // 2. VÒNG LẶP GỬI ĐỊNH KỲ
    while (!stop_cmac_tx) {
        // Gọi hàm gửi (đã viết ở câu trước)
        cmac_dma_run_send(phys_addr, test_size);

        pkt_count++;
        
        // In log mỗi 100 gói (khoảng 1 giây in 1 lần) để tránh tràn màn hình console
        if (pkt_count % 100 == 0) {
            printf("[CMAC_TX] Sent %u packets...\n", pkt_count);
        }

        // Nghỉ 10ms (10.000 microseconds)
        usleep(10000); 
    }

    printf("[CMAC_TX] Thread stopped.\n");
    return NULL;
}

int dma_only_test_send1(uint64_t phys_addr, uint32_t size) {
    // 1. Kiểm tra trạng thái trước khi gửi
    uint32_t initial_status = Xil_In32(ADDR_DMA_CMAC + 0x04);
    if (initial_status & 0x01) { // Nếu DMA đang Halted
        Xil_Out32(ADDR_DMA_CMAC + 0x00, 0x01); // Thử Run nó lên
        usleep(1000);
    }

    // 2. Cấu hình địa chỉ
    Xil_Out32(ADDR_DMA_CMAC + 0x18, (uint32_t)phys_addr);
    Xil_Out32(ADDR_DMA_CMAC + 0x1C, (uint32_t)(phys_addr >> 32));

    // 3. Ghi độ dài để bắt đầu truyền
    Xil_Out32(ADDR_DMA_CMAC + 0x28, size); 

    // 4. Chờ hoàn tất với chẩn đoán
    int timeout = 500000; // Đợi đủ lâu cho 1MB
    while (timeout--) {
        uint32_t status = Xil_In32(ADDR_DMA_CMAC + 0x04);
        
        if (status & 0x02) return 0; // Thành công (Idle/Done)
        
        if (status & 0x70) { // Có lỗi Internal/Slave/Decode
            printf("[DMA_FATAL] Error Detected! SR: 0x%X\n", status);
            return -1; 
        }
    }
    
    // Nếu bị Timeout, đọc thêm thanh ghi CMAC để xem tại sao nó không nhận dữ liệu
    uint32_t cmac_ready = Xil_In32(ADDR_CMAC_REG + 0x0204);
    printf("[DMA_STALL] Timeout! DMA SR: 0x%X | CMAC Status: 0x%X\n", 
            Xil_In32(ADDR_DMA_CMAC + 0x04), cmac_ready);
            
    return -2; // Timeout
}

/**
 * @brief Truyền dữ liệu qua DMA CMAC (Simple Mode)
 * @param check_tready: 1 để kiểm tra CMAC Link trước khi gửi, 0 để gửi ép buộc
 */
int dma_only_test_send(uint64_t phys_addr, uint32_t size, int check_tready) {
    
    // 1. KIỂM TRA TREADY (Thông qua trạng thái CMAC)
    // TREADY của CMAC chỉ lên cao khi Link đã Aligned và TX Core được bật
    if (check_tready) {
        uint32_t cmac_stat = Xil_In32(ADDR_CMAC_REG + 0x0204); // STAT_RX_STATUS
        uint32_t tx_status = Xil_In32(ADDR_CMAC_REG + 0x0200); // STAT_TX_STATUS
        
        // Nếu Link chưa Aligned (bit 1 != 1) hoặc đang bị TX Fault (bit 0 == 1)
        if (((cmac_stat & 0x02) == 0) || (tx_status & 0x01)) {
            printf("[DMA_TX] Skip: CMAC not ready (TREADY likely LOW)\n");
            return -3; // CMAC Not Ready
        }
    }

    // 2. Kiểm tra và dọn dẹp trạng thái DMA
    uint32_t sr = Xil_In32(ADDR_DMA_CMAC + 0x04);
    
    // Nếu DMA đang bị Halted (bit 0 = 1), ra lệnh Run (bit 0 = 1 ở thanh ghi 0x00)
    if (sr & 0x01) {
        Xil_Out32(ADDR_DMA_CMAC + 0x00, 0x01);
    }

    // XÓA CÁC BIT INTERRUPT CŨ (IOC và Error)
    // Rất quan trọng để bit 0x1000 có thể bật lên chính xác cho lần truyền này
    Xil_Out32(ADDR_DMA_CMAC + 0x04, 0x0000F000);

    // 3. Cấu hình địa chỉ nguồn (MM2S)
    Xil_Out32(ADDR_DMA_CMAC + 0x18, (uint32_t)phys_addr);
    Xil_Out32(ADDR_DMA_CMAC + 0x1C, (uint32_t)(phys_addr >> 32));

    // 4. Ghi độ dài (LENGTH) để KÍCH HOẠT DMA
    Xil_Out32(ADDR_DMA_CMAC + 0x28, size); 

    // 5. Chờ hoàn tất (Nhận diện cả Idle 0x02 và IOC 0x1000)
    int timeout = 500000; 
    while (timeout--) {
        uint32_t status = Xil_In32(ADDR_DMA_CMAC + 0x04);
        
        // Nếu thấy bit Idle (1) hoặc IOC (12) nghĩa là đã truyền xong
        if (status & 0x1002) {
            // Xóa bit IOC sau khi xong để chuẩn bị cho lần sau
            Xil_Out32(ADDR_DMA_CMAC + 0x04, 0x00001000);
            return 0; 
        }
        
        // Kiểm tra các bit lỗi (4, 5, 6)
        if (status & 0x70) {
            printf("[DMA_FATAL] TX Error! SR: 0x%X\n", status);
            return -1; 
        }
    }
    
    // Nếu bị Timeout, in thêm chẩn đoán
    uint32_t final_sr = Xil_In32(ADDR_DMA_CMAC + 0x04);
    uint32_t cmac_final = Xil_In32(ADDR_CMAC_REG + 0x0204);
    // printf("[DMA_STALL] Timeout! SR: 0x%X | CMAC Status: 0x%X | Bytes Left: %u\n", 
    //         final_sr, cmac_final, Xil_In32(ADDR_DMA_CMAC + 0x28));
            
    return -2; // Timeout
}

void* DMA_Isolation_Test_Thread(void* arg) {
    uint32_t *src_virt = (uint32_t*)BRAM_Get_Virt_Addr(IDX_CMAC_TX_BUF);
    uint32_t test_size = 1024 * 1024; // 1MB 
    uint64_t phys_addr = 0x60000000;  
    uint32_t loop_count = 0;

    printf("\n==================================================\n");
    printf(" STARTING DMA ISOLATION TEST (EXTENAL LOOPBACK) \n");
    printf("==================================================\n");

    // BƯỚC 1: ÉP CMAC VÀO CHẾ ĐỘ LOOPBACK NỘI BỘ
    // Việc này giúp CMAC luôn sẵn sàng nhận dữ liệu từ DMA từ AXI-Stream
    Xil_Out32(ADDR_CMAC_REG + 0x0090, 0x00000001); 
    
    // Đảm bảo CMAC TX được Enable
    Xil_Out32(ADDR_CMAC_REG + 0x000C, 0x00000001);
    
    // BƯỚC 2: RESET DMA MỘT LẦN DUY NHẤT ĐỂ LÀM SẠCH
    Xil_Out32(ADDR_DMA_CMAC + 0x00, 0x04); 
    usleep(1000);
    Xil_Out32(ADDR_DMA_CMAC + 0x00, 0x01); // Run
    usleep(1000);

    // Khởi tạo dữ liệu
    for(int i = 0; i < (test_size / 4); i++) src_virt[i] = i; //0xDEADBEEF ^ i;

    while (!stop_cmac_tx) {
        // Cập nhật nhãn gói tin
        src_virt[0] = loop_count;

        // THỰC HIỆN GỬI
        int result = dma_only_test_send(phys_addr, test_size, 1);

        if (result == 0) {
            if (loop_count % 100 == 0) {
                printf("[DMA_OK] Loop %u finished 1MB transfer.\n", loop_count);
            }
        } else if (result == -2) {
            //printf("[DMA_FAIL] Timeout at loop %u. Resetting DMA...\n", loop_count);
            // Reset DMA để cứu App
            Xil_Out32(ADDR_DMA_CMAC + 0x00, 0x04);
            usleep(1000);
            Xil_Out32(ADDR_DMA_CMAC + 0x00, 0x01);
        } else {
            printf("[DMA_FAIL] Fatal Error at loop %u. Stopping test.\n", loop_count);
            break;
        }

        loop_count++;
        // Không nghỉ quá lâu để test stress-test DMA
        usleep(1000); 
    }

    // Trả lại trạng thái External khi thoát test
    Xil_Out32(ADDR_CMAC_REG + 0x0090, 0x00000000);
    return NULL;
}

int cmac_dma_execute_send(uint64_t phys_addr, uint32_t size) {
    Xil_Out32(ADDR_DMA_CMAC + 0x00, 0x04); // Reset
    while (Xil_In32(ADDR_DMA_CMAC + 0x00) & 0x04);

    Xil_Out32(ADDR_DMA_CMAC + 0x00, 0x01); // Run
    Xil_Out32(ADDR_DMA_CMAC + 0x18, (uint32_t)phys_addr);
    Xil_Out32(ADDR_DMA_CMAC + 0x1C, (uint32_t)(phys_addr >> 32));
    Xil_Out32(ADDR_DMA_CMAC + 0x28, size);

    int wait_timeout = 10000; 
    while (wait_timeout--) {
        uint32_t status = Xil_In32(ADDR_DMA_CMAC + 0x04);
        if (status & 0x02) return 0; // Xong
        if (status & 0x70) {
            printf("[ERROR] DMA Error Bits: 0x%X\n", status);
            return -1;
        }
        usleep(1);
    }
    
    // NẾU TIMEOUT - ĐỌC CHẨN ĐOÁN
    uint32_t cmac_stat = Xil_In32(ADDR_CMAC_REG + 0x0204);
    uint32_t cmac_ctrl = Xil_In32(ADDR_CMAC_REG + 0x000C);
    printf("--- DIAGNOSTIC ON TIMEOUT ---\n");
    printf("CMAC Link Status (0x0204): 0x%X (Phải là 0x3)\n", cmac_stat);
    printf("CMAC TX Enable  (0x000C): 0x%X (Phải là 0x1)\n", cmac_ctrl);
    
    return -2;
}

int cmac_dma_send_fast(uint64_t phys_addr, uint32_t size) {
    // 1. Reset DMA MM2S
    Xil_Out32(ADDR_DMA_CMAC + 0x00, 0x04); 
    int rst_timeout = 1000;
    while ((Xil_In32(ADDR_DMA_CMAC + 0x00) & 0x04) && rst_timeout--); 

    // 2. Cấu hình Run và Địa chỉ
    Xil_Out32(ADDR_DMA_CMAC + 0x00, 0x01); 
    Xil_Out32(ADDR_DMA_CMAC + 0x18, (uint32_t)phys_addr);
    Xil_Out32(ADDR_DMA_CMAC + 0x1C, (uint32_t)(phys_addr >> 32));

    // 3. Kích hoạt gửi bằng cách ghi độ dài
    Xil_Out32(ADDR_DMA_CMAC + 0x28, size); 

    // 4. Chờ hoàn tất
    int wait_timeout = 20000; // Tăng lên một chút cho gói 1MB
    while (wait_timeout--) {
        uint32_t status = Xil_In32(ADDR_DMA_CMAC + 0x04);
        if (status & 0x02) return 0;  // Thành công (IOC)
        if (status & 0x70) return -1; // Lỗi DMA (Internal/Slave error)
        // usleep(1); // Tùy chọn: có thể bỏ để tăng tốc độ phản ứng
    }
    
    return -2; // Timeout (Nghẽn đầu ra)
}

void* CMAC_TX_Worker_Thread2(void* arg) {
    uint32_t *src_virt = (uint32_t*)BRAM_Get_Virt_Addr(IDX_CMAC_TX_BUF);
    uint32_t test_size = 1024 * 1024; // 1MB 
    uint64_t phys_addr = 0x60000000;  // Vùng cmac_reserved (HPC Port)
    uint32_t loop_count = 0;
    int error_streak = 0;

    printf("[CMAC_TX] Thread started. Coherency: HARDWARE (CCI ON)\n");

    // 1. Đảm bảo CMAC TX Enable và cấu hình Link ổn định
    Xil_Out32(ADDR_CMAC_REG + 0x000C, 0x1); 

    // 2. Khởi tạo pattern mẫu một lần
    for(int i = 0; i < (test_size / 4); i++) {
        src_virt[i] = 0xABCDE000 + i;
    }

    while (!stop_cmac_tx) {
        // 3. Cập nhật dữ liệu động (Giúp soi ILA thấy dữ liệu nhảy số)
        // Vì đã bật CCI và dma-coherent, CPU ghi vào đây là DMA thấy ngay lập tức
        src_virt[0] = 0x55AA0000 | (loop_count & 0xFFFF);
        src_virt[1] = loop_count; 

        // 4. THỰC HIỆN GỬI (KHÔNG CẦN FLUSH CACHE!)
        int result = cmac_dma_send_fast(phys_addr, test_size);

        // 5. Kiểm tra kết quả
        if (result == 0) {
            error_streak = 0;
            if (loop_count % 100 == 0) {
                printf("[CMAC_TX] Sent %u packets OK.\n", loop_count);
            }
        } else {
            error_streak++;
            if (result == -1) 
                printf("[CMAC_TX] DMA ERROR at packet %u (Check Address/SMMU)\n", loop_count);
            else 
                printf("[CMAC_TX] DMA TIMEOUT at packet %u (FIFO Full/Link Down?)\n", loop_count);
            
            if (error_streak > 10) {
                printf("[CMAC_TX] Streak of errors detected. Cooling down 1s...\n");
                sleep(1);
                error_streak = 0;
            }
        }

        loop_count++;
        
        // 6. Nghỉ 10ms định kỳ
        usleep(10000); 
    }

    printf("[CMAC_TX] Thread exiting...\n");
    return NULL;
}

void cmac_dma_run_send(uint64_t src_phys, uint32_t pkt_size) {
    // 1. Đảm bảo TX đã Enable (Ghi đè cho chắc chắn)
    Xil_Out32(ADDR_CMAC_REG + 0x000C, 0x1); 

    // 2. Reset DMA MM2S
    Xil_Out32(ADDR_DMA_CMAC + 0x00, 0x4); 
    while (Xil_In32(ADDR_DMA_CMAC + 0x00) & 0x4);

    // 3. Cấu hình DMA
    Xil_Out32(ADDR_DMA_CMAC + 0x00, 0x1); 
    Xil_Out32(ADDR_DMA_CMAC + 0x18, (uint32_t)src_phys);
    Xil_Out32(ADDR_DMA_CMAC + 0x1C, (uint32_t)(src_phys >> 32));
    
    // Trước khi ghi length, kiểm tra xem DMA có đang Idle không
    if (!(Xil_In32(ADDR_DMA_CMAC + 0x04) & 0x1)) {
        printf("[WARN] DMA MM2S is not Idle before transfer!\n");
    }

    Xil_Out32(ADDR_DMA_CMAC + 0x28, pkt_size); 

    // 4. Chờ kèm theo in trạng thái nếu lỗi
    int timeout = 20000; 
    while (timeout--) {
        uint32_t status = Xil_In32(ADDR_DMA_CMAC + 0x04);
        if (status & 0x2) return; // Xong!

        if (status & 0x70) {
            printf("[FATAL] DMA Error Bit Detected: 0x%X\n", status);
            return;
        }
        usleep(1);
    }

    // Nếu bị Timeout, in thêm các thanh ghi quan trọng của CMAC để chẩn đoán
    uint32_t cmac_tx_status = Xil_In32(ADDR_CMAC_REG + 0x000C);
    uint32_t dma_sr = Xil_In32(ADDR_DMA_CMAC + 0x04);
    printf("[TIMEOUT] DMA Hang! DMA_SR: 0x%X, CMAC_TX_CTRL: 0x%X\n", dma_sr, cmac_tx_status);
}

void cmac_dma_run_transfer(uint32_t src_phys, uint32_t dst_phys, uint32_t pkt_size) {
    // 1. KIỂM TRA TRẠNG THÁI LINK TRƯỚC KHI TRUYỀN
    uint32_t status = Xil_In32(ADDR_CMAC_REG + 0x0204);
    if ((status & 0x3) != 0x3) {
        printf("[WARN] Link Down (Status: 0x%X). Phải chạy lại quy trình Reset!\n", status);
        // Có thể gọi hàm Deep Reset ở đây nếu cần
        return;
    }

    // 2. RESET RIÊNG BỘ DMA (Để xóa buffer cũ, không làm sập Link CMAC)
    Xil_Out32(ADDR_DMA_CMAC + 0x00, 0x4); // MM2S Reset
    Xil_Out32(ADDR_DMA_CMAC + 0x30, 0x4); // S2MM Reset
    while ((Xil_In32(ADDR_DMA_CMAC + 0x00) & 0x4) || (Xil_In32(ADDR_DMA_CMAC + 0x30) & 0x4));

    // 3. CẤU HÌNH DMA (Lưu ý: 512-bit bus cần alignment 64 bytes)
    // Cấu hình bộ nhận (RX)
    Xil_Out32(ADDR_DMA_CMAC + 0x30, 0x1);        
    Xil_Out32(ADDR_DMA_CMAC + 0x48, dst_phys);   
    Xil_Out32(ADDR_DMA_CMAC + 0x4C, 0);          
    Xil_Out32(ADDR_DMA_CMAC + 0x58, pkt_size);   // Sẵn sàng nhận

    // Cấu hình bộ truyền (TX)
    Xil_Out32(ADDR_DMA_CMAC + 0x00, 0x1);        
    Xil_Out32(ADDR_DMA_CMAC + 0x18, src_phys);   
    Xil_Out32(ADDR_DMA_CMAC + 0x1C, 0);          
    Xil_Out32(ADDR_DMA_CMAC + 0x28, pkt_size);   // BẮT ĐẦU ĐẨY DATA RA CÁP QUANG

    // 4. CHỜ HOÀN TẤT VỚI TIMEOUT
    int dma_timeout = 10000; // Tăng timeout cho chắc chắn
    while (dma_timeout--) {
        uint32_t tx_sr = Xil_In32(ADDR_DMA_CMAC + 0x04);
        uint32_t rx_sr = Xil_In32(ADDR_DMA_CMAC + 0x34);
        
        if ((tx_sr & 0x2) && (rx_sr & 0x2)) {
            // printf("Transfer OK!\n");
            break; 
        }
        
        if ((tx_sr & 0x70) || (rx_sr & 0x70)) {
            printf("[ERROR] DMA Error! TX_SR: 0x%X, RX_SR: 0x%X\n", tx_sr, rx_sr);
            return;
        }
        
        if (dma_timeout == 0) {
            printf("[TIMEOUT] DMA Hang! Kiem tra tin hieu TLAST tu FPGA.\n");
            return;
        }
    }
}

