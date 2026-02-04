#include "linux_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <errno.h>
#include <sys/mman.h>

// =============================================================================
// PHẦN 6: SPI
// =============================================================================

// Hàm khởi tạo DUY NHẤT
int SPI_Init_Linux(LinuxSpiDevice *spi_dev, const char* path, uint8_t mode, uint32_t speed) {
    // 1. Lưu lại các thông tin cấu hình
    strncpy(spi_dev->device_path, path, sizeof(spi_dev->device_path) - 1);
    spi_dev->mode = mode;
    spi_dev->bits = 8; // Hard-code 8 bits cho hầu hết các chip
    spi_dev->speed = speed;
    spi_dev->fd = -1;

    // 2. Mở file thiết bị
    spi_dev->fd = open(path, O_RDWR);
    if (spi_dev->fd < 0) {
        fprintf(stderr, "[SPI] Lỗi mở %s: %s\n", path, strerror(errno));
        return XST_FAILURE;
    }

    // 3. Cấu hình mode (thêm SPI_NO_CS để mô phỏng MANUAL_SSELECT)
    uint8_t final_mode = spi_dev->mode;
    if (ioctl(spi_dev->fd, SPI_IOC_WR_MODE, &final_mode) < 0) {
        perror("[SPI] Lỗi cấu hình SPI mode");
        goto spi_init_fail;
    }

    // 4. Cấu hình số bit
    if (ioctl(spi_dev->fd, SPI_IOC_WR_BITS_PER_WORD, &spi_dev->bits) < 0) {
        perror("[SPI] Lỗi cấu hình bits per word");
        goto spi_init_fail;
    }
    
    // 5. Cấu hình tốc độ
    if (ioctl(spi_dev->fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_dev->speed) < 0) {
        perror("[SPI] Lỗi cấu hình max speed");
        goto spi_init_fail;
    }

    return XST_SUCCESS;

spi_init_fail:
    close(spi_dev->fd);
    spi_dev->fd = -1;
    return XST_FAILURE;
}

/**
 * @brief Hàm truyền/nhận SPI chung
 */
int SPI_Transfer(int fd, uint8_t *TxBuffer, uint8_t *RxBuffer, uint32_t Bytecount) {
    if (fd < 0) return -1;

    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)TxBuffer,
        .rx_buf = (unsigned long)RxBuffer,
        .len = Bytecount,
    };

    if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 1) {
        perror("[SPI] FAILED to transfer message");
        return -1;
    }
    return 0;
}

void SPI_Close_Linux(LinuxSpiDevice *spi_dev) {
    if (spi_dev && spi_dev->fd >= 0) {
        close(spi_dev->fd);
        spi_dev->fd = -1;
    }
}