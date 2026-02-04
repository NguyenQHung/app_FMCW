#include "DK_Maythu.h"
#include "ADF4159.h"
#include "ADF4360.h"
#include "Radar_HW.h"
#include "MI_505.h"
#include "Erob.h"
#include "CAN_Control.h"

/****************************************************************************/
void Radar_Update_GPS_Wind(void)
{
	Radar_Debug.Update_Debug 		= 0;
	Radar_Debug.Update_Enable 		= Radar_Debug.Radar_Config[3];
	//Radar_Debug.Config_done		= 0;
	Radar_Debug.Fix_Wind_Angle		= Radar_Debug.Radar_Config[4] * 256 + Radar_Debug.Radar_Config[5];
	Radar_Debug.Fix_Wind_Speed		= Radar_Debug.Radar_Config[6] * 256 + Radar_Debug.Radar_Config[7];
	Radar_Debug.GPS_LAT_Size		= Radar_Debug.Radar_Config[8];
	Radar_Debug.GPS_LON_Size		= Radar_Debug.Radar_Config[9];
	Radar_Debug.GPS_Speed_Size		= Radar_Debug.Radar_Config[10];
	Radar_Debug.GPS_Point_LAT1		= Radar_Debug.Radar_Config[11];
	Radar_Debug.GPS_Point_LAT2		= Radar_Debug.Radar_Config[12];
	Radar_Debug.GPS_Point_LAT3		= Radar_Debug.Radar_Config[13];
	Radar_Debug.GPS_Point_LAT4		= Radar_Debug.Radar_Config[14];
	Radar_Debug.GPS_Point_LAT5		= Radar_Debug.Radar_Config[15];
	Radar_Debug.GPS_Point_LAT6		= Radar_Debug.Radar_Config[16];
	Radar_Debug.GPS_Point_LAT7		= Radar_Debug.Radar_Config[17];
	Radar_Debug.GPS_Point_LAT8		= Radar_Debug.Radar_Config[18];
	Radar_Debug.GPS_Point_LON1		= Radar_Debug.Radar_Config[19];
	Radar_Debug.GPS_Point_LON2		= Radar_Debug.Radar_Config[20];
	Radar_Debug.GPS_Point_LON3		= Radar_Debug.Radar_Config[21];
	Radar_Debug.GPS_Point_LON4		= Radar_Debug.Radar_Config[22];
	Radar_Debug.GPS_Point_LON5		= Radar_Debug.Radar_Config[23];
	Radar_Debug.GPS_Point_LON6		= Radar_Debug.Radar_Config[24];
	Radar_Debug.GPS_Point_LON7		= Radar_Debug.Radar_Config[25];
	Radar_Debug.GPS_Point_LON8		= Radar_Debug.Radar_Config[26];
	Radar_Debug.GPS_Point_Speed1	= Radar_Debug.Radar_Config[27];
	Radar_Debug.GPS_Point_Speed2	= Radar_Debug.Radar_Config[28];
	Radar_Debug.GPS_Point_Speed3	= Radar_Debug.Radar_Config[29];
	Radar_Debug.GPS_Point_Speed4	= Radar_Debug.Radar_Config[30];
	Radar_Debug.GPS_Point_Speed5	= Radar_Debug.Radar_Config[31];
	Radar_Debug.Wind_Point_Angle1	= Radar_Debug.Radar_Config[32];
	Radar_Debug.Wind_Point_Angle2	= Radar_Debug.Radar_Config[33];
	Radar_Debug.Wind_Point_Angle3	= Radar_Debug.Radar_Config[34];
	Radar_Debug.Wind_Point_Speed1	= Radar_Debug.Radar_Config[35];
	Radar_Debug.Wind_Point_Speed2	= Radar_Debug.Radar_Config[36];
	Radar_Debug.Wind_Point_Speed3	= Radar_Debug.Radar_Config[37];
	Radar_Debug.Wind_Point_Speed4	= Radar_Debug.Radar_Config[38];
}

void Radar_Update_DDS_Phase(void)
{
	Radar_Debug.Update_Debug2 		= 0;
	Radar_Debug.Update_Enable2		= Radar_Debug.Radar_Config[3];
	Radar_Debug.Update_Phase_Amp 	= Radar_Debug.Radar_Config[4]; // ma dieu che
	Radar_Debug.Update_LPF			= Radar_Debug.Radar_Config[5];
	Radar_Debug.Update_DDS_Source 	= Radar_Debug.Radar_Config[6];
	Radar_Debug.Update_DDS_Sine		= Radar_Debug.Radar_Config[7];
	Radar_Debug.Update_DDS_Data		= Radar_Debug.Radar_Config[8];
	Radar_Debug.Update_IIR_Mode		= Radar_Debug.Radar_Config[9];
	Radar_Debug.Update_Ngan_Dai_View= Radar_Debug.Radar_Config[10];
	Radar_Debug.Update_Ngan_Dai_Mode= Radar_Debug.Radar_Config[11];
	Radar_Debug.Update_Doggy_EN		= Radar_Debug.Radar_Config[12];

	Radar_Debug.Update_MAX1			= Radar_Debug.Radar_Config[13] +
									  Radar_Debug.Radar_Config[14] * 256 +
									  Radar_Debug.Radar_Config[15] * 65536 +
									  Radar_Debug.Radar_Config[16] * 16777216;

	//Radar_Debug.Update_MAX2			= Radar_Debug.Radar_Config[17] +
	//								  Radar_Debug.Radar_Config[18] * 256 +
	//								  Radar_Debug.Radar_Config[19] * 65536 +
	//								  Radar_Debug.Radar_Config[20] * 16777216;

	//Radar_Debug.Update_MIN2 = Radar_Debug.Update_MAX2 * -1;

	Radar_Debug.Update_MAX2 = 180000;
	Radar_Debug.Update_MIN2 = -180000;

//	Radar_SET_ADC_LPF(Radar_Debug.Update_LPF);
//	Radar_SET_ADC_DDS_Source(Radar_Debug.Update_DDS_Source);
//	Radar_SET_IIR_Source(Radar_Debug.Update_DDS_Data);
//	DDS_Set_Time3(0);//Radar_Debug.Update_DDS_Sine);
	//Radar_SET_DDS_View_Edge(1 - Radar_Debug.Update_IIR_Mode);
//	Radar_Cal_ADC_LPF(Radar_Debug.Update_LPF);
//	Radar_SET_DDS_View_Channel(Radar_Debug.Update_DDS_Sine);
	//AD9628_Set_Offset(Radar_Debug.Update_Offset);
	//Radar_SET_ADC_LPF_Val();

//	Radar_SET_Xung_Ngan_Dai_View(Radar_Debug.Update_Ngan_Dai_View);
//
//	Radar_SET_Xung_Ngan_Dai_Mode(1); //Radar_Debug.Update_Ngan_Dai_Mode);
//
//	//Radar_Debug.Update_Doggy_EN
//	Radar_SET_Save_Length1(Radar_Debug.Update_Doggy_EN);
//	Radar_SET_Save_Length2(Radar_Debug.Update_Doggy_EN);

	//Radar_BitSet1(Radar_Debug.Update_Ngan_Dai_Mode);
	//Radar_BitSet2(Radar_Debug.Update_Ngan_Dai_Mode);

//	Radar_SET_Xung_Ngan_Dai_Max(50000);
//	Radar_SET_DDS_MAX_ENABLE(1);//Radar_Debug.Update_IIR_Mode);
//
//	Radar_SET_DDS_MAX1(Radar_Debug.Update_MAX2);
//	Radar_SET_DDS_MIN1(Radar_Debug.Update_MIN2);

	xil_printf("Update_Phase_Amp = %d\n\r", Radar_Debug.Update_Phase_Amp);
	xil_printf("Update_DDS_Source = %d\n\r", Radar_Debug.Update_DDS_Source);
	xil_printf("Update_DDS_Data = %d\n\r", Radar_Debug.Update_DDS_Data);
	xil_printf("Update_DDS_Sine = %d\n\r", Radar_Debug.Update_DDS_Sine);
	xil_printf("Update_MAX2 = %d\n\r", Radar_Debug.Update_MAX2);
}

void Radar_Frame_GPS_Wind(void)
{
	Radar_Debug.Radar_Debug[0] = 'G';
	Radar_Debug.Radar_Debug[1] = 'P';
	Radar_Debug.Radar_Debug[2] = 'S';

	Radar_Debug.Radar_Debug[3] = Radar_Debug.Update_GPS;
	Radar_Debug.Radar_Debug[4] = Radar_Debug.Update_Wind;
	Radar_Debug.Radar_Debug[5] = 0;
	Radar_Debug.Radar_Debug[6] = 0;
	Radar_Debug.Radar_Debug[7] = 0;
	Radar_Debug.Radar_Debug[8] = 0;
	Radar_Debug.Radar_Debug[9] = 0;
	Radar_Debug.Radar_Debug[10] = 0;
	Radar_Debug.Radar_Debug[11] = 0;
	Radar_Debug.Radar_Debug[12] = 0;

	for(Radar.counter=0; Radar.counter < 44; Radar.counter++)
	{
		Radar_Debug.Radar_Debug[Radar.counter + 13] = Radar_Debug.GPS_Frame[Radar.counter];
	}

	for(Radar.counter=0; Radar.counter < 24; Radar.counter++)
	{
		Radar_Debug.Radar_Debug[Radar.counter + 57] = Radar_Debug.Wind_Frame[Radar.counter];
	}

	for(Radar.counter = 81; Radar.counter < 150; Radar.counter++)
	{
		Radar_Debug.Radar_Debug[Radar.counter] = 0;
	}

	if(Radar_Debug.Update_GPS==1) Radar_Debug.Update_GPS = 0;
	if(Radar_Debug.Update_Wind==1) Radar_Debug.Update_Wind = 0;

}

void Radar_Frame_Phase(void)
{
	PA_Data2[0] = 'D';
	PA_Data2[1] = 'D';
	PA_Data2[2] = 'S';
	PA_Data2[3] = Radar_Debug.Update_Enable2;
	PA_Data2[4] = Radar_Debug.Update_Phase_Amp;
	PA_Data2[5] = 8192 % 256;
	PA_Data2[6] = 8192 / 256;
	PA_Data2[7] = 0;
}

