#include "DK_Maythu.h"
#include "ADF4159.h"
#include "ADF4360.h"
#include "Radar_HW.h"
#include "MI_505.h"
#include "Erob.h"
//#include "xparameters.h"
#include "CAN_Control.h"
#include "User_RAM.h"
#include "linux_hal.h"      // Lớp trừu tượng phần cứng (sysfs GPIO)

/****************************************************************************/

int pi = 0;
int UDP_Counter = 0;

void Radar_Update_ATT(void)
{

	if(PC8.VCO_EN == 1)
	{
		if((PC8.VCO_Val1i != PC8.VCO_Val1)||(PC8.VCO_time > 30))
		{
			PC8.VCO_time = 0;
			PC8.VCO_Val1i = PC8.VCO_Val1;
			ADF4360_Set_Freq(PC8.VCO_Val1);
		}

		if((PC8.VCO_Val2i != PC8.VCO_Val2)||(PC8.VCO_time2 > 60))
		{
			PC8.VCO_time2 = 0;
			PC8.VCO_Val2i = PC8.VCO_Val2;
			ADF4159_Set_Freq(PC8.VCO_Val2);
		}

		PC8.VCO_time++;
		PC8.VCO_time2++;

	}


	Radar.Range 		= 	5;
	Radar.Sample		=	5220;
	Radar.ADC_Freq0		=	10;
	Radar.Noise_Clear 	= 	1;
	Radar.Noise_counter	=	0;

	Radar.Range2		=	5;
	Radar.ADC_Cells		=	4096;
	Radar.ADC_Cells2	=	106800;
	Radar.ADC_Time_us	=	820;
	Radar.ADC_Num_CLK	=	41000;
	Radar.ADC_Pulse_CLK =	200;

	IP_Set_thang_culy(5);//Radar.Range);
	System_Set_Beam17_RAM(Radar.Range);
	Radar.ADC_Cells2	=	106800;
	IP_Set_Range(Radar.ADC_Cells2);

	System_Set_ATT_Dmax(Radar.ADC_Num_CLK / 2);//Radar.ADC_Cells);
	System_Set_trigger(0, Radar.ADC_Pulse_CLK, Radar.ADC_Num_CLK); // 820us x 50 = 41000

	if((PC8.IP_BW_SELi != PC8.IP_BW_SEL)||
	   (Radar.IP_Pulse_Widthi != Radar.IP_Pulse_Width))
	{
		Radar.IP_Pulse_Width = Radar.IP_Pulse_Widthi;
		Radar.IP_Code = PC8.IP_BW_SEL;
		PC8.IP_BW_SELi   = PC8.IP_BW_SEL;

		//PC8.Thang_Cu_lyi = PC8.Thang_Cu_ly;
		usleep(1);
		IP_Set_RST(0);

		//if(Radar.IP_Code == 0) System_Set_IP_code_Sellect(0);
		//else if(Radar.IP_Code >= 1) System_Set_IP_code_Sellect(1);

		System_Set_IP_Pulse_Width(Radar.IP_Pulse_Width);
		System_Set_IP_code(Radar.IP_Code);
		usleep(1);
		IP_Set_RST(1);

		for(clear_Counter = 100; clear_Counter < 6000; clear_Counter++)
		{
			Radar_Frame1[clear_Counter] = 0;
		}

		for(clear_Counter = 0; clear_Counter < 16384; clear_Counter++)
		{
			UDP_Frame.Buffer[clear_Counter] = 0;
		}

	}

	Radar_Update_Uart_DKSG();

	if((PC8.beam_Channeli2 != PC8.beam_Channeli)||
	   (PC8.fb_Channeli2 != PC8.fb_Channeli))
	{
		PC8.beam_Channeli2 = PC8.beam_Channeli;
		PC8.fb_Channeli2 = PC8.fb_Channeli;
		 xil_printf("beam_Channeli = %d\r\n",PC8.beam_Channeli);
		 xil_printf("fb_Channeli = %d\r\n",PC8.fb_Channeli);
	}


	System_Set_BS_Channel(PC8.beam_Channeli);
	System_Set_fb_Channel(PC8.fb_Channeli);

	if(PC8.Beta_Mode == 0) IP_Set_Beta_Mode(0);
	else if(PC8.Beta_Mode == 1) IP_Set_Beta_Mode(1);
	//else if(PC8.Beta_Mode == 2) IP_Set_Beta_Mode(2);

	IP_Set_Beta_start(PC8.Anten_start);
	IP_Set_Beta_stop(PC8.Anten_stop);

	System_Set_ADC_Channel(PC8.ADC_raw_sel);

	 if(PC8.Video_Vali!= PC8.Video_Val)
	 {
		 PC8.Video_Vali= PC8.Video_Val;
		 //xil_printf("Video_Val = %d\r\n",PC8.Video_Vali);
	 }


	System_Set_IQ_Channel1(PC8.IQ_Channel1);
	System_Set_IQ_Channel2(PC8.IQ_Channel2);
	Radar_Update_Phase1(PC8.Sine_offset[0], PC8.Cos_offset[0]);
	Radar_Update_Phase2(PC8.Sine_offset[1], PC8.Cos_offset[1]);
	Radar_Update_Phase3(PC8.Sine_offset[2], PC8.Cos_offset[2]);
	Radar_Update_Phase4(PC8.Sine_offset[3], PC8.Cos_offset[3]);
	Radar_Update_Phase5(PC8.Sine_offset[4], PC8.Cos_offset[4]);
	Radar_Update_Phase6(PC8.Sine_offset[5], PC8.Cos_offset[5]);
	Radar_Update_Phase7(PC8.Sine_offset[6], PC8.Cos_offset[6]);
	Radar_Update_Phase8(PC8.Sine_offset[7], PC8.Cos_offset[7]);
	Radar_Update_Phase9(PC8.Sine_offset[8], PC8.Cos_offset[8]);
	Radar_Update_Phase10(PC8.Sine_offset[9], PC8.Cos_offset[9]);
	Radar_Update_Phase11(PC8.Sine_offset[10], PC8.Cos_offset[10]);
	Radar_Update_Phase12(PC8.Sine_offset[11], PC8.Cos_offset[11]);
	Radar_Update_Phase13(PC8.Sine_offset[12], PC8.Cos_offset[12]);
	Radar_Update_Phase14(PC8.Sine_offset[13], PC8.Cos_offset[13]);
	Radar_Update_Phase15(PC8.Sine_offset[14], PC8.Cos_offset[14]);
	Radar_Update_Phase16(PC8.Sine_offset[15], PC8.Cos_offset[15]);

	if(PC8.IQ_Channel1i != PC8.IQ_Channel1)
	{
		PC8.IQ_Channel1i = PC8.IQ_Channel1;
		xil_printf("IQ_Channel1 = %d\r\n",PC8.IQ_Channel1i);
	}

	if(PC8.IQ_Channel2i != PC8.IQ_Channel2)
	{
		PC8.IQ_Channel2i = PC8.IQ_Channel2;
		xil_printf("IQ_Channel2 = %d\r\n",PC8.IQ_Channel2i);
	}

	//System_Set_ATT_TX_Sel(PC8.ATT_TX_EN);


}

void Radar_Update_Uart_DKSG(void)
{
	//PC8.ATT_Mode		=(Radar.PC8_Config[105] & 0x02) / 2; // 0: KDMT; 1: BAPY + KDMT
	//PC8.ATT_Data_Mode	=(Radar.PC8_Config[105] & 0x04) / 4; // 0: chi dung 1, 2, 3, 4; 1: dung 16 kenh

	Radar.UART_Frame3[0] = 0xFE;
	Radar.UART_Frame3[1] = 0xFE;

	if(PC8.ATT_Mode == 0) // 0: KDMT
	{
		Radar.UART_Frame3[2]  = 0;
		Radar.UART_Frame3[3]  = PC8.ATT1.KDMT;
		Radar.UART_Frame3[4]  = PC8.ATT1.KDMT;
		Radar.UART_Frame3[5]  = PC8.ATT1.KDMT;
		Radar.UART_Frame3[6]  = PC8.ATT1.KDMT;
		Radar.UART_Frame3[7]  = PC8.ATT1.KDMT;
		Radar.UART_Frame3[8]  = PC8.ATT1.KDMT;
		Radar.UART_Frame3[9]  = PC8.ATT1.KDMT;
		Radar.UART_Frame3[10] = PC8.ATT1.KDMT;
		Radar.UART_Frame3[11] = PC8.ATT1.KDMT;
		Radar.UART_Frame3[12] = PC8.ATT1.KDMT;
		Radar.UART_Frame3[13] = PC8.ATT1.KDMT;
		Radar.UART_Frame3[14] = PC8.ATT1.KDMT;
		Radar.UART_Frame3[15] = PC8.ATT1.KDMT;
		Radar.UART_Frame3[16] = PC8.ATT1.KDMT;
		Radar.UART_Frame3[17] = PC8.ATT1.KDMT;
		Radar.UART_Frame3[18] = PC8.ATT1.KDMT;
	}
	if(PC8.ATT_Mode == 1) //  1: BAPY
	{

		PC8.ATT_Delay = PC8.ATT1.Rong_Bapy * 8; // 40us

		if(PC8.ATT1.Sau_Bapy >= PC8.ATT1.KDMT)
		{
			PC8.ATT_Step  = PC8.ATT1.Sau_Bapy - PC8.ATT1.KDMT;// 40;
		}
		else if(PC8.ATT1.Sau_Bapy < PC8.ATT1.KDMT)
		{
			PC8.ATT_Step = PC8.ATT1.KDMT - PC8.ATT1.Sau_Bapy;
		}

		if(PC8.ATT_Step > 0)
		{
			PC8.ATT_Delta = PC8.ATT_Delay / PC8.ATT_Step;
		}
		else
		{
			PC8.ATT_Delta = 8;
		}

		Radar.UART_Frame3[2] = 0;

		if(PC8.ATT1.Sau_Bapy >= 0xFE)
		{
			Radar.UART_Frame3[3] = 0xFD;
			Radar.UART_Frame3[4] = 0xFD;
			Radar.UART_Frame3[5] = 0xFD;
			Radar.UART_Frame3[6] = 0xFD;
		}
		else
		{
			Radar.UART_Frame3[3] = PC8.ATT1.Sau_Bapy;
			Radar.UART_Frame3[4] = PC8.ATT1.Sau_Bapy;
			Radar.UART_Frame3[5] = PC8.ATT1.Sau_Bapy;
			Radar.UART_Frame3[6] = PC8.ATT1.Sau_Bapy;
		}

		Radar.UART_Frame3[7] = PC8.ATT_Delta / 256;
		Radar.UART_Frame3[8] = PC8.ATT_Delta % 256;
		Radar.UART_Frame3[9] = PC8.ATT_Delta / 256;
		Radar.UART_Frame3[10]= PC8.ATT_Delta % 256;

		if(PC8.ATT1.KDMT >= 0xFE)
		{
			Radar.UART_Frame3[11] = 0xFD;
			Radar.UART_Frame3[12] = 0xFD;
			Radar.UART_Frame3[13] = 0xFD;
			Radar.UART_Frame3[14] = 0xFD;
		}
		else
		{
			Radar.UART_Frame3[11] = PC8.ATT1.KDMT;
			Radar.UART_Frame3[12] = PC8.ATT1.KDMT;
			Radar.UART_Frame3[13] = PC8.ATT1.KDMT;
			Radar.UART_Frame3[14] = PC8.ATT1.KDMT;
		}

		Radar.UART_Frame3[15] = 0;
		Radar.UART_Frame3[16] = 0;
		Radar.UART_Frame3[17] = 0;
		Radar.UART_Frame3[18] = 0;
	}

	//Radar.UART_Frame3[18] = PC8.ATT17.KDMT;
	Radar.UART_Frame3[19] = PC8.ATT17.KDMT;
	Radar.UART_Frame3[20] = PC8.ATT_Mode;
	Radar.UART_Frame3[21] = PC8.ATT1.Phase;
	Radar.UART_Frame3[22] = PC8.ATT2.Phase;
	Radar.UART_Frame3[23] = PC8.ATT3.Phase;
	Radar.UART_Frame3[24] = PC8.ATT4.Phase;
	Radar.UART_Frame3[25] = PC8.ATT5.Phase;
	Radar.UART_Frame3[26] = PC8.ATT6.Phase;
	Radar.UART_Frame3[27] = PC8.ATT7.Phase;
	Radar.UART_Frame3[28] = PC8.ATT8.Phase;
	Radar.UART_Frame3[29] = PC8.ATT9.Phase;
	Radar.UART_Frame3[30] = PC8.ATT10.Phase;
	Radar.UART_Frame3[31] = PC8.ATT11.Phase;
	Radar.UART_Frame3[32] = PC8.ATT12.Phase;
	Radar.UART_Frame3[33] = PC8.ATT13.Phase;
	Radar.UART_Frame3[34] = PC8.ATT14.Phase;
	Radar.UART_Frame3[35] = PC8.ATT15.Phase;
	Radar.UART_Frame3[36] = PC8.ATT16.Phase;
	Radar.UART_Frame3[37] = PC8.Update_Phase_start;
	Radar.UART_Frame3[38] = 0;
	UART3_Send_Frame(39);
}

void Radar_Update_Uart_Motor1(void)
{
	Radar.UART_Frame1[0] = 0x01;
	Radar.UART_Frame1[1] = 0x02;
	Radar.UART_Frame1[2] = 0x03;
	Radar.UART_Frame1[3] = 0x04;
	Radar.UART_Frame1[4] = 0x05;
	Radar.UART_Frame1[5] = 0x06;
	Radar.UART_Frame1[6] = 0x07;
	Radar.UART_Frame1[7] = 0x08;
	Radar.UART_Frame1[8] = 0x09;
	Radar.UART_Frame1[9] = 0x0a;

	System_Set_RS485_DIR1(1);
	usleep(50);
	UART1_Send_Frame(10);
	usleep(100);
	System_Set_RS485_DIR1(0);

}

void Radar_Update_Uart_Motor2(void)
{

	Radar.UART_Frame2[0] = 0x01;
	Radar.UART_Frame2[1] = 0x02;
	Radar.UART_Frame2[2] = 0x03;
	Radar.UART_Frame2[3] = 0x04;
	Radar.UART_Frame2[4] = 0x05;
	Radar.UART_Frame2[5] = 0x06;
	Radar.UART_Frame2[6] = 0x07;
	Radar.UART_Frame2[7] = 0x08;
	Radar.UART_Frame2[8] = 0x09;
	Radar.UART_Frame2[9] = 0x0a;

	System_Set_RS485_DIR2(1);
	usleep(50);
	UART2_Send_Frame(10);
	usleep(100);
	System_Set_RS485_DIR2(0);

}

void Radar_Read_PC8_Excel(void)
{
	int i = 0;
	int32_t data1 = 0;
	int32_t data2 = 0;
	int32_t data3 = 0;
	int32_t data4 = 0;

	PC8.Excel.Load_Point = 0;

	for(i = 0; i < 69; i++)
	{
		// du lieu: AngleE
		data1 = (Radar.PC8_Excel[i * 12 + 2] * 0x00010000 +
				 Radar.PC8_Excel[i * 12 + 1] * 0x00000100 +
				 Radar.PC8_Excel[i * 12 + 0] * 0x00000001) - 8000000;

		PC8.Excel.AngleE[i] = data1 / 100000.0;

		// du lieu: Ue_E
		data2 = (Radar.PC8_Excel[i * 12 + 5] * 0x00010000 +
				 Radar.PC8_Excel[i * 12 + 4] * 0x00000100 +
				 Radar.PC8_Excel[i * 12 + 3] * 0x00000001) - 8000000;

		PC8.Excel.Ue_E[i] = data2/ 100000.0;

		// du lieu: AngleH
		data3 = (Radar.PC8_Excel[i * 12 + 8] * 0x00010000 +
				 Radar.PC8_Excel[i * 12 + 7] * 0x00000100 +
				 Radar.PC8_Excel[i * 12 + 6] * 0x00000001) - 8000000;

		PC8.Excel.AngleH[i] = data3/ 100000.0;

		// du lieu: Ue_H
		data4 = (Radar.PC8_Excel[i * 12 + 11] * 0x00010000 +
				 Radar.PC8_Excel[i * 12 + 10] * 0x00000100 +
				 Radar.PC8_Excel[i * 12 + 9]  * 0x00000001) - 8000000;

		PC8.Excel.Ue_H[i] = data4 / 100000.0;
	}

	PC8.Excel.Loading_E_Angle_done 	= 1;
	PC8.Excel.Loading_E_Ue_done		= 1;
	PC8.Excel.Loading_H_Ue_done 	= 1;
	PC8.Excel.Loading_H_Angle_done 	= 1;

	PC8.Excel.Loading_done = PC8.Excel.Loading_E_Angle_done * 1 +
							 PC8.Excel.Loading_E_Ue_done 	* 2 +
							 PC8.Excel.Loading_H_Angle_done * 4 +
							 PC8.Excel.Loading_H_Ue_done 	* 8;

	Radar.UDP_PC8_Start2 = 1;

	//xil_printf("Loading_done = %d\r\n", PC8.Excel.Loading_done);
}

void Radar_Read_PC8_CMD(void)
{
	PC8.Header 			= Radar.PC8_Config[1] * 256 + Radar.PC8_Config[0];
	PC8.Length 			= Radar.PC8_Config[2];

	PC8.Beta_Enable 	= Radar.PC8_Config[3];
	PC8.Beta_Speed 		= Radar.PC8_Config[4];
	PC8.Beta_Rotate 	= Radar.PC8_Config[6] * 256 + Radar.PC8_Config[5];
	PC8.Beta_DIR 		= Radar.PC8_Config[7];

	PC8.Beta_Calib 		= Radar.PC8_Config[8] * 65536 +
						  Radar.PC8_Config[9] * 256 +
						  Radar.PC8_Config[10] * 1;

	PC8.Beta_Mode 		= Radar.PC8_Config[11];
	PC8.Beta_Reset 		= Radar.PC8_Config[12];
	PC8.Beta_Update 	= Radar.PC8_Config[13];
	PC8.Beta_Val		= Radar.PC8_Config[14] * 256 + Radar.PC8_Config[15];

	PC8.Alpha_Enable 	= Radar.PC8_Config[16];
	PC8.Alpha_Speed 	= Radar.PC8_Config[17];
	PC8.Alpha_Rotate 	= Radar.PC8_Config[18] * 256 + Radar.PC8_Config[19];
	PC8.Alpha_DIR 		= Radar.PC8_Config[20];

	PC8.Alpha_Calib 	= Radar.PC8_Config[21] * 65536 +
						  Radar.PC8_Config[22] * 256 +
						  Radar.PC8_Config[23] * 1;

	PC8.Alpha_Mode 		= Radar.PC8_Config[24];
	PC8.Alpha_Reset 	= Radar.PC8_Config[25];
	PC8.Alpha_Update 	= Radar.PC8_Config[26];
	PC8.Alpha_Val 		= Radar.PC8_Config[28] * 256 + Radar.PC8_Config[27];

	PC8.Alpha_Step		= Radar.PC8_Config[29] * 0x010000 +
						  Radar.PC8_Config[30] * 0x000100 +
						  Radar.PC8_Config[31];

	PC8.Beta_Step		= Radar.PC8_Config[32] * 0x010000 +
						  Radar.PC8_Config[33] * 0x000100 +
						  Radar.PC8_Config[34];

	PC8.Anten_mode		= Radar.PC8_Config[35];
	PC8.Anten_start		=(Radar.PC8_Config[36] * 256 + Radar.PC8_Config[37]) - 1000;
	PC8.Anten_stop		=(Radar.PC8_Config[38] * 256 + Radar.PC8_Config[39]) - 1000;
	PC8.Anten_P0		= Radar.PC8_Config[40] * 256 + Radar.PC8_Config[41];

	PC8.Beta_Angle2    	= (PC8.Anten_start + PC8.Anten_stop) / 2;

	PC8.Alpha_Max		=(Radar.PC8_Config[42] * 256 + Radar.PC8_Config[43]) - 1000;// 20
	PC8.Alpha_Min		=(Radar.PC8_Config[44] * 256 + Radar.PC8_Config[45]) - 1000;// 20
	PC8.Alpha_P0		= Radar.PC8_Config[46] * 256 + Radar.PC8_Config[47];// 1228

	PC8.Calib_B_Value	= Radar.PC8_Config[48] * 256 + Radar.PC8_Config[49];
	PC8.Calib_dir		= Radar.PC8_Config[50];
	PC8.Quat_Error		= Radar.PC8_Config[51];

	PC8.ATT1.Sau_Bapy 	= Radar.PC8_Config[52];
	PC8.ATT1.Rong_Bapy 	= Radar.PC8_Config[53];
	PC8.ATT1.KDMT 		= Radar.PC8_Config[54];
	PC8.ATT17.KDMT 		= Radar.PC8_Config[55];

	PC8.ATT2.Sau_Bapy 	= PC8.ATT1.Sau_Bapy;// Radar.PC8_Config[53];
	PC8.ATT3.Sau_Bapy 	= PC8.ATT1.Sau_Bapy;// Radar.PC8_Config[54];
	PC8.ATT4.Sau_Bapy 	= PC8.ATT1.Sau_Bapy;// Radar.PC8_Config[55];
	PC8.ATT5.Sau_Bapy 	= PC8.ATT1.Sau_Bapy;// Radar.PC8_Config[56];
	PC8.ATT6.Sau_Bapy 	= PC8.ATT1.Sau_Bapy;// Radar.PC8_Config[57];
	PC8.ATT7.Sau_Bapy 	= PC8.ATT1.Sau_Bapy;// Radar.PC8_Config[58];
	PC8.ATT8.Sau_Bapy 	= PC8.ATT1.Sau_Bapy;// Radar.PC8_Config[59];
	PC8.ATT9.Sau_Bapy 	= PC8.ATT1.Sau_Bapy;// Radar.PC8_Config[60];
	PC8.ATT10.Sau_Bapy 	= PC8.ATT1.Sau_Bapy;// Radar.PC8_Config[61];
	PC8.ATT11.Sau_Bapy 	= PC8.ATT1.Sau_Bapy;// Radar.PC8_Config[62];
	PC8.ATT12.Sau_Bapy 	= PC8.ATT1.Sau_Bapy;// Radar.PC8_Config[63];
	PC8.ATT13.Sau_Bapy 	= PC8.ATT1.Sau_Bapy;// Radar.PC8_Config[64];
	PC8.ATT14.Sau_Bapy 	= PC8.ATT1.Sau_Bapy;// Radar.PC8_Config[65];
	PC8.ATT15.Sau_Bapy 	= PC8.ATT1.Sau_Bapy;// Radar.PC8_Config[66];
	PC8.ATT16.Sau_Bapy 	= PC8.ATT1.Sau_Bapy;// Radar.PC8_Config[67];

	PC8.ATT2.Rong_Bapy 	= PC8.ATT1.Rong_Bapy;// Radar.PC8_Config[69];
	PC8.ATT3.Rong_Bapy 	= PC8.ATT1.Rong_Bapy;// Radar.PC8_Config[70];
	PC8.ATT4.Rong_Bapy 	= PC8.ATT1.Rong_Bapy;// Radar.PC8_Config[71];
	PC8.ATT5.Rong_Bapy 	= PC8.ATT1.Rong_Bapy;// Radar.PC8_Config[72];
	PC8.ATT6.Rong_Bapy 	= PC8.ATT1.Rong_Bapy;// Radar.PC8_Config[73];
	PC8.ATT7.Rong_Bapy 	= PC8.ATT1.Rong_Bapy;// Radar.PC8_Config[74];
	PC8.ATT8.Rong_Bapy 	= PC8.ATT1.Rong_Bapy;// Radar.PC8_Config[75];
	PC8.ATT9.Rong_Bapy 	= PC8.ATT1.Rong_Bapy;// Radar.PC8_Config[76];
	PC8.ATT10.Rong_Bapy = PC8.ATT1.Rong_Bapy;// Radar.PC8_Config[77];
	PC8.ATT11.Rong_Bapy = PC8.ATT1.Rong_Bapy;// Radar.PC8_Config[78];
	PC8.ATT12.Rong_Bapy = PC8.ATT1.Rong_Bapy;// Radar.PC8_Config[79];
	PC8.ATT13.Rong_Bapy = PC8.ATT1.Rong_Bapy;// Radar.PC8_Config[80];
	PC8.ATT14.Rong_Bapy = PC8.ATT1.Rong_Bapy;// Radar.PC8_Config[81];
	PC8.ATT15.Rong_Bapy = PC8.ATT1.Rong_Bapy;// Radar.PC8_Config[82];
	PC8.ATT16.Rong_Bapy = PC8.ATT1.Rong_Bapy;// Radar.PC8_Config[83];

	PC8.ATT2.KDMT 		= PC8.ATT1.KDMT;// Radar.PC8_Config[85];
	PC8.ATT3.KDMT 		= PC8.ATT1.KDMT;// Radar.PC8_Config[86];
	PC8.ATT4.KDMT 		= PC8.ATT1.KDMT;// Radar.PC8_Config[87];
	PC8.ATT5.KDMT 		= PC8.ATT1.KDMT;// Radar.PC8_Config[88];
	PC8.ATT6.KDMT 		= PC8.ATT1.KDMT;// Radar.PC8_Config[89];
	PC8.ATT7.KDMT 		= PC8.ATT1.KDMT;// Radar.PC8_Config[90];
	PC8.ATT8.KDMT 		= PC8.ATT1.KDMT;// Radar.PC8_Config[91];
	PC8.ATT9.KDMT 		= PC8.ATT1.KDMT;// Radar.PC8_Config[92];
	PC8.ATT10.KDMT 		= PC8.ATT1.KDMT;// Radar.PC8_Config[93];
	PC8.ATT11.KDMT 		= PC8.ATT1.KDMT;// Radar.PC8_Config[94];
	PC8.ATT12.KDMT 		= PC8.ATT1.KDMT;// Radar.PC8_Config[95];
	PC8.ATT13.KDMT 		= PC8.ATT1.KDMT;// Radar.PC8_Config[96];
	PC8.ATT14.KDMT 		= PC8.ATT1.KDMT;// Radar.PC8_Config[97];
	PC8.ATT15.KDMT 		= PC8.ATT1.KDMT;// Radar.PC8_Config[98];
	PC8.ATT16.KDMT 		= PC8.ATT1.KDMT;// Radar.PC8_Config[99];

	PC8.ADC_ATTi  = Radar.PC8_Config[68];// 20

	// if(PC8.ADC_ATTiv != PC8.ADC_ATTi)// 20
	// {
	// 	PC8.ADC_ATTiv = PC8.ADC_ATTi;

	// 	for(pi=0; pi < 16; pi++)
	// 	{
	// 		adcSetDSA3(pi / 4, pi % 4, PC8.ADC_ATTi);
	// 		usleep(100);
	// 	}
	// }

	PC8.FFT_Sel 	= Radar.PC8_Config[69];// 20
	PC8.Sum_Sel 	= Radar.PC8_Config[70];// 20
	PC8.DAC_VOP		= Radar.PC8_Config[71];// cong suat phat (* 1000 mA)



	// huong muc tieu
	PC8.BamSat._Current_Heading =  (Radar.PC8_Config[56] * 256.0 + Radar.PC8_Config[57]);
	// van toc tuong doi
	PC8.BamSat._Current_V 		= ((Radar.PC8_Config[58] * 65536.0 + Radar.PC8_Config[59] * 256.0 + Radar.PC8_Config[60]) - 200000.0) / 100.0;
	// van toc dich ngang (phuong vi)
	PC8.BamSat._Current_Vx 		= ((Radar.PC8_Config[61] * 65536.0 + Radar.PC8_Config[62] * 256.0 + Radar.PC8_Config[63]) - 200000.0) / 100.0;
	// van toc dich doc (cu ly)
	PC8.BamSat._Current_Vy 		= ((Radar.PC8_Config[64] * 65536.0 + Radar.PC8_Config[65] * 256.0 + Radar.PC8_Config[66]) - 200000.0) / 100.0;
	// muc tieu san sang
	PC8.BamSat._Current_Start 	=   Radar.PC8_Config[67];


	PC8.ATT1.Phase = Radar.PC8_Config[72];
	PC8.ATT2.Phase = Radar.PC8_Config[73];
	PC8.ATT3.Phase = Radar.PC8_Config[74];
	PC8.ATT4.Phase = Radar.PC8_Config[75];
	PC8.ATT5.Phase = Radar.PC8_Config[76];
	PC8.ATT6.Phase = Radar.PC8_Config[77];
	PC8.ATT7.Phase = Radar.PC8_Config[78];
	PC8.ATT8.Phase = Radar.PC8_Config[79];
	PC8.ATT9.Phase = Radar.PC8_Config[80];
	PC8.ATT10.Phase = Radar.PC8_Config[81];
	PC8.ATT11.Phase = Radar.PC8_Config[82];
	PC8.ATT12.Phase = Radar.PC8_Config[83];
	PC8.ATT13.Phase = Radar.PC8_Config[84];
	PC8.ATT14.Phase = Radar.PC8_Config[85];
	PC8.ATT15.Phase = Radar.PC8_Config[86];
	PC8.ATT16.Phase = Radar.PC8_Config[87];
	PC8.Update_Phase_start = Radar.PC8_Config[88];
	PC8.Start_BeamV  = Radar.PC8_Config[89];

	PC8.VCO_Val1		= Radar.PC8_Config[101] * 256 + Radar.PC8_Config[102];
	PC8.VCO_Val2		= Radar.PC8_Config[103] * 256 + Radar.PC8_Config[104];
	PC8.VCO_EN			= Radar.PC8_Config[105] & 0x01;
	PC8.ATT_Mode		=(Radar.PC8_Config[105] & 0x02) / 2; // 0: KDMT; 1: BAPY
	PC8.ATT_Data_Mode	=(Radar.PC8_Config[105] & 0x04) / 4; // 0: chi dung 1, 2, 3, 4; 1: dung 16 kenh

	PC8.Thang_Cu_ly 	= Radar.PC8_Config[106];
	PC8.Cu_ly_EN 		= (Radar.PC8_Config[107] & 0x01);
	PC8.Gia_Quay_EN 	= (Radar.PC8_Config[107] & 0x02) / 2;
	PC8.AVG_EN 			= (Radar.PC8_Config[107] & 0x04) / 4;
	PC8.Loopback_Mode   = (Radar.PC8_Config[107] & 0x08) / 8;
	PC8.IP_BW_SEL   	= (Radar.PC8_Config[107] & 0xf0) / 16;

	PC8.IP_dmt			= Radar.PC8_Config[108] * 256 + Radar.PC8_Config[109]; // dmt

	PC8.ADC_SEL			= Radar.PC8_Config[110];

	PC8.Offset1			= Radar.PC8_Config[111];
	PC8.Offset2			= Radar.PC8_Config[112];
	PC8.Offset3			= Radar.PC8_Config[113];

	PC8.Tao_Gia			= Radar.PC8_Config[114];

	PC8.ADC_CHx			= Radar.PC8_Config[115];
	PC8.IQ_Channel1		= Radar.PC8_Config[116];
	PC8.Video_Val		= Radar.PC8_Config[117];
	PC8.IQ_Channel2		= Radar.PC8_Config[118];
	PC8.Phase_Mode		= Radar.PC8_Config[119];

	PC8.Sine_offset[0]   = (Radar.PC8_Config[120] * 256 + Radar.PC8_Config[121]);
	PC8.Cos_offset[0]    = (Radar.PC8_Config[122] * 256 + Radar.PC8_Config[123]);
	PC8.Sine_offset[1]   = (Radar.PC8_Config[124] * 256 + Radar.PC8_Config[125]);
	PC8.Cos_offset[1]    = (Radar.PC8_Config[126] * 256 + Radar.PC8_Config[127]);
	PC8.Sine_offset[2]   = (Radar.PC8_Config[128] * 256 + Radar.PC8_Config[129]);
	PC8.Cos_offset[2]    = (Radar.PC8_Config[130] * 256 + Radar.PC8_Config[131]);
	PC8.Sine_offset[3]   = (Radar.PC8_Config[132] * 256 + Radar.PC8_Config[133]);
	PC8.Cos_offset[3]    = (Radar.PC8_Config[134] * 256 + Radar.PC8_Config[135]);
	PC8.Sine_offset[4]   = (Radar.PC8_Config[136] * 256 + Radar.PC8_Config[137]);
	PC8.Cos_offset[4]    = (Radar.PC8_Config[138] * 256 + Radar.PC8_Config[139]);
	PC8.Sine_offset[5]   = (Radar.PC8_Config[140] * 256 + Radar.PC8_Config[141]);
	PC8.Cos_offset[5]    = (Radar.PC8_Config[142] * 256 + Radar.PC8_Config[143]);
	PC8.Sine_offset[6]   = (Radar.PC8_Config[144] * 256 + Radar.PC8_Config[145]);
	PC8.Cos_offset[6]    = (Radar.PC8_Config[146] * 256 + Radar.PC8_Config[147]);
	PC8.Sine_offset[7]   = (Radar.PC8_Config[148] * 256 + Radar.PC8_Config[149]);
	PC8.Cos_offset[7]    = (Radar.PC8_Config[150] * 256 + Radar.PC8_Config[151]);
	PC8.Sine_offset[8]   = (Radar.PC8_Config[152] * 256 + Radar.PC8_Config[153]);
	PC8.Cos_offset[8]    = (Radar.PC8_Config[154] * 256 + Radar.PC8_Config[155]);
	PC8.Sine_offset[9]   = (Radar.PC8_Config[156] * 256 + Radar.PC8_Config[157]);
	PC8.Cos_offset[9]    = (Radar.PC8_Config[158] * 256 + Radar.PC8_Config[159]);
	PC8.Sine_offset[10]   = (Radar.PC8_Config[160] * 256 + Radar.PC8_Config[161]);
	PC8.Cos_offset[10]    = (Radar.PC8_Config[162] * 256 + Radar.PC8_Config[163]);
	PC8.Sine_offset[11]   = (Radar.PC8_Config[164] * 256 + Radar.PC8_Config[165]);
	PC8.Cos_offset[11]    = (Radar.PC8_Config[166] * 256 + Radar.PC8_Config[167]);
	PC8.Sine_offset[12]   = (Radar.PC8_Config[168] * 256 + Radar.PC8_Config[169]);
	PC8.Cos_offset[12]    = (Radar.PC8_Config[170] * 256 + Radar.PC8_Config[171]);
	PC8.Sine_offset[13]   = (Radar.PC8_Config[172] * 256 + Radar.PC8_Config[173]);
	PC8.Cos_offset[13]    = (Radar.PC8_Config[174] * 256 + Radar.PC8_Config[175]);
	PC8.Sine_offset[14]   = (Radar.PC8_Config[176] * 256 + Radar.PC8_Config[177]);
	PC8.Cos_offset[14]    = (Radar.PC8_Config[178] * 256 + Radar.PC8_Config[179]);
	PC8.Sine_offset[15]   = (Radar.PC8_Config[180] * 256 + Radar.PC8_Config[181]);
	PC8.Cos_offset[15]    = (Radar.PC8_Config[182] * 256 + Radar.PC8_Config[183]);

	PC8.BS_Center_cell		= (Radar.PC8_Config[184] * 256 + Radar.PC8_Config[185]);	// 20
	PC8.BS_Center_start		= (Radar.PC8_Config[186] * 256 + Radar.PC8_Config[187]);	// 20
	PC8.BS_Center_stop		= (Radar.PC8_Config[188] * 256 + Radar.PC8_Config[189]);	// 20
	PC8.BS_Start			=  Radar.PC8_Config[190];// 20
	PC8.BS_Center_angle		= (Radar.PC8_Config[191] * 256 + Radar.PC8_Config[192]);
	PC8.beam_Channeli		=  Radar.PC8_Config[193];
	PC8.fb_Channeli			=  Radar.PC8_Config[194];
	PC8.BS_Center_alpha		= (Radar.PC8_Config[195] * 256 + Radar.PC8_Config[196]);
	PC8.epxilon_Channeli 	=  Radar.PC8_Config[197];
	PC8.Excel.Loading_Reset =  Radar.PC8_Config[198];
	PC8.BS_PC1_Click  		=  Radar.PC8_Config[199];

	PC8.BamSat.Motor_Speed  = Radar.PC8_Config[200];
	PC8.BamSat.Scan_Speed   = Radar.PC8_Config[201];
	PC8.BamSat.Bamsat_Speed = Radar.PC8_Config[202];

	PC8.BamSat.Giam_mau_xuly  = Radar.PC8_Config[203];
	PC8.BamSat.Chieu_DK_Bam_H = (Radar.PC8_Config[204] & 0x01);
	PC8.BamSat.Chieu_DK_Bam_E = (Radar.PC8_Config[204] & 0x02) / 2;
	PC8.CMD_Mixer_Txsig 	  = (Radar.PC8_Config[204] & 0x04) / 4;// 20
	PC8.CMD_Mixer_Sel 		  = (Radar.PC8_Config[204] & 0xF8) / 8;// 20
	PC8.CMD_RF_ON_OFF 		  = (Radar.PC8_Config[205] & 0x01);
	PC8.BamSat.BS_Quat_mode   = 0;//(Radar.PC8_Config[205] & 0x10) / 16;// 20
	PC8.BamSat.BS_Beam_mode   = (Radar.PC8_Config[205] & 0x10) / 16;// 20

	if(PC8.BS_Start == 0) // xoa du lieu
	{
		PC8.BS_K_Point 		 				= 0;
		PC8.BS_Angle_Ez0 	 				= 0;
		PC8.BS_Angle_Hz0 	 				= 0;
		PC8.BS_Center_starti 				= 0;
		PC8.BamSat.Process_Phi_start 		= 0; // chuyen qua ban sat phuong vi
		PC8.BamSat.Process_Epxilon_start 	= 0; // chuyen qua ban sat phuong vi
		PC8.BamSat.Process_BS_done 			= 0; // chuyen qua ban sat phuong vi
		PC8.Sync_time 		= 0;
		PC8.BS_Counter_time = 0;

		PC8.BamSat.BS_Detect_done = 0;// 20
		PC8.BamSat.BS_Detect_Fail = 0;// 20
		PC8.BamSat.BS_Detect_time = 0;// 20
		PC8.BamSat.BS_Detect_Change_Point = 0;// 20
		PC8.BamSat.BS_Detect_Fix = 0;// 20
		PC8.BamSat._CS_H_Ready = 0;  // l dieu khien kenh H
		PC8.BamSat._CS_E_Ready = 0;  // l dieu khien kenh E

	}

	System_Set_Beam17_Bamsat(PC8.Thang_Cu_ly);

	PC8.V1.H_Angle 	= ((Radar.PC8_Config[206] * 256 + Radar.PC8_Config[207]) - 32768) / 1000.0;
	PC8.V1.E_Angle	= ((Radar.PC8_Config[208] * 256 + Radar.PC8_Config[209]) - 32768) / 1000.0;

	PC8.V2.H_Angle 	= ((Radar.PC8_Config[210] * 256 + Radar.PC8_Config[211]) - 32768) / 1000.0;
	PC8.V2.E_Angle	= ((Radar.PC8_Config[212] * 256 + Radar.PC8_Config[213]) - 32768) / 1000.0;

	PC8.V3.H_Angle 	= ((Radar.PC8_Config[214] * 256 + Radar.PC8_Config[215]) - 32768) / 1000.0;
	PC8.V3.E_Angle	= ((Radar.PC8_Config[216] * 256 + Radar.PC8_Config[217]) - 32768) / 1000.0;

	PC8.V4.H_Angle 	= ((Radar.PC8_Config[218] * 256 + Radar.PC8_Config[219]) - 32768) / 1000.0;
	PC8.V4.E_Angle	= ((Radar.PC8_Config[220] * 256 + Radar.PC8_Config[221]) - 32768) / 1000.0;

	PC8.Pvi_dir		= Radar.PC8_Config[222];// 20
	PC8.Gocta_dir	= Radar.PC8_Config[223];// 20

	PC8.BS_Angle_E =Radar.PC8_Config[224] + Radar.PC8_Config[225] * 256;
	PC8.BS_Angle_H =Radar.PC8_Config[226] + Radar.PC8_Config[227] * 256;
	PC8.E_time = Radar.PC8_Config[228];
	PC8.H_time = Radar.PC8_Config[229];

	PC8.Ue_Max = Radar.PC8_Config[230] * 1;//31
	PC8.Ue_Min = Radar.PC8_Config[230] * (-1);//31

	PC8.Save_Z0 = Radar.PC8_Config[231];

	PC8.BamSat.Process_Beta_Mode  = Radar.PC8_Config[232];
	PC8.BamSat.Process_Alpha_Mode = Radar.PC8_Config[233];

	PC8.BamSat.Nguong_H_DK_Ue_H = (Radar.PC8_Config[234] + Radar.PC8_Config[235] * 256) / 100.0;
	PC8.BamSat.Nguong_L_DK_Ue_H =  PC8.BamSat.Nguong_H_DK_Ue_H * (-1);
	PC8.BamSat.Nguong_H_DK_Ue_E = (Radar.PC8_Config[236] + Radar.PC8_Config[237] * 256) / 100.0;
	PC8.BamSat.Nguong_L_DK_Ue_E =  PC8.BamSat.Nguong_H_DK_Ue_E * (-1);

	PC8.BamSat.Ue_Max_kenh_H =  (Radar.PC8_Config[238] + Radar.PC8_Config[239] * 256) / 100.0;
	PC8.BamSat.Ue_Min_kenh_H = 	 PC8.BamSat.Ue_Max_kenh_H * (-1);
	PC8.BamSat.Ue_Max_kenh_E =  (Radar.PC8_Config[240] + Radar.PC8_Config[241] * 256) / 100.0;
	PC8.BamSat.Ue_Min_kenh_E = 	 PC8.BamSat.Ue_Max_kenh_E * (-1);

	PC8.BamSat.BS_Fix_H = Radar.PC8_Config[242];// 20
	PC8.BamSat.BS_Fix_E = Radar.PC8_Config[243];// 20

	PC8.BamSat.BS_Fix_H0 = Radar.PC8_Config[244];// 20
	PC8.BamSat.BS_Fix_E0 = Radar.PC8_Config[245];// 20

	PC8.BamSat.Min_Detect = Radar.PC8_Config[246] + Radar.PC8_Config[247] * 256;
	PC8.BamSat.BS_CS_Delta = Radar.PC8_Config[248];
	PC8.BamSat.Bam_culy_ENA= Radar.PC8_Config[249];

	PC8.BamSat.Max_s1 = Radar.PC8_Config[250]; // cua song lon nhat cho thang 1, [Cell]
	PC8.BamSat.Max_s2 = Radar.PC8_Config[251]; // cua song lon nhat cho thang 2, [Cell]
	PC8.BamSat.Max_s3 = Radar.PC8_Config[252]; // cua song lon nhat cho thang 3, [Cell]
	PC8.BamSat.Max_s4 = Radar.PC8_Config[253]; // cua song lon nhat cho thang 4, [Cell]
	PC8.BamSat.Max_s5 = Radar.PC8_Config[254]; // cua song lon nhat cho thang 5, [Cell]

	PC8.BamSat.Min_R1 = Radar.PC8_Config[255]; // cu ly bam sat nho nhat cho thang 1, [Cell]
	PC8.BamSat.Min_R2 = Radar.PC8_Config[256]; // cu ly bam sat nho nhat cho thang 2, [Cell]
	PC8.BamSat.Min_R3 = Radar.PC8_Config[257]; // cu ly bam sat nho nhat cho thang 3, [Cell]
	PC8.BamSat.Min_R4 = Radar.PC8_Config[258]; // cu ly bam sat nho nhat cho thang 4, [Cell]
	PC8.BamSat.Min_R5 = Radar.PC8_Config[259]; // cu ly bam sat nho nhat cho thang 5, [Cell]
	PC8.BamSat.LoopBack = Radar.PC8_Config[260]; //
	PC8.BamSat._CS_E_Enable = Radar.PC8_Config[261]; //

	PC8.IP_dmt2			= Radar.PC8_Config[262] * 256 + Radar.PC8_Config[263]; // dmt

	Radar.UDP_Delay_T1  = Radar.PC8_Config[264];
	Radar.UDP_Delay_T2  = Radar.PC8_Config[265];
	Radar.UDP_Delay_T3  = Radar.PC8_Config[266];
	Radar.UDP_Delay_T4  = Radar.PC8_Config[267];
	Radar.UDP_Delay_T5  = Radar.PC8_Config[268];

	Radar.UDP_Delay_V1  = Radar.PC8_Config[270];
	Radar.UDP_Delay_V2  = Radar.PC8_Config[271];
	Radar.UDP_Delay_V3  = Radar.PC8_Config[272];
	Radar.UDP_Delay_V4  = Radar.PC8_Config[273];
	Radar.UDP_Delay_V5  = Radar.PC8_Config[274];

	Radar.Screen_Mode   = Radar.PC8_Config[275];

	PC8.BamSat.Min_Detect_mode  = Radar.PC8_Config[269];

	PC8.Bamsat_H_ena	= Radar.PC8_Config[276];
	PC8.Bamsat_E_ena	= Radar.PC8_Config[277];

	PC8.Bamsat_K1		= ((Radar.PC8_Config[277] * 256 + Radar.PC8_Config[278]) - 1000) / 100.0;
	PC8.Bamsat_K2		= ((Radar.PC8_Config[279] * 256 + Radar.PC8_Config[280]) - 1000) / 100.0;
	PC8.Bamsat_K3		= ((Radar.PC8_Config[281] * 256 + Radar.PC8_Config[282]) - 1000) / 100.0;

	PC8.Screen_Phase 	= (Radar.PC8_Config[283] * 65536 + Radar.PC8_Config[284] * 256 + Radar.PC8_Config[285]);
	PC8.IQ_View_Sel		= Radar.PC8_Config[286];
	PC8.ADC_raw_sel		= Radar.PC8_Config[287];
	PC8.Beam_Max_View	= Radar.PC8_Config[288];

	PC8.BamSat.fb_SNR_Scalei = Radar.PC8_Config[289] / 10.0;

	if(PC8.BamSat.fb_SNR_Scalei <= 1.0)
	{
		PC8.BamSat.fb_SNR_Scale = 1.0;
	}
	else
	{
		PC8.BamSat.fb_SNR_Scale = PC8.BamSat.fb_SNR_Scalei;
	}


	PC8.Pulse_Sel = Radar.PC8_Config[290];// 20
	PC8.code_Sel = Radar.PC8_Config[291];// 20

	PC8.Control_View_SS 	  = Radar.PC8_Config[292];// 20

	PC8.BamSat.Beam_Scalei  = Radar.PC8_Config[293] / 100.0;
	PC8.BamSat.Beam_Scalei2  = Radar.PC8_Config[294] / 100.0;

	PC8.TG_EN 			= (Radar.PC8_Config[295] & 0x01);
	PC8.TG_Mode			= (Radar.PC8_Config[295] & 0x02) / 2;
	PC8.TG_Reset		= (Radar.PC8_Config[295] & 0x04) / 4;
	PC8.Beam17_MAX_Mode	= (Radar.PC8_Config[295] & 0x08) / 8;

	Radar.IP_Pulse_Widthi = Radar.PC8_Config[296] * 256 + Radar.PC8_Config[297];

	PC8._Set_Beam_Max_Point = Radar.PC8_Config[298];
	PC8.IP_amplitudei = Radar.PC8_Config[299] / 8192.0f;

	PC8.DAC_KS_Enable		= Radar.PC8_Config[300];
	PC8.DAC_H_E_Val			= Radar.PC8_Config[301] * 256 + Radar.PC8_Config[302];
	PC8.DAC_Center_Val		= Radar.PC8_Config[303] * 256 + Radar.PC8_Config[304];

	// 1. Trích xuất thời gian từ gói tin PC gửi xuống
// Map theo đúng thứ tự byte bạn đã code trên C#
int pc_day   = Radar.PC8_Config[310];
int pc_month = Radar.PC8_Config[311];
int pc_year  = Radar.PC8_Config[312] * 256 + Radar.PC8_Config[313];
int pc_hour  = Radar.PC8_Config[314];
int pc_min   = Radar.PC8_Config[315];
int pc_sec   = Radar.PC8_Config[316];

	// Chỉ xử lý nếu dữ liệu năm hợp lý (tránh trường hợp chưa nhận được gói tin, toàn số 0)
	if (pc_year > 2023) 
	{
		// 2. Lấy thời gian hiện tại của Linux
		time_t rawtime_sys;
		time(&rawtime_sys); // Lấy timestamp hiện tại

		// 3. Chuyển đổi thời gian của PC sang dạng timestamp (time_t) để so sánh
		struct tm tm_pc = {0};
		
		tm_pc.tm_year = pc_year - 1900; // struct tm tính năm từ 1900
		tm_pc.tm_mon  = pc_month - 1;   // struct tm tính tháng từ 0-11
		tm_pc.tm_mday = pc_day;
		tm_pc.tm_hour = pc_hour;
		tm_pc.tm_min  = pc_min;
		tm_pc.tm_sec  = pc_sec;
		tm_pc.tm_isdst = -1; // Để hệ thống tự xác định Daylight Saving Time

		time_t rawtime_pc = mktime(&tm_pc);

		// 4. Tính độ chênh lệch (giây)
		double diff_seconds = difftime(rawtime_sys, rawtime_pc);
		
		// Lấy trị tuyệt đối
		if (diff_seconds < 0) diff_seconds = -diff_seconds;

		// 5. Nếu chênh lệch lớn hơn ngưỡng cho phép (ví dụ: > 2 giây) thì cập nhật
		if (diff_seconds > 2.0) 
		{
			printf("Phat hien sai lech gio! System: %ld, PC: %ld. Dang cap nhat...\n", rawtime_sys, rawtime_pc);

			struct timeval tv;
			tv.tv_sec = rawtime_pc;
			tv.tv_usec = 0;

			// Cập nhật giờ hệ thống (Cần quyền root/sudo mới chạy được lệnh này)
			if (settimeofday(&tv, NULL) < 0) 
			{
				perror("Loi: Khong the settimeofday (can quyen root?)");
			} 
			else 
			{
				// Tùy chọn: Ghi xuống Hardware Clock (RTC) để lưu lại sau khi tắt nguồn
				// system("hwclock -w"); 
				printf("Da cap nhat thoi gian Linux theo PC.\n");
			}
		}
	}

	if(PC8.ADC_ATTiv != PC8.ADC_ATTi)// 20
	{
		PC8.ADC_ATTiv = PC8.ADC_ATTi;

		for(pi=0; pi < 16; pi++)
		{
			adcSetDSA3(pi / 4, pi % 4, PC8.ADC_ATTi);
			usleep(100);
		}
	}

	if(PC8.DAC_VOPi != PC8.DAC_VOP)
	{
		PC8.DAC_VOPi = PC8.DAC_VOP;

		u32 my_cmd_values[3];

		for(pi = 0; pi < 16; pi++)
		{
			my_cmd_values[0] = pi / 4;   // Tile_Id = 0
			my_cmd_values[1] = pi % 4;   // Block_Id = 0 (kenh/khoi DAC dau tien)
			my_cmd_values[2] = PC8.DAC_VOP * 1000; // uACurrent = 1000 uA
			dacSetVOP(my_cmd_values); // cai dat kenh DAC1
			usleep(100);
		}
	}


	UDP_Counter++;
	if((UDP_Counter > 150)&&(PC8.Start_BeamV == 1))
	{
		UDP_Counter = 0;

		adcGetDSA2(PC8.ATT_Val);
		dacGetAllVOP(PC8.VOP_Val);
	}

	if(PC8.FFT_Seli != PC8.FFT_Sel)
	{
		PC8.FFT_Seli = PC8.FFT_Sel;
		System_Set_FFT_Sel(PC8.FFT_Sel);
	}

	if(PC8.Sum_Seli != PC8.Sum_Sel)
	{
		PC8.Sum_Seli = PC8.Sum_Sel;
		System_Set_sum_Sel(PC8.Sum_Sel);
	}


	if(PC8.IP_amplitudei == 0.0) PC8.IP_amplitudei = 0.01;

	if(PC8.DAC_KS_Enable == 0)
	{

		if(PC8._Set_Beam_Max_Pointi != PC8._Set_Beam_Max_Point)
		{
			PC8._Set_Beam_Max_Pointi = PC8._Set_Beam_Max_Point;

			_Update_DAC_Over_Beam(PC8._Set_Beam_Max_Point);
		}

		if(PC8.DAC_KS_Fix == 1)
		{
			_Update_DAC_Over_Beam(16);
			PC8.DAC_KS_Fix = 0;
		}
	}
	else if ((PC8.DAC_KS_Enable == 1)||(PC8.DAC_KS_Enable == 2))
	{
		PC8.DAC_KS_Fix = 1;
		_Update_DAC_Over_Angle();
	}

	if((PC8.TG_ENi == 0)&&(PC8.TG_EN == 1)) // 38
	{
		PC8.TG_ENi = PC8.TG_EN;
		//PC8.TG_Click_time = 0; // 38)
	}
	else if(PC8.TG_EN == 0) // 38
	{
		PC8.TG_ENi = 0;
		PC8.TG_Click_time = 0; // 38)
	}


	if(PC8.BamSat.Beam_Scalei <= 0.05)
	{
		PC8.BS_pH_Kalman = 0.05; // don vi: ly giac
	}
	else
	{
		PC8.BS_pH_Kalman = PC8.BamSat.Beam_Scalei; // don vi: ly giac
	}

	if(PC8.BamSat.Beam_Scalei2 <= 0.05)
	{
		PC8.BS_pE_Kalman = 0.05; // don vi: ly giac
	}
	else
	{
		PC8.BS_pE_Kalman = PC8.BamSat.Beam_Scalei2; // don vi: ly giac
	}

	if(PC8._H_Kalman.r  != PC8.BS_pH_Kalman)
	{
		PC8._H_Kalman.r = PC8.BS_pH_Kalman;
	}

	if(PC8._E_Kalman.r  != PC8.BS_pE_Kalman)
	{
		PC8._E_Kalman.r = PC8.BS_pE_Kalman;
	}

	System_Set_IQ_View(PC8.IQ_View_Sel);
	System_Set_Beam_start(PC8.Screen_Phase);

	System_Set_ADC_SEL(PC8.Pulse_Sel);
	System_Set_ADC_SEL2(PC8.code_Sel);

	if(PC8.Khaosat_Enablei != PC8.Khaosat_Enable)
	{
		PC8.Khaosat_Enablei = PC8.Khaosat_Enable;
		PC8.HW_Dir = PC8.Khaosat_Dir;
		PC8.HW_Dir0 = 0;
	}

	if(PC8.BS_PC1_Click == 1)
	{
		PC8.BS_Center_anglef0 = PC8.BS_Center_angle / 10.0f;
		PC8.BS_Center_alphaf0 = PC8.BS_Center_alpha / 10.0f;
		PC8.BS_Center_anglef = PC8.BS_Center_anglef0;
		PC8.BS_Center_alphaf = PC8.BS_Center_alphaf0;
	}

	if(PC8.BS_Center_angle >= 80)
	{
		PC8.BS_Center_Angle_start 	= PC8.BS_Center_Angle_start - 3590;// 20
	}
	else
	{
		PC8.BS_Center_Angle_start 	= 3600 + PC8.BS_Center_Angle_start - 80;// 20
	}

	PC8.BS_Center_Angle_stop	= PC8.BS_Center_angle + 80;// 20
	if(PC8.BS_Center_Angle_stop >= 3600) PC8.BS_Center_Angle_stop	= PC8.BS_Center_Angle_stop - 3600;// 20

	if(PC8.Anten_start < PC8.Anten_stop)
	{
		PC8.Anten_angle = PC8.Anten_stop - PC8.Anten_start;
	}
	else if(PC8.Anten_start > PC8.Anten_stop)
	{
		PC8.Anten_angle = 3600 + PC8.Anten_stop - PC8.Anten_start;
	}

	if(PC8.Alpha_Val>=100) PC8.real_Angle = PC8.Alpha_Val - 100;
	else PC8.real_Angle = 3500 + PC8.Alpha_Val;

	if(PC8.Excel.Loading_Reset == 1)
	{
		PC8.Excel.Loading_Reset = 0;
		// xoa du lieu
		PC8.Excel.Loading_E_Angle_done = 0;
		PC8.Excel.Loading_E_Ue_done = 0;
		PC8.Excel.Loading_H_Angle_done = 0;
		PC8.Excel.Loading_H_Ue_done = 0;
		PC8.Excel.Loading_done = 0;
		PC8.Excel.Load_Point = 0;
		PC8.Excel.Point = 0;

	}

	System_Set_IP_B17_Sel(PC8.ADC_SEL);
	System_Set_IP_Bx_Sel(PC8.ADC_SEL);


	System_Set_Beam_Sel(PC8.Control_View_SS);
	PC8.CMD_RF_ENABLE = 1;

	PC8.IP_dmt_Val = (uint32_t)(PC8.IP_dmt * 5.4); // data freq = 40mhz

	IP_Set_dmt(PC8.IP_dmt_Val);

	if(PC8.IP_dmt_Val == 4000)
	{
		IP_Set_DAC_Mode(PC8.Tao_Gia, 0, PC8.IP_amplitudei);
	}
	else
	{
		IP_Set_DAC_Mode(PC8.Tao_Gia, 1, PC8.IP_amplitudei);
	}

	System_Set_RF_ENABLE(PC8.CMD_RF_ENABLE); // ENABLE
	System_Set_RF_ON_OFF(PC8.CMD_RF_ON_OFF); // ENABLE
	//System_Set_CMD_Mixer_Txsig(PC8.CMD_Mixer_Txsig); // ENABLE

	System_Set_Mixer_Sel(PC8.CMD_Mixer_Sel); // ENABLE

	Erob_Get_CMD();
	Radar_Update_ATT();
	_Check_Beam_config();
	Cua_Song_Cal_New_Limit();
	Cua_Song_Recal_Range();

}

void PC8_Control_HW(void)
{
	System_Set_trigger(0, 200, 50000);
	Motor1_Mode_Freq(2000, 1, 0);
	Motor1_Mode_Pulse(1000, 0, 0);
	Motor1_Set_Run_Mode(0);
	Motor1_Set_Mode(15000, 10000, 20000);
	Motor1_Send_ENC(0);
	Motor2_Mode_Freq(2000, 1, 0);
	Motor2_Mode_Pulse(1000, 0, 0);
	Motor2_Set_Run_Mode(0);
	Motor2_Set_Mode(15000, 10000, 20000);
	Motor2_Send_ENC(0);

	PC8.IP_BW_SEL = 0;
	PC8.IP_dmt_Val = 68 * 40;
	PC8.Loopback_Mode = 0;
	PC8.Tao_Gia = 0;

	PC8.Pulse_Sel = 0;// 20
	PC8.code_Sel = 0;// 20
	PC8.IP_amplitudei = 0.25f;

	IP_Set_dmt(PC8.IP_dmt_Val);

	IP_Set_DAC_Mode(0, 0, PC8.IP_amplitudei);

	//IP_Set_thang_culy(5);
	IP_Set_Beta_Mode(0);
	IP_Set_Beta_in(0);
	IP_Set_Beta_start(0);
	IP_Set_Beta_stop(0);
	System_Set_ADC_SEL(0);
	System_Set_ADC_SEL2(0);
	System_Set_RS485_DIR1(0);
	System_Set_RS485_DIR2(0);
	System_Set_RF_ON_OFF(0);
	System_Set_Trigger_time1(100);
	System_Set_Trigger_time2(100);
	System_Set_ADC_Channel(0);
	System_Set_IQ_Channel1(0);
	System_Set_IQ_Channel2(0);
	//System_Set_ATT_Dmax(4096);
	System_Set_ATT_Data(	0,
							0,
							0,
							0,
							0,
							0,
							0,
							0,
							0,
							0,
							0,
							0,
							0,
							0,
							0,
							0,
							0);
	//System_Set_trigger(0, 200, 41000);
	Radar.Range			=	5;
	Radar.Range2		=	5;
	Radar.ADC_Cells		=	4096;
	Radar.ADC_Cells2	=	106800;
	Radar.ADC_Time_us	=	820;
	Radar.ADC_Num_CLK	=	41000;
	Radar.ADC_Pulse_CLK =	200;


	IP_Set_thang_culy(Radar.Range);
	IP_Set_Range(Radar.ADC_Cells2);
	System_Set_ATT_Dmax(Radar.ADC_Num_CLK / 2);//Radar.ADC_Cells);
	System_Set_trigger(0, Radar.ADC_Pulse_CLK, Radar.ADC_Num_CLK); // 820us x 50 = 41000
	System_Set_ADC_SEL(0);

	System_Set_RX_SEL(0);


	Radar.IP_culy = 5;
	Radar.IP_Code = 2;
	System_Set_IP_code(Radar.IP_Code);

}

void _Sub_Send_Data_Mode0(void)
{
	memcpy(&UDP_Frame.Buffer[0], &Radar_Frame_M[100], 21360);//
	//memcpy(&UDP_Frame.Buffer2[0], &Radar_Frame1[16484],4976);//

	PC8.Alpha_Vals = 1;
	PC8.Alpha_Vals2 = 0;
}

void _Sub_Send_Data_Mode1(void)
{
	memcpy(&UDP_Frame.Buffer[0], &Radar_Frame_M[100], 21360);//
	//memcpy(&UDP_Frame.Buffer2[0], &Radar_Frame1[16484],4976);//

	PC8.Alpha_Vals = 1;
	PC8.Alpha_Vals2 = 0;
}

void PC8_Read_RAM_Data(void)
{

	_Sub_Send_Data_Mode0();

	if(PC8.Beam17_MAX_Mode == 0)
	{
		// if(PC8.Beam_Max_View == 0)
		// {
		// 	memcpy(&UDP_Frame.Buffer3[0], &Beam17, 800);
		// }
		// else
		// {
		// 	memcpy(&UDP_Frame.Buffer3[0], &Beam17, 800);
		// }
		
		//Radar_Frame_M[0]
		
		memcpy(&UDP_Frame.Buffer3[0], &Beam17, 800);
	}
	else if(PC8.Beam17_MAX_Mode == 1)
	{
		memcpy(&UDP_Frame.Buffer3[0], &Beam17,  800);
	}

	memcpy(&UDP_Frame.Buffer3[800], &Beam17i, 800);
	memcpy(&UDP_Frame.Buffer3[1600], &Beam17q, 800);

	memcpy(&UDP_Frame.Buffer3[2400], &Beam17i, 800);
	memcpy(&UDP_Frame.Buffer3[3200], &Beam17q, 800);

	memcpy(&UDP_Frame.Buffer3[4000], &Beam17i, 800);
	memcpy(&UDP_Frame.Buffer3[4800], &Beam17q, 800);

	memcpy(&UDP_Frame.Buffer4[0],    &Beam17i, 800);
	memcpy(&UDP_Frame.Buffer4[800],  &Beam17q, 800);
	memcpy(&UDP_Frame.Buffer4[1600], &Beam17i, 800);
	memcpy(&UDP_Frame.Buffer4[2400], &Beam17q, 800);

}

void _Get_Phase_calculator(void)
{
	uint8_t p = 0;

//	UDP_Frame.ATT_Val1	 		= PC8.ATT_Val[0];
//	UDP_Frame.ATT_Val2	 		= PC8.ATT_Val[1];
//	UDP_Frame.ATT_Val3	 		= PC8.ATT_Val[2];
//	UDP_Frame.ATT_Val4	 		= PC8.ATT_Val[3];
//	UDP_Frame.ATT_Val5	 		= PC8.ATT_Val[4];
//	UDP_Frame.ATT_Val6	 		= PC8.ATT_Val[5];
//	UDP_Frame.ATT_Val7	 		= PC8.ATT_Val[6];
//	UDP_Frame.ATT_Val8	 		= PC8.ATT_Val[7];
//	UDP_Frame.ATT_Val9	 		= PC8.ATT_Val[8];
//	UDP_Frame.ATT_Val10	 		= PC8.ATT_Val[9];
//	UDP_Frame.ATT_Val11	 		= PC8.ATT_Val[10];
//	UDP_Frame.ATT_Val12	 		= PC8.ATT_Val[11];
//	UDP_Frame.ATT_Val13	 		= PC8.ATT_Val[12];
//	UDP_Frame.ATT_Val14	 		= PC8.ATT_Val[13];
//	UDP_Frame.ATT_Val15	 		= PC8.ATT_Val[14];
//	UDP_Frame.ATT_Val16	 		= PC8.ATT_Val[15];


	for(p = 0; p < 16; p++)
	{
		UDP_Frame.Buffer5[p * 4 + 0]   = PC8.ATT_Val[p] % 256;
		UDP_Frame.Buffer5[p * 4 + 1]   = PC8.ATT_Val[p] / 256;
		UDP_Frame.Buffer5[p * 4 + 2]   = PC8.VOP_Val[p] % 256;
		UDP_Frame.Buffer5[p * 4 + 3]   = PC8.VOP_Val[p] / 256;

		UDP_Frame.Buffer5[p * 4 + 64]  = PC8.ATT_Val[p] % 256;
		UDP_Frame.Buffer5[p * 4 + 65]  = PC8.ATT_Val[p] / 256;
		UDP_Frame.Buffer5[p * 4 + 66]  = PC8.VOP_Val[p] % 256;
		UDP_Frame.Buffer5[p * 4 + 67]  = PC8.VOP_Val[p] / 256;

		UDP_Frame.Buffer5[p * 4 + 128] = PC8.ATT_Val[p] % 256;
		UDP_Frame.Buffer5[p * 4 + 129] = PC8.ATT_Val[p] / 256;
		UDP_Frame.Buffer5[p * 4 + 130] = PC8.VOP_Val[p] % 256;
		UDP_Frame.Buffer5[p * 4 + 131] = PC8.VOP_Val[p] / 256;

		UDP_Frame.Buffer5[p * 4 + 192] = PC8.ATT_Val[p] % 256;
		UDP_Frame.Buffer5[p * 4 + 193] = PC8.ATT_Val[p] / 256;
		UDP_Frame.Buffer5[p * 4 + 194] = PC8.VOP_Val[p] % 256;
		UDP_Frame.Buffer5[p * 4 + 195] = PC8.VOP_Val[p] / 256;
	}

	UDP_Frame.Buffer5[256] = ((uint16_t)(PC8.V1.H_Angle * 1000 + 32768)) / 256;
	UDP_Frame.Buffer5[257] = ((uint16_t)(PC8.V1.H_Angle * 1000 + 32768)) % 256;
	UDP_Frame.Buffer5[258] = ((uint16_t)(PC8.V1.E_Angle * 1000 + 32768)) / 256;
	UDP_Frame.Buffer5[259] = ((uint16_t)(PC8.V1.E_Angle * 1000 + 32768)) % 256;

	UDP_Frame.Buffer5[260] = ((uint16_t)(PC8.V2.H_Angle * 1000 + 32768)) / 256;
	UDP_Frame.Buffer5[261] = ((uint16_t)(PC8.V2.H_Angle * 1000 + 32768)) % 256;
	UDP_Frame.Buffer5[262] = ((uint16_t)(PC8.V2.E_Angle * 1000 + 32768)) / 256;
	UDP_Frame.Buffer5[263] = ((uint16_t)(PC8.V2.E_Angle * 1000 + 32768)) % 256;

	UDP_Frame.Buffer5[264] = ((uint16_t)(PC8.V3.H_Angle * 1000 + 32768)) / 256;
	UDP_Frame.Buffer5[265] = ((uint16_t)(PC8.V3.H_Angle * 1000 + 32768)) % 256;
	UDP_Frame.Buffer5[266] = ((uint16_t)(PC8.V3.E_Angle * 1000 + 32768)) / 256;
	UDP_Frame.Buffer5[267] = ((uint16_t)(PC8.V3.E_Angle * 1000 + 32768)) % 256;

	UDP_Frame.Buffer5[268] = ((uint16_t)(PC8.V4.H_Angle * 1000 + 32768)) / 256;
	UDP_Frame.Buffer5[269] = ((uint16_t)(PC8.V4.H_Angle * 1000 + 32768)) % 256;
	UDP_Frame.Buffer5[270] = ((uint16_t)(PC8.V4.E_Angle * 1000 + 32768)) / 256;
	UDP_Frame.Buffer5[271] = ((uint16_t)(PC8.V4.E_Angle * 1000 + 32768)) % 256;
}

void PC8_Get_Data(void)
{

	UDP_Frame.ENC1_Value 	= Erob_Motor.Motor_PhuongVi.Absolute_ENC;
	UDP_Frame.ENC2_Value 	= Erob_Motor.Motor_GocTa.Absolute_ENC;

	UDP_Frame.Goc_Omega1 	= Erob_Motor.Motor_PhuongVi.Angle;
	UDP_Frame.Goc_Omega2 	= Erob_Motor.Motor_GocTa.Angle;

	UDP_Frame.Motor1_ENA 	= Erob_Motor.Motor_PhuongVi.Erob_Enable + Erob_Motor.Motor_PhuongVi.Erob_DIR * 2;
	UDP_Frame.Motor2_ENA 	= Erob_Motor.Motor_GocTa.Erob_Enable + Erob_Motor.Motor_GocTa.Erob_DIR * 2;

	/// #define	Hyper_Input_SUM_MAX				10
	//  #define	Hyper_Input_SUM_POINT			11
	//PC8.BamSat.Beam_Max 	= PC8.BamSat.Beam_Maxi;
	//PC8.BamSat.Beam_Max_Val = PC8.BamSat.Beam_Max_Vali;
	UDP_Frame.Kiemtra1_ENA 	= PC8.BamSat.Beam_Max;//Radar_Hyper_Input(Hyper_Input_SUM_MAX);
	UDP_Frame.Kiemtra2_ENA 	= PC8.BamSat.Beam_Max_Val;//Radar_Hyper_Input(Hyper_Input_SUM_POINT);

	UDP_Frame.Goc_Kiemtra1 	= Erob_Motor.Motor_PhuongVi.AngleE;
	UDP_Frame.Goc_Kiemtra2 	= Erob_Motor.Motor_GocTa.AngleE;

	UDP_Frame.Motor_Rotate1 = Erob_Motor.Motor_PhuongVi.Motor_Angle;
	UDP_Frame.Motor_Rotate2 = Erob_Motor.Motor_GocTa.Motor_Angle;

	//UDP_Frame.ENC_CMD1 		= PC8.Video_Val + PC8.Alpha_Vals2 * 256;
	UDP_Frame.ENC_Status1 	= CAN_Controls.Motor1_OK;
	UDP_Frame.ENC_CMD2 		= PC8.ADC_CHx;
	UDP_Frame.ENC_Status2 	= CAN_Controls.Motor2_OK;

	UDP_Frame.Calib1_Point 	= Erob_Motor.Motor_PhuongVi.Erob_Calib;
	UDP_Frame.Calib2_Point 	= Erob_Motor.Motor_GocTa.Erob_Calib;

	UDP_Frame.Motor1_Angle	= Erob_Motor.Motor_PhuongVi.UDP_Angle;
	UDP_Frame.Motor1_Lygiac	= Erob_Motor.Motor_PhuongVi.UDP_Lygiac;
	UDP_Frame.Motor2_Angle	= Erob_Motor.Motor_GocTa.UDP_Angle;
	UDP_Frame.Motor2_Lygiac	= Erob_Motor.Motor_GocTa.UDP_Lygiac;

	UDP_Frame.BS_Delta_Angle1 	= Erob_Motor.Motor_PhuongVi.BS_Delta_Angle;   //
	UDP_Frame.BS_Delta_Angle2 	= Erob_Motor.Motor_GocTa.BS_Delta_Angle;    //
	UDP_Frame.Quat_Delta1 		= Erob_Motor.Motor_PhuongVi.Quat_Delta;    //
	UDP_Frame.Quat_Delta2 		= Erob_Motor.Motor_GocTa.Quat_Delta;	//
	//UDP_Frame.Phuong_vi_Dir 	= 0;    //
	//UDP_Frame.Goc_ta_Dir 		= 0;	//

//	UDP_Frame.AXI_GPIO_IN  	= 0;
	UDP_Frame.VCO_Val1   	= PC8.VCO_Val1;
	UDP_Frame.VCO_Val2   	= PC8.VCO_Val2;
	UDP_Frame.ADC_CH0   	= PC8.ADC_Data[0];
	UDP_Frame.ADC_CH1   	= PC8.ADC_Data[1];
	UDP_Frame.ADC_CH2   	= PC8.ADC_Data[2];
	UDP_Frame.ADC_CH3   	= PC8.ADC_Data[3];
	// adc 4- adc 16 khong su dung
	UDP_Frame.ADC_CH4   	= Erob_Motor.Motor_PhuongVi.Pulse_Enable +
							  Erob_Motor.Motor_PhuongVi.Pulse_DIR * 2 +
							  Erob_Motor.Motor_GocTa.Pulse_Enable * 4 +
							  Erob_Motor.Motor_GocTa.Pulse_DIR * 8 +
							  PC8.Excel.Loading_done * 16 +
							  PC8.S17_Max_point * 256 +
							  Radar.UDP_PC8_Screen_Point * 65536+
							  PC8.BS_Start * 16777216;
	UDP_Frame.ADC_CH5   	= (PC8.BS_Center_anglef + 720) * 1000000;//Erob_Motor.Motor_PhuongVi.Pulse_number;
	UDP_Frame.ADC_CH6   	= (PC8.BS_Center_alphaf + 720) * 1000000;//Erob_Motor.Motor_GocTa.Pulse_number;
	UDP_Frame.ADC_CH7   	= PC8.BS_Center_start_bs + PC8.BS_Center_stop_bs * 65536;
	UDP_Frame.ADC_CH8   	= (PC8.BamSat.Delta_beta);
	UDP_Frame.ADC_CH9   	= ((PC8.S17E_Goc + 8) * 1000000);
	UDP_Frame.ADC_CH10   	= (PC8.BamSat.Delta_alpha);
	UDP_Frame.ADC_CH11  	= ((PC8.S17H_Goc + 8) * 1000000);
	UDP_Frame.Frame_Lengh   = Radar.ADC_Cells;

	UDP_Frame.BS_S17 = PC8.S17_Amplitude;	//
	UDP_Frame.BS_V1  = PC8.Amplitude_V1;	//
	UDP_Frame.BS_V2  = PC8.Amplitude_V2;	//
	UDP_Frame.BS_V3  = PC8.Amplitude_V3;	//
	UDP_Frame.BS_V4  = PC8.Amplitude_V4;	//
	UDP_Frame.BS_V1I = PC8._V1I;	//
	UDP_Frame.BS_V2I = PC8._V2I;	//
	UDP_Frame.BS_V3I = PC8._V3I;	//
	UDP_Frame.BS_V4I = PC8._V4I;	//
    UDP_Frame.BS_V1Q = PC8._V1Q;	//
    UDP_Frame.BS_V2Q = PC8._V2Q;	//
    UDP_Frame.BS_V3Q = PC8._V3Q;	//
    UDP_Frame.BS_V4Q = PC8._V4Q;	//
    UDP_Frame.BS_UeH = PC8.S17H_Ue;	// Ue FPGA kenh H
    UDP_Frame.BS_GocH= PC8.S17H_Goc;// goc truc quang kenh H
    UDP_Frame.BS_UeE = PC8.S17E_Ue;	// Ue FPGA kenh E
    UDP_Frame.BS_GocE= PC8.S17E_Goc;// goc truc quang kenh E
    UDP_Frame.Phi_final		= PC8.BamSat.TL_Phi_final;		// goc dieu khien H: FPGA
    UDP_Frame.Epxilon_final = PC8.BamSat.TL_Epxilon_final;	// goc dieu khien E: FPGA
    UDP_Frame.BS_S17I = PC8._S17I;	//
    UDP_Frame.BS_S17Q = PC8._S17Q;	//
    UDP_Frame.BS_PS17 = PC8.BS_PS17; // 38
    UDP_Frame.BS_PV1 =  PC8.BS_PV1; // 38
    UDP_Frame.BS_PV2 =  PC8.BS_PV2; // 38
    UDP_Frame.BS_PV3 =  PC8.BS_PV3; // 38
    UDP_Frame.BS_PV4 =  PC8.BS_PV4; // 38

    UDP_Frame.BS_Angle_E = Erob_Motor.Motor_GocTa.Motor_BS_Value;	// goc dieu khien E tu pc8 xuong FPGA
    UDP_Frame.BS_Angle_H = Erob_Motor.Motor_PhuongVi.Motor_BS_Value;// goc dieu khien H tu pc8 xuong FPGA
    UDP_Frame.BS_Status = PC8.BS_Start +
						  PC8.BamSat.Process_Epxilon_Ready * 2 +
						  PC8.BamSat.Process_Phi_start * 4 + // chuyen qua ban sat phuong vi
						  PC8.BamSat.Process_Epxilon_start * 8 +
						  PC8.BamSat.Process_BS_done * 16 +
						  PC8.BamSat.BS_Detect_done * 32 +
						  PC8.BamSat.BS_Detect_Change_Point * 64 +
						  PC8.BamSat._CS_H_Ready * 128 +
						  PC8.BamSat._CS_E_Ready * 256;  // l dieu khien kenh H;	//


    UDP_Frame.BS_Angle_Eb0 = PC8.BS_Angle_Eb0;
    UDP_Frame.BS_Angle_Hb0 = PC8.BS_Angle_Hb0;
    UDP_Frame.BS_Angle_Ez0 = PC8.BS_Angle_Ez0;
    UDP_Frame.BS_Angle_Hz0 = PC8.BS_Angle_Hz0;
    UDP_Frame.Motor_BS_ValueE = PC8.BamSat.Value_Max[16];
    UDP_Frame.Motor_BS_ValueH = PC8.BS_Center_start_bs0;
    UDP_Frame.Delta_V12 = PC8.Delta_V12;//31
    UDP_Frame.Delta_V34 = PC8.Delta_V34;//31
    UDP_Frame.SS1_MAX	= PC8.SS1_MAX;
    UDP_Frame.SS2_MAX	= PC8.SS2_MAX;
    UDP_Frame.SS3_MAX	= PC8.SS3_MAX;
    UDP_Frame.SS4_MAX	= PC8.SS4_MAX;
    UDP_Frame.Radar_Range = Radar.Range;

    UDP_Frame.Target_Radius0 	= PC8.Target_Radius0;
    UDP_Frame.Target_Beta0		= PC8.Target_Beta0;
	UDP_Frame.Target_Alpha0		= PC8.Target_Alpha0;

	UDP_Frame.x0				= (float)PC8.BamSat.Max_channel_AVG;//PC8.x0;
	UDP_Frame.y0				= (float)PC8.BamSat.fb_Start;//PC8.y0;
	UDP_Frame.z0				= (float)PC8.BamSat.fb_Stop;//PC8.z0;

	UDP_Frame.Target_Radius1	= PC8.Target_Radius1;
	UDP_Frame.Target_Beta1		= PC8.Target_Beta1;
	UDP_Frame.Target_Alpha1		= PC8.Target_Alpha1;
	UDP_Frame.x1				= PC8.x1;
	UDP_Frame.y1				= (float)PC8.TG_Beta0;//PC8.TG_Click_time;//PC8.y1;
	UDP_Frame.z1				= (float)PC8.TG_Alpha0;//PC8.TG_time;//PC8.z1;

	UDP_Frame.step_counter		= PC8.BamSat.Min_Detect_bamsat;//PC8.step_counter;
	UDP_Frame.Delta_Step		= PC8.BamSat.Beam_SNR;//PC8.Delta_Step;
	UDP_Frame.Delta_time		= PC8.BamSat.BS_Detect_low_counter;//Delta_time;

    UDP_Frame.Target_Radius		= PC8.Target_Radius;
	UDP_Frame.Target_Beta  		= PC8.Target_Beta;
	UDP_Frame.Target_Alpha 		= PC8.Target_Alpha;
	UDP_Frame.Target_Hight 		= PC8.Target_Hight;
	UDP_Frame.Target_Speed 		= PC8.Target_Speed;

	PC8_Read_RAM_Data(); // copy du lieu vao cac buffer
	_Get_Phase_calculator(); // gui bang tinh toan phase len pc8

	UDP_Frame.ENC_CMD1 		= PC8.Video_Val + PC8.Alpha_Vals2 * 256;
}

void PC8_Get_ADC_Val(void)
{
		XGpio_DiscreteWrite(&GPIO_HS3, 1, 0);
		usleep(1);
		PC8.ADC_Data[0] = XGpio_DiscreteRead(&GPIO_HS3, 2);
		usleep(1);

		XGpio_DiscreteWrite(&GPIO_HS3, 1, 1);
		usleep(1);
		PC8.ADC_Data[1] = XGpio_DiscreteRead(&GPIO_HS3, 2);
		usleep(1);

		XGpio_DiscreteWrite(&GPIO_HS3, 1, 2);
		usleep(1);
		PC8.ADC_Data[2] = XGpio_DiscreteRead(&GPIO_HS3, 2);
		usleep(1);

		XGpio_DiscreteWrite(&GPIO_HS3, 1, 3);
		usleep(1);
		PC8.ADC_Data[3] = XGpio_DiscreteRead(&GPIO_HS3, 2);
		usleep(1);
}

void PC8_Read_Phase_Data(void)
{
	if(PC8.CMD_Mixer_Txsig == 0)
	{
		Linux_memcpy(&Radar_Video8, Video_Phase_I1_Addr, 48000);
		Linux_memcpy(&Radar_Video9, Video_Phase_Q1_Addr, 48000);
		Linux_memcpy(&Radar_Video10, Video_Phase_I2_Addr, 48000);
		Linux_memcpy(&Radar_Video11, Video_Phase_Q2_Addr, 48000);
	}
	else if(PC8.CMD_Mixer_Txsig == 1) // xem du lieu IQ
	{
		if(PC8.CMD_Mixer_Sel <= 19) // ADC RAW (1, 16), Txsig
		{
			Linux_memcpy(&Radar_Video8, Video_Phase_I1_Addr, 48000);
			Linux_memcpy(&Radar_Video9, Video_Phase_Q1_Addr, 48000);
			//Linux_memcpy(&Radar_Video10, Video_Phase_I2_Addr, 48000);
			//Linux_memcpy(&Radar_Video11, Video_Phase_Q2_Addr, 48000);
		}
		else if(PC8.CMD_Mixer_Sel == 20) // Fb17, Fbx
		{
			Linux_memcpy(&Radar_Video8, Video_fb17_Addr, 48000);
			Linux_memcpy(&Radar_Video9, Video_fb1_16_Addr, 48000);
			//Linux_memcpy(&Radar_Video10, Video_Phase_I2_Addr, 48000);
			//Linux_memcpy(&Radar_Video11, Video_Phase_Q2_Addr, 48000);
		}
		else if(PC8.CMD_Mixer_Sel == 21) // I, Q
		{
			Linux_memcpy(&Radar_Video8, Video_fb17_Addr, 48000);
			Linux_memcpy(&Radar_Video9, Video_b17S_Addr, 48000);
			//Linux_memcpy(&Radar_Video10, Video_Phase_I2_Addr, 48000);
			//Linux_memcpy(&Radar_Video11, Video_Phase_Q2_Addr, 48000);
		}
		else if(PC8.CMD_Mixer_Sel == 22) // I, Q
		{
			Linux_memcpy(&Radar_Video8, Video_fb17_Addr, 48000);
			Linux_memcpy(&Radar_Video9, Video_Code_Addr, 48000);
			//Linux_memcpy(&Radar_Video10, Video_Phase_I2_Addr, 48000);
			//Linux_memcpy(&Radar_Video11,  Video_Phase_Q2_Addr, 48000);
		}
	}

}

void PC8_Get_Phase(struct UDP_Phase *UDP_Phase, u32 Addr)
{

	UDP_Phase->ENC1_Value 		= Erob_Motor.Motor_PhuongVi.Absolute_ENC;
	UDP_Phase->ENC2_Value 		= Erob_Motor.Motor_GocTa.Absolute_ENC;

	UDP_Phase->Goc_Omega1 		= Erob_Motor.Motor_PhuongVi.Angle;
	UDP_Phase->Goc_Omega2 		= Erob_Motor.Motor_GocTa.Angle;

	UDP_Phase->Motor1_ENA 		= Erob_Motor.Motor_PhuongVi.Erob_Enable + Erob_Motor.Motor_PhuongVi.Erob_DIR * 2;
	UDP_Phase->Motor2_ENA 		= Erob_Motor.Motor_GocTa.Erob_Enable + Erob_Motor.Motor_GocTa.Erob_DIR * 2;

	UDP_Phase->Kiemtra1_ENA 	= 0;
	UDP_Phase->Kiemtra2_ENA 	= 0;

	UDP_Phase->Goc_Kiemtra1 	= Erob_Motor.Motor_PhuongVi.AngleE;
	UDP_Phase->Goc_Kiemtra2 	= Erob_Motor.Motor_GocTa.AngleE;

	UDP_Phase->Motor_Rotate1 	= Erob_Motor.Motor_PhuongVi.Motor_Angle;
	UDP_Phase->Motor_Rotate2 	= Erob_Motor.Motor_GocTa.Motor_Angle;

	UDP_Phase->ENC_CMD1 		= PC8.Video_Val + PC8.Alpha_Vals2 * 256;
	UDP_Phase->ENC_Status1 		= CAN_Controls.Motor1_OK;
	UDP_Phase->ENC_CMD2 		= PC8.ADC_CHx;
	UDP_Phase->ENC_Status2 		= CAN_Controls.Motor2_OK;

	UDP_Phase->Calib1_Point 	= Erob_Motor.Motor_PhuongVi.Erob_Calib;
	UDP_Phase->Calib2_Point 	= Erob_Motor.Motor_GocTa.Erob_Calib;

	UDP_Phase->Motor1_Angle		= Erob_Motor.Motor_PhuongVi.UDP_Angle;
	UDP_Phase->Motor1_Lygiac	= Erob_Motor.Motor_PhuongVi.UDP_Lygiac;
	UDP_Phase->Motor2_Angle		= Erob_Motor.Motor_GocTa.UDP_Angle;
	UDP_Phase->Motor2_Lygiac	= Erob_Motor.Motor_GocTa.UDP_Lygiac;

	UDP_Phase->BS_Delta_Angle1 	= Erob_Motor.Motor_PhuongVi.BS_Delta_Angle;;    //
	UDP_Phase->BS_Delta_Angle2 	= Erob_Motor.Motor_GocTa.BS_Delta_Angle;;    //
	UDP_Phase->Quat_Delta1 		= Erob_Motor.Motor_PhuongVi.Quat_Delta;    //
	UDP_Phase->Quat_Delta2 		= Erob_Motor.Motor_GocTa.Quat_Delta;	//
	//UDP_Frame.Phuong_vi_Dir 	= 0;    //
	//UDP_Frame.Goc_ta_Dir 		= 0;	//

//	UDP_Frame.AXI_GPIO_IN  	= 0;
	UDP_Phase->VCO_Val1   		= PC8.VCO_Val1;
	UDP_Phase->VCO_Val2   		= PC8.VCO_Val2;
	UDP_Phase->ADC_CH0   		= PC8.ADC_Data[0];
	UDP_Phase->ADC_CH1   		= PC8.ADC_Data[1];
	UDP_Phase->ADC_CH2   		= PC8.ADC_Data[2];
	UDP_Phase->ADC_CH3   		= PC8.ADC_Data[3];

	UDP_Phase->ADC_CH4   	= Erob_Motor.Motor_PhuongVi.Pulse_Enable +
								  Erob_Motor.Motor_PhuongVi.Pulse_DIR * 2 +
								  Erob_Motor.Motor_GocTa.Pulse_Enable * 4 +
								  Erob_Motor.Motor_GocTa.Pulse_DIR * 8 +
								  PC8.Excel.Loading_done * 16 +
								  PC8.S17_Max_point * 256 +
								  Radar.UDP_PC8_Screen_Point * 65536+
								  PC8.BS_Start * 16777216;
	UDP_Phase->ADC_CH5   	= (PC8.BS_Center_anglef + 360) * 1000000;//Erob_Motor.Motor_PhuongVi.Pulse_number;
	UDP_Phase->ADC_CH6   	= (PC8.BS_Center_alphaf + 360) * 1000000;//Erob_Motor.Motor_GocTa.Pulse_number;
	UDP_Phase->ADC_CH7   	= PC8.BS_Center_start_bs + PC8.BS_Center_stop_bs * 65536;
	UDP_Phase->ADC_CH8   	= (PC8.BamSat.Delta_beta);
	UDP_Phase->ADC_CH9   	= ((PC8.S17E_Goc + 8) * 1000000);
	UDP_Phase->ADC_CH10   	= (PC8.BamSat.Delta_alpha);
	UDP_Phase->ADC_CH11  	= ((PC8.S17H_Goc + 8) * 1000000);
	UDP_Phase->Frame_Lengh   = Radar.ADC_Cells;
	// adc 4- adc 16 khong su dung

	if(Addr == 0) 		memcpy(&UDP_Phase->Buffer, &Radar_Video9, 48000);
	else if(Addr == 1) 	memcpy(&UDP_Phase->Buffer, &Radar_Video8, 48000);
	else if(Addr == 2) 	memcpy(&UDP_Phase->Buffer, &Radar_Video11, 48000);
	else if(Addr == 3) 	memcpy(&UDP_Phase->Buffer, &Radar_Video10, 48000);

}

void PC8_Get_Phase1(u32 Addr)
{

	UDP_Phase1.ENC1_Value 		= Erob_Motor.Motor_PhuongVi.Absolute_ENC;
	UDP_Phase1.ENC2_Value 		= Erob_Motor.Motor_GocTa.Absolute_ENC;

	UDP_Phase1.Goc_Omega1 		= Erob_Motor.Motor_PhuongVi.Angle;
	UDP_Phase1.Goc_Omega2 		= Erob_Motor.Motor_GocTa.Angle;

	UDP_Phase1.Motor1_ENA 		= Erob_Motor.Motor_PhuongVi.Erob_Enable + Erob_Motor.Motor_PhuongVi.Erob_DIR * 2;
	UDP_Phase1.Motor2_ENA 		= Erob_Motor.Motor_GocTa.Erob_Enable + Erob_Motor.Motor_GocTa.Erob_DIR * 2;

	UDP_Phase1.Kiemtra1_ENA 	= 0;
	UDP_Phase1.Kiemtra2_ENA 	= 0;

	UDP_Phase1.Goc_Kiemtra1 	= Erob_Motor.Motor_PhuongVi.AngleE;
	UDP_Phase1.Goc_Kiemtra2 	= Erob_Motor.Motor_GocTa.AngleE;

	UDP_Phase1.Motor_Rotate1 	= Erob_Motor.Motor_PhuongVi.Motor_Angle;
	UDP_Phase1.Motor_Rotate2 	= Erob_Motor.Motor_GocTa.Motor_Angle;

	UDP_Phase1.ENC_CMD1 		= PC8.Video_Val + PC8.Alpha_Vals2 * 256;
	UDP_Phase1.ENC_Status1 		= CAN_Controls.Motor1_OK;
	UDP_Phase1.ENC_CMD2 		= PC8.ADC_CHx;
	UDP_Phase1.ENC_Status2 		= CAN_Controls.Motor2_OK;

	UDP_Phase1.Calib1_Point 	= Erob_Motor.Motor_PhuongVi.Erob_Calib;
	UDP_Phase1.Calib2_Point 	= Erob_Motor.Motor_GocTa.Erob_Calib;

	UDP_Phase1.Motor1_Angle		= Erob_Motor.Motor_PhuongVi.UDP_Angle;
	UDP_Phase1.Motor1_Lygiac	= Erob_Motor.Motor_PhuongVi.UDP_Lygiac;
	UDP_Phase1.Motor2_Angle		= Erob_Motor.Motor_GocTa.UDP_Angle;
	UDP_Phase1.Motor2_Lygiac	= Erob_Motor.Motor_GocTa.UDP_Lygiac;

	UDP_Phase1.BS_Delta_Angle1 	= Erob_Motor.Motor_PhuongVi.BS_Delta_Angle;;    //
	UDP_Phase1.BS_Delta_Angle2 	= Erob_Motor.Motor_GocTa.BS_Delta_Angle;;    //
	UDP_Phase1.Quat_Delta1 		= Erob_Motor.Motor_PhuongVi.Quat_Delta;    //
	UDP_Phase1.Quat_Delta2 		= Erob_Motor.Motor_GocTa.Quat_Delta;	//
	//UDP_Frame.Phuong_vi_Dir 	= 0;    //
	//UDP_Frame.Goc_ta_Dir 		= 0;	//

//	UDP_Frame.AXI_GPIO_IN  	= 0;
	UDP_Phase1.VCO_Val1   		= PC8.VCO_Val1;
	UDP_Phase1.VCO_Val2   		= PC8.VCO_Val2;
	UDP_Phase1.ADC_CH0   		= PC8.ADC_Data[0];
	UDP_Phase1.ADC_CH1   		= PC8.ADC_Data[1];
	UDP_Phase1.ADC_CH2   		= PC8.ADC_Data[2];
	UDP_Phase1.ADC_CH3   		= PC8.ADC_Data[3];

	UDP_Phase1.ADC_CH4   	= Erob_Motor.Motor_PhuongVi.Pulse_Enable +
								  Erob_Motor.Motor_PhuongVi.Pulse_DIR * 2 +
								  Erob_Motor.Motor_GocTa.Pulse_Enable * 4 +
								  Erob_Motor.Motor_GocTa.Pulse_DIR * 8 +
								  PC8.Excel.Loading_done * 16 +
								  PC8.S17_Max_point * 256 +
								  Radar.UDP_PC8_Screen_Point * 65536+
								  PC8.BS_Start * 16777216;
	UDP_Phase1.ADC_CH5   	= (PC8.BS_Center_anglef + 360) * 1000000;//Erob_Motor.Motor_PhuongVi.Pulse_number;
	UDP_Phase1.ADC_CH6   	= (PC8.BS_Center_alphaf + 360) * 1000000;//Erob_Motor.Motor_GocTa.Pulse_number;
	UDP_Phase1.ADC_CH7   	= PC8.BS_Center_start_bs + PC8.BS_Center_stop_bs * 65536;
	UDP_Phase1.ADC_CH8   	= (PC8.BamSat.Delta_beta);
	UDP_Phase1.ADC_CH9   	= ((PC8.S17E_Goc + 8) * 1000000);
	UDP_Phase1.ADC_CH10   	= (PC8.BamSat.Delta_alpha);
	UDP_Phase1.ADC_CH11  	= ((PC8.S17H_Goc + 8) * 1000000);
	UDP_Phase1.Frame_Lengh   = Radar.ADC_Cells;
	// adc 4- adc 16 khong su dung

	if(Addr == 0) 		memcpy(&UDP_Phase1.Buffer, &Radar_Video9, 48000);
	else if(Addr == 1) 	memcpy(&UDP_Phase1.Buffer, &Radar_Video8, 48000);
	else if(Addr == 2) 	memcpy(&UDP_Phase1.Buffer, &Radar_Video11, 48000);
	else if(Addr == 3) 	memcpy(&UDP_Phase1.Buffer, &Radar_Video10, 48000);

}

int PC8_Check_Beta_In_Window(void)
{
	int check = 0;

	//PC8.BS_Center_Angle_start;// 20
	//PC8.BS_Center_Angle_stop;// 20

	if(PC8.BS_Center_Angle_start < PC8.BS_Center_Angle_stop)
	{
		if((Radar.Beta_HW >= PC8.BS_Center_Angle_start)&&
		   (Radar.Beta_HW <= PC8.BS_Center_Angle_stop))
		{
			check = 1;
		}
		else
		{
			check = 0;
		}
	}
	else if(PC8.BS_Center_Angle_start > PC8.BS_Center_Angle_stop)
		{
			if((Radar.Beta_HW >= PC8.BS_Center_Angle_start)||
			   (Radar.Beta_HW <= PC8.BS_Center_Angle_stop))
			{
				check = 1;
			}
			else
			{
				check = 0;
			}
		}
	return check;
}

void PC8_Get_Data_Min_Max(void)
{
	//memcpy(&Radar_Video12, (u32*) Video_Tong_hieu_Addr, 16384);

}

void PC8_innit_Bamsat(void)
{
	int point = 0;
	PC8.BamSat.Beam_Max 	= 0;
	PC8.BamSat.Beam_Max_Val	= 0;
	PC8.BamSat.Value_Max[0]	= 0;
	PC8.BamSat.Value_Max[1]	= 0;
	PC8.BamSat.Value_Max[2]	= 0;
	PC8.BamSat.Value_Max[3]	= 0;
	PC8.BamSat.Value_Max[4]	= 0;
	PC8.BamSat.Value_Max[5]	= 0;
	PC8.BamSat.Value_Max[6]	= 0;
	PC8.BamSat.Value_Max[7]	= 0;
	PC8.BamSat.Value_Max[8]	= 0;
	PC8.BamSat.Value_Max[9]	= 0;
	PC8.BamSat.Value_Max[10]	= 0;
	PC8.BamSat.Value_Max[11]	= 0;
	PC8.BamSat.Value_Max[12]	= 0;
	PC8.BamSat.Value_Max[13]	= 0;
	PC8.BamSat.Value_Max[14]	= 0;
	PC8.BamSat.Value_Max[15]	= 0;
	PC8.BamSat.Value_Max[16]	= 0;

	PC8.ATT1.Phase = 0;
	PC8.ATT2.Phase = 0;
	PC8.ATT3.Phase = 0;
	PC8.ATT4.Phase = 0;
	PC8.ATT5.Phase = 0;
	PC8.ATT6.Phase = 0;
	PC8.ATT7.Phase = 0;
	PC8.ATT8.Phase = 0;
	PC8.ATT9.Phase = 0;
	PC8.ATT10.Phase = 0;
	PC8.ATT11.Phase = 0;
	PC8.ATT12.Phase = 0;
	PC8.ATT13.Phase = 0;
	PC8.ATT14.Phase = 0;
	PC8.ATT15.Phase = 0;
	PC8.ATT16.Phase = 0;
	PC8.Update_Phase_start = 0;
	PC8.Start_BeamV = 0;


	PC8.E_time 			= 0;
	PC8.H_time 			= 0;
	PC8.Save_Z0 		= 0;
	PC8.BS_Steps 		= 0;
	PC8.AVG_EN 			= 0;
	PC8.Loopback_Mode 	= 0;

	PC8.VCO_Val1 		= 0;
	PC8.VCO_Val2 		= 0;
	PC8.VCO_Val1i 		= 0;
	PC8.VCO_Val2i 		= 0;

	PC8.VCO_time		= 0;
	PC8.VCO_time2		= 0;

	Radar.UDP_PC1_Start = 0;
	Radar.UDP_Step3 	= 0;

	PC8.Bamsat_K1		= 0.8;
	PC8.Bamsat_K2		= 0.1;
	PC8.Bamsat_K3		= 0.05;
	PC8.Bamsat_H_ena	= 1;
	PC8.Bamsat_E_ena	= 1;

	PC8.ADC_raw_sel = 0;

	Radar.UDP_Delay_T1 = 160;
	Radar.UDP_Delay_T2 = 160;
	Radar.UDP_Delay_T3 = 160;
	Radar.UDP_Delay_T4 = 80;
	Radar.UDP_Delay_T5 = 40;

	Radar.UDP_Delay_V1  = 16;
	Radar.UDP_Delay_V2  = 16;
	Radar.UDP_Delay_V3  = 16;
	Radar.UDP_Delay_V4  = 16;
	Radar.UDP_Delay_V5  = 16;

	Radar.Screen_Mode 	= 0;
	Radar.IP_Data_ready = 0;
	Radar.IP_Data_time  = 0;

	PC8.IP_BW_SELi 		= 0;
	PC8.IP_BW_SEL 		= 0;

	PC8.Beam17_MAX_Mode = 0;

	PC8.BamSat.Nguong_H_DK_Ue_H = 0.05;
	PC8.BamSat.Nguong_L_DK_Ue_H = -0.05;
	PC8.BamSat.Nguong_H_DK_Ue_E = 0.05;
	PC8.BamSat.Nguong_L_DK_Ue_E = -0.05;

	PC8.BamSat.Ue_Min_kenh_H = -2.5;
	PC8.BamSat.Ue_Max_kenh_H =  2.5;
	PC8.BamSat.Ue_Min_kenh_E = -2.5;
	PC8.BamSat.Ue_Max_kenh_E =  2.5;

	PC8.BamSat.Giam_mau_xuly   = 3;
	PC8.BamSat.Chieu_DK_Bam_H  = 0;
	PC8.BamSat.Chieu_DK_Bam_E  = 0;
	PC8.BamSat.Beam_Max_Vali   = 0;

	PC8.BamSat.BS_Fix_H = 4;// 20
	PC8.BamSat.BS_Fix_E = 3;// 20

	PC8.BamSat.BS_Fix_H0 = 1;// 20
	PC8.BamSat.BS_Fix_E0 = 1;// 20

	PC8.BamSat.Min_Detect = 512;
	PC8.BamSat.BS_CS_Delta = 10;
	PC8.BamSat.Bam_culy_ENA= 1;

	PC8.ATT_Mode = 1;
	PC8.ATT_Part = 0;
	PC8.ATT_Data_Mode = 0;

	for(point = 0; point < 16; point++)
	{
		PC8.S17H_Ueb[point] = 0;//31
		PC8.S17E_Ueb[point] = 0;//31
	}

	for(point = 0; point < 16; point++)
	{
		PC8.BamSat.TL_Phi[point] 		= 0;
		PC8.BamSat.TL_Epxilon[point] 	= 0;
	}

	PC8.BamSat.TL_Phi_final 	= 0.0;
	PC8.BamSat.TL_Epxilon_final = 0.0;

	// for(point = 0; point < 200; point++)
	// {
	// 	Video_CS_fb1[point] = 0;
	// 	Video_CS_fb2[point] = 0;
	// 	Video_CS_fb3[point] = 0;
	// 	Video_CS_fb4[point] = 0;
	// 	Video_CS_fb5[point] = 0;
	// 	Video_CS_fb6[point] = 0;
	// 	Video_CS_fb7[point] = 0;
	// 	Video_CS_fb8[point] = 0;
	// 	Video_CS_fb9[point] = 0;
	// 	Video_CS_fb10[point] = 0;
	// 	Video_CS_fb11[point] = 0;
	// 	Video_CS_fb12[point] = 0;
	// 	Video_CS_fb13[point] = 0;
	// 	Video_CS_fb14[point] = 0;
	// 	Video_CS_fb15[point] = 0;
	// 	Video_CS_fb16[point] = 0;
	// 	Video_CS_fb17[point] = 0;
	// }

	PC8.Excel.Loading_E_Angle_done = 0;
	PC8.Excel.Loading_E_Ue_done = 0;
	PC8.Excel.Loading_H_Angle_done = 0;
	PC8.Excel.Loading_H_Ue_done = 0;
	PC8.Excel.Loading_done = 0;
	PC8.Excel.Load_Point = 0;
	PC8.Excel.Point = 0;

	for(point = 0; point < 432; point++)
	{
		PC8.Excel.AngleE[point] = 0.0f;
		PC8.Excel.Ue_E[point] 	= 0.0f;
		PC8.Excel.AngleH[point] = 0.0f;
		PC8.Excel.Ue_H[point] 	= 0.0f;
	}

	PC8.BamSat.Delta_beta= 0;
	PC8.BamSat.Delta_alpha= 0;

	PC8.BamSat.Motor_Speed = 3;
	PC8.BamSat.Scan_Speed = 2;
	PC8.BamSat.Bamsat_Speed = 1;

	PC8.BS_Center_start2 = 0;
	Radar.Screen_Modei = 0;
	PC8.Gia_Quay_ENi = 0;

	PC8.BamSat._CS_H_Ready = 0;  // l dieu khien kenh H
	PC8.BamSat._CS_E_Ready = 0;  // l dieu khien kenh E
	PC8.BamSat.BS_Beam_mode = 0;

	PC8.BamSat._Current_beta 	= 0.0;
	PC8.BamSat._Current_Heading = 0.0;
	PC8.BamSat._Current_V 		= 0.0;
	PC8.BamSat._Current_Vx 		= 0.0;
	PC8.BamSat._Current_Vy 		= 0.0;
	PC8.BamSat._Current_Start 	= 0;

	PC8.BamSat.BS_Quat_mode = 0;
	PC8.BamSat.fb_SNR_Scale = 2.5;
	PC8.BamSat.Beam_Scale = 0.55;

	PC8.DAC_KS_Fix = 0;

	PC8.Sync_time = 0;
	PC8.TG_EN = 0;
	PC8.TG_Stop = 0;
	PC8.TG_Mode = 0;

	PC8.TG_ENi = 0; // 38
	PC8.TG_Click_time = 0; // 38

	PC8.TG_Alpha0 = 0; // 38
	PC8.TG_Beta0 = 0; // 38

	PC8.TG_Alpha = 0; // 38
	PC8.TG_Beta = 0; // 38

	PC8.TG_Alpha_table[0] = -1; // 38
	PC8.TG_Alpha_table[1] =  1; // 38
	PC8.TG_Alpha_table[2] =  0; // 38
	PC8.TG_Alpha_table[3] = -1; // 38
	PC8.TG_Alpha_table[4] =  1; // 38
	PC8.TG_Alpha_table[5] = -1; // 38
	PC8.TG_Alpha_table[6] = -1; // 38
	PC8.TG_Alpha_table[7] = -2; // 38
	PC8.TG_Alpha_table[8] =  0; // 38
	PC8.TG_Alpha_table[9] = -1; // 38
	PC8.TG_Alpha_table[10] = 1; // 38
	PC8.TG_Alpha_table[11] =-1; // 38
	PC8.TG_Alpha_table[12] =-1; // 38
	PC8.TG_Alpha_table[13] = 0; // 38
	PC8.TG_Alpha_table[14] = 1; // 38
	PC8.TG_Alpha_table[15] = 0; // 38

	PC8.TG_Beta_table[15] = -1; // 38
	PC8.TG_Beta_table[14] =  1; // 38
	PC8.TG_Beta_table[13] =  0; // 38
	PC8.TG_Beta_table[12] = -1; // 38
	PC8.TG_Beta_table[11] =  1; // 38
	PC8.TG_Beta_table[10] =  0; // 38
	PC8.TG_Beta_table[9] = -1; // 38
	PC8.TG_Beta_table[8] =  1; // 38
	PC8.TG_Beta_table[7] =  0; // 38
	PC8.TG_Beta_table[6] = -1; // 38
	PC8.TG_Beta_table[5] = 1; // 38
	PC8.TG_Beta_table[4] = 0; // 38
	PC8.TG_Beta_table[3] = -1; // 38
	PC8.TG_Beta_table[2] = 0; // 38
	PC8.TG_Beta_table[1] = 1; // 38
	PC8.TG_Beta_table[0] =-1; // 38

	PC8.Beam_Max_View = 0;

	PC8.Khaosat_Enable = 0;
	PC8.Khaosat_Enablei = 0;

	PC8.Ue_Min = -1;//31
	PC8.Ue_Max = 1;//31

	PC8.FFT_Sel = 0;// 20
	PC8.FFT_Seli = 0;// 20

	PC8.Sum_Sel = 0;// 20
	PC8.Sum_Seli = 0;// 20

	PC8.BS_Angle_Ez0 = 0;
	PC8.BS_Angle_Hz0 = 0;

	Radar.UDP_PC8_Start2 = 0;

	PC8.Control_View_SS = 0; // quet lien tuc

	PC8.BS_Angle_Last_Ez0 = 0;
	PC8.BS_Angle_Last_Hz0 = 0;

	PC8.BS_Scan_Start = 0;
	PC8.Screen_Phase = 0;

	PC8.beam_Channeli2 = 0;
	PC8.fb_Channeli2 = 0;

	PC8.BS_pH_Kalman = 0.45; // don vi: ly giac
	PC8.BS_pE_Kalman = 0.45; // don vi: ly giac

	PC8.BS_Center_start = 0;// 20
	PC8.BS_Center_starti= 0;// 20
	PC8.BS_Center_start_bs0 = 0;

	PC8._KS_Pvi.Loading 	= 0;
	PC8._KS_Pvi.Point 		= 0;
	PC8._KS_Pvi.Max_Point 	= 0;
	PC8._KS_Pvi.Number 		= 0;

	PC8._KS_Ta.Loading 		= 0;
	PC8._KS_Ta.Point 		= 0;
	PC8._KS_Ta.Max_Point 	= 0;
	PC8._KS_Ta.Number 		= 0;

	PC8.ADC_ATTi  = 0;// 20
	PC8.ADC_ATTiv = 0;// 20

	PC8.DAC_VOP = 30;// 20
	PC8.DAC_VOPi= 30;// 20

	PC8.DAC_KS_Enable		= 0;
	PC8.DAC_H_E_Val			= 0;
	PC8.DAC_Center_Val		= 100;

	for(point = 0; point < 334; point++)
	{
		PC8._KS_Pvi.V1[point] = 0;
		PC8._KS_Pvi.V2[point] = 0;
		PC8._KS_Pvi.S17[point]= 0;
		PC8._KS_Pvi.Ue[point] = 0;

		PC8._KS_Ta.V1[point] = 0;
		PC8._KS_Ta.V2[point] = 0;
		PC8._KS_Ta.S17[point]= 0;
		PC8._KS_Ta.Ue[point] = 0;
	}

}
