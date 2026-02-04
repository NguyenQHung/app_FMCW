#include "Erob.h"
#include "MI_505.h"
#include "DK_Maythu.h"
#include "CAN_Control.h"
#define _GNU_SOURCE
#include <math.h>

extern struct Erob_Struct  Erob_Motor;

void _Khao_Sat_Get_Max(void)
{
	int i = 0;

	PC8._KS_Pvi.Max_Point 	= 0;
	PC8._KS_Pvi.Max_V1 		= 0;
	PC8._KS_Pvi.Max_V2 		= 0;
	PC8._KS_Pvi.Max_S17 	= 0;
	PC8._KS_Pvi.Max_Ue 		= 0;

	for(i = 0; i < 334; i++)
	{
		if(PC8._KS_Pvi.Max_S17 < PC8._KS_Pvi.S17[i])
		{
			PC8._KS_Pvi.Max_Point 	= i;
			PC8._KS_Pvi.Max_V1 		= PC8._KS_Pvi.V1[i];
			PC8._KS_Pvi.Max_V2 		= PC8._KS_Pvi.V2[i];
			PC8._KS_Pvi.Max_S17 	= PC8._KS_Pvi.S17[i];
			PC8._KS_Pvi.Max_Ue 		= PC8._KS_Pvi.Ue[i];
		}
	}

	// chuyen doi ly giac -> do
	PC8._KS_Pvi.ly_giac = PC8._KS_Pvi.Max_Point - 167;
	PC8._KS_Pvi.Center  = PC8._KS_Pvi.ly_giac * 0.06;

}

void _Khao_Sat_Phuong_Vi(void)
{

	PC8._KS_Pvi.Delta_Angle = Erob_Motor.Motor_PhuongVi.BS_Angle - PC8.BS_Angle_Hz0;

	if(PC8._KS_Pvi.Delta_Angle > 3000)
	{
		PC8._KS_Pvi.Delta_Angle = PC8._KS_Pvi.Delta_Angle - 6000;
	}
	else if(PC8._KS_Pvi.Delta_Angle < -3000)
	{
		PC8._KS_Pvi.Delta_Angle = 6000 + PC8._KS_Pvi.Delta_Angle;
	}

	if(PC8._KS_Pvi.Delta_Angle <= -167)
	{
		PC8._KS_Pvi.Point = 0;
	}
	else if(PC8._KS_Pvi.Delta_Angle >= 167)
	{
		PC8._KS_Pvi.Point = 333;
	}
	else if((PC8._KS_Pvi.Delta_Angle >-167)&&
		    (PC8._KS_Pvi.Delta_Angle < 167))
	{
		PC8._KS_Pvi.Number = PC8._KS_Pvi.Delta_Angle + 167;
		PC8._KS_Pvi.Point  = (uint16_t)(PC8._KS_Pvi.Number);
	}

	PC8._KS_Pvi.V1[PC8._KS_Pvi.Point] = PC8.Amplitude_V1;
	PC8._KS_Pvi.V2[PC8._KS_Pvi.Point] = PC8.Amplitude_V2;
	PC8._KS_Pvi.S17[PC8._KS_Pvi.Point]= PC8.S17_Amplitude;
	PC8._KS_Pvi.Ue[PC8._KS_Pvi.Point] = PC8.S17E_Ue;

}
