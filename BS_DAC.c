#include "DK_Maythu.h"
#define _GNU_SOURCE
#include <math.h>

#define _Set_Beam1_Max 		0
#define _Set_Beam2_Max 		1
#define _Set_Beam3_Max 		2
#define _Set_Beam4_Max 		3
#define _Set_Beam5_Max 		4
#define _Set_Beam6_Max 		5
#define _Set_Beam7_Max 		6
#define _Set_Beam8_Max 		7
#define _Set_Beam9_Max 		8
#define _Set_Beam10_Max 	9
#define _Set_Beam11_Max 	10
#define _Set_Beam12_Max 	11
#define _Set_Beam13_Max 	12
#define _Set_Beam14_Max 	13
#define _Set_Beam15_Max 	14
#define _Set_Beam16_Max 	15
#define _Set_Beam17_Max 	16
#define _Set_SS1_Max 		17
#define _Set_SS2_Max 		18
#define _Set_SS3_Max 		19
#define _Set_SS4_Max 		20
#define _Set_SS5_Max 		21
#define _Set_SS6_Max 		22
#define _Set_SS7_Max 		23
#define _Set_SS8_Max 		24
#define _Set_SS9_Max 		25
#define _Set_SS10_Max 		26
#define _Set_V1_Max 		27
#define _Set_V2_Max 		28


int DAC_counter  = 0;

float Pha_out[4][4];

void Caculator_Beam_Value(double H_angle, double E_angle)
{
	uint8_t k = 0;

	double epsilonB = E_angle * M_PI / 180.0; // Convert degrees to radians
	double phiB 	= H_angle * M_PI / 180.0; // Convert degrees to radians

	for(k = 0; k< 16; k++)
	{
		PC8.DAC.tau[k] 	= (cos(epsilonB)*sin(phiB)*PC8.yPos[k]+sin(epsilonB)*PC8.zPos[k]);
		PC8.DAC.A[k]   		= cexp(-_Complex_I * 2 * M_PI * PC8.DAC.tau[k] / PC8.Lamda);
		PC8.DAC.Angle[k] 	=  cargf(PC8.DAC.A[k]) * 180.0 / M_PI;
	}

	PC8.DAC.Innit_done = 1;

}

/****************************************************************************/
void Radar_Update_DAC(uint16_t Beam, uint16_t Sine, uint16_t cos)
{
	if(Beam == 0)
	{
		Radar_Hyper_Output(sindac1, Sine);
		Radar_Hyper_Output(cosdac1, cos);
	}
	else if(Beam == 1)
	{
		Radar_Hyper_Output(sindac2, Sine);
		Radar_Hyper_Output(cosdac2, cos);
	}
	else if(Beam == 2)
	{
		Radar_Hyper_Output(sindac3, Sine);
		Radar_Hyper_Output(cosdac3, cos);
	}
	else if(Beam == 3)
	{
		Radar_Hyper_Output(sindac4, Sine);
		Radar_Hyper_Output(cosdac4, cos);
	}
	else if(Beam == 4)
	{
		Radar_Hyper_Output(sindac5, Sine);
		Radar_Hyper_Output(cosdac5, cos);
	}
	else if(Beam == 5)
	{
		Radar_Hyper_Output(sindac6, Sine);
		Radar_Hyper_Output(cosdac6, cos);
	}
	else if(Beam == 6)
	{
		Radar_Hyper_Output(sindac7, Sine);
		Radar_Hyper_Output(cosdac7, cos);
	}
	else if(Beam == 7)
	{
		Radar_Hyper_Output(sindac8, Sine);
		Radar_Hyper_Output(cosdac8, cos);
	}
	else if(Beam == 8)
	{
		Radar_Hyper_Output(sindac9, Sine);
		Radar_Hyper_Output(cosdac9, cos);
	}
	else if(Beam == 9)
	{
		Radar_Hyper_Output(sindac10, Sine);
		Radar_Hyper_Output(cosdac10, cos);
	}
	else if(Beam == 10)
	{
		Radar_Hyper_Output(sindac11, Sine);
		Radar_Hyper_Output(cosdac11, cos);
	}
	else if(Beam == 11)
	{
		Radar_Hyper_Output(sindac12, Sine);
		Radar_Hyper_Output(cosdac12, cos);
	}
	else if(Beam == 12)
	{
		Radar_Hyper_Output(sindac13, Sine);
		Radar_Hyper_Output(cosdac13, cos);
	}
	else if(Beam == 13)
	{
		Radar_Hyper_Output(sindac14, Sine);
		Radar_Hyper_Output(cosdac14, cos);
	}
	else if(Beam == 14)
	{
		Radar_Hyper_Output(sindac15, Sine);
		Radar_Hyper_Output(cosdac15, cos);
	}
	else if(Beam == 15)
	{
		Radar_Hyper_Output(sindac16, Sine);
		Radar_Hyper_Output(cosdac16, cos);
	}
}

void _Get_Phase_Val(int Beam)
{

	double H_angle = 0;
	double E_angle = 0;

	if(Beam == _Set_Beam1_Max) 			{ H_angle = -6; E_angle =  6; }
	else if(Beam == _Set_Beam2_Max) 	{ H_angle = -2; E_angle =  6; }
	else if(Beam == _Set_Beam3_Max) 	{ H_angle =  2; E_angle =  6; }
	else if(Beam == _Set_Beam4_Max) 	{ H_angle =  6; E_angle =  6; }
	else if(Beam == _Set_Beam5_Max) 	{ H_angle = -6; E_angle =  2; }
	else if(Beam == _Set_Beam6_Max) 	{ H_angle = -2; E_angle =  2; }
	else if(Beam == _Set_Beam7_Max) 	{ H_angle =  2; E_angle =  2; }
	else if(Beam == _Set_Beam8_Max) 	{ H_angle =  6; E_angle =  2; }
	else if(Beam == _Set_Beam9_Max) 	{ H_angle = -6; E_angle = -2; }
	else if(Beam == _Set_Beam10_Max) 	{ H_angle = -2; E_angle = -2; }
	else if(Beam == _Set_Beam11_Max) 	{ H_angle =  2; E_angle = -2; }
	else if(Beam == _Set_Beam12_Max) 	{ H_angle =  6; E_angle = -2; }
	else if(Beam == _Set_Beam13_Max) 	{ H_angle = -6; E_angle = -6; }
	else if(Beam == _Set_Beam14_Max) 	{ H_angle = -2; E_angle = -6; }
	else if(Beam == _Set_Beam15_Max) 	{ H_angle =  2; E_angle = -6; }
	else if(Beam == _Set_Beam16_Max) 	{ H_angle =  6; E_angle = -6; }
	else if(Beam == _Set_Beam17_Max) 	{ H_angle =  0; E_angle =  0; }
	else if(Beam == _Set_SS1_Max) 		{ H_angle =  0; E_angle =  6; }
	else if(Beam == _Set_SS2_Max) 		{ H_angle =  0; E_angle =  2; }
	else if(Beam == _Set_SS3_Max) 		{ H_angle =  0; E_angle = -2; }
	else if(Beam == _Set_SS4_Max) 		{ H_angle =  0; E_angle = -6; }
	else if(Beam == _Set_SS5_Max) 		{ H_angle =  0; E_angle =  10; }
	else if(Beam == _Set_SS6_Max) 		{ H_angle =  0; E_angle =  8; }
	else if(Beam == _Set_SS7_Max) 		{ H_angle =  0; E_angle =  4; }
	else if(Beam == _Set_SS8_Max) 		{ H_angle =  0; E_angle = -4; }
	else if(Beam == _Set_SS9_Max) 		{ H_angle =  0; E_angle = -8; }
	else if(Beam == _Set_SS10_Max) 		{ H_angle =  0; E_angle = -10; }
	else if(Beam == _Set_V1_Max) 		{ H_angle = -2; E_angle =  0; }
	else if(Beam == _Set_V2_Max) 		{ H_angle =  2; E_angle =  0; }

	Caculator_Beam_Value(H_angle, E_angle);

	Pha_out[0][0] = PC8.DAC.Angle[0];
	Pha_out[0][1] = PC8.DAC.Angle[1];
	Pha_out[0][2] = PC8.DAC.Angle[2];
	Pha_out[0][3] = PC8.DAC.Angle[3];
	Pha_out[1][0] = PC8.DAC.Angle[4];
	Pha_out[1][1] = PC8.DAC.Angle[5];
	Pha_out[1][2] = PC8.DAC.Angle[6];
	Pha_out[1][3] = PC8.DAC.Angle[7];
	Pha_out[2][0] = PC8.DAC.Angle[8];
	Pha_out[2][1] = PC8.DAC.Angle[9];
	Pha_out[2][2] = PC8.DAC.Angle[10];
	Pha_out[2][3] = PC8.DAC.Angle[11];
	Pha_out[3][0] = PC8.DAC.Angle[12];
	Pha_out[3][1] = PC8.DAC.Angle[13];
	Pha_out[3][2] = PC8.DAC.Angle[14];
	Pha_out[3][3] = PC8.DAC.Angle[15];

}


void _Update_DAC_Over_Beam(int Beam)
{
	_Get_Phase_Val(Beam);

	double angle = 0;
	int16_t Sine_o = 0;
	int16_t cos_o  = 0;

	uint16_t Sine_offset = 0;
	uint16_t cos_offset  = 0;

	for(DAC_counter = 0; DAC_counter < 16; DAC_counter++)
	{
		angle = Pha_out[DAC_counter / 4][DAC_counter % 4] * M_PI / 180.0; // Convert degrees to radians

		Sine_o = (int16_t)(32767 * sin(angle));
		cos_o  = (int16_t)(32767 * cos(angle));

		// chuyen doi sang dinh dang fix_16_15
		if(Sine_o >= 0)
		{
			Sine_offset = (uint16_t)(Sine_o);
		}
		else
		{
			Sine_offset = (uint16_t)(Sine_o + 65536);
		}

		if(cos_o >= 0)
		{
			cos_offset = (uint16_t)(cos_o);
		}
		else
		{
			cos_offset = (uint16_t)(cos_o + 65536);
		}

		Radar_Update_DAC(DAC_counter, Sine_offset, cos_offset);
	}
}

void _Update_DAC_Over_Angle(void)
{
	double H_angle = 0;
	double E_angle = 0;
	double angle = 0;
	int16_t Sine_o = 0;
	int16_t cos_o  = 0;

	uint16_t Sine_offset = 0;
	uint16_t cos_offset  = 0;

	//PC8.DAC_KS_Enable		= Radar.PC8_Config[300];
	//PC8.DAC_H_E_Val			= Radar.PC8_Config[301] * 256 + Radar.PC8_Config[302];
	//PC8.DAC_Center_Val		= Radar.PC8_Config[303] * 256 + Radar.PC8_Config[304];

	if(PC8.DAC_KS_Enable == 1) // bam sat H
	{
		H_angle = (PC8.DAC_H_E_Val - PC8.DAC_Center_Val) * 0.06;
		E_angle = 0;
	}
	else if(PC8.DAC_KS_Enable == 2) // bam sat H
	{
		E_angle = (PC8.DAC_H_E_Val - PC8.DAC_Center_Val) * 0.06;
		H_angle = 0;
	}

	Caculator_Beam_Value(H_angle, E_angle);

	Pha_out[0][0] = PC8.DAC.Angle[0];
	Pha_out[0][1] = PC8.DAC.Angle[1];
	Pha_out[0][2] = PC8.DAC.Angle[2];
	Pha_out[0][3] = PC8.DAC.Angle[3];
	Pha_out[1][0] = PC8.DAC.Angle[4];
	Pha_out[1][1] = PC8.DAC.Angle[5];
	Pha_out[1][2] = PC8.DAC.Angle[6];
	Pha_out[1][3] = PC8.DAC.Angle[7];
	Pha_out[2][0] = PC8.DAC.Angle[8];
	Pha_out[2][1] = PC8.DAC.Angle[9];
	Pha_out[2][2] = PC8.DAC.Angle[10];
	Pha_out[2][3] = PC8.DAC.Angle[11];
	Pha_out[3][0] = PC8.DAC.Angle[12];
	Pha_out[3][1] = PC8.DAC.Angle[13];
	Pha_out[3][2] = PC8.DAC.Angle[14];
	Pha_out[3][3] = PC8.DAC.Angle[15];

	for(DAC_counter = 0; DAC_counter < 16; DAC_counter++)
	{
		angle = Pha_out[DAC_counter / 4][DAC_counter % 4] * M_PI / 180.0; // Convert degrees to radians

		Sine_o = (int16_t)(32767 * sin(angle));
		cos_o  = (int16_t)(32767 * cos(angle));

		// chuyen doi sang dinh dang fix_16_15
		if(Sine_o >= 0)
		{
			Sine_offset = (uint16_t)(Sine_o);
		}
		else
		{
			Sine_offset = (uint16_t)(Sine_o + 65536);
		}

		if(cos_o >= 0)
		{
			cos_offset = (uint16_t)(cos_o);
		}
		else
		{
			cos_offset = (uint16_t)(cos_o + 65536);
		}

		Radar_Update_DAC(DAC_counter, Sine_offset, cos_offset);
	}
}

void Radar_Innit_DAC_Phase(void)
{

	PC8._Set_Beam_Max_Point = _Set_Beam17_Max;
	PC8._Set_Beam_Max_Pointi = _Set_Beam17_Max;
	_Update_DAC_Over_Beam(_Set_Beam17_Max);
}
