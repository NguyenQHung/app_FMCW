#include "DK_Maythu.h"
#include "ADF4159.h"
#include "ADF4360.h"
#include "Radar_HW.h"
#include "MI_505.h"
#include "Erob.h"
#include "CAN_Control.h"

/****************************************************************************/

void PC2_Get_Frame(void)
{
	uint32_t LAT_buff = (uint32_t)(Frame.Radar_GPS_Latitude * 10000);
	uint32_t LONG_buff = (uint32_t)(Frame.Radar_GPS_Longitude * 10000);
	uint16_t SPEED_buff = Radar.Board_speed1;
	uint16_t Compass1_buff = (uint16_t)(Frame.Radar_Azimuth);
	uint16_t Compass2_buff = 0;//grayToBinary(ENC_Compass);
	//uint16_t buff3 = 0;
	uint16_t gpio_in = 0;//XGpio_DiscreteRead(&GPIO_MI505, 1);

	//buff3 = (int)(PC8.Delta_Angle0 * 10);
	//Compass1_Status=0;
	//Compass2_Status=0;
	Radar.TC_TX			= (gpio_in & 0x0001) ? 1 : 0;
	Radar.Phat_TC		= (gpio_in & 0x0004) ? 1 : 0;
	Radar.SoiDot_TC		= (gpio_in & 0x0002) ? 1 : 0;
	Radar.Dong_SoiDot1	= (gpio_in & 0x0010) ? 1 : 0;
	Radar.Dong_SoiDot2	= (gpio_in & 0x0040) ? 1 : 0;
	//xil_printf("E1 = %d\n\r",PC8.Beta_ENC);
	//xil_printf("E2 = %d\n\r",PC8.Alpha_ENC);

	if(Radar.GPS_Status==1)
		{
		Radar.GPS_Connect=1;
		Radar.GPS_Status = 0;
		Radar.GPS_counter=0;
		}
	else if(Radar.GPS_Status==0)
		{
		if(Radar.GPS_counter<180) Radar.GPS_counter++;

		if(Radar.GPS_counter>=180)
			{
			Radar.GPS_Connect=0;
			}
		}


	if(Radar.Compass1_Status==1)
		{
		Radar.Compass1_Connect=1;
		Radar.Compass1_Status = 0;
		Radar.Compass_counter=0;
		}
	else if(Radar.Compass1_Status==0)
		{
		if(Radar.Compass_counter<30) Radar.Compass_counter++;

		if(Radar.Compass_counter>=30)
			{
			Radar.Compass1_Connect=0;
			}

		}


	if(Radar.Wind_Connect==1)
		{
		Radar.Wind_Connect2 = 1;
		Radar.Wind_Connect = 0;
		Radar.Wind_Connect1= 0;
		}
	else if(Radar.Wind_Connect==0)
		{
		if(Radar.Wind_Connect1<120) Radar.Wind_Connect1++;

		if(Radar.Wind_Connect1>=120)
			{
			Radar.Wind_Connect2=0;
			}
		}


		if(Radar.AIS_Counter < 600)
			{

			Radar.AIS_Counter++;

			}
		else if(Radar.AIS_Counter >= 600)
			{
			Radar.AIS_Connect = 0;
			}


	PC2_Frame[0] = 0xaa;
	PC2_Frame[1] = 0x55;
	PC2_Frame[2] = Radar.GPS_Connect;
	PC2_Frame[3] = (LAT_buff  & 0x00ff0000) >> 16;
	PC2_Frame[4] = (LAT_buff  & 0x0000ff00) >> 8;
	PC2_Frame[5] = (LONG_buff & 0x00ff0000) >> 16;
	PC2_Frame[6] = (LONG_buff & 0x0000ff00) >> 8;
	PC2_Frame[7] = SPEED_buff / 256;
	PC2_Frame[8] = SPEED_buff % 256;
	PC2_Frame[9] = Radar.Compass1_Connect;
	PC2_Frame[10] = Compass1_buff / 256;
	PC2_Frame[11] = Compass1_buff % 256;
	PC2_Frame[12] = Radar.Compass2_Connect;
	PC2_Frame[13] = Compass2_buff / 256;
	PC2_Frame[14] = Compass2_buff % 256;
	PC2_Frame[15] = 1;
	PC2_Frame[16] = PC8.Beta_Angle / 256;
	PC2_Frame[17] = PC8.Beta_Angle % 256;
	PC2_Frame[18] = PC8.Beta_Stop;
	PC2_Frame[19] = gpio_in / 256;
	PC2_Frame[20] = gpio_in % 256;
	PC2_Frame[21] = Radar.TC_TX;
	PC2_Frame[22] = Radar.Phat_TC;
	PC2_Frame[23] = Radar.SoiDot_TC;
	PC2_Frame[24] = (LAT_buff & 0x000000ff);
	PC2_Frame[25] = (LONG_buff & 0x000000ff);
	PC2_Frame[26] = Radar.AVG_Value;
	PC2_Frame[27] = AFC1.Error_counter;
	PC2_Frame[28] = AFC1.Error_counter2;
	PC2_Frame[29] = AFC1.Max_Xung_Tot / 256;
	PC2_Frame[30] = AFC1.Max_Xung_Tot % 256;
	PC2_Frame[31] = AFC1.Max_Lech_Phai / 256;
	PC2_Frame[32] = AFC1.Max_Lech_Phai % 256;
	PC2_Frame[33] = AFC1.Max_Lech_Trai / 256;
	PC2_Frame[34] = AFC1.Max_Lech_Trai % 256;
	PC2_Frame[35] = AFC1.Fnsai / 256;
	PC2_Frame[36] = AFC1.Fnsai % 256;
	PC2_Frame[37] = AFC1.Fphat / 256;
	PC2_Frame[38] = AFC1.Fphat % 256;
	PC2_Frame[39] = Radar.AFC;
	PC2_Frame[40] = AFC1.Led_Tot;
	PC2_Frame[41] = AFC1.Level_Phai;
	PC2_Frame[42] = AFC1.Level_Trai;
	PC2_Frame[43] = AFC1.Control_Value / 256;
	PC2_Frame[44] = AFC1.Control_Value % 256;
	PC2_Frame[45] = AFC1.U_nguong / 256;
	PC2_Frame[46] = AFC1.U_nguong % 256;
	PC2_Frame[47] = Radar.ENC_Error1 / 256;
	PC2_Frame[48] = Radar.ENC_Error1 % 256;
	PC2_Frame[49] = Radar.Volt_Data[0] / 256;
	PC2_Frame[50] = Radar.Volt_Data[0] % 256;
	PC2_Frame[51] = Radar.Volt_Data[1] / 256;
	PC2_Frame[52] = Radar.Volt_Data[1] % 256;
	PC2_Frame[53] = Radar.Volt_Data[2] / 256;
	PC2_Frame[54] = Radar.Volt_Data[2] % 256;
	PC2_Frame[55] = Radar.Volt_Data[3] / 256;
	PC2_Frame[56] = Radar.Volt_Data[3] % 256;
	PC2_Frame[57] = Radar.Volt_Data[4] / 256;
	PC2_Frame[58] = Radar.Volt_Data[4] % 256;
	PC2_Frame[59] = Radar.Volt_Data[5] / 256;
	PC2_Frame[60] = Radar.Volt_Data[5] % 256;
	PC2_Frame[61] = Radar.Volt_Data[6] / 256;
	PC2_Frame[62] = Radar.Volt_Data[6] % 256;
	PC2_Frame[63] = Radar.Volt_Data[7] / 256;
	PC2_Frame[64] = Radar.Volt_Data[7] % 256;
	PC2_Frame[65] = Radar.Temp_val / 256;
	PC2_Frame[66] = Radar.Temp_val % 256;
	PC2_Frame[67] = Radar.Temp_min / 256;
	PC2_Frame[68] = Radar.Temp_min % 256;
	PC2_Frame[69] = Radar.Temp_max / 256;
	PC2_Frame[70] = Radar.Temp_max % 256;
	PC2_Frame[71] = Radar.AIS_Connect;


	PC2_Frame[72] =  AIS_Data.AIS_PC2;
	PC2_Frame[73] =  Frame.Radar_AIS_Status;
	PC2_Frame[74] = (Frame.Radar_AIS_MMSI & 0x000000ff);
	PC2_Frame[75] = (Frame.Radar_AIS_MMSI & 0x0000ff00)>>8;
	PC2_Frame[76] = (Frame.Radar_AIS_MMSI & 0x00ff0000)>>16;
	PC2_Frame[77] = (Frame.Radar_AIS_MMSI & 0xff000000)>>24;
	PC2_Frame[78] = (Frame.Radar_AIS_Longitude & 0x000000ff);
	PC2_Frame[79] = (Frame.Radar_AIS_Longitude & 0x0000ff00)>>8;
	PC2_Frame[80] = (Frame.Radar_AIS_Longitude & 0x00ff0000)>>16;
	PC2_Frame[81] = (Frame.Radar_AIS_Longitude & 0xff000000)>>24;
	PC2_Frame[82] = (Frame.Radar_AIS_Latitude & 0x000000ff);
	PC2_Frame[83] = (Frame.Radar_AIS_Latitude & 0x0000ff00)>>8;
	PC2_Frame[84] = (Frame.Radar_AIS_Latitude & 0x00ff0000)>>16;
	PC2_Frame[85] = (Frame.Radar_AIS_Latitude & 0xff000000)>>24;
	PC2_Frame[86] = AIS_Data.Speed_Over_Ground % 256;
	PC2_Frame[87] = AIS_Data.Speed_Over_Ground / 256;
	PC2_Frame[88] = AIS_Data.True_Heading % 256;
	PC2_Frame[89] = AIS_Data.True_Heading / 256;

	PC2_Frame[90] = Radar.Wind_Connect2;
	PC2_Frame[91] = Frame.Radar_Wind_Speed % 256;
	PC2_Frame[92] = Frame.Radar_Wind_Speed / 256;
	PC2_Frame[93] = Frame.Radar_Wind_Angle % 256;
	PC2_Frame[94] = Frame.Radar_Wind_Angle / 256;
	PC2_Frame[95] = Frame.Radar_GPS_Speed % 256;
	PC2_Frame[96] = Frame.Radar_GPS_Speed / 256;
	PC2_Frame[97] = Radar.ENC_Compass % 256;
	PC2_Frame[98] = Radar.ENC_Compass / 256;
	PC2_Frame[99] = 0x0d;

	AIS_Data.AIS_PC2 = 0;

	memcpy(&Radar_Frame2[0]  , &PC2_Frame, 100);

}

void Radar_Update_PC2_Packet(void)
{
	Radar_Load_Config();
	Radar_Setup_AFC();
	Radar_Setup_HW();
}

void Radar_Load_Config(void)
{
	uint16_t Buff11=0;
	// dieu khien may thu
	//PC8.Trigger			= Radar.Radar_Config2[38];
	Radar.Compass_PC2_Fix =(Radar.Radar_Config2[0] + (Radar.Radar_Config2[1] & 0x0f) * 256);
	Radar.Compass_PC2_Val =(Radar.Radar_Config2[0] + (Radar.Radar_Config2[1] & 0x0f) * 256);
	Radar.Compass_PC2_EN  = Radar.Radar_Config2[1] / 16;
	Radar.GPS_Setup		  = Radar.Radar_Config2[2];

	if(Radar.GPS_Setup == 50)
	{
		Radar.GPS_LAT[0] = Radar.Radar_Config2[3];
		Radar.GPS_LAT[1] = Radar.Radar_Config2[4];
		Radar.GPS_LAT[2] = Radar.Radar_Config2[5];
		Radar.GPS_LAT[3] = Radar.Radar_Config2[6];
		Radar.GPS_LAT[4] = Radar.Radar_Config2[7];
		Radar.GPS_LAT[5] = Radar.Radar_Config2[8];
		Radar.GPS_LAT[6] = Radar.Radar_Config2[9];
		Radar.GPS_LAT[7] = Radar.Radar_Config2[10];

		Radar.GPS_LON[0] = Radar.Radar_Config2[11];
		Radar.GPS_LON[1] = Radar.Radar_Config2[12];
		Radar.GPS_LON[2] = Radar.Radar_Config2[13];
		Radar.GPS_LON[3] = Radar.Radar_Config2[14];
		Radar.GPS_LON[4] = Radar.Radar_Config2[15];
		Radar.GPS_LON[5] = Radar.Radar_Config2[16];
		Radar.GPS_LON[6] = Radar.Radar_Config2[17];
		Radar.GPS_LON[7] = Radar.Radar_Config2[18];
	}
	else if(Radar.GPS_Setup == 51)
	{
		PC8.CSP		= Radar.Radar_Config2[4];
		PC8.BoLoc	= Radar.Radar_Config2[3];
		//xil_printf("CSP = %d\n\r", PC8.CSP);
		//xil_printf("BoLoc = %d\n\r", PC8.BoLoc);
	}

	Radar.Muc_Nhieu		= Radar.Radar_Config2[19];

	Radar.PC7_Control	= Radar.Radar_Config2[20];
	Radar.PC7_ThangCuLy	= Radar.Radar_Config2[21];

	Radar.Debug_line	= Radar.Radar_Config2[22];
	Radar.ENC_Filter_time=Radar.Radar_Config2[23];
	Radar.ENC_Filter_Ena= Radar.Radar_Config2[24];
	Radar.AFC_Point		= Radar.Radar_Config2[25];
	PC8.Sau_Bapy		= Radar.Radar_Config2[26];
	PC8.Rong_Bapy		= Radar.Radar_Config2[27];
	PC8.KDMT			= Radar.Radar_Config2[28];
	PC8.ADC_Channel		= Radar.Radar_Config2[29];
	PC8.CLK_10MHz_Source= Radar.Radar_Config2[30];
	Radar.Delta = (PC8.Sau_Bapy - PC8.KDMT);

	Radar.Ngoai_sai		= Radar.Radar_Config2[32] * 256 + Radar.Radar_Config2[31];
	Radar.Angle_Start 	= Radar.Radar_Config2[34] * 256 + Radar.Radar_Config2[33];
	Radar.Angle_Stop  	= Radar.Radar_Config2[36] * 256 + Radar.Radar_Config2[35];
	Radar.Angle_mode_ENA= Radar.Radar_Config2[37];
	Radar.Trigger_MUX 	= Radar.Radar_Config2[38];
	Radar.Pulse_Source 	= Radar.Radar_Config2[39];
	Radar.Pulse_Width 	= Radar.Radar_Config2[40];
	PC8.Tan_so_Lap		= Radar.Radar_Config2[42] * 256 + Radar.Radar_Config2[41];
	//AD9957_config.IO_Update_rate = PC8.Tan_so_Lap;
	Radar.B1 = (Radar.Angle_Start * 3600) / 360;
	Radar.B2 = (Radar.Angle_Stop * 3600) / 360;
	Radar.AFC 			= Radar.Radar_Config2[43];
	PC8.Trigger			= Radar.Radar_Config2[44];
	Radar.U_Nguong		= Radar.Radar_Config2[46] * 256 + Radar.Radar_Config2[45];

	Radar.Phat			= Radar.Radar_Config2[47];
	Radar.Soi_Dot		= Radar.Radar_Config2[48];
	Radar.GPIO			= Radar.Radar_Config2[49]; // chong nhieu
	Radar.DK1			= Radar.Radar_Config2[50]; // Anten/Tai gia
	Radar.DK2			= Radar.Radar_Config2[51]; // ban do nhieu
	Radar.DK4			= Radar.Radar_Config2[52]; // vi phan
	Radar.Radar_2_2A  	= Radar.Radar_Config2[53] & 0x01; // May 2/2A
	Radar.Ma_Dieu_Che 	= (Radar.Radar_Config2[53] & 0x0E)/2;

	if(Radar.Ma_Dieu_Che == 2) Radar.Ma_Dieu_Che = 3;
	else if(Radar.Ma_Dieu_Che == 3) Radar.Ma_Dieu_Che = 2;

	Radar.CSP_Updatei   = (Radar.Radar_Config2[53] & 0x80) / 128;

	AFC1.U_Lech			= Radar.Radar_Config2[55] * 256 + Radar.Radar_Config2[54];
	Radar.Sample_AVG    = Radar.Radar_Config2[57] * 256 + Radar.Radar_Config2[56];
	AFC1.AFC_DIR		= Radar.Radar_Config2[58];
	AFC1.TSP_AFC		= Radar.Radar_Config2[59];
	Radar.Chong_Nhieu   = Radar.GPIO;
	Radar.Vi_Phan		= Radar.DK2;
	AFC1.U_nguong_PC 	= Radar.U_Nguong;
	AFC1.AFC_Active 	= Radar.AFC;
	Radar.Bando_Nhieu   = Radar.DK2;
	//Radar.ENC_Filter_time=0;
	Radar.ENC_Filter_Ena=0;

}

void Radar_Setup_AFC(void)
{
	//if(AFC1.TSP_AFC<=3) AFC1.Control_MAX = 700; else AFC1.Control_MAX = 1300;


	if(PC8.VCO_EN == 0) //(AFC1.AFC_Active == 0)
	{
		AFC1.Control_Value  = Radar.Ngoai_sai;
		AFC1.Control_Value0 = AFC1.Control_Value;

		if(Radar.Ngoai_sai0 != Radar.Ngoai_sai)
		{
			xil_printf("NS = %d\n\r",Radar.Ngoai_sai/2 + 8600);
			Radar.Ngoai_sai0 = Radar.Ngoai_sai;
			//ADF_4113_Write(Radar.Ngoai_sai/2 + 8950, Radar.Ngoai_sai % 2);
			ADF4159_Set_Freq(AFC1.Control_Value + 7200);
			ADF4360_Set_Freq2(1400);
			//ADF4360_Set_Freq(ADF4360_Freq0);
		}
	}
}

void Radar_Setup_HW(void)
{

}

void Radar_Setup_DDS(void)
{

}
