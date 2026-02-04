#ifndef XUARTPS_HAL_H
#define XUARTPS_HAL_H

#include "linux_hal.h"

// Định nghĩa thanh ghi UART PS (dựa trên tài liệu Xilinx)
#ifndef XUARTPS_FIFO_OFFSET
#define XUARTPS_FIFO_OFFSET 0x0030  // Offset của thanh ghi FIFO UART
#endif

// Định nghĩa hàm đọc thanh ghi giả lập
#ifndef XUartPs_ReadReg
#define XUartPs_ReadReg(BaseAddress, RegOffset) \
    (*(volatile uint32_t *)((BaseAddress) + (RegOffset)))
#endif

// Định nghĩa rỗng các hàm không có trong Linux
#define Xil_DCacheFlushRange(addr, len)
#define isb()

// Định nghĩa lệnh rào cản chỉ thị (Instruction Synchronization Barrier)
#ifndef isb
#define isb() __asm__ __volatile__ ("isb sy" : : : "memory")
#endif

// Memory Barrier cho ARMv8
#define dsb() __asm__ __volatile__ ("dsb sy" : : : "memory")

// Stub cho UART (để không bị lỗi link)
#define XUartPs_IsReceiveData(addr) (0)
#define XCOREIGHTPS_DCC_STATUS_RX   (0x01)
#define my_XcoresightPs_DccGetStatus() (0)


typedef struct {
    uint32_t BaseAddress;
} XUartPs;


#endif // XUARTPS_HAL_H