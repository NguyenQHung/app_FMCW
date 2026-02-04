/*
 * Erob.h
 *
 *  Created on: Jul 24, 2023
 *      Author: tienn
 */

#ifndef SRC_EROB_H_
#define SRC_EROB_H_

#include "main.h"

void Erob_Motor_Control(uint32_t Pulse, uint8_t Dir, uint8_t EN, uint8_t Motor);
void Erob_Motor_Innit();
void Erob_Motor_Send_Frame(uint8_t Length, uint8_t Motor);
void Erob_Motor_Enable(uint8_t Motor);
void Erob_Motor_Disable(uint8_t Motor);
void Erob_Motor_Stop_Motion(uint8_t Motor);
void Erob_Motor_Start_to_Move(uint8_t Motor);
void Erob_Motor_Read_Speed(uint8_t Motor);
void Erob_Motor_Read_Position(uint8_t Motor);
void Erob_Motor_Control_Mode3(uint8_t Motor);
void Erob_Motor_Control_Mode0(uint8_t Motor);
void Erob_Motor_Set_ACC(uint32_t Val, uint8_t Motor);
void Erob_Motor_Set_DEC(uint32_t Val, uint8_t Motor);
void Erob_Motor_Set_SPEED(uint32_t Val, uint8_t Motor);
void Erob_Motor_Read_Current(uint8_t Motor);
void Erob_Motor_Read_Voltage(uint8_t Motor);
void Erob_Motor_Read_Temerature(uint8_t Motor);
void Erob_Motor_Read_Encoder(uint8_t Motor);
void Erob_Motor_PhuongVi_Get_CMD(void);
void Erob_Motor_GocTa_Get_CMD(void);
void Erob_Control_Motor1_Rotate(void);
void Erob_Motor_PhuongVi_Get_CMD(void);
void Erob_BS_Phuong_vi_pulse(void);
void Erob_BS_Goc_ta_pulse(void);
void _Tichluy_Epxilon_Phi(void);
void Erob_BS_Window(void);
void _Process_Amplitude(void);
void Clear_Check_Max(void);
void Erob_Get_CMD(void);
void Erob_IO_CMD(void);
void Erob_Control_Bamsat1(void);
void Erob_Control_Bamsat2(void);
void Erob_Control_Pulse_Mode1(void);
void Erob_Control_Pulse_Mode2(void);
void Erob_Control_Motor1(void);
void Erob_Control_Motor2(void);
void Erob_Get_ENC1_Data(void);
void Erob_Get_ENC2_Data(void);
void Erob_Read_Posision(void);
void Erob_Process(void);
void Erob_Read_UART(void);
void Erob_Read_ENC1(void);
void Erob_Read_ENC2(void);
void Erob_Motor_Stop(uint8_t Motor);
void Erob_Data_Innit(void);

#endif /* SRC_EROB_H_ */
