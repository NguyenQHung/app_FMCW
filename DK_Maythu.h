/*
 * DK_Maythu.h
 *  Ported to Linux user-space (aarch64) - Jan 2026
 */

#ifndef DK_MAYTHU_H_
#define DK_MAYTHU_H_

#include "main.h"

#define _Beam1_Max 		1
#define _Beam2_Max 		2
#define _Beam3_Max 		3
#define _Beam4_Max 		4
#define _Beam5_Max 		5
#define _Beam6_Max 		6
#define _Beam7_Max 		7
#define _Beam8_Max 		8
#define _Beam9_Max 		9
#define _Beam10_Max 	10
#define _Beam11_Max 	11
#define _Beam12_Max 	12
#define _Beam13_Max 	13
#define _Beam14_Max 	14
#define _Beam15_Max 	15
#define _Beam16_Max 	16
#define _Beam17_Max 	17
#define _SS6_Max 		18
#define _SS1_Max 		19
#define _SS7_Max 		20
#define _SS2_Max 		21
#define _SS3_Max 		22
#define _SS8_Max 		23
#define _SS4_Max 		24
#define _SS9_Max 		25
#define _V1_Max 		26
#define _V2_Max 		27

void Radar_Intr_Handler(void *baseaddr_p);
void ZDMA1_DoneHandler(void *CallBackRef);
void ZDMA2_DoneHandler(void *CallBackRef);
void ZDMA3_DoneHandler(void *CallBackRef);
void ZDMA4_DoneHandler(void *CallBackRef);
//int Radar_InterruptSystemSetup(XScuGic *XScuGicInstancePtr);
int Radar_IntcInitFuntion(	u16 		DeviceId,
							XGpio 		*GpioInstancePtr,
							XGpio 		*GpioInstancePtr2);
void Radar_Innit_Interrupt(void);
void Radar_Enable_Interrupt(void);
double fixed32_to_double(int32_t input, uint8_t fractional_bits);
void Radar_Update_Phase1(uint16_t Sine, uint16_t cos);
void Radar_Update_Phase2(uint16_t Sine, uint16_t cos);
void Radar_Update_Phase3(uint16_t Sine, uint16_t cos);
void Radar_Update_Phase4(uint16_t Sine, uint16_t cos);
void Radar_Update_Phase5(uint16_t Sine, uint16_t cos);
void Radar_Update_Phase6(uint16_t Sine, uint16_t cos);
void Radar_Update_Phase7(uint16_t Sine, uint16_t cos);
void Radar_Update_Phase8(uint16_t Sine, uint16_t cos);
void Radar_Update_Phase9(uint16_t Sine, uint16_t cos);
void Radar_Update_Phase10(uint16_t Sine, uint16_t cos);
void Radar_Update_Phase11(uint16_t Sine, uint16_t cos);
void Radar_Update_Phase12(uint16_t Sine, uint16_t cos);
void Radar_Update_Phase13(uint16_t Sine, uint16_t cos);
void Radar_Update_Phase14(uint16_t Sine, uint16_t cos);
void Radar_Update_Phase15(uint16_t Sine, uint16_t cos);
void Radar_Update_Phase16(uint16_t Sine, uint16_t cos);

void Radar_Update_V1(uint16_t Beam, uint16_t Sine, uint16_t cos);
void Radar_Update_V2(uint16_t Beam, uint16_t Sine, uint16_t cos);
void Radar_Update_V3(uint16_t Beam, uint16_t Sine, uint16_t cos);
void Radar_Update_V4(uint16_t Beam, uint16_t Sine, uint16_t cos);

void Caculator_Beam_Value(double H_angle, double E_angle);
void Radar_Update_DAC(uint16_t Beam, uint16_t Sine, uint16_t cos);
void _Update_DAC_Over_Beam(int Beam);
void _Update_DAC_Over_Angle(void);
void Radar_Innit_DAC_Phase(void);

void _Khao_Sat_Phuong_Vi(void);
void _Khao_Sat_Get_Max(void);

// boloc kalman
void kalman_E_khoi_tao(double q_param, double r_param, double x_ban_dau, double p_ban_dau);
void kalman_H_khoi_tao(double q_param, double r_param, double x_ban_dau, double p_ban_dau);
double kalman_E_cap_nhat(double gia_tri_do);
double kalman_H_cap_nhat(double gia_tri_do);
void kalman_Innit(void);

void _BS_Mode_Scan_Quat(void);
void _BS_Mode_Scan_Normal(void);
void _BS_Work_Off(void);
void _BS_First_Process(double Fix_Angle);
void _BS_Second_Process(void);
void _BS_Reload_Work(void);
void _BS_Reload_H_E(void);
void _BS_E_Mode_Angle(void);
void _Sub_E_Mode0(void);
void _Sub_E_Mode1(void);
void _Sub_E_over_Center(void);
void _Sub_E_over_Limmiter(void);
void _Sub_BamSat_E_Speed(void);
void _Sub_E_Mode_P0(void);
void _Sub_E_Mode_P1(void);
void _Sub_BamSat_E_Control_Motor(void);
void _BS_Cal_New_E(void);
void _Sub_H_Mode0(void);
void _Sub_H_Mode1(void);
void _Sub_H_control_EH_Ready(void);
void _Sub_H_over_Center(void);
void _Sub_H_Mode_P0(void);
void _Sub_H_Mode_P1(void);
void _Sub_BamSat_H_Speed(void);
void _Sub_BamSat_H_Control_Motor(void);
void _Sub_BamSat_H_Control_Fix_Over_Point(void);
void _BS_Cal_New_H(void);
void _BS_Culy_Process(void);
void _BS_Mode_0_Process(void);
void _BS_Mode_1_Process(void);
void _BS_Check_Target_in_Window(void);
void _BS_Culy_Process(void);
void _BS_First_Click(void);
void _BS_Second_Click(void);
void Update_V1(void);
void Update_V2(void);
void Update_V3(void);
void Update_V4(void);
void _Get_Phase_calculator(void);
void Innit_Beam_data(void);
uint32_t bytes_to_uint32(uint8_t* bytes, uint16_t S1, uint16_t S2, uint16_t S3, uint16_t S4);
void _Check_Beam_config(void);
void Caculator_V1_Value(void);
void Caculator_V2_Value(void);
void Caculator_V3_Value(void);
void Caculator_V4_Value(void);
void Radar_Innit_Phase(void);
void Radar_Innit_Frame(void);
void Radar_Read_Frame(void);
void Innit_Radar_Debug(void);
void Radar_Update_DDS_Phase(void);
void Radar_Reload_Frame(void);
void Radar_Read_Encoder(void);
void Radar_taogia_Encoder(void);
void Radar_Frame_Phase(void);
void Radar_check_Disconect(void);
void Radar_Setup_Header(void);
void Radar_Update_PC1_Packet(void);
void Radar_Update_PC2_Packet(void);
void Radar_Update_Uart_DKSG(void);
void Radar_Update_Uart_Motor1(void);
void Radar_Update_Uart_Motor2(void);
void Erob_Control_Bamsat_PC8_E(void);
void Erob_Control_Bamsat_PC8_H(void);
int compare_ints(const void *a, const void *b);
void median_filter(int *input, int *output, int N, int window_size);
void Radar_Setup_AFC(void);
void Radar_Read_PC8_Excel(void);
void Radar_Load_Config(void);
void Radar_Setup_HW(void);
void Radar_Update_ATT(void);
void PC8_Read_Phase_Data(void);
void PC8_Get_ADC_Val(void);
void Radar_Read_PC8_CMD(void);
void PC8_Get_Phase(struct UDP_Phase *UDP_Phase, u32 Addr);
void PC8_Get_Phase1(u32 Addr);
void PC8_Get_Data(void);
void PC8_Read_RAM_Data(void);
void PC8_Control_HW(void);
void PC8_Get_Data_Min_Max(void);
void PC8_innit_Bamsat(void);
void PC8_Get_BeamMax_data(void);
void PC8_Get_V1234_data(void);
void PC8_Caculator_AVG_Data(void);
void PC8_Get_BeamMax_channel(void);

//void PC8_High_Speed_BeamMax(void);
void Get_beam17_Delta_Angle(void);
int PC8_Check_Beta_In_Window(void);
void PC8_Get_BeamMax_channel(void);
void _Sub_Send_Data_Mode0(void);
void _Sub_Send_Data_Mode1(void);
void _Get_SS1_SS2_SS3_SS4_Max_Val(void);

int _Get_angle_E(uint8_t Beam);
int _Get_angle_H(uint8_t Beam);
double Convert_int26(uint data);
void _Get_Max_Beam(void);
//void _Get_Max_Beam17(void);
void _Get_Max_Point_data(uint8_t point);
void _Get_Max_Point_channel(uint8_t channel, uint8_t point);
void _Cal_Beam_AVG_Data(uint8_t channel);
void _Cal_Beam_Max(void);
void _Cal_AVG_I_Q_Data(void);
void _Caculator_point(uint16_t i, uint16_t point1, uint16_t point2, uint16_t point3, uint16_t point4);
void _Caculator_IQ_point(uint16_t i, uint16_t point1, uint16_t point2, uint16_t point3, uint16_t point4);
void _Get_Max_Point_V1234(void);
void Radar_Innit(void);
void Radar_Anten_Speed(void);
void Radar_COMPASS(void);
void Radar_GPS_Longitude(void);
void Radar_Intr_Handler(void *baseaddr_p);
void GPS_Intr_Handler(void *baseaddr_p);
void COMPASS_Intr_Handler(void *baseaddr_p);
void COMPASS_A_Intr_Handler(void *baseaddr_p);
void ANTEN_Intr_Handler(void *baseaddr_p);
void PC2_Get_Frame(void);
void Radar_Show_Freq(void);
void Innit_Radar_Debug(void);
void Radar_Update_GPS_Wind(void);
void Radar_Frame_GPS_Wind(void);
void Radar_Update_HW4(void);

// Cua_song.c
void Cua_Song_Innit(void);
void Cua_Song_Recal_Range(void);
void Cua_Song_Auto_Fix(void);
void Cua_Song_Cal_New_center(void);
void Cua_Song_Cal_New_Limit(void);
void Cua_Song_Cal_target_information(void);

#endif /* DK_MAYTHU_H_ */
