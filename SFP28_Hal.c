#include "SFP28_Hal.h"

extern struct CMAC_Struct  			CMAC_Struct1;

// Định nghĩa I2C Bus và Địa chỉ
#define I2C_BUS_ID      "1"    // Bus i2c-1
#define SFP_ADDR        "0x50" // Địa chỉ Module QSFP28
#define SFP_STATUS_ADDR "0x02" // Địa chỉ thanh ghi cờ lỗi chính (Flags)
#define QSFP_CTRL_REG_ADDR "0x7B" // Thanh ghi điều khiển Module (Byte 123)

// Hàm thực hiện lệnh shell và trả về kết quả
int execute_i2c_get(const char* reg_addr) {
    char cmd[128];
    char result[8];
    FILE *fp;
    int val = -1;

    // Lệnh: i2cget -y <bus> <addr> <register> b (Đọc 1 byte)
    snprintf(cmd, sizeof(cmd), "i2cget -y %s %s %s b 2>/dev/null", I2C_BUS_ID, SFP_ADDR, reg_addr);
    
    fp = popen(cmd, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Failed to run command.\n");
        return -1;
    }

    // Đọc kết quả (ví dụ: 0xXX)
    if (fgets(result, sizeof(result), fp) != NULL) {
        if (sscanf(result, "0x%x", &val) != 1) {
             val = -1; // Lỗi đọc
        }
    }
    pclose(fp);
    return val;
}

// Hàm thực hiện lệnh i2cset (Ghi 1 byte)
void execute_i2c_set(const char* reg_addr, uint8_t value) {
    char cmd[128];
    // Lệnh: i2cset -y <bus> <addr> <reg> <val> b
    snprintf(cmd, sizeof(cmd), "i2cset -y %s %s %s 0x%02x b 2>/dev/null", I2C_BUS_ID, SFP_ADDR, reg_addr, value);
    system(cmd);
}

// Hàm đọc một chuỗi byte từ I2C
void execute_i2c_read_block(int start_reg, int count, uint8_t* buffer) {
    for (int i = 0; i < count; ++i) {
        char reg_str[8];
        snprintf(reg_str, sizeof(reg_str), "0x%02X", start_reg + i);
        buffer[i] = (uint8_t)execute_i2c_get(reg_str);
    }
}

// Hàm chính để đọc, hiển thị và phân tích
void dump_and_analyze_qsfp_registers() {
    uint8_t page0_data[128];

    printf("\n======================================================\n");
    printf("     QSFP28 MODULE FULL REGISTER DUMP & ANALYSIS\n");
    printf("======================================================\n");

    // 1. Đọc toàn bộ 128 byte của Page 0
    execute_i2c_read_block(0, 128, page0_data);

    // 2. Hiển thị bảng Hex (giống i2cdump)
    printf("--- Raw Register Dump (Page 0) ---\n");
    for (int i = 0; i < 128; ++i) {
        if (i % 16 == 0) printf("%02x: ", i);
        printf("%02x ", page0_data[i]);
        if (i % 16 == 15) printf("\n");
    }
    printf("\n");

    // 3. Phân tích các byte quan trọng
    printf("--- Key Register Analysis ---\n");

    // Byte 2: Status Flags/Interrupts
    uint8_t byte2 = page0_data[2];
    printf("Byte 02 (Status Flags)      : 0x%02X\n", byte2);
    if (!(byte2 & 0x80)) printf("  [FAIL] Data Not Ready: Module is busy or in low-power state. Data is unreliable.\n");
    else                printf("  [ OK ] Data Ready: Module has completed initialization.\n");
    if (byte2 & 0x01)   printf("  [WARN] Low Power Mode: Module is in low power mode.\n");

    // Byte 3: Channel/Lane Status (LOS, TX Fault, TX Disable)
    uint8_t byte3 = page0_data[3];
    printf("Byte 03 (Channel Status)    : 0x%02X\n", byte3);
    uint8_t rx_los = (byte3 >> 4) & 0x0F;
    uint8_t tx_fault = byte3 & 0x0F;
    if (rx_los != 0) printf("  [FAIL] RX Loss of Signal detected on lanes: %s%s%s%s\n", 
                            (rx_los & 0x8) ? "3 " : "", (rx_los & 0x4) ? "2 " : "", 
                            (rx_los & 0x2) ? "1 " : "", (rx_los & 0x1) ? "0 " : "");
    else             printf("  [ OK ] RX LOS: All lanes have optical signal.\n");

    if (tx_fault != 0) printf("  [FAIL] TX Fault detected on lanes: %s%s%s%s\n", 
                              (tx_fault & 0x8) ? "3 " : "", (tx_fault & 0x4) ? "2 " : "", 
                              (tx_fault & 0x2) ? "1 " : "", (tx_fault & 0x1) ? "0 " : "");
    else               printf("  [ OK ] TX Fault: No transmitter faults detected.\n");
                              
    // Byte 92: Vendor Name
    char vendor_name[17];
    memcpy(vendor_name, &page0_data[92], 16);
    vendor_name[16] = '\0';
    printf("Byte 92-107 (Vendor Name)  : %s\n", vendor_name);
    
    // Byte 108: Vendor OUI
    printf("Byte 108-110 (Vendor OUI)   : %02X-%02X-%02X\n", page0_data[108], page0_data[109], page0_data[110]);

    // Byte 116: Part Number
    char part_number[17];
    memcpy(part_number, &page0_data[116], 16);
    part_number[16] = '\0';
    printf("Byte 116-131 (Part Number)  : %s\n", part_number);

    // Byte 132: Serial Number
    char serial_number[17];
    memcpy(serial_number, &page0_data[132], 16);
    serial_number[16] = '\0';
    printf("Byte 132-147 (Serial Number): %s\n", serial_number);


    // 4. KẾT LUẬN TỔNG QUAN
    printf("\n--- Overall Conclusion ---\n");
    if (!(byte2 & 0x80)) {
        printf("=> LỖI NGHIÊM TRỌNG: Module quang báo 'Data Not Ready'.\n");
        printf("   Nguyên nhân: Module đang ở chế độ Low Power hoặc chưa khởi động xong.\n");
        printf("   Hành động: Cần ghi vào thanh ghi 0x7B để ép Module sang High Power Mode.\n");
    } else if (rx_los != 0 || tx_fault != 0) {
        printf("=> LỖI VẬT LÝ: Module quang báo mất tín hiệu (RX LOS) hoặc lỗi phát (TX Fault).\n");
        printf("   Nguyên nhân: Dây loopback quang/cáp bị lỗi, hỏng hoặc cắm không chặt.\n");
        printf("   Hành động: Kiểm tra lại kết nối vật lý.\n");
    } else {
        printf("=> MODULE BÁO CÁO OK: Module quang không báo lỗi vật lý (No RX LOS, No TX Fault) và đã sẵn sàng (Data Ready).\n");
        printf("   Nếu CMAC vẫn báo lỗi `rx_local_fault=1`, vấn đề 100%% nằm ở cấu hình GT RX Datapath (CDR/Equalizer) bên trong FPGA.\n");
        printf("   Hành động: Thực hiện Toggle Reset `gtwiz_reset_rx_datapath`.\n");
    }
    
    printf("======================================================\n");
}

// Hàm đọc một khối 256 byte
int execute_i2c_read_full_256(uint8_t* buffer) {
    for (int i = 0; i < 256; ++i) {
        char reg_str[8];
        snprintf(reg_str, sizeof(reg_str), "0x%02X", i);
        int val = execute_i2c_get(reg_str);
        if (val == -1 && i == 0) return -1;
        buffer[i] = (uint8_t)val;
    }
    return 0;
}

// Hàm chính để đọc, hiển thị và phân tích
void dump_and_analyze_qsfp_full_registers() {
    uint8_t full_page_data[256];

    printf("\n======================================================\n");
    printf("     QSFP28 MODULE FULL REGISTER DUMP & ANALYSIS\n");
    printf("======================================================\n");

    if (execute_i2c_read_full_256(full_page_data) != 0) {
        printf("[ERROR] Failed to read from I2C device at 0x50. Is module connected?\n");
        printf("======================================================\n");
        return;
    }

    // Phần 1: Hiển thị Raw Data giống i2cdump
    printf("--- Raw Register Dump (256 Bytes) ---\n");
    printf("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f    0123456789abcdef\n");
    for (int i = 0; i < 256; ++i) {
        if (i % 16 == 0) printf("%02x: ", i);
        printf("%02x ", full_page_data[i]);
        if (i % 16 == 15) {
            printf("   ");
            for (int j = i - 15; j <= i; ++j) {
                printf("%c", isprint(full_page_data[j]) ? full_page_data[j] : '.');
            }
            printf("\n");
        }
    }
    printf("\n");

    // Phần 2: Phân tích các byte quan trọng
    printf("--- Key Register Analysis (Page 00h) ---\n");

    // --- Lower Page (Trạng thái thời gian thực) ---
    uint8_t byte2 = full_page_data[2];
    printf("[Byte 02] Status Flags      : 0x%02X\n", byte2);
    if (!(byte2 & 0x80)) printf("  [FAIL] Data Not Ready (Bit 7): Module is busy or in low-power state.\n");
    else                printf("  [ OK ] Data Ready (Bit 7): Module has completed initialization.\n");
    
    uint8_t byte3 = full_page_data[3];
    printf("[Byte 03] Channel Status    : 0x%02X\n", byte3);
    uint8_t rx_los = (byte3 >> 4) & 0x0F;
    uint8_t tx_fault = byte3 & 0x0F;
    if (rx_los != 0) printf("  [FAIL] RX Loss of Signal detected on lanes: %s%s%s%s\n", 
                            (rx_los & 0x8) ? "3 " : "", (rx_los & 0x4) ? "2 " : "", 
                            (rx_los & 0x2) ? "1 " : "", (rx_los & 0x1) ? "0 " : "");
    else             printf("  [ OK ] RX LOS: All lanes have optical signal.\n");

    if (tx_fault != 0) printf("  [FAIL] TX Fault detected on lanes: %s%s%s%s\n", 
                              (tx_fault & 0x8) ? "3 " : "", (tx_fault & 0x4) ? "2 " : "", 
                              (tx_fault & 0x2) ? "1 " : "", (tx_fault & 0x1) ? "0 " : "");
    else               printf("  [ OK ] TX Fault: No transmitter faults detected.\n");
                              
    // --- Upper Page (Thông tin tĩnh) ---
    char buffer[17];
    printf("\n--- Static Information (Upper Page 00h) ---\n");

    memcpy(buffer, &full_page_data[148], 16); // Địa chỉ 0x94 (148)
    buffer[16] = '\0';
    printf("[Byte 148-163] Vendor Name      : %s\n", buffer);
    
    memcpy(buffer, &full_page_data[168], 16); // Địa chỉ 0xA8 (168)
    buffer[16] = '\0';
    printf("[Byte 168-183] Vendor PN        : %s\n", buffer);

    memcpy(buffer, &full_page_data[196], 16); // Địa chỉ 0xC4 (196)
    buffer[16] = '\0';
    printf("[Byte 196-211] Vendor SN        : %s\n", buffer);

    // Phần 3: Kết luận tổng quan
    printf("\n--- Overall Conclusion ---\n");
    if (!(byte2 & 0x80)) {
        printf("=> LỖI NGHIÊM TRỌNG: Module quang báo 'Data Not Ready'.\n");
        printf("   Nguyên nhân: Module đang bị khóa ở chế độ Low Power hoặc chưa khởi động xong.\n");
        printf("   Hành động: 1. Thử Reset mềm module (ghi 0x80 vào 0x7B rồi ghi 0x00).\n");
        printf("              2. Kiểm tra lại nguồn cấp cho module.\n");
    } else if (rx_los != 0 || tx_fault != 0) {
        printf("=> LỖI VẬT LÝ: Module quang báo mất tín hiệu (RX LOS) hoặc lỗi phát (TX Fault).\n");
        printf("   Nguyên nhân: Dây loopback quang/cáp bị lỗi, hỏng hoặc cắm không chặt.\n");
        printf("   Hành động: Kiểm tra lại kết nối vật lý và thiết bị đối tác.\n");
    } else {
        printf("=> MODULE BÁO CÁO OK: Module không báo lỗi vật lý và đã sẵn sàng (Data Ready).\n");
        printf("   Nếu CMAC vẫn lỗi, vấn đề 100%% nằm ở cấu hình GT RX Datapath (CDR/Equalizer) của FPGA.\n");
        printf("   Hành động: Thực hiện Toggle Reset `gtwiz_reset_rx_datapath`.\n");
    }
    
    printf("======================================================\n");
}

void reset_qsfp_module() {
    printf("[SFP RESET] Performing module soft reset...\n");
    
    // 1. Kích hoạt Reset (Kéo Bit 7 lên 1)
    execute_i2c_set("0x7B", 0x80); 
    usleep(400000); // Giữ reset 100ms
    
    // 2. Nhả Reset (Kéo Bit 7 về 0)
    execute_i2c_set("0x7B", 0x00);
    usleep(1000000); // Chờ 500ms cho module khởi động lại hoàn toàn
    
    printf("[SFP RESET] Module reset complete.\n");
}

// =================================================================
//                 HÀM CHÍNH: KÍCH HOẠT MODULE VÀ ALIGN CMAC
// =================================================================

void CMAC_SFP_Power_Up_And_Align() {
    uint32_t reg_status;
    int module_ctrl_byte;
    int max_attempts = 10;
    int attempt = 0;

    printf("\n--- SFP MODULE POWER-UP AND CMAC ALIGNMENT ---\n");

    // BƯỚC 1: KÍCH HOẠT MODULE QUANG (Buộc ra khỏi Low Power Mode)
    // Thanh ghi 0x7B (Control Flags) - Bit 0 là LPMode, Bit 7 là Module Reset
    
    // Đọc giá trị hiện tại
    module_ctrl_byte = execute_i2c_get(QSFP_CTRL_REG_ADDR);
    if (module_ctrl_byte == -1) {
        printf("[SFP FAIL] Module not found or I2C error. Check SFP presence.\n");
        return;
    }
    
    // Ghi lại giá trị: Đặt Bit 0 (LPMode) = 0 và Bit 7 (Module Reset) = 0 (Nhả Reset Module)
    // Module Reset (Bit 7) = 0 (Run), LPMode (Bit 0) = 0 (High Power)
    uint8_t desired_value = (uint8_t)(module_ctrl_byte & (~0x81)); 
    
    printf("[SFP INIT] Current Control Byte (0x7B): 0x%02X. New Value: 0x%02X (Force HP Mode).\n", 
           module_ctrl_byte, desired_value);
    
    execute_i2c_set(QSFP_CTRL_REG_ADDR, desired_value);
    usleep(100000); // Chờ 100ms cho Module Quang tỉnh dậy và ổn định nguồn

    // BƯỚC 2: CHỜ MODULE BÁO SẴN SÀNG (Data_Ready) VÀ ALIGN CMAC
    printf("[CMAC ALIGN] Waiting for CMAC to Align...\n");
    
    while(attempt < max_attempts) {
        
        // --- (A) BẮT BUỘC: RESET GT RX DATAPATH (CDR FIX) ---
        // Phải Toggle sau khi Module quang ổn định
        if (attempt % 2 == 0) {
            CMAC_Reset_rx_datapath(); 
            usleep(5000); // 5ms cho mạch Analog ổn định sau Reset
        }

        // --- (B) KIỂM TRA TRẠNG THÁI THẬT TỪ CMAC (Giả sử bạn có hàm Check_CMAC_Pin) ---
        // Check_CMAC_Pin(); 
        
        // Tạm thời dùng thanh ghi để check (Nếu bạn không thể dùng Check_CMAC_Pin ở đây)
        Xil_Out32(ADDR_CMAC_REG + 0x02B0, 0x00000001); // Kích pm_tick
        reg_status = Xil_In32(ADDR_CMAC_REG + 0x0204);
        
        if ((reg_status & 0x03) == 0x03) { 
            printf("[SUCCESS] Link UP after %d attempts. (Status 0x03).\n", attempt);
            return;
        }

        printf("[ALIGN FAILED] Attempt %d: Status 0x%X (LOS/LocalFault expected).\n", attempt + 1, reg_status);
        
        attempt++;
        usleep(100000); // Đợi 100ms
    }
    
    printf("[FAIL] Alignment Timeout. Check SFP/Cable Signal Integrity.\n");
}

// Hàm chính để kiểm tra trạng thái Module
void check_qsfp_status() {
    int byte_02_flags; // Base ID Byte 2 (0x02): Status Flags/Interrupts
    int byte_03_flags; // Base ID Byte 3 (0x03): Status Flags/Warnings
    int byte_12_los_tx_fault; // Base ID Byte 12 (0x0C): LOS and TX Fault for all channels

    printf("\n--- QSFP28 MODULE DIAGNOSTICS (I2C-1 @ 0x50) ---\n");

    // 1. Đọc Byte 2: Status Flags (Phân tích lỗi tức thời)
    byte_02_flags = execute_i2c_get(SFP_STATUS_ADDR); 
    
    if (byte_02_flags == -1) {
        printf("[ERROR] Cannot communicate with 0x50. Module may be disconnected/disabled.\n");
        return;
    }

    printf("[STATUS] SFF-8636 Byte 2 (Interrupt Flags) = 0x%02X\n", byte_02_flags);

    // Byte 2 (0x02) - Báo lỗi Cấp cứu (Flags) - Bit 7 là Data_Ready
    if (!(byte_02_flags & (1 << 7))) {
        printf(" [FAIL] Module Not Ready (Bit 7 = 0). Cannot Trust Data.\n");
    }
    if (byte_02_flags & 0x01) {
        printf(" [WARN] Low Power Mode is Active (Bit 0 = 1).\n");
    }
    
    // 2. Đọc Byte 3: Warning Flags (Cảnh báo)
    byte_03_flags = execute_i2c_get("0x03");
    printf("[STATUS] SFF-8636 Byte 3 (Warning Flags)   = 0x%02X\n", byte_03_flags);

    // 3. Đọc Byte 12: Cờ LOS/TX Fault (Chi tiết tình trạng Link từng làn)
    // Byte 12: Bits [7:4] = Rx_LOS (4 Làn); Bits [3:0] = Tx_Fault (4 Làn)
    byte_12_los_tx_fault = execute_i2c_get("0x0C");
    printf("[STATUS] SFF-8636 Byte 12 (LOS/TxFault) = 0x%02X\n", byte_12_los_tx_fault);

    if (byte_12_los_tx_fault == -1) {
        printf(" [FAIL] Could not read Byte 12.\n");
        return;
    }

    // 4. PHÂN TÍCH LINK STATUS DỰA TRÊN BIT LOS/TX FAULT
    uint8_t rx_los_bits = (byte_12_los_tx_fault >> 4) & 0x0F; // Bits 7:4
    uint8_t tx_fault_bits = byte_12_los_tx_fault & 0x0F;     // Bits 3:0

    printf("\n--- CMAC LINK STATUS DEDUCTION ---\n");

    if (rx_los_bits == 0x00) {
        printf(" [ OK ] RX LOS (Lane 0-3): All LANS have signal.\n");
    } else {
        printf(" [FAIL] RX LOS (Lane 0-3): Loss of Signal on LANS 0x%02X (CMAC Link Down).\n", rx_los_bits);
    }
    
    if (tx_fault_bits == 0x00) {
        printf(" [ OK ] TX Fault (Lane 0-3): No TX Fault detected.\n");
    } else {
        printf(" [FAIL] TX Fault (Lane 0-3): TX Fault on LANS 0x%02X (CMAC Link Down).\n", tx_fault_bits);
    }
    
    if (rx_los_bits == 0x00 && tx_fault_bits == 0x00 && (byte_02_flags & (1 << 7))) {
        printf("\n=> KET LUAN: MODULE CÓ TÍN HIỆU. LỖI Ở CMAC CDR/ALIGNMENT.\n");
    } else {
        printf("\n=> KET LUAN: MODULE BÁO LỖI TÍN HIỆU QUANG. LỖI NGOÀI.\n");
    }
    printf("--------------------------------------------------\n");
}

// Hàm chính để kiểm tra trạng thái Module
void periodic_qsfp_status_check() {
    int byte_02_flags, byte_03_flags, byte_12_los_tx_fault;

    printf("\n--- QSFP28 PERIODIC DIAGNOSTICS (I2C-1 @ 0x50) ---\n");

    // Đọc các byte trạng thái quan trọng
    byte_02_flags = execute_i2c_get("0x02");
    byte_03_flags = execute_i2c_get("0x03");
    byte_12_los_tx_fault = execute_i2c_get("0x0C");
    
    if (byte_02_flags == -1) {
        printf("[ERROR] Cannot communicate with 0x50. Module may be disconnected.\n");
        printf("--------------------------------------------------\n");
        return;
    }

    // Phân tích trạng thái
    printf("[STATUS] SFF-8636 Byte 2 (Flags)   = 0x%02X\n", byte_02_flags);
    printf("[STATUS] SFF-8636 Byte 3 (Warnings) = 0x%02X\n", byte_03_flags);
    printf("[STATUS] SFF-8636 Byte 12 (LOS/Fault) = 0x%02X\n", byte_12_los_tx_fault);

    uint8_t rx_los = (byte_12_los_tx_fault >> 4) & 0x0F;
    uint8_t tx_fault = byte_12_los_tx_fault & 0x0F;

    printf("\n--- CMAC LINK STATUS DEDUCTION ---\n");

    // 1. Phân tích trạng thái Data Ready
    if (!(byte_02_flags & 0x80)) {
        printf(" [FAIL] Module Not Ready (Data_Not_Ready = 0): Module is in a non-operational state.\n");
    } else {
        printf(" [ OK ] Module Ready (Data_Ready = 1).\n");
    }

    // 2. Phân tích lỗi quang học (LOS/TX Fault)
    if (rx_los != 0) {
        printf(" [FAIL] RX Loss of Signal detected on lanes: 0x%X\n", rx_los);
    } else {
        printf(" [ OK ] RX LOS: All lanes have optical signal.\n");
    }
    
    if (tx_fault != 0) {
        printf(" [FAIL] TX Fault detected on lanes: 0x%X\n", tx_fault);
    } else {
        printf(" [ OK ] TX Fault: No transmitter faults detected.\n");
    }

    // 3. Đưa ra kết luận cuối cùng (Logic đã chỉnh sửa)
    printf("\n--- Overall Conclusion ---\n");

    if (!(byte_02_flags & 0x80)) {
        printf("=> LỖI NGHIÊM TRỌNG: Module bị khóa ở trạng thái 'Not Ready'.\n");
        printf("   Hành động: Cần Reset cứng module (rút ra cắm lại) hoặc thay thế.\n");
    } else if (rx_los != 0 || tx_fault != 0) {
        printf("=> LỖI VẬT LÝ NGOÀI: Module báo lỗi quang học (LOS/TX Fault).\n");
        printf("   Nguyên nhân: Dây cáp/loopback bị lỗi, hoặc thiết bị đối tác bị lỗi.\n");
    } else {
        // Module báo OK (Ready, No LOS, No TX Fault)
        // Bây giờ, ta so sánh với trạng thái của CMAC để tìm lỗi
        
        // // (Giả sử bạn đọc lại CMAC_Struct1 ở đây)
        // Check_CMAC_Pin(); 
        
        // if (CMAC_Struct1.stat_rx_aligned == 1) 
        // {
        //     printf("=> HỆ THỐNG OK: Module và CMAC đều báo Link UP.\n");
        // } else {
        //     printf("=> LỖI NỘI TẠI FPGA: Module báo OK nhưng CMAC không Align được.\n");
        //     printf("   Nguyên nhân: Lỗi GT RX Datapath (CDR/Equalizer).\n");
        //     printf("   Hành động: Thực hiện Toggle Reset `gtwiz_reset_rx_datapath`.\n");
        // }
    }
    
    printf("--------------------------------------------------\n");
}

// Hàm đọc và hiển thị công suất quang
void read_and_display_optical_power() {
    printf("\n--- Reading QSFP28 Optical Power (DDM) ---\n");

    // BƯỚC 1: CHUYỂN SANG TRANG 03H (DDM PAGE)
    printf("[INFO] Selecting Page 03h (writing 0x03 to 0x7F)...\n");
    execute_i2c_set("0x7F", 0x03);
    usleep(10000); // Đợi module chuyển trang

    // BƯỚC 2: ĐỌC CÔNG SUẤT THU (RX POWER)
    printf("\n--- Received Optical Power ---\n");
    for (int i = 0; i < 4; ++i) {
        char addr_msb_str[8], addr_lsb_str[8];
        snprintf(addr_msb_str, sizeof(addr_msb_str), "0x%02X", 134 + i * 2);
        snprintf(addr_lsb_str, sizeof(addr_lsb_str), "0x%02X", 135 + i * 2);

        int msb = execute_i2c_get(addr_msb_str);
        int lsb = execute_i2c_get(addr_lsb_str);

        if (msb == -1 || lsb == -1) {
            printf("Lane %d: Read Error\n", i + 1);
            continue;
        }

        uint16_t raw_value = (msb << 8) | lsb;
        double power_mw = raw_value * 0.0001;
        double power_dbm = (power_mw > 0) ? 10 * log10(power_mw) : -40.0; // -40dBm là mức sàn (no signal)

        printf("Lane %d: 0x%04X -> %.4f mW -> %.2f dBm\n", i + 1, raw_value, power_mw, power_dbm);
    }

    // BƯỚC 3: ĐỌC CÔNG SUẤT PHÁT (TX POWER)
    printf("\n--- Transmitted Optical Power ---\n");
    for (int i = 0; i < 4; ++i) {
        char addr_msb_str[8], addr_lsb_str[8];
        snprintf(addr_msb_str, sizeof(addr_msb_str), "0x%02X", 174 + i * 2);
        snprintf(addr_lsb_str, sizeof(addr_lsb_str), "0x%02X", 175 + i * 2);
        
        int msb = execute_i2c_get(addr_msb_str);
        int lsb = execute_i2c_get(addr_lsb_str);

        if (msb == -1 || lsb == -1) {
            printf("Lane %d: Read Error\n", i + 1);
            continue;
        }
        
        uint16_t raw_value = (msb << 8) | lsb;
        double power_mw = raw_value * 0.0001;
        double power_dbm = (power_mw > 0) ? 10 * log10(power_mw) : -40.0;
        
        printf("Lane %d: 0x%04X -> %.4f mW -> %.2f dBm\n", i + 1, raw_value, power_mw, power_dbm);
    }
    
    // BƯỚC 4: QUAY VỀ TRANG MẶC ĐỊNH 00H
    printf("\n[INFO] Returning to Page 00h (writing 0x00 to 0x7F)...\n");
    execute_i2c_set("0x7F", 0x00);
    
    printf("----------------------------------------\n");
}