#include "DK_Maythu.h"
#include "ADF4159.h"
#include "ADF4360.h"
#include "Radar_HW.h"
#include "MI_505.h"
#include "Erob.h"
#include "CAN_Control.h"

/****************************************************************************/

void Radar_Update_PC1_Packet(void)
{
	//Radar.Thang_Culy	=5;
	//Radar.Sample		=5100;
	unsigned char counter1=0;

	Radar.Radar_2_2Ai	= Radar.Radar_Config[3];
	Radar.Active		= Radar.Radar_Config[4];
	Radar.On			= Radar.Radar_Config[5];
	Radar.Start_Angle	= Radar.Radar_Config[6]*256 + Radar.Radar_Config[7];
	Radar.End_Angle		= Radar.Radar_Config[8]*256 + Radar.Radar_Config[9];
	Radar.Sample_Size	= Radar.Radar_Config[10];
	Radar.Test_mode		= Radar.Radar_Config[11];
	Radar.Delay_2		= Radar.Radar_Config[12];
	Radar.Delay_2A		= Radar.Radar_Config[13];
	Radar.TriggerLevel	= Radar.Radar_Config[14];
	Radar.ShortTime		= Radar.Radar_Config[15];

	if(PC8.Cu_ly_EN == 0)
	{
		xil_printf("PC1_Config \r\n" );
		//Radar.Sample		=5220;
		//Radar.Config_Done	=0;
		AFC1.Auto_Counter	= 0;
		AFC1.Auto_Start		= 0;
		AFC1.Disconect 		= 0;
		Radar.Range			= Radar.Radar_Config[2];

		for (counter1 = 0; counter1 < 34; counter1++)
		Radar.Dutru2[counter1]=Radar.Radar_Config[counter1 + 16];

		if(Radar.Range<=1)
		{
			Radar.Sample		=	256;
			//Radar.Sample		=	612;
			Radar.ADC_Freq0		=	20;
			Radar.Noise_Clear 	= 	1;
			Radar.Noise_counter	=	0;

			Radar.Range2		=	1;
			Radar.ADC_Cells		=	256;
			Radar.ADC_Time_us	=	30;
			Radar.ADC_Num_CLK	=	1500;
			Radar.ADC_Pulse_CLK =	200;

		}
		else if(Radar.Range==2)
		{
			Radar.Sample		=	1024;
			//Radar.Sample		=	2660;
			Radar.ADC_Freq0		=	20;
			Radar.Noise_Clear 	= 	1;
			Radar.Noise_counter	=	0;

			Radar.Range2		=	2;
			Radar.ADC_Cells		=	1024;
			Radar.ADC_Time_us	=	120;
			Radar.ADC_Num_CLK	=	6000;
			Radar.ADC_Pulse_CLK =	200;

		}
		else if(Radar.Range==3)
		{
			Radar.Sample		=	2048;
			//Radar.Sample		=	5220;
			Radar.ADC_Freq0		=	20;
			Radar.Noise_Clear 	= 	1;
			Radar.Noise_counter	=	0;

			Radar.Range2		=	3;
			Radar.ADC_Cells		=	2048;
			Radar.ADC_Time_us	=	210;
			Radar.ADC_Num_CLK	=	10500;
			Radar.ADC_Pulse_CLK =	200;

		}
		else if(Radar.Range==4)
		{
			//Radar.Sample		=	5220;
			Radar.Sample		=	4096;
			Radar.ADC_Freq0		=	20;
			Radar.Noise_Clear 	= 	1;
			Radar.Noise_counter	=	0;

			Radar.Range2		=	4;
			Radar.ADC_Cells		=	4096;
			Radar.ADC_Time_us	=	420;
			Radar.ADC_Num_CLK	=	21000;
			Radar.ADC_Pulse_CLK =	200;

		}
		else if(Radar.Range==5)
		{
			Radar.Sample		=	4096;
			//Radar.Sample		=	5220;
			Radar.ADC_Freq0		=	10;
			Radar.Noise_Clear 	= 	1;
			Radar.Noise_counter	=	0;

			Radar.Range2		=	5;
			Radar.ADC_Cells		=	4096;
			Radar.ADC_Time_us	=	820;
			Radar.ADC_Num_CLK	=	41000;
			Radar.ADC_Pulse_CLK =	200;
		}

		IP_Set_thang_culy(Radar.Range);
		System_Set_Beam17_RAM(Radar.Range);
		IP_Set_Range(Radar.ADC_Cells);
		System_Set_ATT_Dmax(Radar.ADC_Cells);
		System_Set_trigger(0, Radar.ADC_Pulse_CLK, Radar.ADC_Num_CLK); // 820us x 50 = 41000

//		xil_printf("Radar.Range 		= %d\r\n",Radar.Range2);
//		xil_printf("Radar.ADC_Cells 	= %d\r\n",Radar.ADC_Cells);
//		xil_printf("Radar.ADC_Time_us 	= %d\r\n",Radar.ADC_Time_us);
//		xil_printf("Radar.ADC_Num_CLK 	= %d\r\n",Radar.ADC_Num_CLK);
//		xil_printf("Radar.ADC_Pulse_CLK = %d\r\n",Radar.ADC_Pulse_CLK);
	}

}

