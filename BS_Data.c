#include "Erob.h"
#include "MI_505.h"
#include "DK_Maythu.h"
#include "CAN_Control.h"
#define _GNU_SOURCE
#include <math.h>
#include <stdint.h>
#include <stddef.h> // For size_t
#include "linux_hal.h"      // Lớp trừu tượng phần cứng (sysfs GPIO)

extern int TL_Point;
int _Pi = 0;
int start_view = 0;


// Convert 32-bit fixed-point to double
double fixed32_to_double(int32_t input, uint8_t fractional_bits)
{
    return ((double)input / (double)(1 << fractional_bits));
}

uint32_t bytes_to_uint32(uint8_t* bytes, uint16_t S1, uint16_t S2, uint16_t S3, uint16_t S4)
{
  return ((uint32_t)bytes[S4] << 24) |
         ((uint32_t)bytes[S3] << 16) |
         ((uint32_t)bytes[S2] << 8)  |
          (uint32_t)bytes[S1];
}

int _Get_angle_E(uint8_t Beam)
{
	if(Beam == 1) return 6;
	else if(Beam == 2) return 6;
	else if(Beam == 3) return 6;
	else if(Beam == 4) return 6;

	else if(Beam == 5) return 2;
	else if(Beam == 6) return 2;
	else if(Beam == 7) return 2;
	else if(Beam == 8) return 2;

	else if(Beam == 9) return -2;
	else if(Beam == 10) return -2;
	else if(Beam == 11) return -2;
	else if(Beam == 12) return -2;

	else if(Beam == 13) return -6;
	else if(Beam == 14) return -6;
	else if(Beam == 15) return -6;
	else if(Beam == 16) return -6;

	else if(Beam == 17) return 0;

	else if(Beam == 18) return 8; // SS6 0; 8
	else if(Beam == 19) return 6; // SS1 0; 6
	else if(Beam == 20) return 4; // SS7 0; 4
	else if(Beam == 21) return 2; // SS2 0; 2
	else if(Beam == 22) return -2; // SS3 0; -2
	else if(Beam == 23) return -4; // SS8 0; -4
	else if(Beam == 24) return -6; // SS4 0; -6
	else if(Beam == 25) return -8; // SS9 0; -8
	else if(Beam == 26) return  0; // V1 -2; 0
	else if(Beam == 27) return  0; // V2 2; 0

	else return 0;
}

int _Get_angle_H(uint8_t Beam)
{
	if(Beam == 1) return -6;
	else if(Beam == 2) return -2;
	else if(Beam == 3) return 2;
	else if(Beam == 4) return 6;

	else if(Beam == 5) return -6;
	else if(Beam == 6) return -2;
	else if(Beam == 7) return 2;
	else if(Beam == 8) return 6;

	else if(Beam == 9) return -6;
	else if(Beam == 10) return -2;
	else if(Beam == 11) return 2;
	else if(Beam == 12) return 6;

	else if(Beam == 13) return -6;
	else if(Beam == 14) return -2;
	else if(Beam == 15) return 2;
	else if(Beam == 16) return 6;

	else if(Beam == 17) return 0;

	else if(Beam == 18) return 0; // SS6 0; 8
	else if(Beam == 19) return 0; // SS1 0; 6
	else if(Beam == 20) return 0; // SS7 0; 4
	else if(Beam == 21) return 0; // SS2 0; 2
	else if(Beam == 22) return 0; // SS3 0; -2
	else if(Beam == 23) return 0; // SS8 0; -4
	else if(Beam == 24) return 0; // SS4 0; -6
	else if(Beam == 25) return 0; // SS9 0; -8
	else if(Beam == 26) return -2; // V1 -2; 0
	else if(Beam == 27) return 2; // V2 2; 0

	else return 0;
}

double Convert_int26(uint data)
{
	int32_t val = 0;

    if (data >= 33554432) val = data - 67108864; else val = data;

    return fixed32_to_double(val, 25);
}

double Convert_int27(uint data)
{
	int32_t val = 0;

    if (data >= 67108864) val = data - 134217728; else val = data;

    return fixed32_to_double(val, 26);
}

double Convert_int23(uint data)
{
	int32_t val = 0;

    if (data >= 4194304) val = data - 8388608; else val = data;

    return fixed32_to_double(val, 22);
}

double Convert_int22(uint data)
{
	int32_t val = 0;

    if (data >= 2097152) val = data - 4194304; else val = data;

    return fixed32_to_double(val, 21);
}

void PC8_Get_Phi_epxilon_data(unsigned channel)
{
	// doc phi, epxilon: CH1
	System_Set_Beta_Epxilon_sel(channel);
	usleep(1);
	PC8.BamSat.phi[channel]		= Radar_Hyper_Input(_Input_Phi);
	PC8.BamSat.epxilon[channel]	= Radar_Hyper_Input(_Input_Epxilon);
	usleep(1);
}

void PC8_Get_BeamMax_data(void)
{
	// // Linux_memcpy(&Video_CS_fb1, Video_b17S_Addr, 800);

	// if(PC8.Beam17_MAX_Mode == 1)
	// {
	// 	// Linux_memcpy(&_SS_data, Video_b17S_Addr, 800);
	// }

	//	PC8.BS_Center_start
	// Radar_Frame_M[0]
	//Video_Pulse_Addr

	// doc ram
	Linux_memcpy(&Radar_Window[0], Video_Pulse_Addr, 48000);

	start_view = PC8.BS_Center_start * 4;
	if(start_view >= 47200) start_view = 47200;
	// copy cua so
	memcpy(&Beam17[0], &Radar_Window[start_view], 800);

	//Linux_memcpy(&Beam17, Video_b17S_Addr, 800);

	PC8_Get_V1234_data();
}

void PC8_Get_V1234_data(void)
{
	memcpy(&Beam17i[0], &Radar_Window[start_view], 800);
	memcpy(&Beam17q[0], &Radar_Window[start_view], 800);

	//Linux_memcpy(&Beam17i, Video_fb17i_Addr, 800);
	//Linux_memcpy(&Beam17q, Video_fb17q_Addr, 800);
}

void _Get_Max_Beam(void)
{
	// gia tri beam max: kenh 1
	PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.fb_AVG[0];
	PC8.BamSat.Beam_Max_Vali= _Beam1_Max;
	PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[0] / PC8.BamSat.fb_SNR_Scale);

	// kiem tra gia tri beam max: kenh 2
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.fb_AVG[1])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.fb_AVG[1];
		PC8.BamSat.Beam_Max_Vali= _Beam2_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[1] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 3
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.fb_AVG[2])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.fb_AVG[2];
		PC8.BamSat.Beam_Max_Vali= _Beam3_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[2] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 4
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.fb_AVG[3])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.fb_AVG[3];
		PC8.BamSat.Beam_Max_Vali= _Beam4_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[3] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 5
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.fb_AVG[4])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.fb_AVG[4];
		PC8.BamSat.Beam_Max_Vali= _Beam5_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[4] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 6
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.fb_AVG[5])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.fb_AVG[5];
		PC8.BamSat.Beam_Max_Vali= _Beam6_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[5] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 7
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.fb_AVG[6])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.fb_AVG[6];
		PC8.BamSat.Beam_Max_Vali= _Beam7_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[6] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 8
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.fb_AVG[7])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.fb_AVG[7];
		PC8.BamSat.Beam_Max_Vali= _Beam8_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[7] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 9
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.fb_AVG[8])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.fb_AVG[8];
		PC8.BamSat.Beam_Max_Vali= _Beam9_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[8] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 10
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.fb_AVG[9])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.fb_AVG[9];
		PC8.BamSat.Beam_Max_Vali= _Beam10_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[9] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 11
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.fb_AVG[10])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.fb_AVG[10];
		PC8.BamSat.Beam_Max_Vali= _Beam11_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[10] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 12
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.fb_AVG[11])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.fb_AVG[11];
		PC8.BamSat.Beam_Max_Vali= _Beam12_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[11] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 13
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.fb_AVG[12])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.fb_AVG[12];
		PC8.BamSat.Beam_Max_Vali= _Beam13_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[12] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 14
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.fb_AVG[13])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.fb_AVG[13];
		PC8.BamSat.Beam_Max_Vali= _Beam14_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[13] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 15
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.fb_AVG[14])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.fb_AVG[14];
		PC8.BamSat.Beam_Max_Vali= _Beam15_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[14] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 16
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.fb_AVG[15])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.fb_AVG[15];
		PC8.BamSat.Beam_Max_Vali= _Beam16_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[15] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 17
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.fb_AVG[16])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.fb_AVG[16];
		PC8.BamSat.Beam_Max_Vali= _Beam17_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[16] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 17
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.fb_AVG[17])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.fb_AVG[17];
		PC8.BamSat.Beam_Max_Vali= _SS6_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[17] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 17
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.fb_AVG[18])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.fb_AVG[18];
		PC8.BamSat.Beam_Max_Vali= _SS1_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[18] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 17
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.fb_AVG[19])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.fb_AVG[19];
		PC8.BamSat.Beam_Max_Vali= _SS7_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[19] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 17
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.fb_AVG[20])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.fb_AVG[20];
		PC8.BamSat.Beam_Max_Vali= _SS2_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[20] / PC8.BamSat.fb_SNR_Scale);
	}
	// kiem tra gia tri beam max: kenh 17
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.fb_AVG[21])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.fb_AVG[21];
		PC8.BamSat.Beam_Max_Vali= _SS3_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[21] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 17
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.fb_AVG[22])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.fb_AVG[22];
		PC8.BamSat.Beam_Max_Vali= _SS8_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[22] / PC8.BamSat.fb_SNR_Scale);
	}
	// kiem tra gia tri beam max: kenh 17
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.fb_AVG[23])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.fb_AVG[23];
		PC8.BamSat.Beam_Max_Vali= _SS4_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[23] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 17
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.fb_AVG[24])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.fb_AVG[24];
		PC8.BamSat.Beam_Max_Vali= _SS9_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[24] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 17
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.fb_AVG[25])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.fb_AVG[25];
		PC8.BamSat.Beam_Max_Vali= _V1_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[25] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 17
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.fb_AVG[26])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.fb_AVG[26];
		PC8.BamSat.Beam_Max_Vali= _V2_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[26] / PC8.BamSat.fb_SNR_Scale);
	}


	// cap nhat beam max, gia tri max
	PC8.BamSat.Beam_Max 	= PC8.BamSat.Beam_Maxi;
	PC8.BamSat.Beam_Max_Val = PC8.BamSat.Beam_Max_Vali;

	PC8.BamSat.Beam_SNR		= (PC8.BamSat.Beam_Max * 1.0) / PC8.BamSat.Max_channel_AVG; // ty so tin / tap (SNR)


}

void _Get_Max_Beam2(void)
{
	// gia tri beam max: kenh 1
	PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.Value_Max[0];
	PC8.BamSat.Beam_Max_Vali= _Beam1_Max;
	PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[0] / PC8.BamSat.fb_SNR_Scale);

	// kiem tra gia tri beam max: kenh 2
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.Value_Max[1])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.Value_Max[1];
		PC8.BamSat.Beam_Max_Vali= _Beam2_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[1] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 3
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.Value_Max[2])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.Value_Max[2];
		PC8.BamSat.Beam_Max_Vali= _Beam3_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[2] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 4
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.Value_Max[3])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.Value_Max[3];
		PC8.BamSat.Beam_Max_Vali= _Beam4_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[3] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 5
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.Value_Max[4])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.Value_Max[4];
		PC8.BamSat.Beam_Max_Vali= _Beam5_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[4] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 6
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.Value_Max[5])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.Value_Max[5];
		PC8.BamSat.Beam_Max_Vali= _Beam6_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[5] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 7
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.Value_Max[6])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.Value_Max[6];
		PC8.BamSat.Beam_Max_Vali= _Beam7_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[6] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 8
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.Value_Max[7])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.Value_Max[7];
		PC8.BamSat.Beam_Max_Vali= _Beam8_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[7] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 9
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.Value_Max[8])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.Value_Max[8];
		PC8.BamSat.Beam_Max_Vali= _Beam9_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[8] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 10
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.Value_Max[9])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.Value_Max[9];
		PC8.BamSat.Beam_Max_Vali= _Beam10_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[9] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 11
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.Value_Max[10])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.Value_Max[10];
		PC8.BamSat.Beam_Max_Vali= _Beam11_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[10] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 12
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.Value_Max[11])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.Value_Max[11];
		PC8.BamSat.Beam_Max_Vali= _Beam12_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[11] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 13
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.Value_Max[12])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.Value_Max[12];
		PC8.BamSat.Beam_Max_Vali= _Beam13_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[12] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 14
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.Value_Max[13])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.Value_Max[13];
		PC8.BamSat.Beam_Max_Vali= _Beam14_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[13] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 15
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.Value_Max[14])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.Value_Max[14];
		PC8.BamSat.Beam_Max_Vali= _Beam15_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[14] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 16
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.Value_Max[15])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.Value_Max[15];
		PC8.BamSat.Beam_Max_Vali= _Beam16_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[15] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 17
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.Value_Max[16])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.Value_Max[16];
		PC8.BamSat.Beam_Max_Vali= _Beam17_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[16] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 17
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.Value_Max[17])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.Value_Max[17];
		PC8.BamSat.Beam_Max_Vali= _SS6_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[17] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 17
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.Value_Max[18])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.Value_Max[18];
		PC8.BamSat.Beam_Max_Vali= _SS1_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[18] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 17
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.Value_Max[19])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.Value_Max[19];
		PC8.BamSat.Beam_Max_Vali= _SS7_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[19] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 17
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.Value_Max[20])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.Value_Max[20];
		PC8.BamSat.Beam_Max_Vali= _SS2_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[20] / PC8.BamSat.fb_SNR_Scale);
	}
	// kiem tra gia tri beam max: kenh 17
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.Value_Max[21])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.Value_Max[21];
		PC8.BamSat.Beam_Max_Vali= _SS3_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[21] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 17
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.Value_Max[22])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.Value_Max[22];
		PC8.BamSat.Beam_Max_Vali= _SS8_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[22] / PC8.BamSat.fb_SNR_Scale);
	}
	// kiem tra gia tri beam max: kenh 17
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.Value_Max[23])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.Value_Max[23];
		PC8.BamSat.Beam_Max_Vali= _SS4_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[23] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 17
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.Value_Max[24])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.Value_Max[24];
		PC8.BamSat.Beam_Max_Vali= _SS9_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[24] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 17
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.Value_Max[25])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.Value_Max[25];
		PC8.BamSat.Beam_Max_Vali= _V1_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[25] / PC8.BamSat.fb_SNR_Scale);
	}

	// kiem tra gia tri beam max: kenh 17
	if(PC8.BamSat.Beam_Maxi <= PC8.BamSat.Value_Max[26])
	{
		PC8.BamSat.Beam_Maxi 	= (uint32_t)PC8.BamSat.Value_Max[26];
		PC8.BamSat.Beam_Max_Vali= _V2_Max;
		PC8.BamSat.Max_channel_AVG = (uint32_t)(PC8.BamSat.Value_AVG[26] / PC8.BamSat.fb_SNR_Scale);
	}


	// cap nhat beam max, gia tri max
	PC8.BamSat.Beam_Max 	= PC8.BamSat.Beam_Maxi;
	PC8.BamSat.Beam_Max_Val = PC8.BamSat.Beam_Max_Vali;

	PC8.BamSat.Beam_SNR		= (PC8.BamSat.Beam_Max * 1.0) / PC8.BamSat.Max_channel_AVG; // ty so tin / tap (SNR)


}


void _Caculator_IQ_point(uint16_t i, uint16_t point1, uint16_t point2, uint16_t point3, uint16_t point4)
{
	PC8.S17Im=  1;//bytes_to_uint32(Beam17i, point1, point2, point3, point4);//31
	PC8.S17Qm=  1;//bytes_to_uint32(Beam17q, point1, point2, point3, point4);//31
	PC8.V1Im =	1;//bytes_to_uint32(_V1_I_data, point1, point2, point3, point4);//31
	PC8.V1Qm =	1;//bytes_to_uint32(_V1_Q_data, point1, point2, point3, point4);//31
	PC8.V2Im =	1;//bytes_to_uint32(_V2_I_data, point1, point2, point3, point4);//31
	PC8.V2Qm =	1;//bytes_to_uint32(_V2_Q_data, point1, point2, point3, point4);//31
	PC8.V3Im =	1;//bytes_to_uint32(_V3_I_data, point1, point2, point3, point4);//31
	PC8.V3Qm =	1;//bytes_to_uint32(_V3_Q_data, point1, point2, point3, point4);//31
	PC8.V4Im =	1;//bytes_to_uint32(_V4_I_data, point1, point2, point3, point4);//31
	PC8.V4Qm =	1;//bytes_to_uint32(_V4_Q_data, point1, point2, point3, point4);//31

	PC8.S17Im_[i] = Convert_int22(PC8.S17Im); // 38
	PC8.S17Qm_[i] = Convert_int22(PC8.S17Qm); // 38

	PC8.V1Im_[i] = Convert_int22(PC8.V1Im); // 38
	PC8.V1Qm_[i] = Convert_int22(PC8.V1Qm); // 38

	PC8.V2Im_[i] = Convert_int22(PC8.V2Im); // 38
	PC8.V2Qm_[i] = Convert_int22(PC8.V2Qm); // 38

	PC8.V3Im_[i] = Convert_int22(PC8.V3Im); // 38
	PC8.V3Qm_[i] = Convert_int22(PC8.V3Qm); // 38

	PC8.V4Im_[i] = Convert_int22(PC8.V4Im); // 38
	PC8.V4Qm_[i] = Convert_int22(PC8.V4Qm); // 38

	PC8.S17ImA[i] = sqrt(PC8.S17Im_[i] * PC8.S17Im_[i] + PC8.S17Qm_[i] * PC8.S17Qm_[i]); // 38
	PC8.V1ImA[i]  = sqrt(PC8.V1Im_[i] * PC8.V1Im_[i] + PC8.V1Qm_[i] * PC8.V1Qm_[i]); // 38
	PC8.V2ImA[i]  = sqrt(PC8.V2Im_[i] * PC8.V2Im_[i] + PC8.V2Qm_[i] * PC8.V2Qm_[i]); // 38
	PC8.V3ImA[i]  = sqrt(PC8.V3Im_[i] * PC8.V3Im_[i] + PC8.V3Qm_[i] * PC8.V3Qm_[i]); // 38
	PC8.V4ImA[i]  = sqrt(PC8.V4Im_[i] * PC8.V4Im_[i] + PC8.V4Qm_[i] * PC8.V4Qm_[i]); // 38

}

void _Caculator_point(uint16_t i, uint16_t point1, uint16_t point2, uint16_t point3, uint16_t point4)
{
	PC8.BamSat.fb_Val[0][i]  = 1;//bytes_to_uint32(Video_CS_fb1, point1, point2, point3, point4);
	PC8.BamSat.fb_Val[1][i]  = 1;//bytes_to_uint32(Video_CS_fb2, point1, point2, point3, point4);
	PC8.BamSat.fb_Val[2][i]  = 1;//bytes_to_uint32(Video_CS_fb3, point1, point2, point3, point4);
	PC8.BamSat.fb_Val[3][i]  = 1;//bytes_to_uint32(Video_CS_fb4, point1, point2, point3, point4);
	PC8.BamSat.fb_Val[4][i]  = 1;//bytes_to_uint32(Video_CS_fb5, point1, point2, point3, point4);
	PC8.BamSat.fb_Val[5][i]  = 1;//bytes_to_uint32(Video_CS_fb6, point1, point2, point3, point4);
	PC8.BamSat.fb_Val[6][i]  = 1;//bytes_to_uint32(Video_CS_fb7, point1, point2, point3, point4);
	PC8.BamSat.fb_Val[7][i]  = 1;//bytes_to_uint32(Video_CS_fb8, point1, point2, point3, point4);
	PC8.BamSat.fb_Val[8][i]  = 1;//bytes_to_uint32(Video_CS_fb9, point1, point2, point3, point4);
	PC8.BamSat.fb_Val[9][i]  = 1;//bytes_to_uint32(Video_CS_fb10, point1, point2, point3, point4);
	PC8.BamSat.fb_Val[10][i] = 1;//bytes_to_uint32(Video_CS_fb11, point1, point2, point3, point4);
	PC8.BamSat.fb_Val[11][i] = 1;//bytes_to_uint32(Video_CS_fb12, point1, point2, point3, point4);
	PC8.BamSat.fb_Val[12][i] = 1;//bytes_to_uint32(Video_CS_fb13, point1, point2, point3, point4);
	PC8.BamSat.fb_Val[13][i] = 1;//bytes_to_uint32(Video_CS_fb14, point1, point2, point3, point4);
	PC8.BamSat.fb_Val[14][i] = 1;//bytes_to_uint32(Video_CS_fb15, point1, point2, point3, point4);
	PC8.BamSat.fb_Val[15][i] = 1;//bytes_to_uint32(Video_CS_fb16, point1, point2, point3, point4);
	PC8.BamSat.fb_Val[16][i] = 1;//bytes_to_uint32(Video_CS_fb17, point1, point2, point3, point4);
	PC8.BamSat.fb_Val[17][i] = 1;//bytes_to_uint32(_SS6_data, point1, point2, point3, point4);
	PC8.BamSat.fb_Val[18][i] = 1;//bytes_to_uint32(_SS1_data, point1, point2, point3, point4);
	PC8.BamSat.fb_Val[19][i] = 1;//bytes_to_uint32(_SS7_data, point1, point2, point3, point4);
	PC8.BamSat.fb_Val[20][i] = 1;//bytes_to_uint32(_SS2_data, point1, point2, point3, point4);
	PC8.BamSat.fb_Val[21][i] = 1;//bytes_to_uint32(_SS3_data, point1, point2, point3, point4);
	PC8.BamSat.fb_Val[22][i] = 1;//bytes_to_uint32(_SS8_data, point1, point2, point3, point4);
	PC8.BamSat.fb_Val[23][i] = 1;//bytes_to_uint32(_SS4_data, point1, point2, point3, point4);
	PC8.BamSat.fb_Val[24][i] = 1;//bytes_to_uint32(_SS9_data, point1, point2, point3, point4);
	PC8.BamSat.fb_Val[25][i] = 1;//bytes_to_uint32(_V1_data, point1, point2, point3, point4);
	PC8.BamSat.fb_Val[26][i] = 1;//bytes_to_uint32(_V2_data, point1, point2, point3, point4);

	PC8.BamSat.Value_SUM[0] += PC8.BamSat.fb_Val[0][i];
	PC8.BamSat.Value_SUM[1] += PC8.BamSat.fb_Val[1][i];
	PC8.BamSat.Value_SUM[2] += PC8.BamSat.fb_Val[2][i];
	PC8.BamSat.Value_SUM[3] += PC8.BamSat.fb_Val[3][i];
	PC8.BamSat.Value_SUM[4] += PC8.BamSat.fb_Val[4][i];
	PC8.BamSat.Value_SUM[5] += PC8.BamSat.fb_Val[5][i];
	PC8.BamSat.Value_SUM[6] += PC8.BamSat.fb_Val[6][i];
	PC8.BamSat.Value_SUM[7] += PC8.BamSat.fb_Val[7][i];
	PC8.BamSat.Value_SUM[8] += PC8.BamSat.fb_Val[8][i];
	PC8.BamSat.Value_SUM[9] += PC8.BamSat.fb_Val[9][i];
	PC8.BamSat.Value_SUM[10] += PC8.BamSat.fb_Val[10][i];
	PC8.BamSat.Value_SUM[11] += PC8.BamSat.fb_Val[11][i];
	PC8.BamSat.Value_SUM[12] += PC8.BamSat.fb_Val[12][i];
	PC8.BamSat.Value_SUM[13] += PC8.BamSat.fb_Val[13][i];
	PC8.BamSat.Value_SUM[14] += PC8.BamSat.fb_Val[14][i];
	PC8.BamSat.Value_SUM[15] += PC8.BamSat.fb_Val[15][i];
	PC8.BamSat.Value_SUM[16] += PC8.BamSat.fb_Val[16][i];
	PC8.BamSat.Value_SUM[17] += PC8.BamSat.fb_Val[17][i];
	PC8.BamSat.Value_SUM[18] += PC8.BamSat.fb_Val[18][i];
	PC8.BamSat.Value_SUM[19] += PC8.BamSat.fb_Val[19][i];
	PC8.BamSat.Value_SUM[20] += PC8.BamSat.fb_Val[20][i];
	PC8.BamSat.Value_SUM[21] += PC8.BamSat.fb_Val[21][i];
	PC8.BamSat.Value_SUM[22] += PC8.BamSat.fb_Val[22][i];
	PC8.BamSat.Value_SUM[23] += PC8.BamSat.fb_Val[23][i];
	PC8.BamSat.Value_SUM[24] += PC8.BamSat.fb_Val[24][i];
	PC8.BamSat.Value_SUM[25] += PC8.BamSat.fb_Val[25][i];
	PC8.BamSat.Value_SUM[26] += PC8.BamSat.fb_Val[26][i];
}

void _Get_Max_Point_channel(uint8_t channel, uint8_t point)
{
	if(PC8.BamSat.fb_Val[channel][point] >= PC8.BamSat.Value_AVG[channel])
	{
		PC8.BamSat.fb_Pass[channel][PC8.BamSat.fb_cnt[channel]] = point; // luu lai toa do du lieu
		PC8.BamSat.fb_cnt[channel]++;
	}
}

void _Get_Max_Point_channel2(uint8_t channel, uint8_t point)
{
	if(PC8.BamSat.fb_Val[channel][point] >= PC8.BamSat.Value_AVG[channel])
	{
		PC8.BamSat.fb_Pass[channel][PC8.BamSat.fb_cnt[channel]] = point; // luu lai toa do du lieu
		PC8.BamSat.fb_cnt[channel]++;
	}
}

void _Get_Max_Point_V1234S(uint16_t point)
{
	if(PC8.S17max <= PC8.S17ImA[point])
	{
		PC8.S17max = PC8.S17ImA[point];
		PC8._S17I  = PC8.S17Im_[point]; // 38
		PC8._S17Q  = PC8.S17Qm_[point]; // 38
		PC8.BS_PS17= point; // 38
	}

	if(PC8.V1max <= PC8.V1ImA[point])
	{
		PC8.V1max = PC8.V1ImA[point];
		PC8._V1I   = PC8.V1Im_[point]; // 38
		PC8._V1Q   = PC8.V1Qm_[point]; // 38
		PC8.BS_PV1 = point; // 38
//	}
//
//	if(PC8.V2max <= PC8.V2ImA)
//	{
		PC8.V2max = PC8.V2ImA[point];
		PC8._V2I   = PC8.V2Im_[point]; // 38
		PC8._V2Q   = PC8.V2Qm_[point]; // 38
		PC8.BS_PV2 = point; // 38
	}

	if(PC8.V3max <= PC8.V3ImA[point])
	{
		PC8.V3max = PC8.V3ImA[point];
		PC8._V3I   = PC8.V3Im_[point]; // 38
		PC8._V3Q   = PC8.V3Qm_[point]; // 38
		PC8.BS_PV3 = point; // 38
//	}
//
//	if(PC8.V4max <= PC8.V4ImA)
//	{
		PC8.V4max = PC8.V4ImA[point];
		PC8._V4I   = PC8.V4Im_[point]; // 38
		PC8._V4Q   = PC8.V4Qm_[point]; // 38
		PC8.BS_PV4 = point; // 38
	}
}

void _Get_Max_Point_dataS(uint16_t point)
{
	int i = 0;
	for(i = 0; i < 27; i++)
	{
		if(PC8.BamSat.Value_Max[i] <= PC8.BamSat.fb_Val[i][point])
		{
			PC8.BamSat.Value_Max[i] = PC8.BamSat.fb_Val[i][point];

			if(i == 16)
			{
				PC8.S17_Max_point 		= point;
				PC8.S17_Max 			= PC8.BamSat.fb_Val[i][point] - 1; // 38
			}

		}
	}
}

void _Get_Max_Point_data(uint8_t point)
{
	// tim gia tri max cho tung kenh
	_Get_Max_Point_channel2(0, point); // tim vung tin hieu kenh 1
	_Get_Max_Point_channel2(1, point); // tim vung tin hieu kenh 2
	_Get_Max_Point_channel2(2, point); // tim vung tin hieu kenh 3
	_Get_Max_Point_channel2(3, point); // tim vung tin hieu kenh 4
	_Get_Max_Point_channel2(4, point); // tim vung tin hieu kenh 5
	_Get_Max_Point_channel2(5, point); // tim vung tin hieu kenh 6
	_Get_Max_Point_channel2(6, point); // tim vung tin hieu kenh 7
	_Get_Max_Point_channel2(7, point); // tim vung tin hieu kenh 8
	_Get_Max_Point_channel2(8, point); // tim vung tin hieu kenh 9
	_Get_Max_Point_channel2(9, point); // tim vung tin hieu kenh 10
	_Get_Max_Point_channel2(10, point); // tim vung tin hieu kenh 11
	_Get_Max_Point_channel2(11, point); // tim vung tin hieu kenh 12
	_Get_Max_Point_channel2(12, point); // tim vung tin hieu kenh 13
	_Get_Max_Point_channel2(13, point); // tim vung tin hieu kenh 14
	_Get_Max_Point_channel2(14, point); // tim vung tin hieu kenh 15
	_Get_Max_Point_channel2(15, point); // tim vung tin hieu kenh 16
	_Get_Max_Point_channel2(16, point); // tim vung tin hieu kenh 17
	_Get_Max_Point_channel2(17, point); // tim vung tin hieu kenh SS6
	_Get_Max_Point_channel2(18, point); // tim vung tin hieu kenh SS1
	_Get_Max_Point_channel2(19, point); // tim vung tin hieu kenh SS7
	_Get_Max_Point_channel2(20, point); // tim vung tin hieu kenh SS2
	_Get_Max_Point_channel2(21, point); // tim vung tin hieu kenh SS3
	_Get_Max_Point_channel2(22, point); // tim vung tin hieu kenh SS8
	_Get_Max_Point_channel2(23, point); // tim vung tin hieu kenh SS4
	_Get_Max_Point_channel2(24, point); // tim vung tin hieu kenh SS9
	_Get_Max_Point_channel2(25, point); // tim vung tin hieu kenh V1
	_Get_Max_Point_channel2(26, point); // tim vung tin hieu kenh V2

//	// tim gia tri max cho tung kenh
//	_Get_Max_Point_channel(0, point); // tim vung tin hieu kenh 1
//	_Get_Max_Point_channel(1, point); // tim vung tin hieu kenh 2
//	_Get_Max_Point_channel(2, point); // tim vung tin hieu kenh 3
//	_Get_Max_Point_channel(3, point); // tim vung tin hieu kenh 4
//	_Get_Max_Point_channel(4, point); // tim vung tin hieu kenh 5
//	_Get_Max_Point_channel(5, point); // tim vung tin hieu kenh 6
//	_Get_Max_Point_channel(6, point); // tim vung tin hieu kenh 7
//	_Get_Max_Point_channel(7, point); // tim vung tin hieu kenh 8
//	_Get_Max_Point_channel(8, point); // tim vung tin hieu kenh 9
//	_Get_Max_Point_channel(9, point); // tim vung tin hieu kenh 10
//	_Get_Max_Point_channel(10, point); // tim vung tin hieu kenh 11
//	_Get_Max_Point_channel(11, point); // tim vung tin hieu kenh 12
//	_Get_Max_Point_channel(12, point); // tim vung tin hieu kenh 13
//	_Get_Max_Point_channel(13, point); // tim vung tin hieu kenh 14
//	_Get_Max_Point_channel(14, point); // tim vung tin hieu kenh 15
//	_Get_Max_Point_channel(15, point); // tim vung tin hieu kenh 16
//	_Get_Max_Point_channel(16, point); // tim vung tin hieu kenh 17
//	_Get_Max_Point_channel(17, point); // tim vung tin hieu kenh SS6
//	_Get_Max_Point_channel(18, point); // tim vung tin hieu kenh SS1
//	_Get_Max_Point_channel(19, point); // tim vung tin hieu kenh SS7
//	_Get_Max_Point_channel(20, point); // tim vung tin hieu kenh SS2
//	_Get_Max_Point_channel(21, point); // tim vung tin hieu kenh SS3
//	_Get_Max_Point_channel(22, point); // tim vung tin hieu kenh SS8
//	_Get_Max_Point_channel(23, point); // tim vung tin hieu kenh SS4
//	_Get_Max_Point_channel(24, point); // tim vung tin hieu kenh SS9
//	_Get_Max_Point_channel(25, point); // tim vung tin hieu kenh V1
//	_Get_Max_Point_channel(26, point); // tim vung tin hieu kenh V2
}

void _Cal_Beam_AVG_Data(uint8_t channel)
{
	uint8_t point = 0;
	uint8_t data_idx = 0;

	PC8.BamSat.fb_Sum[channel] = 0;

	for(point = 0; point < PC8.BamSat.fb_cnt[channel]; point++)
	{
		data_idx = PC8.BamSat.fb_Pass[channel][point]; // lay toa do tu danh sach
		PC8.BamSat.fb_Sum[channel] += PC8.BamSat.fb_Val[channel][data_idx];
	}

	PC8.BamSat.fb_AVG[channel] = PC8.BamSat.fb_Sum[channel] / PC8.BamSat.fb_cnt[channel];
	PC8.BamSat.fb_Size[channel] = PC8.BamSat.fb_cnt[channel];

}

void _Cal_AVG_I_Q_Data(void)
{
	uint8_t Max_point 	= PC8.BamSat.Beam_Max_Vali - 1; // so hieu kenh lon nhat
	uint8_t size 		= PC8.BamSat.fb_Size[Max_point]; // so hieu kenh lon nhat
	uint8_t point 		= 0; // so hieu kenh lon nhat
	uint8_t data_idx 	= 0;

	PC8.BamSat.fb_Start = PC8.BamSat.fb_Pass[Max_point][0];
	PC8.BamSat.fb_Stop  = PC8.BamSat.fb_Pass[Max_point][size - 1];
	PC8.BamSat.fb_center= (PC8.BamSat.fb_Start + PC8.BamSat.fb_Stop) / 2;
	PC8.BamSat.fb_cells = size;
	PC8.BamSat.fb_Beam_Max = Max_point;

	PC8.S17Im_Sum = 0; // 38
	PC8.S17Qm_Sum = 0; // 38
	PC8.V1Im_Sum = 0; // 38
	PC8.V1Qm_Sum = 0; // 38
	PC8.V2Im_Sum = 0; // 38
	PC8.V2Qm_Sum = 0; // 38
	PC8.V3Im_Sum = 0; // 38
	PC8.V3Qm_Sum = 0; // 38
	PC8.V4Im_Sum = 0; // 38
	PC8.V4Qm_Sum = 0; // 38

	PC8.S17ImA_Sum = 0; // 38
	PC8.V1ImA_Sum = 0; // 38
	PC8.V2ImA_Sum = 0; // 38
	PC8.V3ImA_Sum = 0; // 38
	PC8.V4ImA_Sum = 0; // 38


	PC8.BamSat.fb_Sum[Max_point] = 0;

	for(point = 0; point < size; point++)
	{
		data_idx = PC8.BamSat.fb_Pass[Max_point][point]; // lay toa do tu danh sach

		PC8.S17Im_Sum += PC8.S17Im_[data_idx]; // 38
		PC8.S17Qm_Sum += PC8.S17Qm_[data_idx]; // 38
		PC8.V1Im_Sum  += PC8.V1Im_[data_idx]; // 38
		PC8.V1Qm_Sum  += PC8.V1Qm_[data_idx]; // 38
		PC8.V2Im_Sum  += PC8.V2Im_[data_idx]; // 38
		PC8.V2Qm_Sum  += PC8.V2Qm_[data_idx]; // 38
		PC8.V3Im_Sum  += PC8.V3Im_[data_idx]; // 38
		PC8.V3Qm_Sum  += PC8.V3Qm_[data_idx]; // 38
		PC8.V4Im_Sum  += PC8.V4Im_[data_idx]; // 38
		PC8.V4Qm_Sum  += PC8.V4Qm_[data_idx]; // 38

		PC8.S17ImA_Sum += PC8.S17ImA[data_idx]; // 38
		PC8.V1ImA_Sum  += PC8.V1ImA[data_idx]; // 38
		PC8.V2ImA_Sum  += PC8.V2ImA[data_idx]; // 38
		PC8.V3ImA_Sum  += PC8.V3ImA[data_idx]; // 38
		PC8.V4ImA_Sum  += PC8.V4ImA[data_idx]; // 38
	}
}

void _AVG_Data_IQ(uint8_t Channel)
{
	if(Channel == _S17I_AVG_ch)//#define	_S17I_AVG_ch 0  // 38
	{
		PC8._S17I_AVG[7] = PC8._S17I_AVG[6];
		PC8._S17I_AVG[6] = PC8._S17I_AVG[5];
		PC8._S17I_AVG[5] = PC8._S17I_AVG[4];
		PC8._S17I_AVG[4] = PC8._S17I_AVG[3];
		PC8._S17I_AVG[3] = PC8._S17I_AVG[2];
		PC8._S17I_AVG[2] = PC8._S17I_AVG[1];
		PC8._S17I_AVG[1] = PC8._S17I_AVG[0];
		PC8._S17I_AVG[0] = PC8.S17Im_Sum / PC8.BamSat.fb_cells;

		PC8._S17I  = (  PC8._S17I_AVG[7] +
						PC8._S17I_AVG[6] +
						PC8._S17I_AVG[5] +
						PC8._S17I_AVG[4] +
						PC8._S17I_AVG[3] +
						PC8._S17I_AVG[2] +
						PC8._S17I_AVG[1] +
						PC8._S17I_AVG[0]) / 8.0;
	}
	else if(Channel == _S17Q_AVG_ch)//#define	_S17Q_AVG_ch 1 // 38
	{
		PC8._S17Q_AVG[7] = PC8._S17Q_AVG[6];
		PC8._S17Q_AVG[6] = PC8._S17Q_AVG[5];
		PC8._S17Q_AVG[5] = PC8._S17Q_AVG[4];
		PC8._S17Q_AVG[4] = PC8._S17Q_AVG[3];
		PC8._S17Q_AVG[3] = PC8._S17Q_AVG[2];
		PC8._S17Q_AVG[2] = PC8._S17Q_AVG[1];
		PC8._S17Q_AVG[1] = PC8._S17Q_AVG[0];
		PC8._S17Q_AVG[0] = PC8.S17Qm_Sum / PC8.BamSat.fb_cells;

		PC8._S17Q  = (  PC8._S17Q_AVG[7] +
						PC8._S17Q_AVG[6] +
						PC8._S17Q_AVG[5] +
						PC8._S17Q_AVG[4] +
						PC8._S17Q_AVG[3] +
						PC8._S17Q_AVG[2] +
						PC8._S17Q_AVG[1] +
						PC8._S17Q_AVG[0]) / 8.0;
	}
	else if(Channel == _V1I_AVG_ch)//#define	_V1I_AVG_ch 2  // 38
	{
		PC8._V1I_AVG[7] = PC8._V1I_AVG[6];
		PC8._V1I_AVG[6] = PC8._V1I_AVG[5];
		PC8._V1I_AVG[5] = PC8._V1I_AVG[4];
		PC8._V1I_AVG[4] = PC8._V1I_AVG[3];
		PC8._V1I_AVG[3] = PC8._V1I_AVG[2];
		PC8._V1I_AVG[2] = PC8._V1I_AVG[1];
		PC8._V1I_AVG[1] = PC8._V1I_AVG[0];
		PC8._V1I_AVG[0] = PC8.V1Im_Sum / PC8.BamSat.fb_cells;

		PC8._V1I  = (  PC8._V1I_AVG[7] +
						PC8._V1I_AVG[6] +
						PC8._V1I_AVG[5] +
						PC8._V1I_AVG[4] +
						PC8._V1I_AVG[3] +
						PC8._V1I_AVG[2] +
						PC8._V1I_AVG[1] +
						PC8._V1I_AVG[0]) / 8.0;
	}
	else if(Channel == _V1Q_AVG_ch)//#define	_V1Q_AVG_ch 3 // 38
	{
		PC8._V1Q_AVG[7] = PC8._V1Q_AVG[6];
		PC8._V1Q_AVG[6] = PC8._V1Q_AVG[5];
		PC8._V1Q_AVG[5] = PC8._V1Q_AVG[4];
		PC8._V1Q_AVG[4] = PC8._V1Q_AVG[3];
		PC8._V1Q_AVG[3] = PC8._V1Q_AVG[2];
		PC8._V1Q_AVG[2] = PC8._V1Q_AVG[1];
		PC8._V1Q_AVG[1] = PC8._V1Q_AVG[0];
		PC8._V1Q_AVG[0] = PC8.V1Qm_Sum / PC8.BamSat.fb_cells;

		PC8._V1Q  = (  PC8._V1Q_AVG[7] +
						PC8._V1Q_AVG[6] +
						PC8._V1Q_AVG[5] +
						PC8._V1Q_AVG[4] +
						PC8._V1Q_AVG[3] +
						PC8._V1Q_AVG[2] +
						PC8._V1Q_AVG[1] +
						PC8._V1Q_AVG[0]) / 8.0;
	}
	else if(Channel == _V2I_AVG_ch)//#define	_V2I_AVG_ch 4 // 38
	{
		PC8._V2I_AVG[7] = PC8._V2I_AVG[6];
		PC8._V2I_AVG[6] = PC8._V2I_AVG[5];
		PC8._V2I_AVG[5] = PC8._V2I_AVG[4];
		PC8._V2I_AVG[4] = PC8._V2I_AVG[3];
		PC8._V2I_AVG[3] = PC8._V2I_AVG[2];
		PC8._V2I_AVG[2] = PC8._V2I_AVG[1];
		PC8._V2I_AVG[1] = PC8._V2I_AVG[0];
		PC8._V2I_AVG[0] = PC8.V2Im_Sum / PC8.BamSat.fb_cells;

		PC8._V2I  = (  PC8._V2I_AVG[7] +
						PC8._V2I_AVG[6] +
						PC8._V2I_AVG[5] +
						PC8._V2I_AVG[4] +
						PC8._V2I_AVG[3] +
						PC8._V2I_AVG[2] +
						PC8._V2I_AVG[1] +
						PC8._V2I_AVG[0]) / 8.0;
	}
	else if(Channel == _V2Q_AVG_ch)//#define	_V2Q_AVG_ch 5 // 38
	{
		PC8._V2Q_AVG[7] = PC8._V2Q_AVG[6];
		PC8._V2Q_AVG[6] = PC8._V2Q_AVG[5];
		PC8._V2Q_AVG[5] = PC8._V2Q_AVG[4];
		PC8._V2Q_AVG[4] = PC8._V2Q_AVG[3];
		PC8._V2Q_AVG[3] = PC8._V2Q_AVG[2];
		PC8._V2Q_AVG[2] = PC8._V2Q_AVG[1];
		PC8._V2Q_AVG[1] = PC8._V2Q_AVG[0];
		PC8._V2Q_AVG[0] = PC8.V2Qm_Sum / PC8.BamSat.fb_cells;

		PC8._V2Q  = (  PC8._V2Q_AVG[7] +
						PC8._V2Q_AVG[6] +
						PC8._V2Q_AVG[5] +
						PC8._V2Q_AVG[4] +
						PC8._V2Q_AVG[3] +
						PC8._V2Q_AVG[2] +
						PC8._V2Q_AVG[1] +
						PC8._V2Q_AVG[0]) / 8.0;
	}
	else if(Channel == _V3I_AVG_ch)//#define	_V3I_AVG_ch 6 // 38
	{
		PC8._V3I_AVG[7] = PC8._V3I_AVG[6];
		PC8._V3I_AVG[6] = PC8._V3I_AVG[5];
		PC8._V3I_AVG[5] = PC8._V3I_AVG[4];
		PC8._V3I_AVG[4] = PC8._V3I_AVG[3];
		PC8._V3I_AVG[3] = PC8._V3I_AVG[2];
		PC8._V3I_AVG[2] = PC8._V3I_AVG[1];
		PC8._V3I_AVG[1] = PC8._V3I_AVG[0];
		PC8._V3I_AVG[0] = PC8.V3Im_Sum / PC8.BamSat.fb_cells;

		PC8._V3I  = (  PC8._V3I_AVG[7] +
						PC8._V3I_AVG[6] +
						PC8._V3I_AVG[5] +
						PC8._V3I_AVG[4] +
						PC8._V3I_AVG[3] +
						PC8._V3I_AVG[2] +
						PC8._V3I_AVG[1] +
						PC8._V3I_AVG[0]) / 8.0;
	}
	else if(Channel == _V3Q_AVG_ch)//#define	_V3Q_AVG_ch 7 // 38
	{
		PC8._V3Q_AVG[7] = PC8._V3Q_AVG[6];
		PC8._V3Q_AVG[6] = PC8._V3Q_AVG[5];
		PC8._V3Q_AVG[5] = PC8._V3Q_AVG[4];
		PC8._V3Q_AVG[4] = PC8._V3Q_AVG[3];
		PC8._V3Q_AVG[3] = PC8._V3Q_AVG[2];
		PC8._V3Q_AVG[2] = PC8._V3Q_AVG[1];
		PC8._V3Q_AVG[1] = PC8._V3Q_AVG[0];
		PC8._V3Q_AVG[0] = PC8.V3Qm_Sum / PC8.BamSat.fb_cells;

		PC8._V3Q  = (   PC8._V3Q_AVG[7] +
						PC8._V3Q_AVG[6] +
						PC8._V3Q_AVG[5] +
						PC8._V3Q_AVG[4] +
						PC8._V3Q_AVG[3] +
						PC8._V3Q_AVG[2] +
						PC8._V3Q_AVG[1] +
						PC8._V3Q_AVG[0]) / 8.0;
	}
	else if(Channel == _V4I_AVG_ch)//#define	_V4I_AVG_ch 8 // 38
	{
		PC8._V4I_AVG[7] = PC8._V4I_AVG[6];
		PC8._V4I_AVG[6] = PC8._V4I_AVG[5];
		PC8._V4I_AVG[5] = PC8._V4I_AVG[4];
		PC8._V4I_AVG[4] = PC8._V4I_AVG[3];
		PC8._V4I_AVG[3] = PC8._V4I_AVG[2];
		PC8._V4I_AVG[2] = PC8._V4I_AVG[1];
		PC8._V4I_AVG[1] = PC8._V4I_AVG[0];
		PC8._V4I_AVG[0] = PC8.V4Im_Sum / PC8.BamSat.fb_cells;

		PC8._V4I  = (  PC8._V4I_AVG[7] +
						PC8._V4I_AVG[6] +
						PC8._V4I_AVG[5] +
						PC8._V4I_AVG[4] +
						PC8._V4I_AVG[3] +
						PC8._V4I_AVG[2] +
						PC8._V4I_AVG[1] +
						PC8._V4I_AVG[0]) / 8.0;
	}
	else if(Channel == _V4Q_AVG_ch)//#define	_V4Q_AVG_ch 9 // 38
	{
		PC8._V4Q_AVG[7] = PC8._V4Q_AVG[6];
		PC8._V4Q_AVG[6] = PC8._V4Q_AVG[5];
		PC8._V4Q_AVG[5] = PC8._V4Q_AVG[4];
		PC8._V4Q_AVG[4] = PC8._V4Q_AVG[3];
		PC8._V4Q_AVG[3] = PC8._V4Q_AVG[2];
		PC8._V4Q_AVG[2] = PC8._V4Q_AVG[1];
		PC8._V4Q_AVG[1] = PC8._V4Q_AVG[0];
		PC8._V4Q_AVG[0] = PC8.V4Qm_Sum / PC8.BamSat.fb_cells;

		PC8._V4Q  = (  PC8._V4Q_AVG[7] +
						PC8._V4Q_AVG[6] +
						PC8._V4Q_AVG[5] +
						PC8._V4Q_AVG[4] +
						PC8._V4Q_AVG[3] +
						PC8._V4Q_AVG[2] +
						PC8._V4Q_AVG[1] +
						PC8._V4Q_AVG[0]) / 8.0;
	}
}

void _Get_Max_Point_V1234(void)
{

	PC8.S17max = PC8.S17ImA_Sum / PC8.BamSat.fb_cells;
	_AVG_Data_IQ(_S17I_AVG_ch);
	_AVG_Data_IQ(_S17Q_AVG_ch);
	PC8.BS_PS17= PC8.BamSat.fb_center; // 38

	PC8.V1max = PC8.V1ImA_Sum / PC8.BamSat.fb_cells;
	_AVG_Data_IQ(_V1I_AVG_ch);
	_AVG_Data_IQ(_V1Q_AVG_ch);
	PC8.BS_PV1 = PC8.BamSat.fb_center; // 38

	PC8.V2max = PC8.V2ImA_Sum / PC8.BamSat.fb_cells;
	_AVG_Data_IQ(_V2I_AVG_ch);
	_AVG_Data_IQ(_V2Q_AVG_ch);
	PC8.BS_PV2 = PC8.BamSat.fb_center; // 38

	PC8.V3max = PC8.V3ImA_Sum / PC8.BamSat.fb_cells;
	_AVG_Data_IQ(_V3I_AVG_ch);
	_AVG_Data_IQ(_V3Q_AVG_ch);
	PC8.BS_PV3 = PC8.BamSat.fb_center; // 38

	PC8.V4max = PC8.V4ImA_Sum / PC8.BamSat.fb_cells;
	_AVG_Data_IQ(_V4I_AVG_ch);
	_AVG_Data_IQ(_V4Q_AVG_ch);
	PC8.BS_PV4 = PC8.BamSat.fb_center; // 38

}

void _Cal_Beam_Max(void)
{
	_Cal_Beam_AVG_Data(0); // tinh trung binh tin hieu tren nguong
	_Cal_Beam_AVG_Data(1); // tinh trung binh tin hieu tren nguong
	_Cal_Beam_AVG_Data(2); // tinh trung binh tin hieu tren nguong
	_Cal_Beam_AVG_Data(3); // tinh trung binh tin hieu tren nguong
	_Cal_Beam_AVG_Data(4); // tinh trung binh tin hieu tren nguong
	_Cal_Beam_AVG_Data(5); // tinh trung binh tin hieu tren nguong
	_Cal_Beam_AVG_Data(6); // tinh trung binh tin hieu tren nguong
	_Cal_Beam_AVG_Data(7); // tinh trung binh tin hieu tren nguong
	_Cal_Beam_AVG_Data(8); // tinh trung binh tin hieu tren nguong
	_Cal_Beam_AVG_Data(9); // tinh trung binh tin hieu tren nguong
	_Cal_Beam_AVG_Data(10); // tinh trung binh tin hieu tren nguong
	_Cal_Beam_AVG_Data(11); // tinh trung binh tin hieu tren nguong
	_Cal_Beam_AVG_Data(12); // tinh trung binh tin hieu tren nguong
	_Cal_Beam_AVG_Data(13); // tinh trung binh tin hieu tren nguong
	_Cal_Beam_AVG_Data(14); // tinh trung binh tin hieu tren nguong
	_Cal_Beam_AVG_Data(15); // tinh trung binh tin hieu tren nguong
	_Cal_Beam_AVG_Data(16); // tinh trung binh tin hieu tren nguong
	_Cal_Beam_AVG_Data(17); // tinh trung binh tin hieu tren nguong
	_Cal_Beam_AVG_Data(18); // tinh trung binh tin hieu tren nguong
	_Cal_Beam_AVG_Data(19); // tinh trung binh tin hieu tren nguong
	_Cal_Beam_AVG_Data(20); // tinh trung binh tin hieu tren nguong
	_Cal_Beam_AVG_Data(21); // tinh trung binh tin hieu tren nguong
	_Cal_Beam_AVG_Data(22); // tinh trung binh tin hieu tren nguong
	_Cal_Beam_AVG_Data(23); // tinh trung binh tin hieu tren nguong
	_Cal_Beam_AVG_Data(24); // tinh trung binh tin hieu tren nguong
	_Cal_Beam_AVG_Data(25); // tinh trung binh tin hieu tren nguong
	_Cal_Beam_AVG_Data(26); // tinh trung binh tin hieu tren nguong

	_Get_Max_Beam(); // tim beam lon nhat
	_Cal_AVG_I_Q_Data();

}

void _Get_SS1_SS2_SS3_SS4_Max_Val(void)
{
	if(PC8.BS_Start == 1)
	{
		PC8.BamSat.BS_Fix_Epxilon 	= 1;// 20
		PC8.BamSat.BS_Fix_Dir 		= _Epxilon_Ok;// 20
	}
}

void PC8_Caculator_AVG_DataS(void)
{
	PC8.BamSat.Cell_Screen = PC8.BamSat.Stop_Screen - PC8.BamSat.Start_Screen;

	int i = 0;
	for(i = 0; i < 27; i++)
	{
		PC8.BamSat.Value_AVG[i]	= PC8.BamSat.Value_SUM[i] / (PC8.BamSat.Cell_Screen);
	}
}


void PC8_Clear_AVG_Data(void)
{
	PC8.BamSat.Beam_Maxi 	= 0;
	PC8.BamSat.Beam_Max_Vali= 0;

	int i = 0;

	for(i = 0; i < 27; i++)
	{
		PC8.BamSat.Value_Max[i]	= 0;
		PC8.BamSat.fb_cnt[i] 	= 0;
		PC8.BamSat.Value_SUM[i]	= 0;
	}

	PC8.S17max	= 0; // 38
	PC8.V1max	= 0; // 38
	PC8.V2max	= 0; // 38
	PC8.V3max	= 0; // 38
	PC8.V4max	= 0; // 38

}

void PC8_Caculator_AVG_Data(void)
{
	PC8.BamSat.Cell_Screen = PC8.BamSat.Stop_Screen - PC8.BamSat.Start_Screen;

	int i = 0;
	double Scalex = PC8.BamSat.fb_SNR_Scale / (PC8.BamSat.Cell_Screen);

	for(i = 0; i < 27; i++)
	{
		PC8.BamSat.Value_AVG[i]	= PC8.BamSat.Value_SUM[i] * Scalex;
	}

}

void PC8_Caculator_AVG_Data2(void)
{
	PC8.BamSat.Cell_Screen = PC8.BamSat.Stop_Screen - PC8.BamSat.Start_Screen;

	int i = 0;
	double Scalex = PC8.BamSat.fb_SNR_Scale / (PC8.BamSat.Cell_Screen);

	for(i = 0; i < 27; i++)
	{
		PC8.BamSat.Value_AVG[i]	= PC8.BamSat.Value_SUM[i] * Scalex;
	}

}

void PC8_Get_BeamMax_channel(void)
{
	uint16_t point = 0;
	uint16_t point1 = 0;
	uint16_t point2 = 1;
	uint16_t point3 = 2;
	uint16_t point4 = 3;

	PC8_Clear_AVG_Data();

	PC8.S17_Max_point = (uint8_t)(PC8.BamSat.Start_Screen & 0x00ff);
	PC8.S17_Max = 0; // 38

	for(point = PC8.BamSat.Start_Screen; point < PC8.BamSat.Stop_Screen; point++)
	{
		point1 = point * 4;
		point2 = point1 + 1;
		point3 = point1 + 2;
		point4 = point1 + 3;

		_Caculator_IQ_point(point, point1, point2, point3, point4);
		_Caculator_point(point, point1, point2, point3, point4);
		_Get_Max_Point_dataS(point);
		_Get_Max_Point_V1234S(point);

	}

//	// tinh trung binh nen tin hieu
//	PC8_Caculator_AVG_Data();
//
//	// xac dinh cac dien vuot nguong = PC8.BamSat.fb_SNR_Scale * AVG
//	for(point = PC8.BamSat.Start_Screen; point < PC8.BamSat.Stop_Screen; point++)
//	{
//		// xac dinh muc tieu
//		_Get_Max_Point_data(point);
//	}

	// xac dinh beam co du lieu lon nhat
	//_Cal_Beam_Max();

	// tinh toan I, Q trung binh
	//_Get_Max_Point_V1234();

	PC8_Caculator_AVG_DataS();
	_Get_Max_Beam2();

}

void _Tichluy_Epxilon_Phi(void)
{

	for(TL_Point = 8; TL_Point > 0; TL_Point--)
	{
		PC8.BamSat.TL_Phi[TL_Point] 	= PC8.BamSat.TL_Phi[TL_Point-1];
		PC8.BamSat.TL_Epxilon[TL_Point] = PC8.BamSat.TL_Epxilon[TL_Point-1];
	}

	Get_beam17_Delta_Angle();

	if((PC8.BamSat.Beam_Max_Val == 17)||
	 (((PC8.BamSat.Beam_Max_Val==  6)||(PC8.BamSat.Beam_Max_Val ==  7)||
	   (PC8.BamSat.Beam_Max_Val == 10)||(PC8.BamSat.Beam_Max_Val == 11))&&
	  ((PC8.S17H_Ue >= PC8.BamSat.Ue_Min_kenh_H)&&(PC8.S17H_Ue <= PC8.BamSat.Ue_Max_kenh_H))))
	//if(PC8.BamSat.Beam_Max_Val == 17)
	{
		PC8.BamSat.TL_Phi_final0 		= PC8.S17H_Goc;
	}
	else
	{
		PC8.BamSat.TL_Phi_final0 		= _Get_angle_H(PC8.BamSat.Beam_Max_Val);//
	}

	if((PC8.BamSat.Beam_Max_Val == 17)||
	 (((PC8.BamSat.Beam_Max_Val==  6)||(PC8.BamSat.Beam_Max_Val ==  7)||
	   (PC8.BamSat.Beam_Max_Val == 10)||(PC8.BamSat.Beam_Max_Val == 11))&&
	  ((PC8.S17E_Ue >= PC8.BamSat.Ue_Min_kenh_E)&&(PC8.S17E_Ue <= PC8.BamSat.Ue_Max_kenh_E))))
	//if(PC8.BamSat.Beam_Max_Val == 17)
	{
		PC8.BamSat.TL_Epxilon_final0 	= PC8.S17E_Goc;
	}
	else
	{
		PC8.BamSat.TL_Epxilon_final0 	= _Get_angle_E(PC8.BamSat.Beam_Max_Val);//
	}

	PC8.BamSat.TL_Phi[0] 		= PC8.BamSat.TL_Phi_final0;
	PC8.BamSat.TL_Epxilon[0] 	= PC8.BamSat.TL_Epxilon_final0;

	PC8.BamSat.TL_Phi_sum 		= 0.0;
	PC8.BamSat.TL_Epxilon_sum 	= 0.0;

	for(TL_Point = 0; TL_Point < 8; TL_Point++)
	{
		PC8.BamSat.TL_Phi_sum 		+= PC8.BamSat.TL_Phi[TL_Point];
		PC8.BamSat.TL_Epxilon_sum 	+= PC8.BamSat.TL_Epxilon[TL_Point];
	}

	PC8.BamSat.TL_Phi_final		= PC8.BamSat.TL_Phi_sum / 8.0;
	PC8.BamSat.TL_Epxilon_final = PC8.BamSat.TL_Epxilon_sum / 8.0;

	// luu lai goc tinh toan duoc
	if(PC8.BS_Start == 1)
	{
		PC8.BS_UE0_AVG[15] = PC8.BS_UE0_AVG[14];
		PC8.BS_UE0_AVG[14] = PC8.BS_UE0_AVG[13];
		PC8.BS_UE0_AVG[13] = PC8.BS_UE0_AVG[12];
		PC8.BS_UE0_AVG[12] = PC8.BS_UE0_AVG[11];
		PC8.BS_UE0_AVG[11] = PC8.BS_UE0_AVG[10];
		PC8.BS_UE0_AVG[10] = PC8.BS_UE0_AVG[9];
		PC8.BS_UE0_AVG[9] = PC8.BS_UE0_AVG[8];
		PC8.BS_UE0_AVG[8] = PC8.BS_UE0_AVG[7];
		PC8.BS_UE0_AVG[7] = PC8.BS_UE0_AVG[6];
		PC8.BS_UE0_AVG[6] = PC8.BS_UE0_AVG[5];
		PC8.BS_UE0_AVG[5] = PC8.BS_UE0_AVG[4];
		PC8.BS_UE0_AVG[4] = PC8.BS_UE0_AVG[3];
		PC8.BS_UE0_AVG[3] = PC8.BS_UE0_AVG[2];
		PC8.BS_UE0_AVG[2] = PC8.BS_UE0_AVG[1];
		PC8.BS_UE0_AVG[1] = PC8.BS_UE0_AVG[0];
		PC8.BS_UE0_AVG[0] = PC8.S17E_Ue;

		PC8.BS_UH0_AVG[15] = PC8.BS_UH0_AVG[14];
		PC8.BS_UH0_AVG[14] = PC8.BS_UH0_AVG[13];
		PC8.BS_UH0_AVG[13] = PC8.BS_UH0_AVG[12];
		PC8.BS_UH0_AVG[12] = PC8.BS_UH0_AVG[11];
		PC8.BS_UH0_AVG[11] = PC8.BS_UH0_AVG[10];
		PC8.BS_UH0_AVG[10] = PC8.BS_UH0_AVG[9];
		PC8.BS_UH0_AVG[9] = PC8.BS_UH0_AVG[8];
		PC8.BS_UH0_AVG[8] = PC8.BS_UH0_AVG[7];
		PC8.BS_UH0_AVG[7] = PC8.BS_UH0_AVG[6];
		PC8.BS_UH0_AVG[6] = PC8.BS_UH0_AVG[5];
		PC8.BS_UH0_AVG[5] = PC8.BS_UH0_AVG[4];
		PC8.BS_UH0_AVG[4] = PC8.BS_UH0_AVG[3];
		PC8.BS_UH0_AVG[3] = PC8.BS_UH0_AVG[2];
		PC8.BS_UH0_AVG[2] = PC8.BS_UH0_AVG[1];
		PC8.BS_UH0_AVG[1] = PC8.BS_UH0_AVG[0];
		PC8.BS_UH0_AVG[0] = PC8.S17H_Ue;

		PC8.BS_UE0 =(PC8.BS_UE0_AVG[15] + //= PC8.BS_UE0_AVG[4];
					 PC8.BS_UE0_AVG[14] + //= PC8.BS_UE0_AVG[3];
					 PC8.BS_UE0_AVG[13] + //= PC8.BS_UE0_AVG[2];
					 PC8.BS_UE0_AVG[12] + //= PC8.BS_UE0_AVG[1];
					 PC8.BS_UE0_AVG[11] + //= PC8.BS_UE0_AVG[0];
					 PC8.BS_UE0_AVG[10] +
					 PC8.BS_UE0_AVG[9] + //= PC8.BS_UE0_AVG[6];
					 PC8.BS_UE0_AVG[8] + //= PC8.BS_UE0_AVG[6];
					 PC8.BS_UE0_AVG[7] + //= PC8.BS_UE0_AVG[6];
					 PC8.BS_UE0_AVG[6] + //= PC8.BS_UE0_AVG[5];
					 PC8.BS_UE0_AVG[5] + //= PC8.BS_UE0_AVG[4];
					 PC8.BS_UE0_AVG[4] + //= PC8.BS_UE0_AVG[3];
					 PC8.BS_UE0_AVG[3] + //= PC8.BS_UE0_AVG[2];
					 PC8.BS_UE0_AVG[2] + //= PC8.BS_UE0_AVG[1];
					 PC8.BS_UE0_AVG[1] + //= PC8.BS_UE0_AVG[0];
					 PC8.BS_UE0_AVG[0]) / 16.0;


		PC8.BS_UH0 =(PC8.BS_UH0_AVG[15] + //= PC8.BS_UE0_AVG[4];
					 PC8.BS_UH0_AVG[14] + //= PC8.BS_UE0_AVG[3];
					 PC8.BS_UH0_AVG[13] + //= PC8.BS_UE0_AVG[2];
					 PC8.BS_UH0_AVG[12] + //= PC8.BS_UE0_AVG[1];
					 PC8.BS_UH0_AVG[11] + //= PC8.BS_UE0_AVG[0];
					 PC8.BS_UH0_AVG[10] +
					 PC8.BS_UH0_AVG[9] + //= PC8.BS_UE0_AVG[6];
					 PC8.BS_UH0_AVG[8] + //= PC8.BS_UE0_AVG[6];
					 PC8.BS_UH0_AVG[7] + //= PC8.BS_UE0_AVG[6];
					 PC8.BS_UH0_AVG[6] + //= PC8.BS_UE0_AVG[5];
					 PC8.BS_UH0_AVG[5] + //= PC8.BS_UE0_AVG[4];
					 PC8.BS_UH0_AVG[4] + //= PC8.BS_UE0_AVG[3];
					 PC8.BS_UH0_AVG[3] + //= PC8.BS_UE0_AVG[2];
					 PC8.BS_UH0_AVG[2] + //= PC8.BS_UE0_AVG[1];
					 PC8.BS_UH0_AVG[1] + //= PC8.BS_UE0_AVG[0];
					 PC8.BS_UH0_AVG[0]) / 16.0;

	}

}

void Get_beam17_Delta_Angle(void)
{

	int pi = 0;

	// chuyen do uint32 -> int26
	PC8.a3 = PC8._S17I;
	PC8.b3 = PC8._S17Q;

	PC8.Ha4 = PC8._V2I - PC8._V1I;//31
	PC8.Hb4 = PC8._V2Q - PC8._V1Q;//31

	PC8.Ea4 = PC8._V4I - PC8._V3I;//31
	PC8.Eb4 = PC8._V4Q - PC8._V3Q;//31

	PC8.S17Ei = PC8._V2I - PC8._V1I;
	PC8.S17Eq = PC8._V2Q - PC8._V1Q;

	PC8.S17Hi = PC8._V4I - PC8._V3I;
	PC8.S17Hq = PC8._V4Q - PC8._V3Q;

	// tinh toan I2 + Q2
	PC8.Sum17  = PC8.a3  * PC8.a3  + PC8.b3  * PC8.b3;
	PC8.SumH17 = PC8.Ha4 * PC8.Ha4 + PC8.Hb4 * PC8.Hb4;
	PC8.SumE17 = PC8.Ea4 * PC8.Ea4 + PC8.Eb4 * PC8.Eb4;

	// tinh bien do
	PC8.S17_Amplitude  = sqrt(PC8.Sum17);
	PC8.S17E_Amplitude = sqrt(PC8.SumE17);
	PC8.S17H_Amplitude = sqrt(PC8.SumH17);

	PC8.Amplitude_V1 = PC8.V1max;//sqrt(PC8.V1I * PC8.V1I + PC8.V1Q * PC8.V1Q);
	PC8.Amplitude_V2 = PC8.V2max;//sqrt(PC8.V2I * PC8.V2I + PC8.V2Q * PC8.V2Q);
	PC8.Amplitude_V3 = PC8.V3max;//sqrt(PC8.V3I * PC8.V3I + PC8.V3Q * PC8.V3Q);
	PC8.Amplitude_V4 = PC8.V4max;//sqrt(PC8.V4I * PC8.V4I + PC8.V4Q * PC8.V4Q);

	PC8.S17max_out= PC8.BamSat.Value_Max[16];//31
	PC8.Delta_V12 = (PC8.Amplitude_V2 - PC8.Amplitude_V1) * 4194304;//31
	PC8.Delta_V34 = (PC8.Amplitude_V4 - PC8.Amplitude_V3) * 4194304;//31

	// tinh CosE
	if(PC8.Sum17 != 0)
	{
		PC8.Ha5 = (PC8.Ha4 * PC8.a3 + PC8.Hb4 * PC8.b3) / PC8.Sum17;//31
		PC8.Hb5 = (PC8.Hb4 * PC8.a3 - PC8.Ha4 * PC8.b3) / PC8.Sum17;//31

		PC8.Ea5 = (PC8.Ea4 * PC8.a3 + PC8.Eb4 * PC8.b3) / PC8.Sum17;//31
		PC8.Eb5 = (PC8.Eb4 * PC8.a3 - PC8.Ea4 * PC8.b3) / PC8.Sum17;//31

		// tinh cos_buf
		PC8.S17H_Cos_buf = PC8.Ha5 * PC8.Ha5 + PC8.Hb5 * PC8.Hb5;//31
		PC8.S17E_Cos_buf = PC8.Ea5 * PC8.Ea5 + PC8.Eb5 * PC8.Eb5;//31

		// tinh Cos
		if(PC8.S17E_Cos_buf != 0)
		{
			PC8.S17E_Cos = PC8.Ea5 / sqrt(PC8.S17E_Cos_buf);//31
		}
		else
		{
			PC8.S17E_Cos = 1;
		}

		if(PC8.S17H_Cos_buf != 0)
		{
			PC8.S17H_Cos = PC8.Ha5 / sqrt(PC8.S17H_Cos_buf);//31
		}
		else
		{
			PC8.S17H_Cos = 1;
		}

		PC8.S17E_Uei = PC8.S17E_Cos * PC8.S17E_Amplitude / PC8.S17_Amplitude;//31
		PC8.S17H_Uei = PC8.S17H_Cos * PC8.S17H_Amplitude / PC8.S17_Amplitude;//31

		if(PC8.S17E_Uei > PC8.Ue_Max)  PC8.S17E_Uei = PC8.Ue_Max;
		if(PC8.S17E_Uei < PC8.Ue_Min) PC8.S17E_Uei = PC8.Ue_Min;

		if(PC8.S17H_Uei > PC8.Ue_Max)  PC8.S17H_Uei = PC8.Ue_Max;
		if(PC8.S17H_Uei < PC8.Ue_Min) PC8.S17H_Uei = PC8.Ue_Min;

		// xu ly trung binh Ue
		PC8.S17H_Ueb[8] = PC8.S17H_Ueb[7];//31
		PC8.S17H_Ueb[7] = PC8.S17H_Ueb[6];//31
		PC8.S17H_Ueb[6] = PC8.S17H_Ueb[5];//31
		PC8.S17H_Ueb[5] = PC8.S17H_Ueb[4];//31
		PC8.S17H_Ueb[4] = PC8.S17H_Ueb[3];//31
		PC8.S17H_Ueb[3] = PC8.S17H_Ueb[2];//31
		PC8.S17H_Ueb[2] = PC8.S17H_Ueb[1];//31
		PC8.S17H_Ueb[1] = PC8.S17H_Ueb[0];//31
		PC8.S17H_Ueb[0] = PC8.S17H_Uei;//31

		PC8.S17E_Ueb[8] = PC8.S17E_Ueb[7];//31
		PC8.S17E_Ueb[7] = PC8.S17E_Ueb[6];//31
		PC8.S17E_Ueb[6] = PC8.S17E_Ueb[5];//31
		PC8.S17E_Ueb[5] = PC8.S17E_Ueb[4];//31
		PC8.S17E_Ueb[4] = PC8.S17E_Ueb[3];//31
		PC8.S17E_Ueb[3] = PC8.S17E_Ueb[2];//31
		PC8.S17E_Ueb[2] = PC8.S17E_Ueb[1];//31
		PC8.S17E_Ueb[1] = PC8.S17E_Ueb[0];//31
		PC8.S17E_Ueb[0] = PC8.S17E_Uei;//31

		if(PC8.Pvi_dir == 1) // cho phep tich luy UeH
		{
			PC8.S17H_Ue = ( PC8.S17H_Ueb[7] +
							PC8.S17H_Ueb[6] +
							PC8.S17H_Ueb[5] +
							PC8.S17H_Ueb[4] +
							PC8.S17H_Ueb[3] +
							PC8.S17H_Ueb[2] +
							PC8.S17H_Ueb[1] +
							PC8.S17H_Ueb[0]) / 8.0;
		}
		else // khong tich luy
		{
			PC8.S17H_Ue = PC8.S17H_Uei;
		}

		if(PC8.Gocta_dir == 1) // cho phep tich luy UeE
		{

			PC8.S17E_Ue = ( PC8.S17E_Ueb[7] +
							PC8.S17E_Ueb[6] +
							PC8.S17E_Ueb[5] +
							PC8.S17E_Ueb[4] +
							PC8.S17E_Ueb[3] +
							PC8.S17E_Ueb[2] +
							PC8.S17E_Ueb[1] +
							PC8.S17E_Ueb[0]) / 8.0;
		}
		else // khong tich luy
		{
			PC8.S17E_Ue = PC8.S17E_Uei;
		}

		// tham chieu bang du lieu
		if(PC8.S17E_Ue >= PC8.Excel.Ue_E[0])
		{
			PC8.S17E_Goc = PC8.Excel.AngleE[0] *  0.06;
		}
		else if((PC8.S17E_Ue <= PC8.Excel.Ue_E[1])&&(PC8.S17E_Ue > PC8.Excel.Ue_E[68]))
		{
			for(pi = 1; pi < 68; pi++)
			{
				if((PC8.S17E_Ue <= PC8.Excel.Ue_E[pi])&&(PC8.S17E_Ue > PC8.Excel.Ue_E[pi + 1]))
				{
					PC8.S17E_Goc = PC8.Excel.AngleE[pi] * 0.06;
					break;
				}
			}
		}
		else if(PC8.S17E_Ue <= PC8.Excel.Ue_E[68])
		{
			PC8.S17E_Goc = PC8.Excel.AngleE[68] *  0.06;
		}

		// tham chieu bang du lieu
		if(PC8.S17H_Ue <= PC8.Excel.Ue_H[0])
		{
			PC8.S17H_Goc = PC8.Excel.AngleH[0] *  0.06;
		}
		else if((PC8.S17H_Ue >= PC8.Excel.Ue_H[1])&&(PC8.S17H_Ue < PC8.Excel.Ue_H[68]))
		{
			for(pi = 1; pi < 59; pi++)
			{
				if((PC8.S17H_Ue >= PC8.Excel.Ue_H[pi])&&(PC8.S17H_Ue < PC8.Excel.Ue_H[pi + 1]))
				{
					PC8.S17H_Goc = PC8.Excel.AngleH[pi] *  0.06;
					break;
				}
			}
		}
		else if(PC8.S17H_Ue >= PC8.Excel.Ue_H[68])
		{
			PC8.S17H_Goc = PC8.Excel.AngleH[68] *  0.06;
		}
	}
	else
	{

		PC8.S17E_Cos = 0;
		PC8.S17E_Cos = 0;

		PC8.S17E_Ue  = 0;//31
		PC8.S17E_Goc = 0;//31

		PC8.S17H_Ue  = 0;//31
		PC8.S17H_Goc = 0;//31
	}
}

