#ifndef SI570_HAL_H
#define SI570_HAL_H

#include <linux_hal.h>

// Tần số thạch anh nội của Si570 trên board của bạn.
// Giá trị này rất quan trọng và cần được xác nhận.
// 114.285 MHz là một giá trị phổ biến trên các board của Xilinx.
#define ON_BOARD_SI570_FXTAL_HZ 114285000.0

/**
 * @struct Si570_Config
 * @brief  Cấu trúc chứa các thông tin cấu hình cần thiết cho một chip Si570.
 */
typedef struct {
    char i2c_bus_path[32]; // Đường dẫn đến file bus I2C, ví dụ "/dev/i2c-3"
    uint8_t i2c_addr;      // Địa chỉ I2C của chip, ví dụ 0x5D
    double fxtal_hz;       // Tần số thạch anh nội tham chiếu, tính bằng Hz
} Si570_Config;

/**
 * @struct Si570_Handle
 * @brief  Cấu trúc quản lý "handle" hoặc "instance" của một chip Si570.
 */
typedef struct {
    int fd;                // File descriptor cho bus I2C
    Si570_Config config;   // Bản sao của cấu hình
} Si570_Handle;

static int si570_write_reg(int fd, uint8_t reg, uint8_t value);
static int si570_read_regs(int fd, uint8_t start_reg, uint8_t *buffer, int length);
int si570_init(Si570_Handle *handle, const Si570_Config *config);
void si570_close(Si570_Handle *handle);
int si570_get_current_freq(Si570_Handle *handle, double *current_freq_mhz);
int si570_set_frequency(Si570_Handle *handle, double freq_mhz);
void SI570_Set_Freq_MHz(double target_freq);

#endif // SI570_HAL_H