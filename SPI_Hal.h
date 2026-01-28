#ifndef SPI_HAL_H
#define SPI_HAL_H

#include "linux_hal.h"

// --- Stub cho SPI ---
typedef struct { uint32_t BaseAddress; } XSpi_Config;
#define XSpi_LookupConfig(Id) ((XSpi_Config*)1)
#define XSpi_CfgInitialize(Inst, Cfg, Addr) XST_SUCCESS

#define XPAR_AXI_QUAD_SPI_0_DEVICE_ID 0
#define XPAR_AXI_QUAD_SPI_1_DEVICE_ID 1
#define XPAR_AXI_QUAD_SPI_2_DEVICE_ID 2
#define XPAR_XGPIOPS_0_DEVICE_ID      0
#define GPIO_PS_DEVICE_ID             0

// Định nghĩa lại cấu trúc XSpi để tương thích với code cũ của bạn
typedef struct {
    int fd;           // File descriptor của spidev
    char device_path[64];
    uint8_t mode;     // SPI Mode (0,1,2,3)
    uint32_t speed;   // Tốc độ Hz
    uint8_t bits;
    int spi_bus_id;   // ID để debug
} LinuxSpiDevice;

int SPI_Init(LinuxSpiDevice *spi_dev, const char* path, uint8_t mode, uint8_t bits, uint32_t speed);
int SPI_Transfer(int fd, uint8_t *TxBuffer, uint8_t *RxBuffer, uint32_t Bytecount);
void SPI_Close_Linux(LinuxSpiDevice *spi_dev);

#endif // SPI_H