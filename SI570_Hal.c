#include "SI570_Hal.h"

#define _GNU_SOURCE
#include <math.h>

// --- Các hàm nội bộ (static) ---

// Hàm ghi một byte vào một thanh ghi
static int si570_write_reg(int fd, uint8_t reg, uint8_t value) {
    uint8_t buf[2] = {reg, value};
    if (write(fd, buf, 2) != 2) {
        perror("Lỗi ghi vào thanh ghi I2C");
        return -1;
    }
    return 0;
}

// Hàm đọc các thanh ghi
static int si570_read_regs(int fd, uint8_t start_reg, uint8_t *buffer, int length) {
    if (write(fd, &start_reg, 1) != 1) {
        perror("Lỗi gửi địa chỉ thanh ghi để đọc");
        return -1;
    }
    if (read(fd, buffer, length) != length) {
        perror("Lỗi đọc từ các thanh ghi I2C");
        return -1;
    }
    return 0;
}

// --- Các hàm công khai (public) ---

int si570_init(Si570_Handle *handle, const Si570_Config *config) {
    if (!handle || !config) {
        return -1;
    }

    handle->config = *config; // Sao chép cấu hình

    handle->fd = open(handle->config.i2c_bus_path, O_RDWR);
    if (handle->fd < 0) {
        perror("[SI570 HAL] Lỗi: Không thể mở bus I2C");
        return -1;
    }

    if (ioctl(handle->fd, I2C_SLAVE, handle->config.i2c_addr) < 0) {
        perror("[SI570 HAL] Lỗi: Không thể đặt địa chỉ slave");
        close(handle->fd);
        handle->fd = -1;
        return -1;
    }

    printf("[SI570 HAL] Khởi tạo thành công cho chip tại địa chỉ 0x%02X trên bus %s.\n", 
           handle->config.i2c_addr, handle->config.i2c_bus_path);

    return 0;
}

int si570_set_frequency(Si570_Handle *handle, double freq_mhz) {
    if (!handle || handle->fd < 0) {
        return -1;
    }
    
    // --- Bắt buộc sử dụng thông số tối ưu cho 156.25 MHz (hoặc dải gần đó) ---
    // Loại bỏ việc đọc thanh ghi để lấy N1 và HS_DIV cũ
    double dco_freq;
    uint64_t rfreq;
    uint32_t n1 = 8;        // Divisor N1 tối ưu
    uint32_t hs_div = 4;    // Divisor HS_DIV tối ưu
    uint32_t n1_value_to_write = n1 - 1; // N_value = 7
    uint8_t regs[6]; // Cần 6 thanh ghi (R7 đến R12)

    printf("[SI570 HAL] Bắt đầu lập trình tần số: %.6f MHz\n", freq_mhz);
    printf("[SI570 HAL] Sử dụng giá trị cố định: HS_DIV = %u, N1 = %u\n", hs_div, n1);

    // 1. (Bỏ qua việc đọc và tính toán lại N1/HS_DIV)

    // 2. Đóng băng DCO (Thanh ghi 137, bit 4)
    if (si570_write_reg(handle->fd, 137, 0x10) != 0) return -1;

    // 3. Tính toán giá trị mới
    // f_DCO = f_out * HS_DIV * N1
    dco_freq = freq_mhz * 1000000.0 * (double)hs_div * (double)n1;
    // RFREQ = (f_DCO / f_xtal) * 2^28
    rfreq = (uint64_t)((dco_freq / handle->config.fxtal_hz) * (double)(1ULL << 28));
    printf("[SI570 HAL] Giá trị RFREQ mới (hex): 0x%010lX\n", rfreq);

    // --- MÃ HÓA CÁC THANH GHI (R7 đến R12) ---

    // R7: HS_DIV (7-5) và N1_value (4-0)
    // HS_DIV được mã hóa là (HS_DIV - 4).
    regs[0] = ((hs_div - 4) << 5) | ((n1_value_to_write >> 2) & 0x1F);

    // R8: N1_value (7-6) và RFREQ (37-32)
    // SỬA LỖI: RFREQ 38-bit phải dịch >> 32 cho R8
    regs[1] = ((n1_value_to_write & 0x03) << 6) | ((uint8_t)(rfreq >> 32) & 0x3F);

    // R9: RFREQ (31-24)
    // SỬA LỖI: RFREQ phải dịch >> 24
    regs[2] = (uint8_t)(rfreq >> 24);

    // R10: RFREQ (23-16)
    // SỬA LỖI: RFREQ phải dịch >> 16
    regs[3] = (uint8_t)(rfreq >> 16);

    // R11: RFREQ (15-8)
    // SỬA LỖI: RFREQ phải dịch >> 8
    regs[4] = (uint8_t)(rfreq >> 8); 

    // R12: RFREQ (7-0)
    // SỬA LỖI: Bổ sung thanh ghi R12 (byte cuối cùng của RFREQ)
    regs[5] = (uint8_t)rfreq; 

    // 4. Ghi các thanh ghi mới (từ R7 đến R12, tổng cộng 6 thanh ghi)
    for (int i = 0; i < 6; i++) { // SỬA LỖI: Chạy tới 6 (cho R7..R12)
        if (si570_write_reg(handle->fd, 7 + i, regs[i]) != 0) {
            // Cố gắng mở băng trước khi thoát
            si570_write_reg(handle->fd, 137, 0x00);
            return -1;
        }
    }

    // 5. Mở băng DCO (Thanh ghi 137, set về 0x00)
    if (si570_write_reg(handle->fd, 137, 0x00) != 0) return -1;
    
    // 6. Kích hoạt tần số mới (NewFreq) (Thanh ghi 135, bit 6)
    if (si570_write_reg(handle->fd, 135, 0x40) != 0) return -1;

    printf("[SI570 HAL] Lập trình tần số thành công.\n");
    return 0;
}

void si570_close(Si570_Handle *handle) {
    if (handle && handle->fd >= 0) {
        close(handle->fd);
        handle->fd = -1;
        printf("[SI570 HAL] Đã đóng kết nối.\n");
    }
}

// Hàm đọc tần số thực tế từ chip Si570
int si570_get_current_freq(Si570_Handle *handle, double *current_freq_mhz) {
    if (!handle || handle->fd < 0) return -1;

    uint8_t regs[6];
    
    // Sử dụng hàm si570_read_regs bạn đã định nghĩa ở trên thay vì i2c_read_bytes
    // Thanh ghi bắt đầu từ 7, đọc 6 byte (từ thanh ghi 7 đến 12)
    if (si570_read_regs(handle->fd, 7, regs, 6) != 0) {
        fprintf(stderr, "[SI570 HAL] Lỗi đọc thanh ghi để tính tần số.\n");
        return -1; 
    }

    // --- Giải mã thông số theo Datasheet Si570 ---
    
    // 1. HS_DIV (Thanh ghi 7, bits 7-5)
    uint32_t hs_div_val = ((regs[0] & 0xE0) >> 5) + 4;

    // 2. N1 (Thanh ghi 7 bits 4-0 và thanh ghi 8 bits 7-6)
    uint32_t n1_val = ((regs[0] & 0x1F) << 2) | ((regs[1] & 0xC0) >> 6);
    n1_val = n1_val + 1; // n1 = giá trị thanh ghi + 1

    // 3. RFREQ (38-bit: Thanh ghi 8 bits 5-0 đến thanh ghi 12)
    uint64_t rfreq_raw = ((uint64_t)(regs[1] & 0x3F) << 32) |
                         ((uint64_t)regs[2] << 24) |
                         ((uint64_t)regs[3] << 16) |
                         ((uint64_t)regs[4] << 8)  |
                         ((uint64_t)regs[5]);

    // Chuyển đổi RFREQ sang số thực (chia cho 2^28)
    double rfreq_float = (double)rfreq_raw / (double)(1ULL << 28);

    // 4. Tính toán tần số đầu ra (MHz)
    // Công thức: f_out = (f_xtal * RFREQ) / (HS_DIV * N1)
    // handle->config.fxtal_hz đơn vị Hz, nên chia 1.000.000 để ra MHz
    double f_xtal_mhz = handle->config.fxtal_hz / 1000000.0;
    *current_freq_mhz = (f_xtal_mhz * rfreq_float) / (double)(hs_div_val * n1_val);
    
    return 0;
}

void SI570_Set_Freq_MHz(double target_freq)
{
    // --- KIỂM TRA TRẠNG THÁI LINK CMAC TRƯỚC ---
    // Nếu Link 100G đã Up (0x03), không nên cấu hình lại Clock vì sẽ làm rớt Link
    uint32_t cmac_status = Xil_In32(0xA0270000 + 0x0204); 
    if ((cmac_status & 0x03) == 0x03) {
        printf("[SKIP] CMAC Link is already UP. Skipping Si570 configuration to maintain stability.\n");
        return;
    }

    Si570_Config my_si570_config = {
        .i2c_bus_path = "/dev/i2c-3",
        .i2c_addr = 0x5D,
        .fxtal_hz = 114285000.0 // Tần số thạch anh nội bộ chuẩn của Si570
    };

    Si570_Handle my_si570_handle;

    if (si570_init(&my_si570_handle, &my_si570_config) != 0) {
        fprintf(stderr, "Không thể khởi tạo Si570 HAL.\n");
        return;
    }

    // --- KIỂM TRA TẦN SỐ TRÊN CHIP (Đã chính xác) ---
    double current_f = 0.0;
    if (si570_get_current_freq(&my_si570_handle, &current_f) == 0) {
        // So sánh tần số hiện tại với mục tiêu (cho phép sai số 100Hz)
        if (fabs(current_f - target_freq) < 0.0001) {
            printf("[INFO] Si570 current freq is %.6f MHz. Matches target. Skipping.\n", current_f);
            si570_close(&my_si570_handle);
            return;
        }
        printf("[INFO] Si570 current freq is %.6f MHz. Target is %.6f MHz. Proceeding.\n", current_f, target_freq);
    }

    // Thiết lập tần số nếu chưa đúng
    if (si570_set_frequency(&my_si570_handle, target_freq) != 0) {
        fprintf(stderr, "Không thể thiết lập tần số mới.\n");
    } else {
        printf("Si570 set to %.6f MHz done\n", target_freq);
    }

    usleep(100000);

    if (si570_get_current_freq(&my_si570_handle, &current_f) == 0) 
    {
        printf("[INFO] Si570 current freq is %.6f MHz. Target is %.6f MHz. Proceeding.\n", current_f, target_freq);
    }

    si570_close(&my_si570_handle);
}
