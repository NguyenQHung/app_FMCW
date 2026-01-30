#ifndef SFP28_HAL_H
#define SFP28_HAL_H

#include "linux_hal.h"

// // Định nghĩa I2C Bus và Địa chỉ
// #define I2C_BUS_ID      "1"    // Bus i2c-1
// #define SFP_ADDR        "0x50" // Địa chỉ Module QSFP28
// #define SFP_STATUS_ADDR "0x02" // Địa chỉ thanh ghi cờ lỗi chính (Flags)
// #define QSFP_CTRL_REG_ADDR "0x7B" // Thanh ghi điều khiển Module (Byte 123)

// Hàm thực hiện lệnh shell và trả về kết quả
int execute_i2c_get(const char* reg_addr);

// Hàm thực hiện lệnh i2cset (Ghi 1 byte)
void execute_i2c_set(const char* reg_addr, uint8_t value);
// Hàm đọc một chuỗi byte từ I2C
void execute_i2c_read_block(int start_reg, int count, uint8_t* buffer);

// Hàm chính để đọc, hiển thị và phân tích
void dump_and_analyze_qsfp_registers();

// Hàm đọc một khối 256 byte
int execute_i2c_read_full_256(uint8_t* buffer);
// Hàm chính để đọc, hiển thị và phân tích
void dump_and_analyze_qsfp_full_registers();

void reset_qsfp_module();

// =================================================================
//                 HÀM CHÍNH: KÍCH HOẠT MODULE VÀ ALIGN CMAC
// =================================================================

void CMAC_SFP_Power_Up_And_Align();

// Hàm chính để kiểm tra trạng thái Module
void check_qsfp_status();
void periodic_qsfp_status_check();
void read_and_display_optical_power();
#endif // LINUX_HAL_H
