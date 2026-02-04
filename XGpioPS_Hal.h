#ifndef XGPIOPS_HAL_H
#define XGPIOPS_HAL_H

#include "linux_hal.h"

// Struct này bây giờ gần như trống, chỉ để tương thích API.
typedef struct {
    // Không cần thành viên nào
} XGpioPs_Config;

// Struct này cũng trống, vì chúng ta sẽ xác định GPIO cần điều khiển
// dựa trên con trỏ InstancePtr (ví dụ: if (InstancePtr == &ADCV4_PS) ... )
typedef struct {
    // Không cần thành viên nào
} XGpioPs;


int XGpioPs_CfgInitialize(XGpioPs *InstancePtr, XGpioPs_Config *ConfigPtr, unsigned long EffectiveAddr);
void XGpioPs_SetDirectionPin(XGpioPs* InstancePtr, int Pin, int Direction);
void XGpioPs_SetOutputEnablePin(XGpioPs* InstancePtr, int Pin, int IsOutput) ;
void XGpioPs_WritePin(XGpioPs* InstancePtr, int Pin, int State);
int XGpioPs_ReadPin(XGpioPs* InstancePtr, int Pin);
// Thêm hàm dọn dẹp
void XGpioPs_Cleanup(void);

#endif // XGPIOPS_HAL_H