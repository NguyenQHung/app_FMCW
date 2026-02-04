#ifndef XGPIO_HAL_GPIOD_H
#define XGPIO_HAL_GPIOD_H

// #include <stdint.h>
// #include <gpiod.h> // Có thể cần include ở đây

#include "linux_hal.h"
#include <stdint.h> // Thêm cho uint8_t, uint32_t, v.v. (thay xil_types.h)

// --- CÁC ĐỊNH NGHĨA KIỂU DỮ LIỆU CŨ ---
// Giả sử đây là định nghĩa gốc của bạn từ Xilinx hoặc một định nghĩa tạm
// để code biên dịch được. Rất quan trọng là không thay đổi nó.
typedef struct {
  uint32_t BaseAddress;
  uint32_t IsReady;
  uint32_t index;
  // ... các trường khác nếu có
} XGpio;

#define XST_SUCCESS 0
#define XST_FAILURE -1

// --- KHAI BÁO CÁC BIẾN INSTANCE TOÀN CỤC ---
// Những biến này phải được định nghĩa ở một file .c nào đó trong dự án của bạn
extern XGpio GPIO_HS1;
extern XGpio GPIO_HS2;
extern XGpio GPIO_HS3;
extern XGpio GPIO_ENDFRAME_IRQ;
extern XGpio GPIO_ENDFRAME_IRQ1;
extern XGpio GPIO_SYNC_CMAC;

// --- CÁC HÀM XỬ LÝ NGẮT ---
int XGpio_Interrupt_Innit();
int XGpio_Interrupt_IRQ();

// --- CÁC HÀM API GIỮ NGUYÊN ---
int XGpio_Initialize(XGpio *InstancePtr, uint32_t Chip_index);
void XGpio_SetDataDirection(XGpio *InstancePtr, int Channel,
                            uint32_t DirectionMask);
void XGpio_DiscreteWrite(XGpio *InstancePtr, int Channel, uint32_t Data);
uint32_t XGpio_DiscreteRead(XGpio *InstancePtr, int Channel);
void XGpio_CleanupAll(void);

#endif // XGPIO_HAL_GPIOD_H