#ifndef XUART_HAL_H
#define XUART_HAL_H

#include "linux_hal.h" // Include file header của chính nó


// --- Stub cho UARTLite (Vì Linux dùng driver ttyUL) ---
typedef struct {
    int fd;
    const char* device_path;
} XUartLite;

// --- Định nghĩa các XPAR thiếu (Lấy từ Address Editor của bạn) ---
#define XPAR_UARTLITE_0_BASEADDR 0xA00A0000
#define XPAR_UARTLITE_1_BASEADDR 0xA00B0000
#define XPAR_UARTLITE_2_BASEADDR 0xA0070000
#define XPAR_UARTLITE_3_BASEADDR 0xA0080000
#define XPAR_UARTLITE_4_BASEADDR 0xA0090000

// Hàm helper để cấu hình các thông số Serial (Baudrate 115200, 8N1)
int uart_configure(int fd);
// Hàm khởi tạo UART chung cho Linux
int XUartLite_Initialize_Linux(XUartLite *Instance, const char* path);
// Các hàm Init cụ thể (Refactored)
int UART_Generic_Init(XUartLite *Instance, const char* path, const char* name);

#endif // XUART_HAL_H