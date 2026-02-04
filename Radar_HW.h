/*
 * Radar_HW.h
 *
 *  Created on: Jun 30, 2020
 *      Author: TienNam
 */

#ifndef SRC_RADAR_HW_H_
#define SRC_RADAR_HW_H_

#include "DK_Maythu.h"



void Radar_Intr_DMA(void);
int Radar_XDmaPs_Innit(u16 DeviceId);
//int XUartLite_SelfTest(XUartLite *InstancePtr);
int UART0_Innit(void);
int UART1_Innit(void);
int UART2_Innit(void);
int UART3_Innit(void);
int UART4_Innit(void);

void UART_Frame_Innit(void);
void UART0_Send_Frame(unsigned char Length);
void UART1_Send_Frame(unsigned char Length);
void UART2_Send_Frame(unsigned char Length);
void UART3_Send_Frame(unsigned char Length);
void UART4_Send_Frame(unsigned char Length);
void Radar_Innit_Uart(void);
//void InitializeECC(XBram_Config *ConfigPtr, u32 EffectiveAddr);
int Radar_Innit_RAM(u16 DeviceId);
void Radar_Innit_SPI(void);
void Radar_Innit_GPIO(void);
void Radar_Set_Z_encoder(unsigned char Val);
void Radar_Write_Uart0(	unsigned char Data,
							unsigned char Data1,
							unsigned char Data2,
							unsigned char Data3,
							unsigned char Data4);
void Radar_Check_motor(void);

#endif /* SRC_RADAR_HW_H_ */
