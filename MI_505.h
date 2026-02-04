/*
 * MI_505.h
 *
 *  Created on: Apr 7, 2020
 *      Author: TienNam
 */

#ifndef _MI_505_H_
#define _MI_505_H_

#include "DK_Maythu.h"

void Radar_Hyper_Output(unsigned int Channel, unsigned int Data);
unsigned int Radar_Hyper_Input(unsigned char Channel);

void Motor1_Mode_Freq(unsigned int Freq, unsigned char Dir, unsigned char Enable);
void Motor1_Mode_Pulse(unsigned int Pulse, unsigned char Dir, unsigned char Enable);
void Motor1_Set_Run_Mode(unsigned char Mode);
void Motor1_Set_Mode(unsigned int ENC_Center, unsigned int ENC_Start, unsigned int ENC_Stop);
void Motor1_Send_ENC(unsigned int ENC);
void Motor2_Mode_Freq(unsigned int Freq, unsigned char Dir, unsigned char Enable);
void Motor2_Mode_Pulse(unsigned int Pulse, unsigned char Dir, unsigned char Enable);
void Motor2_Set_Run_Mode(unsigned char Mode);
void Motor2_Set_Mode(unsigned int ENC_Center, unsigned int ENC_Start, unsigned int ENC_Stop);
void Motor2_Send_ENC(unsigned int ENC);
void CAN1_Set_Dir(unsigned int Dir);
void CAN2_Set_Dir(unsigned int Dir);
void IP_Set_RST(unsigned int Val);
void IP_Set_Range(unsigned int Val);
//void IP_Set_SW(unsigned int Val);
//void IP_Set_Target_Mode(unsigned int Val);
void IP_Set_DAC_Mode(unsigned int _IP_SW_mode, unsigned int _IP_target_Mode, float Scale);
void System_Set_IP_code(unsigned int Val);
void IP_Set_dmt(unsigned int Val);
void IP_Set_amplitude(float Scale);
void IP_Set_thang_culy(unsigned int Val);
void IP_Set_Beta_Mode(unsigned int Val);
void IP_Set_Beta_in(unsigned int Val);
void IP_Set_Beta_start(unsigned int Val);
void IP_Set_Beta_stop(unsigned int Val);
void System_Set_ADC_Sel(unsigned int Val);
void System_Set_RS485_DIR1(unsigned int Val);
void System_Set_RS485_DIR2(unsigned int Val);
void System_Set_RF_ON_OFF(unsigned int Val);
void System_Set_IP_Pulse_Width(unsigned int Val);
void System_Set_RF_ON_OFF2(unsigned int Val);
void System_Set_RF_ENABLE(unsigned int Val);
void System_Set_Trigger_time1(unsigned int Val);
void System_Set_Trigger_time2(unsigned int Val);
void System_Set_Phase_Start_Read(unsigned int Val);
void System_Set_ADC_SEL(unsigned int Val);
void System_Set_ADC_SEL2(unsigned int Val);
void System_Set_ATT_Dmax(unsigned int Val);
void System_Set_ATT_TX_Sel(unsigned int Val);
void System_Set_ADC_Channel(unsigned int Val);
void System_Set_IQ_Channel1(unsigned int Val);
void System_Set_IQ_Channel2(unsigned int Val);
void System_Set_RX_SEL(unsigned int Val);
void System_Set_DAC_ON_OFF(unsigned int Val);
void System_Set_RDY_EN(unsigned int Val);
void System_Set_FIFO_RST(unsigned int Val);
void System_Set_trigger(unsigned int mode, unsigned int Width, unsigned int Max);
void System_Set_ATT_Data(	unsigned int Val1,
							unsigned int Val2,
							unsigned int Val3,
							unsigned int Val4,
							unsigned int Val5,
							unsigned int Val6,
							unsigned int Val7,
							unsigned int Val8,
							unsigned int Val9,
							unsigned int Val10,
							unsigned int Val11,
							unsigned int Val12,
							unsigned int Val13,
							unsigned int Val14,
							unsigned int Val15,
							unsigned int Val16,
							unsigned int Val17);

int IIR_LPF(int32_t v);

void System_Set_Beta_Epxilon_sel(unsigned int Val);
void System_Set_BS_Enable(unsigned int Val);
void System_Set_BS_Angle(unsigned int Val);
void System_Set_BS_Start(unsigned int Val);
void System_Set_BS_Stop(unsigned int Val);
void System_Set_BS_Channel(unsigned int Val);
void System_Set_BS_Sel(unsigned int Val);
void System_Set_BS_Size(unsigned int Val);
void System_Set_BS_Start4(unsigned int Val);
void System_Set_BS_Stop4(unsigned int Val);
void System_Set_BS_Size4(unsigned int Val);
void System_Set_Beam_start(unsigned int Val);
void System_Set_Beam_stop(unsigned int Val);
void System_Set_Beam_size(unsigned int Val);
void System_Set_Beam_Max_Cell(unsigned int Val);
void System_Set_Scope_Delay(unsigned int Val);
void System_Set_Beam17_Bamsat(unsigned int Thang_cu_ly);
void System_Set_Beam17_RAM(unsigned int Val);
void System_Set_Beam_Sel(unsigned int Val);
void System_Set_Beam_Channel(unsigned int Val);
void System_Set_fb_Channel(unsigned int Val);
void Radar_Control_Beta(void);
void Radar_Control_Alpha(void);
void Radar_Control_IO(void);
void Radar_SET_AFC_Point(unsigned char data1);
void Radar_SET_AFC_Trigger(unsigned char data1);
void System_Set_IQ_View(unsigned int Val);
void System_Set_Mixer_Sel(unsigned int Val);
unsigned int gray_to_binary(unsigned int gray);
unsigned int grayToBinary(unsigned int num);
unsigned int Radar_Read_Beta(void);
unsigned int Radar_Read_Alpha(void);
unsigned int Radar_Read_Input(void);
unsigned int Radar_Read_Phuongvi(void);
void Radar_Read_Volt(void);
void Radar_Set_ouput(unsigned char Phat_tx,
					 unsigned char Soidot_tx,
					 unsigned char Gpio);
void Radar_Set_ouput2(	unsigned char Phat_tx,
						unsigned char May_2_2A,
						unsigned char ANTEN,
						unsigned char AFC,
						unsigned char Soi_Dot,
						unsigned char Vi_Phan);
void Radar_Anten_Speed(void);
void Radar_Set_Pulse(unsigned int Pulse_width);
void Radar_SET_Tan_So_Lap(unsigned int Tlap);
void System_Set_IP_B17_Sel(unsigned int Val);
void System_Set_IP_Bx_Sel(unsigned int Val);
void System_Set_IP_code_Sellect(unsigned int Val);
void System_Set_RF_ON_OFF2(unsigned int Val);
void Radar_Show_Freq(void);
void Radar_Update_VCO(void);
void Radar_Update_AFC(void);
void Radar_Set_Chong_Nhieu_Start_Point(unsigned int data1);
void Radar_SET_Chong_Nhieu_Rangle(unsigned int data1);
void Radar_Update_Chong_Nhieu(void);
void Radar_SET_ENC_Mode(unsigned char Mode);
void System_Set_IP_PulseSel(unsigned int Val);
void System_Set_FFT_Sel(unsigned int Val);
void System_Set_sum_Sel(unsigned int Val);

#endif /* SRC_MI_505_H_ */
