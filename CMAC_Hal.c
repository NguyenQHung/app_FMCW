#include "CMAC_Hal.h"
#include "main.h"

// Giả định địa chỉ GPIO cho Preamble (Bạn thay bằng địa chỉ thực tế trong Vivado)
#define GPIO_TX_PREAMBLE_ADDR   0xA0290000 // Ví dụ địa chỉ GPIO phát
#define GPIO_RX_PREAMBLE_ADDR   0xA02A0000 // Ví dụ địa chỉ GPIO thu

void smart_init_cmac() {

    int cnt_idle = 0;

    Xil_Out32(ADDR_CMAC_REG + STAT_TICK_REG, 0x00000001); // Chốt số liệu
    usleep(400);

    // 1. Kiểm tra xem Link đã UP chưa
    uint32_t status = Xil_In32(ADDR_CMAC_REG + 0x0204);
    //uint32_t fec_status = Xil_In32(ADDR_CMAC_REG + 0x1004);
    uint32_t loopback_mode = Xil_In32(ADDR_CMAC_REG + 0x0090); // Đọc thanh ghi 0x90

    loopback_mode = Xil_In32(ADDR_CMAC_REG + 0x0090); // Đọc thanh ghi 0x90
    usleep(400);
    loopback_mode = Xil_In32(ADDR_CMAC_REG + 0x0090); // Đọc thanh ghi 0x90
    usleep(400);

    // 1. KIỂM TRA PHIÊN BẢN & KẾT NỐI AXI
    uint32_t reg = Xil_In32(ADDR_CMAC_REG + REG_CORE_VERSION);
    if (reg == 0xFFFFFFFF || reg == 0) {
        printf("[ERROR] Khong ket noi duoc CMAC qua AXI-Lite! Kiem tra Base Address.\n");
        return;
    }
    printf("[INFO] CMAC IP Version: %d.%d\n", (reg >> 8) & 0xFF, reg & 0xFF);


    printf(" [CHECK] loopback_mode: 0x%X, RX_Status: 0x%X\n", loopback_mode, status);

    printf("[FIX] Step 0: Deep GT Reset (0x0000)...\n");
    Xil_Out32(ADDR_CMAC_REG + REG_GT_RESET, 0x00000001); 

    // --- BƯỚC 2: ĐIỀU KIỆN LINK ĐÃ SẴN SÀNG ---
    // Link phải 0x3, FEC phải Locked (0x4000) và KHÔNG được ở mode Loopback (0x0)
    if ((status == 0x3) && (loopback_mode == 0)) {
        printf("[INFO] CMAC Link dang hoat dong on dinh (0x3). Duy tri ket noi...\n");
        
        // --- BƯỚC MỞ KHÓA QUAN TRỌNG NHẤT ---
        // Ghi 0x1 vào 0x000C để TẮT bit phát IDLE nội bộ (Bit 5 = 0)
        // và BẬT TX Enable (Bit 0 = 1).
        Xil_Out32(ADDR_CMAC_REG + 0x000C, 0x00000001); 
        
        printf("[OK] Da mo van du lieu (TREADY se san sang cho DMA).\n");
        
        setup_advanced_cmac_features();
        check_signal_stability();
        return; // Thoát, giữ nguyên Link không cho Reset
    }

    // --- BƯỚC 3: NẾU LINK DOWN HOẶC SAI MODE -> KHOI TẠO LẠI TỪ ĐẦU ---
    printf("[INFO] Link dang Down hoac dang o Loopback. Bat dau khoi tao full...\n");

    process_cmac_100g_sr4_full(); 

    for(cnt_idle = 0; cnt_idle < 2; cnt_idle++)
    {
        test_internal_loopback_with_fec();
	    usleep(40000);
    }
    
    final_link_up_force();
    
    setup_advanced_cmac_features();
    check_signal_stability();

}


// ==========================================================
// HÀM XỬ LÝ CMAC FULL QUY TRÌNH (PHIÊN BẢN DEFINITIVE)
// ==========================================================
void process_cmac_100g_sr4_full() {
    uint32_t reg, dummy;
    uint32_t timeout = 0;

    printf("\n==================================================\n");
    printf(" KHOI DONG QUY TRINH CMAC 100G (SR4 + RS-FEC) V7 \n");
    printf("==================================================\n");

    // // 1. KIỂM TRA PHIÊN BẢN & KẾT NỐI AXI
    // reg = Xil_In32(ADDR_CMAC_REG + REG_CORE_VERSION);
    // if (reg == 0xFFFFFFFF || reg == 0) {
    //     printf("[ERROR] Khong ket noi duoc CMAC qua AXI-Lite! Kiem tra Base Address.\n");
    //     return;
    // }
    // printf("[INFO] CMAC IP Version: %d.%d\n", (reg >> 8) & 0xFF, reg & 0xFF);

    // 2. [BƯỚC 0] RESET TRANSCEIVER (GT) ĐỂ KHÓA PLL, xóa hết các thanh ghi trong GT
    printf("[FIX] Step 0: Deep GT Reset (0x0000)...\n");
    Xil_Out32(ADDR_CMAC_REG + REG_GT_RESET, 0x00000001); 
    usleep(100000); 
    Xil_Out32(ADDR_CMAC_REG + REG_GT_RESET, 0x00000000); // co the khong ghi, da reset roi
    
    // Đợi và kiểm tra PLL Lock (Đọc 0x0200)
    printf("[CHECK] Waiting for Transceiver PLL Lock...");
    timeout = 0;
    while(1) {
        reg = Xil_In32(ADDR_CMAC_REG + STAT_TX_STATUS);
        if (!(reg & 0x01)) { // Bit 0 (TX Local Fault) = 0 nghĩa là PLL OK
            printf(" LOCKED!\n");
            break;
        } else {
            printf("UNLOCK LOCKED!\n");
        }
        usleep(10000);
        if (++timeout > 100) {
            printf(" TIMEOUT!\n [!] LOI: PLL khong Lock. Kiem tra SI570 Clock (156.25MHz).\n");
            break;
        }
    }


    // 3. THIẾT LẬP CHẾ ĐỘ EXTERNAL (Tắt Loopback)
    Xil_Out32(ADDR_CMAC_REG + REG_GT_LOOPBACK, 0x00000000);

    // // 4. CẤU HÌNH RS-FEC THEO CHUẨN IEEE (Ép mode 0x7 cho SR4/Broadcom)
    // printf("[STEP 1] Configuring RS-FEC (IEEE Mode + Correction)...\n");
    // Xil_Out32(ADDR_CMAC_REG + REG_RSFEC_CONFIG_ENABLE, 0x00000003); // Enable TX/RX FEC
    // Xil_Out32(ADDR_CMAC_REG + REG_RSFEC_IND_CORR, 0x00000007);      // IEEE Compliant Mode

    // 5. RESET CORE ĐỂ ÁP DỤNG CẤU HÌNH FEC
    printf("[STEP 2] Performing CMAC Core Soft Reset...\n");
    Xil_Out32(ADDR_CMAC_REG + REG_SOFT_RESET, 0xC0000000); 
    usleep(50000); 
    Xil_Out32(ADDR_CMAC_REG + REG_SOFT_RESET, 0x00000000);
    usleep(50000);

    // 6. QUY TRÌNH BRING-UP (Enable RX & TX Idles)
    printf("[STEP 3] Enabling RX & TX (Sending Idles)...\n");
    Xil_Out32(ADDR_CMAC_REG + REG_RX_CONFIG, 0x00000001); 
    Xil_Out32(ADDR_CMAC_REG + REG_TX_CONFIG, 0x00000001); // ctl_tx_enable=1, ctl_tx_send_rfi=0

    // 7. ĐỢI ĐỒNG BỘ TOÀN PHẦN (FEC + PCS)
    // printf("[WAIT] Waiting for RS-FEC & PCS Alignment...");
    // timeout = 0;
    // while (1) {
    //     // Đọc 2 lần để xóa bit chốt (Sticky bits)
    //     Xil_In32(ADDR_CMAC_REG + STAT_RX_STATUS);
    //     reg = Xil_In32(ADDR_CMAC_REG + STAT_RX_STATUS);
    //     //uint32_t fec_stat = Xil_In32(ADDR_CMAC_REG + STAT_RSFEC_STATUS);
        
    //     // Điều kiện Link Up: RX Aligned (bit 1) và RS-FEC Locked (bit 14)
    //     if ((reg & 0x00000002)) { 
    //         printf(" SUCCESS!\n");
    //         break;
    //     }
    //     usleep(10000); 
    //     if (++timeout > 300) { 
    //         printf(" FAILED!\n");
    //         break;
    //     }
    // }

    // 8. BÁO CÁO TRẠNG THÁI CUỐI CÙNG (DIAGNOSTICS)
    printf("\n--- FINAL DIAGNOSTICS REPORT ---\n");
    
    // Đọc trạng thái chính
    reg = Xil_In32(ADDR_CMAC_REG + STAT_RX_STATUS);
    printf("RX Main Status (0x204): 0x%X\n", reg);
    printf(" |- Aligned    : %s\n", (reg & 0x2) ? "YES" : "NO");
    printf(" |- RemoteFault: %s\n", (reg & 0x20) ? "YES (PC reported error)" : "NO (OK)");
    printf(" |- High BER   : %s\n", (reg & 0x10) ? "YES (Bad Signal)" : "NO");

    // Trạng thái khóa Lane
    uint32_t lock = Xil_In32(ADDR_CMAC_REG + STAT_RX_BLOCK_LOCK);
    uint32_t sync = Xil_In32(ADDR_CMAC_REG + STAT_RX_LANE_SYNC);
    printf("PCS Block Lock (0x20C) : 0x%05X %s\n", lock & 0xFFFFF, (lock & 0xFFFFF) == 0xFFFFF ? "(OK)" : "(BAD)");
    printf("PCS Lane Sync  (0x210) : 0x%05X %s\n", sync & 0xFFFFF, (sync & 0xFFFFF) == 0xFFFFF ? "(OK)" : "(BAD)");

    // Trạng thái RS-FEC
    //uint32_t rsfec = Xil_In32(ADDR_CMAC_REG + STAT_RSFEC_STATUS);
    //printf("RS-FEC Status  (0x1004): 0x%04X %s\n", rsfec & 0xFFFF, (rsfec & 0x4000) ? "(LOCKED)" : "(UNLOCKED)");

    // 9. KIỂM TRA CHẤT LƯỢNG TÍN HIỆU (Thống kê FEC)
    Xil_Out32(ADDR_CMAC_REG + STAT_TICK_REG, 0x00000001); // Chốt số liệu
    //uint32_t corrected = Xil_In32(ADDR_CMAC_REG + STAT_FEC_CORRECTED);
    //uint32_t uncorrected = Xil_In32(ADDR_CMAC_REG + STAT_FEC_UNCORRECTED);
    //printf("\n--- SIGNAL QUALITY ---\n");
    //printf(" |- FEC Corrected Blocks  : %u\n", corrected);
    //printf(" |- FEC Uncorrected Blocks: %u %s\n", uncorrected, (uncorrected > 0) ? "(CRITICAL: CHANGE CABLE!)" : "(CLEAN)");

    // Lời khuyên cuối dựa trên trạng thái
    if (reg & 0x20) {
        printf("\n[!] HUONG DAN: Remote Fault = YES. FPGA da thay PC nhung PC khong thay FPGA.\n");
        printf("    -> Kiem tra ngay: Cap MPO phai la Type B. Kiem tra chieu TX cua FPGA.\n");
    }
    // if (!(rsfec & 0x4000) && (lock & 0xFFFFF) == 0xFFFFF) {
    //     printf("\n[!] HUONG DAN: Block Lock OK nhung FEC Unlocked. PC dang tat RS-FEC. Hay bat FEC tren PC.\n");
    // }

    printf("==================================================\n");
}

void test_internal_loopback_with_fec() {
    printf("\n--- KIEM TRA NOI BO (INTERNAL LOOPBACK + FEC) ---\n");
    
    // Kiểm tra trạng thái Reset hiện tại
    uint32_t gt_reset = Xil_In32(ADDR_CMAC_REG + 0x0000);
    printf(" |- GT Reset Register (0x0000): 0x%X\n", gt_reset);

    // 1. Bat Loopback noi bo
    Xil_Out32(ADDR_CMAC_REG + 0x0090, 0x00000001);
    usleep(1000000);
    
    // 2. Doc trang thai de chẩn đoán PLL
    uint32_t tx_status = Xil_In32(ADDR_CMAC_REG + 0x0200);
    if (tx_status & 0x1) {
        printf(" [!] PLL LOCK ERROR: Transceiver khong the khoa xung nhip (TX Fault).\n");
        printf("     -> Vui long kiem tra SI570 xem da set 156.25MHz chua.\n");
    }

    uint32_t rx_stat = Xil_In32(ADDR_CMAC_REG + 0x0204);
    //uint32_t fec_stat = Xil_In32(ADDR_CMAC_REG + 0x1004);

    printf("Result Loopback:\n");
    printf(" |- RX Main Status (0x204): 0x%X\n", rx_stat);
    //printf(" |- RS-FEC Status  (0x1004): 0x%X\n", fec_stat);

    if (rx_stat == 0x3) {
        printf("\n=> KET LUAN: LOGIC FPGA CHUAN 100%%.\n");
    }
}

void final_check() {
    //uint32_t fec_status = Xil_In32(ADDR_CMAC_REG + 0x1004);
    uint32_t rx_status  = Xil_In32(ADDR_CMAC_REG + 0x0204);
    
    printf("\n[DIAG] RX Main: 0x%X, FEC Status: 0x%X\n", rx_status);//, fec_status);
    
    if (rx_status & 0x10) {
        //printf("=> KET LUAN: FPGA thay tin hieu dien nhung RS-FEC khong the giai ma.\n");
        //printf("   1. Hay kiem tra xem Card Broadcom da set FEC sang CL91 (RS-FEC) chua?\n");
        printf("   2. Hay kiem tra lai xem Cap MPO co phai Type B khong?\n");
    }
}

void check_signal_stability() {
    Xil_Out32(ADDR_CMAC_REG + 0x02B0, 0x00000001); // Tick
    //uint32_t corrected = Xil_In32(ADDR_CMAC_REG + 0x1008);
    //printf("[MONITOR] FEC Corrected: %u\n", corrected);
    
    //if (corrected > 10000) {
    //    printf(" -> NGUYEN NHAN: Tin hieu quang qua nhiễu. Hay lau sach dau quang!\n");
    //}
}

void final_link_up_force() {
    printf("\n[FINAL] Mo van du lieu: Chuyen tu phat IDLE sang nhan DMA...\n");

    // 1. Đảm bảo Loopback đã tắt
    Xil_Out32(ADDR_CMAC_REG + 0x0090, 0x00000000); 

    // 2. QUAN TRỌNG NHẤT: Ghi 0x01 (Tắt bit 5 - Send Idle)
    // Bit 0 = 1 (Enable TX), Bit 4 = 0 (No RFI), Bit 5 = 0 (No Force Idle)
    Xil_Out32(ADDR_CMAC_REG + 0x000C, 0x00000001); 

    // 3. Reset nhẹ tầng PCS để xóa các trạng thái treo cũ trong FIFO
    Xil_Out32(ADDR_CMAC_REG + 0x0004, 0xC0000000); 
    usleep(10000);
    Xil_Out32(ADDR_CMAC_REG + 0x0004, 0x00000000);

    printf("[FINAL] Van du lieu da mo. TREADY se len 1 khi DMA bat dau truyen.\n");
}

void force_tx_and_check_pc(void* arg) {
    uint32_t reg_status;
    uint32_t dummy_read;
    uint32_t packet_count;

    printf("smart_init_cmac...\n");
    smart_init_cmac();
    usleep(100); // Nghỉ một chút để IP cập nhật trạng thái thực tế

    printf("[THREAD] Bat dau giam sat CMAC & Preamble (Chu ky 5s)...\n");

    while(1) 
    {
        if (trigger_idle == 1)
        {
            trigger_idle = 0;
            
            Xil_Out32(ADDR_CMAC_REG + STAT_TICK_REG, 0x00000001); // Chốt số liệu
            usleep(400);

            // --- BƯỚC QUAN TRỌNG: ĐỌC 2 LẦN ĐỂ XÓA BIT CHỐT (STICKY BITS) ---
            // Lần đọc 1: Xóa các giá trị "Aligned" hoặc "Lock" cũ đã lưu trong bộ nhớ đệm của IP
            dummy_read = Xil_In32(ADDR_CMAC_REG + 0x0204);
            usleep(100); // Nghỉ một chút để IP cập nhật trạng thái thực tế
            
            // Lần đọc 2: Lấy giá trị THỰC TẾ tại thời điểm hiện tại
            reg_status = Xil_In32(ADDR_CMAC_REG + 0x0204);
            // --------------------------------------------------------------

            printf("\n========== CMAC REAL-TIME DIAGNOSTIC ==========\n");

            // 1. KIỂM TRA TRẠNG THÁI LINK (Bit 0 và Bit 1)
            // 0x3 nghĩa là: stat_rx_status=1 VÀ stat_rx_aligned=1
            if ((reg_status & 0x03) == 0x03) {
                printf(">> LINK STATUS: [ UP ] (0x3 - Aligned & Stable)\n");
            } else {
                printf(">> LINK STATUS: [ DOWN! ] (Status: 0x%X)\n", reg_status);
                
                // Phân tích lý do sập link (Dựa trên PG203 trang 197)
                if (reg_status & (1 << 7))  printf(" |- Nguyen nhan: LOCAL FAULT (Co the do RUT CAP)\n");
                if (reg_status & (1 << 5))  printf(" |- Nguyen nhan: REMOTE FAULT (Doi tac bao loi)\n");
                if (reg_status & (1 << 4))  printf(" |- Nguyen nhan: HIGH BER (Loi bit qua cao/Tin hieu xau)\n");
                if (!(reg_status & 0x02))   printf(" |- Nguyen nhan: LOSS OF ALIGNMENT\n");
            }

            // 2. KIỂM TRA LỖI BIT TRONG PATTERN (Bits 11:9)
            uint32_t mismatch = (reg_status >> 9) & 0x07;
            if (mismatch > 0) {
                printf("[ALERT] Pattern Mismatch: Phat hien %u loi bit!\n", mismatch);
            } else {
                printf("[ OK ] Pattern: Clean (Khong co loi bit)\n");
            }

            // 3. KIỂM TRA LỖI GIAO THỨC (Preamble & SFD)
            // Chỉ báo "Passed" nếu không có bit lỗi nào được chốt lại ở lần đọc thứ 2
            if (reg_status & (1 << 12)) {
                printf("[!] ERROR: Bad Preamble detected!\n");
            } else {
                printf("[ OK ] Preamble Check: Passed\n");
            }
            
            if (reg_status & (1 << 13)) {
                printf("[!] ERROR: Bad SFD detected!\n");
            } else {
                printf("[ OK ] SFD Check: Passed\n");
            }

            // 4. KIỂM TRA MODE LOOPBACK (Thanh ghi 0x90)
            uint32_t loopback_mode = Xil_In32(ADDR_CMAC_REG + 0x0090);
            printf(" |- Connection Mode: %s\n", (loopback_mode == 1) ? "INTERNAL LOOPBACK" : "EXTERNAL (SFP)");

            // 5. THỐNG KÊ GÓI TIN (Cần TICK để chốt số liệu)
            Xil_Out32(ADDR_CMAC_REG + 0x02B0, 0x00000001);
            packet_count = Xil_In32(ADDR_CMAC_REG + 0x0608); 
            printf("[DATA] Total Packets Received: %u\n", packet_count);
            
            printf("===============================================\n");
        }     
        usleep(10000); 
    }
}

void run_cmac_test_pattern(int enable) {
    uint32_t reg;

    if (enable) {
        printf("[TEST] Bat che do phat Pattern lien tuc (Scrambled Idle)...\n");
        
        // 1. Bật bộ phát Pattern (TX)
        reg = Xil_In32(ADDR_CMAC_REG + 0x000C);
        reg |= (1 << 16); // Set ctl_tx_test_pattern
        Xil_Out32(ADDR_CMAC_REG + 0x000C, reg);

        // 2. Bật bộ kiểm tra Pattern (RX)
        reg = Xil_In32(ADDR_CMAC_REG + 0x0014);
        reg |= (1 << 8); // Set ctl_rx_test_pattern
        Xil_Out32(ADDR_CMAC_REG + 0x0014, reg);
  
        printf("[TEST] Dang phat... Hay kiem tra loi tren PC (neu Card mang ho tro).\n");
    } else {
        // Tắt chế độ test
        Xil_Out32(ADDR_CMAC_REG + 0x000C, Xil_In32(ADDR_CMAC_REG + 0x000C) & ~(1 << 16));
        Xil_Out32(ADDR_CMAC_REG + 0x0014, Xil_In32(ADDR_CMAC_REG + 0x0014) & ~(1 << 8));
        printf("[TEST] Da tat che do Test Pattern.\n");
    }
}

void check_pattern_errors() {
    // Đọc lỗi từ thanh ghi trạng thái (0x0204)
    uint32_t reg = Xil_In32(ADDR_CMAC_REG + 0x0204);
    uint32_t mismatch = (reg >> 9) & 0x07; // Lay 3 bit 11:9
    
    if (mismatch > 0) {
        printf("[ALERT] Phat hien %u loi mismatch trong Pattern!\n", mismatch);
    } else {
        printf("[OK] Pattern clean. Khong co loi bit.\n");
    }
}

void setup_advanced_cmac_features() {
    uint32_t reg;

    printf("\n--- CAU HINH TINH NANG NANG CAO (Preamble/SFD/LFI) ---\n");

    // 1 & 2 & 3. Bật Check Preamble, SFD và Process LFI (Thanh ghi 0x14)
    // Đọc giá trị hiện tại để không làm mất các cấu hình khác (như ctl_rx_enable)
    reg = Xil_In32(ADDR_CMAC_REG + REG_RX_CONFIG);
    reg |= (1 << 2); // ctl_rx_check_preamble
    reg |= (1 << 3); // ctl_rx_check_sfd
    reg |= (1 << 7); // ctl_rx_process_lfi
    Xil_Out32(ADDR_CMAC_REG + REG_RX_CONFIG, reg);
    printf("[OK] Enabled: Check Preamble, Check SFD, Process LFI.\n");

    // 4. Kích hoạt Custom Preamble Mode (Thanh ghi 0x159)
    // Phải bật cái này thì CMAC mới lấy data từ port vật lý tx_preamblein
    //Xil_Out32(ADDR_CMAC_REG + REG_TX_CUSTOM_PREAMBLE, 0x00000001);
    //printf("[OK] Enabled: Custom Preamble Mode.\n");
}

/**
 * Kiểm tra Preamble qua GPIO
 * Preamble chuẩn Ethernet là: 55 55 55 55 55 55 55 (7 bytes)
 */
void test_custom_preamble() {
    // 56 bit Preamble = 7 bytes. 
    // Vì GPIO thường là 32-bit, bạn có thể cần 2 channel GPIO hoặc ghi 64-bit
    uint64_t my_preamble = 0x55555555555555ULL; 

    printf("\n--- KIEM TRA CUSTOM PREAMBLE ---\n");
    
    // Ghi Preamble vào chân tx_preamblein qua GPIO
    // (Giả sử GPIO của bạn hỗ trợ ghi 64-bit hoặc chia 2 lần 32-bit)
    XGpio_DiscreteWrite(&GPIO_TX_PREAMBLEIN, 1, (uint32_t)(my_preamble & 0xFFFFFFFF));
    XGpio_DiscreteWrite(&GPIO_TX_PREAMBLEIN, 2, (uint32_t)(my_preamble >> 32));
    
    printf("[TX] Da ghi Preamble vao GPIO: 0x%llX\n", my_preamble);

    // Đọc Preamble nhận được từ rx_preambleout qua GPIO
    uint32_t rx_p_low = XGpio_DiscreteRead(&GPIO_RX_PREAMBLEOUT, 1);
    uint32_t rx_p_high = XGpio_DiscreteRead(&GPIO_RX_PREAMBLEOUT, 2);
    uint64_t rx_preamble = ((uint64_t)rx_p_high << 32) | rx_p_low;

    printf("[RX] Preamble nhan duoc tu doi tac: 0x%llX\n", rx_preamble);
}

void check_rx_protocol_errors() {
    uint32_t reg = Xil_In32(ADDR_CMAC_REG + 0x0204);
    
    if (reg & (1 << 12)) printf("[ALERT] Phat hien loi BAD PREAMBLE!\n");
    if (reg & (1 << 13)) printf("[ALERT] Phat hien loi BAD SFD!\n");
}