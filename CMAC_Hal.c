#include "CMAC_Hal.h"
#include "main.h"

extern struct CMAC_Struct  			CMAC_Struct1;

// Các chế độ Loopback (Áp dụng cho cả 4 Lanes cùng lúc)
// Cấu trúc nhị phân: [Lane3_3bit] [Lane2_3bit] [Lane1_3bit] [Lane0_3bit]

#define LB_MODE_NORMAL      0x000 // 000 000 000 000 -> Chạy bình thường (External)
#define LB_MODE_NE_PCS      0x249 // 001 001 001 001 -> Near-End PCS (Loopback số)
#define LB_MODE_NE_PMA      0x492 // 010 010 010 010 -> Near-End PMA (Loopback Analog - QUAN TRỌNG NHẤT)
#define LB_MODE_FE_PMA      0x924 // 100 100 100 100 -> Far-End PMA
#define LB_MODE_FE_PCS      0xDB6 // 110 110 110 110 -> Far-End PCS

void CMAC_Innit()
{
    //CMAC_Struct1
    // ==========================================================
    // 1. THÔNG TIN CẤU HÌNH CƠ BẢN
    // ==========================================================
    CMAC_Struct1.base_address           = ADDR_CMAC_REG; ///< Địa chỉ cơ sở của CMAC IP Core (ADDR_CMAC_REG)
    CMAC_Struct1.core_version_major     = 0;  ///< Phiên bản phần cứng (Từ 0x0008)
    CMAC_Struct1.core_version_minor     = 0;

    // ==========================================================
    // 2. CẤU HÌNH HIỆN TẠI (Được ghi vào thanh ghi)
    // ==========================================================
    CMAC_Struct1.gt_reset_reg           = 0;   ///< Thanh ghi 0x0000 (GT Reset)
    CMAC_Struct1.tx_config_reg          = 0;   ///< Thanh ghi 0x000C (ctl_tx_enable, ctl_tx_send_idle)
    CMAC_Struct1.rx_config_reg          = 0;   ///< Thanh ghi 0x0014 (ctl_rx_enable, check_preamble/sfd)
    CMAC_Struct1.loopback_mode          = 0;   ///< Thanh ghi 0x0090 (0: External, 1: Internal Loopback)
    CMAC_Struct1.rsfec_config_reg       = 0;   ///< Thanh ghi 0x1000 (FEC Enable/Config)
    
    // ==========================================================
    // 3. TRẠNG THÁI HIỆN TẠI (Đọc từ thanh ghi)
    // ==========================================================
    CMAC_Struct1.rx_main_status         = 0;   ///< Thanh ghi 0x0204 (Quan trọng nhất: Aligned, Faults)
    CMAC_Struct1.tx_main_status         = 0;   ///< Thanh ghi 0x0200 (TX Fault/PLL Lock)
    CMAC_Struct1.rsfec_status           = 0;   ///< Thanh ghi 0x1004 (FEC Lock)
    CMAC_Struct1.pcs_block_lock         = 0;   ///< Thanh ghi 0x020C (Tình trạng khóa block của 20 lane)
    CMAC_Struct1.pcs_lane_sync          = 0;   ///< Thanh ghi 0x0210 (Tình trạng đồng bộ lane)

    // ==========================================================
    // 4. TRẠNG THÁI CHẨN ĐOÁN (Phân tích từ thanh ghi)
    // ==========================================================
    CMAC_Struct1.link_is_up             = 0;   ///< (rx_main_status & 0x3) == 0x3
    CMAC_Struct1.pll_is_locked          = 0;   ///< !(tx_main_status & 0x01)
    CMAC_Struct1.fec_is_locked          = 0;   ///< (rsfec_status & 0x4000)
    
    CMAC_Struct1.remote_fault           = 0;   ///< Lỗi do đối tác báo (0x0204 Bit 5)
    CMAC_Struct1.high_ber_flag          = 0;   ///< Tín hiệu xấu (0x0204 Bit 4)
    
    CMAC_Struct1.pattern_mismatch_count = 0;   ///< Số lỗi bit Pattern (0x0204 Bits 11:9)
    CMAC_Struct1.bad_preamble           = 0;   ///< Lỗi Preamble (0x0204 Bit 12)
    CMAC_Struct1.bad_sfd                = 0;   ///< Lỗi SFD (0x0204 Bit 13)

    // ==========================================================
    // 5. THỐNG KÊ (Đọc sau khi TICK: 0x02B0/0x02B4)
    // ==========================================================
    CMAC_Struct1.total_packets_rx       = 0;    ///< Thanh ghi 0x0608 (Tổng số gói nhận được)
    CMAC_Struct1.fec_corrected_blocks   = 0;    ///< Thanh ghi 0x1008
    CMAC_Struct1.fec_uncorrected_blocks = 0;    ///< Thanh ghi 0x100C

    // ==========================================================
    // 6. DỮ LIỆU LỊCH SỬ VÀ ĐIỀU KHIỂN THỜI GIAN THỰC (Tham khảo AFC_Struct)
    // ==========================================================
    CMAC_Struct1.rx_status_history[0] = 0; ///< Lưu trữ 10 trạng thái 0x0204 gần nhất
    CMAC_Struct1.rx_status_history[1] = 0; ///< Lưu trữ 10 trạng thái 0x0204 gần nhất
    CMAC_Struct1.rx_status_history[2] = 0; ///< Lưu trữ 10 trạng thái 0x0204 gần nhất
    CMAC_Struct1.rx_status_history[3] = 0; ///< Lưu trữ 10 trạng thái 0x0204 gần nhất
    CMAC_Struct1.rx_status_history[4] = 0; ///< Lưu trữ 10 trạng thái 0x0204 gần nhất
    CMAC_Struct1.rx_status_history[5] = 0; ///< Lưu trữ 10 trạng thái 0x0204 gần nhất
    CMAC_Struct1.rx_status_history[6] = 0; ///< Lưu trữ 10 trạng thái 0x0204 gần nhất
    CMAC_Struct1.rx_status_history[7] = 0; ///< Lưu trữ 10 trạng thái 0x0204 gần nhất
    CMAC_Struct1.rx_status_history[8] = 0; ///< Lưu trữ 10 trạng thái 0x0204 gần nhất
    CMAC_Struct1.rx_status_history[9] = 0; ///< Lưu trữ 10 trạng thái 0x0204 gần nhất

    CMAC_Struct1.history_index          = 0;    ///< Index hiện tại cho mảng history
    
    CMAC_Struct1.trigger_diag           = 0;    ///< Tương đương với 'trigger_idle', yêu cầu chẩn đoán
    CMAC_Struct1.is_fully_initialized   = 0;    ///< Flag báo đã hoàn tất quá trình smart_init_cmac
    CMAC_Struct1.init_timeout_counter   = 0;    ///< Bộ đếm cho các bước chờ (PLL Lock, Alignment)

    CMAC_Struct1.stat_tx_local_fault    = 0;    ///
    CMAC_Struct1.stat_rx_aligned        = 0;    ///
    CMAC_Struct1.interal_rx_local_fault = 0;    ///
    CMAC_Struct1.usr_rx_reset           = 0;    ///
    CMAC_Struct1.usr_tx_reset           = 0;    ///

}

void CMAC_Innit_by_HW()
{
    uint32_t Err_counter = 0;   
    uint32_t timeout_max = 50000; // 50 giây chờ (50000 * 1ms/vòng)
    uint32_t reg_mode, reg_loopback;

    printf("\n==================================================\n");
    printf(" CMAC Innit - Start Bring-up Sequence\n");
    printf("==================================================\n");

    dump_and_analyze_qsfp_full_registers();

    reset_qsfp_module();

    CMAC_SFP_Power_Up_And_Align();

    read_and_display_optical_power();

    // BƯỚC 1: CẤU HÌNH BAN ĐẦU (Thep Datasheet PG203 - Step 1)
    
    // Ghi 0x00014: Bật RX Enable (Bit 0 = 1)
    Xil_Out32(ADDR_CMAC_REG + 0x00014, 0x00000001); 
    
    // Ghi 0x0000C: TẮT ctl_tx_enable (Bit 0 = 0) VÀ TẮT ctl_tx_send_rfi (Bit 4 = 0)
    Xil_Out32(ADDR_CMAC_REG + 0x0000C, 0x00000000); 

    // Ghi 0x0090: Đặt chế độ Loopback (Cần phải đặt giá trị mong muốn ở đây)
    // Ví dụ: Đặt Internal Loopback (0x1) cho mục đích debug
    Xil_Out32(ADDR_CMAC_REG + 0x0090, 0x00000001);
    
    // =========================================================================
    // LOGGING CẤU HÌNH IP (ĐỂ KIỂM TRA ĐÃ GHI ĐÚNG CHƯA)
    // =========================================================================
    
    // Đọc Mode (0x0020)
    reg_mode = Xil_In32(ADDR_CMAC_REG + 0x0020);
    printf("[CMAC CONFIG] CORE_MODE_REG (0x0020) = 0x%08x\n", reg_mode);
    
    // Đọc Loopback (0x0090)
    reg_loopback = Xil_In32(ADDR_CMAC_REG + 0x0090);
    printf("[CMAC CONFIG] GT_LOOPBACK_REG (0x0090) = 0x%08x\n", reg_loopback);
    
    // Đọc TX_REG1 (0x000C) để xác nhận TX/RFI đã tắt (bits 0, 4 = 0)
    printf("[CMAC CONFIG] CONFIG_TX_REG1 (0x000C) = 0x%08x\n", Xil_In32(ADDR_CMAC_REG + 0x0000C));
    
    printf("[CMAC] Waiting for RX Alignment (stat_rx_aligned = 1)...\n");
    
    // BƯỚC 2: ĐỢI RX_ALIGNED VÀ KÍCH HOẠT GT RX CDR (Toggle Reset)
    while(Err_counter < timeout_max) 
    {
        // 2A: THỰC HIỆN KỸ THUẬT TỈNH DẬY CDR (CDR STUCK FIX) - Cứ 1s 1 lần
        if(Err_counter % 1000 == 0) {
            if (Err_counter > 0) printf("[CMAC] Performing RX Datapath Toggle Reset...\n");
            CMAC_Reset_rx_datapath(); 
            usleep(100); // Chờ 100us sau mỗi lần reset
        }

        // 2B: KIỂM TRA TRẠNG THÁI THỰC TẾ
        Check_CMAC_Pin(); 
        
        if(CMAC_Struct1.stat_rx_aligned == 1) 
        {
            printf("[CMAC] : SUCCESS! Link ALIGNED after %ums.\n", Err_counter);
            break; // THÀNH CÔNG: Thoát khỏi vòng lặp chờ
        }
        
        // 2C: LOGGING (Ghi log 5s/lần)
        if(Err_counter % 5000 == 0)
        {
            check_qsfp_status();

            printf("[CMAC Status] : stat_rx_aligned=%d, stat_tx_local_fault=%d, rx_local_fault=%d\n", 
                CMAC_Struct1.stat_rx_aligned,
                CMAC_Struct1.stat_tx_local_fault,
                CMAC_Struct1.interal_rx_local_fault);

            Xil_Out32(ADDR_CMAC_REG + 0x02B0, 0x00000001); // Kích pm_tick
            usleep(100);
            printf("[CMAC Status Reg] : 0x0204 = 0x%08x (LATCHED VAL)\n", Xil_In32(ADDR_CMAC_REG + 0x0204));
        }

        Err_counter++;
        usleep(1000); // Đợi 1ms cho mỗi vòng lặp
    }
    
    // BƯỚC 3: KÍCH HOẠT TX ENABLE (Sau khi RX đã ALIGN - Datasheet Step 2)
    if(CMAC_Struct1.stat_rx_aligned == 1) {
        printf("[CMAC] Finalizing: Enabling TX...\n");
        // Ghi 0x0000C: Bật ctl_tx_enable (Bit 0 = 1) và TẮT ctl_tx_send_rfi (Bit 4 = 0)
        Xil_Out32(ADDR_CMAC_REG + 0x0000C, 0x00000001);
        printf("[CMAC] Initialization Complete. Link is Up and Data path is Open.\n");
    } else {
        printf("[CMAC] FAILURE: Initialization Timeout. Link is Down.\n");
    }
}

// void CMAC_Innit_by_HW()
// {
//     uint32_t Err_counter = 0;   
//     uint32_t timeout_max = 50000; // 50 giây chờ (50000 * 1ms/vòng)

//     printf("\n==================================================\n");
//     printf(" CMAC Innit - Start Bring-up Sequence\n");
//     printf("==================================================\n");

//     // BƯỚC 1: CẤU HÌNH BAN ĐẦU (Thep Datasheet PG203 - Step 1)
//     // - Bật RX Enable: ctl_rx_enable = 1 (Bit 0)
//     // - Tắt TX Enable, Tắt TX Send RFI/LFI (Đảm bảo Core không gửi lỗi ra ngoài)
    
//     // Ghi 0x00014: Bật RX Enable (Bit 0 = 1)
//     Xil_Out32(ADDR_CMAC_REG + 0x00014, 0x00000001); 
    
//     // Ghi 0x0000C: TẮT ctl_tx_enable (Bit 0 = 0) VÀ TẮT ctl_tx_send_rfi (Bit 4 = 0)
//     // *Lưu ý: Mức 0x10 trong Datasheet là để gửi RFI, ta không dùng khi Loopback*
//     Xil_Out32(ADDR_CMAC_REG + 0x0000C, 0x00000000); 

//     // BƯỚC 2: ĐỢI RX_ALIGNED VÀ KÍCH HOẠT GT RX CDR (Toggle Reset)
//     printf("[CMAC] Waiting for RX Alignment (stat_rx_aligned = 1)...\n");
    
//     while(Err_counter < timeout_max) 
//     {
//         // 2A: THỰC HIỆN KỸ THUẬT TỈNH DẬY CDR (CDR STUCK FIX)
//         // Cứ 1000ms (1s) thì tạo 1 xung Reset GT RX Datapath
//         if(Err_counter % 1000 == 0) {
//             printf("[CMAC] Performing RX Datapath Toggle Reset...\n");
//             CMAC_Reset_rx_datapath(); 
//             usleep(100); // Chờ 100us sau mỗi lần reset để mạch Analog ổn định
//         }

//         // 2B: KIỂM TRA TRẠNG THÁI THỰC TẾ
//         Check_CMAC_Pin(); 
        
//         if(CMAC_Struct1.stat_rx_aligned == 1) 
//         {
//             printf("[CMAC] : SUCCESS! Link ALIGNED after %ums.\n", Err_counter);
//             break; // THÀNH CÔNG: Thoát khỏi vòng lặp chờ
//         }
        
//         // 2C: LOGGING (Ghi log 5s/lần)
//         if(Err_counter % 5000 == 0)
//         {
//             printf("[CMAC Status] : stat_rx_aligned=%d, stat_tx_local_fault=%d, rx_local_fault=%d\n", 
//                 CMAC_Struct1.stat_rx_aligned,
//                 CMAC_Struct1.stat_tx_local_fault,
//                 CMAC_Struct1.interal_rx_local_fault);

//             // Cập nhật thanh ghi AXI Status (Để xóa Latch cũ và lấy giá trị mới)
//             Xil_Out32(ADDR_CMAC_REG + 0x02B0, 0x00000001); // Kích pm_tick
//             usleep(100);
//             printf("[CMAC Status Reg] : 0x0204 = 0x%08x\n", Xil_In32(ADDR_CMAC_REG + 0x0204));
//         }

//         Err_counter++;
//         usleep(1000); // Đợi 1ms cho mỗi vòng lặp
//     }
    
//     // BƯỚC 3: KÍCH HOẠT TX ENABLE (Sau khi RX đã ALIGN - Datasheet Step 2)
//     if(CMAC_Struct1.stat_rx_aligned == 1) {
//         printf("[CMAC] Finalizing: Enabling TX...\n");
//         // Ghi 0x0000C: Bật ctl_tx_enable (Bit 0 = 1) và TẮT ctl_tx_send_rfi (Bit 4 = 0)
//         Xil_Out32(ADDR_CMAC_REG + 0x0000C, 0x00000001);
//         printf("[CMAC] Initialization Complete. Link is Up and Data path is Open.\n");
//     } else {
//         printf("[CMAC] FAILURE: Initialization Timeout. Link is Down.\n");
//     }

//     // BƯỚC 4: (Tùy chọn) Cấu hình Flow Control nếu cần thiết
//     // (Bỏ qua vì không có trong code mẫu của bạn, nhưng là bước 3 trong Datasheet)
// }

void CMAC_Innit_by_HW2()
{

int Err_counter = 0;   
int Err_second = 0;   

printf(" [CMAC] setup step 1\n");
// 1. Write the following registers:
Xil_Out32(ADDR_CMAC_REG + 0x00014, 0x00000001); // 0x00014 : 32'h00000001 [CONFIGURATION_RX_REG1 for ctl_rx_enable]
Xil_Out32(ADDR_CMAC_REG + 0x0000C, 0x00000010); // 0x0000C : 32'h00000010 [CONFIGURATION_TX_REG1 for ctl_tx_send_rfi]

// 2. Wait for RX_aligned then write the following registers:

while(1) // 0x0000C : 32'h00000001 [CONFIGURATION_TX_REG1 for ctl_tx_enable to 1'b1
{
    Check_CMAC_Pin(); // doc trang thai cac pin tu GPIO
    
    //if(CMAC_Struct1.stat_tx_local_fault  == 0) break; //  = (input & 0x00000001) ? 1 : 0;    ///
    
    if(CMAC_Struct1.stat_rx_aligned == 1) 
    {
        printf("[CMAC] : stat_rx_aligned = 1, start setup CMAC!\n");
        break;
    }
    else
    {

        if(Err_counter % 1000 == 0) CMAC_Reset_rx_datapath();

        if(Err_counter % 5000 == 0)
        {
            printf("[CMAC] : stat_rx_aligned = %d stat_tx_local_fault = % d usr_rx_reset = %d usr_tx_reset = %d rx_local_fault = %d\n", 
                CMAC_Struct1.stat_rx_aligned,
                CMAC_Struct1.stat_tx_local_fault,
                CMAC_Struct1.usr_rx_reset,
                CMAC_Struct1.usr_tx_reset,
                CMAC_Struct1.interal_rx_local_fault);

            // --- BƯỚC QUAN TRỌNG: ĐỌC 2 LẦN ĐỂ XÓA BIT CHỐT (STICKY BITS) ---
            // Lần đọc 1: Xóa các giá trị "Aligned" hoặc "Lock" cũ đã lưu trong bộ nhớ đệm của IP
           //dummy_read = Xil_In32(ADDR_CMAC_REG + 0x0204);
            //usleep(100); // Nghỉ một chút để IP cập nhật trạng thái thực tế
            
            // Lần đọc 2: Lấy giá trị THỰC TẾ tại thời điểm hiện tại
            int reg_status1 = Xil_In32(ADDR_CMAC_REG + 0x0204);
            // --------------------------------------------------------------
        }
    }

    Err_counter++;
    if(Err_counter > 45000) break;
    usleep(1000);

}

printf(" [CMAC] setup step 2\n");
Xil_Out32(ADDR_CMAC_REG + 0x0000C, 0x00000001); // 0x0000C : 32'h00000001 [CONFIGURATION_TX_REG1 for ctl_tx_enable to 1'b1 and ctl_tx_send_rfi to 1'b0]
// 3. When TX/RX flow control is enabled in the Vivado IDE (the default setting), write the
// following registers.
// Note: If you have disabled TX/RX flow control in the Vivado IDE, skip to step 4.
printf(" [CMAC] setup step 3\n");
Xil_Out32(ADDR_CMAC_REG + 0x0084, 0x00003DFF); //// 0x0084 : 32'h00003DFF [CONFIGURATION_RX_FLOW_CONTROL_CONTROL_REG1]
Xil_Out32(ADDR_CMAC_REG + 0x0088, 0x0001C631); //// 0x0088 : 32'h0001C631 [CONFIGURATION_RX_FLOW_CONTROL_CONTROL_REG2]
Xil_Out32(ADDR_CMAC_REG + 0x0048, 0xFFFFFFFF); //// 0x0048 : 32'hFFFFFFFF [CONFIGURATION_TX_FLOW_CONTROL_QUANTA_REG1]
Xil_Out32(ADDR_CMAC_REG + 0x0050, 0xFFFFFFFF); //// 0x004C : 32'hFFFFFFFF [CONFIGURATION_TX_FLOW_CONTROL_QUANTA_REG2]
Xil_Out32(ADDR_CMAC_REG + 0x0050, 0xFFFFFFFF); //// 0x0050 : 32'hFFFFFFFF [CONFIGURATION_TX_FLOW_CONTROL_QUANTA_REG3]
Xil_Out32(ADDR_CMAC_REG + 0x0054, 0xFFFFFFFF); //// 0x0054 : 32'hFFFFFFFF [CONFIGURATION_TX_FLOW_CONTROL_QUANTA_REG4]
Xil_Out32(ADDR_CMAC_REG + 0x0058, 0x0000FFFF); //// 0x0058 : 32'h0000FFFF [CONFIGURATION_TX_FLOW_CONTROL_QUANTA_REG5]
Xil_Out32(ADDR_CMAC_REG + 0x0034, 0x0000FFFF); //// 0x0034 : 32'h0000FFFF [CONFIGURATION_TX_FLOW_CONTROL_REFRESH_REG1]

printf(" [CMAC] setup step 4\n");
Xil_Out32(ADDR_CMAC_REG + 0x0038, 0xFFFFFFFF); //// 0x0038 : 32'hFFFFFFFF [CONFIGURATION_TX_FLOW_CONTROL_REFRESH_REG2]
Xil_Out32(ADDR_CMAC_REG + 0x003C, 0xFFFFFFFF); //// 0x003C : 32'hFFFFFFFF [CONFIGURATION_TX_FLOW_CONTROL_REFRESH_REG3]
Xil_Out32(ADDR_CMAC_REG + 0x0040, 0xFFFFFFFF); //// 0x0040 : 32'hFFFFFFFF [CONFIGURATION_TX_FLOW_CONTROL_REFRESH_REG4]
Xil_Out32(ADDR_CMAC_REG + 0x0044, 0x0000FFFF); //// 0x0044 : 32'h0000FFFF [CONFIGURATION_TX_FLOW_CONTROL_REFRESH_REG5]
Xil_Out32(ADDR_CMAC_REG + 0x0030, 0x000001FF); //// 0x0030 : 32'h000001FF [CONFIGURATION_TX_FLOW_CONTROL_CONTROL_REG1]


}

void smart_init_cmac() {

    int cnt_idle = 0;

    Xil_Out32(ADDR_CMAC_REG + STAT_TICK_REG, 0x00000001); // Chốt số liệu
    usleep(400);

    // 1. Kiểm tra xem Link đã UP chưa
    uint32_t status = Xil_In32(ADDR_CMAC_REG + 0x0204);
    //uint32_t fec_status = Xil_In32(ADDR_CMAC_REG + 0x1004);
    uint32_t loopback_mode = Xil_In32(ADDR_CMAC_REG + 0x0090); // Đọc thanh ghi 0x90

    usleep(400);
    Xil_Out32(ADDR_CMAC_REG + REG_GT_LOOPBACK, 0x00000000); // internal loopback
    usleep(400);
    Xil_Out32(ADDR_CMAC_REG + REG_GT_LOOPBACK, 0x00000000); // internal loopback
    usleep(400);

    CMAC_Innit_by_HW();

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

    // --- BƯỚC 2: ĐIỀU KIỆN LINK ĐÃ SẴN SÀNG ---
    // Link phải 0x3, FEC phải Locked (0x4000) và KHÔNG được ở mode Loopback (0x0)
    if ((status == 0x3) && (loopback_mode == 0)) {
        printf("[INFO] CMAC Link dang hoat dong on dinh (0x3). Duy tri ket noi...\n");
        
        // --- BƯỚC MỞ KHÓA QUAN TRỌNG NHẤT ---
        // Ghi 0x1 vào 0x000C để TẮT bit phát IDLE nội bộ (Bit 5 = 0)
        // và BẬT TX Enable (Bit 0 = 1).
        Xil_Out32(ADDR_CMAC_REG + 0x000C, 0x00000001); 
        
        printf("[OK] Da mo van du lieu (TREADY se san sang cho DMA).\n");
        
        // 1. Đảm bảo Loopback đã tắt
        Xil_Out32(ADDR_CMAC_REG + 0x0090, 0x00000000); 

        //setup_advanced_cmac_features();
        //check_signal_stability();
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
    
    //setup_advanced_cmac_features();
    //check_signal_stability();

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
    //Xil_Out32(ADDR_CMAC_REG + 0x0090, 0x00000001);
    //usleep(1000000);
    
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
    uint32_t reg_status_latched; // Giá trị LATCHED (0x0204)
    uint32_t reg_status_cleared; // Giá trị LATCHED sau khi Clear-on-Read
    uint32_t packet_count;

    printf("smart_init_cmac...\n");
    // Giả định hàm này (smart_init_cmac) đã gọi CMAC_Innit_by_HW() và hoàn tất init core.
    smart_init_cmac(); 
    usleep(1000); // Đợi 1ms sau Init để các tín hiệu ổn định

    printf("[THREAD] Bat dau giam sat CMAC & Preamble (Chu ky 5s)...\n");

    while(1) 
    {
        if (trigger_idle == 1)
        {
            trigger_idle = 0;
            
            // ===================================================================
            // QUY TRÌNH ĐỌC TRẠNG THÁI LATCHED (STICKY BITS)
            // CẦN 2 BƯỚC ĐỌC VÀ 1 BƯỚC KÍCH TICK TRƯỚC VÀ SAU KHI ĐỌC
            // ===================================================================
            
            // 1. Kích hoạt PM_TICK để CHỐT số liệu (Chuyển trạng thái từ Accumulator -> Register)
            Xil_Out32(ADDR_CMAC_REG + STAT_TICK_REG, 0x00000001); 
            usleep(100); 

            // 2. Đọc LẦN 1: Đọc giá trị để XÓA các Latch/Sticky bits (Clear-on-Read)
            reg_status_latched = Xil_In32(ADDR_CMAC_REG + 0x0204);
            usleep(100); 
            
            // 3. Đọc LẦN 2: Đọc lại ngay để có giá trị THỰC TẾ (Non-Latched) tại thời điểm này
            // (Thường giá trị này phải về 0 nếu trạng thái không phải là LATCHED)
            reg_status_cleared = Xil_In32(ADDR_CMAC_REG + 0x0204);
            // => Ở đây chúng ta chỉ cần giá trị LATCHED để phân tích lỗi.

            //check_qsfp_status();
            periodic_qsfp_status_check();

            read_and_display_optical_power();

            // 4. Đọc GPIO để lấy Logic THẬT
            Check_CMAC_Pin();
            
            // 5. Thống kê gói tin (Lần 2)
            Xil_Out32(ADDR_CMAC_REG + STAT_TICK_REG, 0x00000001); // Cập nhật lại Packets
            usleep(100);
            packet_count = Xil_In32(ADDR_CMAC_REG + 0x0608); 

            printf("\n========== CMAC REAL-TIME DIAGNOSTIC ==========\n");

            // 1. CHẨN ĐOÁN DỰA TRÊN LOGIC THẬT (GPIO)
            // stat_rx_aligned = 0 là Link Down thật
            if (CMAC_Struct1.stat_rx_aligned == 1) {
                printf(">> LINK STATUS: [ UP ] (Aligned & Stable)\n");
            } else {
                printf(">> LINK STATUS: [ DOWN! ] (GPIO Aligned=0)\n");
                
                // Phân tích lý do (Dựa trên giá trị LATCHED để chẩn đoán lỗi trong chu kỳ vừa qua)
                printf(" |- Register Status (0x0204 Latch): 0x%X\n", reg_status_latched);
                if (reg_status_latched & (1 << 7))  printf(" |- Nguyen nhan: LOCAL FAULT (Co the do RUT CAP)\n");
                if (reg_status_latched & (1 << 5))  printf(" |- Nguyen nhan: REMOTE FAULT (Doi tac bao loi)\n");
                if (reg_status_latched & (1 << 4))  printf(" |- Nguyen nhan: HIGH BER (Loi bit qua cao/Tin hieu xau)\n");
                printf(" |- Nguyen nhan: LOSS OF ALIGNMENT\n");
            }

            // 2. PHÂN TÍCH LỖI STICKY (Bits 11:9 trong giá trị LATCHED)
            uint32_t mismatch = (reg_status_latched >> 9) & 0x07;
            if (mismatch > 0) {
                printf("[ALERT] Pattern Mismatch: Phat hien %u loi bit!\n", mismatch);
            } else {
                printf("[ OK ] Pattern: Clean (Khong co loi bit)\n");
            }

            // 3. KIỂM TRA LỖI GIAO THỨC (Preamble & SFD)
            // Lỗi này là STICKY/LATCHED, nằm trong thanh ghi 0x0204 (Bad Preamble/SFD)
            if (reg_status_latched & (1 << 12)) {
                printf("[!] ERROR: Bad Preamble detected! (Needs SW Reset/Clear)\n");
            } else {
                printf("[ OK ] Preamble Check: Passed\n");
            }
            
            if (reg_status_latched & (1 << 13)) {
                printf("[!] ERROR: Bad SFD detected! (Needs SW Reset/Clear)\n");
            } else {
                printf("[ OK ] SFD Check: Passed\n");
            }

            // 4. IN CẤU HÌNH VÀ STATUS LOGIC THẬT
            uint32_t loopback_mode = Xil_In32(ADDR_CMAC_REG + 0x0090);
            printf(" |- Connection Mode: %s\n", (loopback_mode == 1) ? "INTERNAL LOOPBACK" : "EXTERNAL (SFP)");
            printf("[DATA] Total Packets Received: %u\n", packet_count);
            
            // IN TRẠNG THÁI LOGIC THẬT TỪ GPIO
            printf("[CMAC LOGIC] : aligned=%d, tx_fault=%d, rx_local_fault=%d\n", 
                CMAC_Struct1.stat_rx_aligned,
                CMAC_Struct1.stat_tx_local_fault,
                CMAC_Struct1.interal_rx_local_fault);
            
            printf("===============================================\n");
        }     
        usleep(10000); 
    }
}

void force_tx_and_check_pc2(void* arg) {
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

            Check_CMAC_Pin(); // doc trang thai cac pin tu GPIO
            printf("[CMAC] : stat_rx_aligned = %d stat_tx_local_fault = % d usr_rx_reset = %d usr_tx_reset = %d rx_local_fault = %d\n", 
                CMAC_Struct1.stat_rx_aligned,
                CMAC_Struct1.stat_tx_local_fault,
                CMAC_Struct1.usr_rx_reset,
                CMAC_Struct1.usr_tx_reset,
                CMAC_Struct1.interal_rx_local_fault);
            
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
    // uint32_t rx_p_low = XGpio_DiscreteRead(&GPIO_RX_PREAMBLEOUT, 1);
    // uint32_t rx_p_high = XGpio_DiscreteRead(&GPIO_RX_PREAMBLEOUT, 2);
    // uint64_t rx_preamble = ((uint64_t)rx_p_high << 32) | rx_p_low;

    // printf("[RX] Preamble nhan duoc tu doi tac: 0x%llX\n", rx_preamble);
}

void check_rx_protocol_errors() {
    uint32_t reg = Xil_In32(ADDR_CMAC_REG + 0x0204);
    
    if (reg & (1 << 12)) printf("[ALERT] Phat hien loi BAD PREAMBLE!\n");
    if (reg & (1 << 13)) printf("[ALERT] Phat hien loi BAD SFD!\n");
}

void Check_CMAC_Pin()
{
    uint32_t input = XGpio_DiscreteRead(&GPIO_SYNC_CMAC, 1);    

    CMAC_Struct1.stat_tx_local_fault    = (input & 0x00000001) ? 1 : 0;    ///
    CMAC_Struct1.stat_rx_aligned        = (input & 0x00000002) ? 1 : 0;    ///
    CMAC_Struct1.usr_rx_reset           = (input & 0x00000004) ? 1 : 0;    ///
    CMAC_Struct1.usr_tx_reset           = (input & 0x00000008) ? 1 : 0;    ///
    CMAC_Struct1.interal_rx_local_fault = (input & 0x00000010) ? 1 : 0;    ///
    CMAC_Struct1.stat_rx_local_fault    = (input & 0x00000020) ? 1 : 0;    ///
    CMAC_Struct1.stat_rx_miss_aligned   = (input & 0x00000040) ? 1 : 0;    ///
    CMAC_Struct1.usr_tx_bad_fcs         = (input & 0x00000080) ? 1 : 0;    ///
}

void CMAC_Reset_rx_datapath()
{
    XGpio_DiscreteWrite(&GPIO_SYNC_CMAC, 2, 1);
	usleep(1000);
	XGpio_DiscreteWrite(&GPIO_SYNC_CMAC, 2, 0);
	usleep(200000);
}

void Set_CMAC_Hardware_Loopback(uint32_t mode_val, const char* mode_name) {
    printf("\n[LOOPBACK CMD] Setting Hardware Loopback: %s (Val: 0x%03X)\n", mode_name, mode_val);
    
    // 1. Ghi giá trị xuống chân gt_loopback_in[11:0] thông qua GPIO
    XGpio_DiscreteWrite(&GPIO_RX_PREAMBLEOUT, 1, mode_val);
    
    // 2. Chờ một chút để mạch chuyển chế độ
    usleep(10000); // 10ms
    
    // 3. QUAN TRỌNG: Toggle Reset GT RX Datapath để CDR khóa lại vào tín hiệu mới
    // (Giả sử bạn đã có hàm hoặc địa chỉ GPIO điều khiển reset này)
    printf("[LOOPBACK CMD] Toggling GT RX Reset to re-lock CDR...\n");
    CMAC_Reset_rx_datapath(); 
    
    usleep(100000); // Chờ 100ms cho Link Up
}

void Test_All_Loopback_Modes() {
    uint32_t reg_status;
    
    printf("===========================================\n");
    printf(" STARTING HARDWARE LOOPBACK TEST SEQUENCE  \n");
    printf("===========================================\n");

    // --- TEST 1: NEAR-END PMA (QUAN TRỌNG NHẤT) ---
    // Đây là chế độ Loopback ngay sau bộ đệm Analog TX/RX.
    // Nếu chế độ này chạy, Core và GT OK. Lỗi do dây/module.
    Set_CMAC_Hardware_Loopback(LB_MODE_NE_PMA, "Near-End PMA (Analog)");
    
    // Kiểm tra trạng thái
    Check_CMAC_Pin(); // Cập nhật từ GPIO Status
    if (CMAC_Struct1.stat_rx_aligned == 1) {
        printf(">>> RESULT: NE PMA Loopback PASSED! (Core & GT Analog OK)\n");
    } else {
        printf(">>> RESULT: NE PMA Loopback FAILED. (Check GT RefClk or Equalizer)\n");
    }
    sleep(2); // Dừng 2s để quan sát

    // --- TEST 2: NEAR-END PCS ---
    // Loopback tại tầng số, bỏ qua mạch Analog.
    // Nếu Test 1 fail mà Test 2 pass -> Lỗi ở mạch Analog/Equalizer.
    Set_CMAC_Hardware_Loopback(LB_MODE_NE_PCS, "Near-End PCS (Digital)");
    
    Check_CMAC_Pin();
    if (CMAC_Struct1.stat_rx_aligned == 1) {
        printf(">>> RESULT: NE PCS Loopback PASSED! (Core Logic OK)\n");
    } else {
        printf(">>> RESULT: NE PCS Loopback FAILED. (Core Logic/Clock Issue)\n");
    }
    sleep(2);

    // --- TEST 3: NORMAL MODE (EXTERNAL) ---
    // Chạy với dây cáp thật
    Set_CMAC_Hardware_Loopback(LB_MODE_NORMAL, "Normal Mode (External)");
    
    Check_CMAC_Pin();
    if (CMAC_Struct1.stat_rx_aligned == 1) {
        printf(">>> RESULT: External Link PASSED!\n");
    } else {
        printf(">>> RESULT: External Link FAILED. (Check Cable/Module)\n");
    }
    
    printf("===========================================\n");
}