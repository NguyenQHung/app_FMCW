#include "Erob.h"
#include "MI_505.h"
#include "DK_Maythu.h"
#define _GNU_SOURCE
#include <math.h>


void Cua_Song_Innit(void)
{
	PC8.BamSat.Max_s1 = 100; // cua song lon nhat cho thang 1, [Cell]
	PC8.BamSat.Max_s2 = 100; // cua song lon nhat cho thang 2, [Cell]
	PC8.BamSat.Max_s3 = 100; // cua song lon nhat cho thang 3, [Cell]
	PC8.BamSat.Max_s4 = 100; // cua song lon nhat cho thang 4, [Cell]
	PC8.BamSat.Max_s5 = 100; // cua song lon nhat cho thang 5, [Cell]

	PC8.BamSat.Min_R1 = 10; // cu ly bam sat nho nhat cho thang 1, [Cell]
	PC8.BamSat.Min_R2 = 10; // cu ly bam sat nho nhat cho thang 2, [Cell]
	PC8.BamSat.Min_R3 = 10; // cu ly bam sat nho nhat cho thang 3, [Cell]
	PC8.BamSat.Min_R4 = 10; // cu ly bam sat nho nhat cho thang 4, [Cell]
	PC8.BamSat.Min_R5 = 10; // cu ly bam sat nho nhat cho thang 5, [Cell]

	PC8.Target_Radius0 = 0;
	PC8.Target_Beta0 = 0;
	PC8.Target_Alpha0 = 0;
	PC8.x0 = 0;
	PC8.y0 = 0;
	PC8.z0 = 0;

	PC8.Target_Radius1 = 0;
	PC8.Target_Beta1 = 0;
	PC8.Target_Alpha1 = 0;
	PC8.x1 = 0;
	PC8.y1 = 0;
	PC8.z1 = 0;

	PC8.step_counter = 0; // 38
	PC8.step_counter_Max = 487;
	PC8.step_counter1 = 0;
	PC8.Delta_Step = 0;
	PC8.Delta_time = 0;

	PC8.Target_Radius = 0;
	PC8.Target_Beta = 0;
	PC8.Target_Alpha = 0;
	PC8.Target_Hight = 0;
	PC8.Target_Speed = 0;

	PC8.BamSat.Max_Cell1 = 10679;
	PC8.BamSat.Max_Cell2 = 10679;
	PC8.BamSat.Max_Cell3 = 10679;
	PC8.BamSat.Max_Cell4 = 10679;
	PC8.BamSat.Max_Cell5 = 10679;
	PC8.BamSat.Max_Cell  = 10679;
	PC8.BamSat.Safe_Cell = 10479;

	PC8.BamSat.Min_Detect_mode = 0;

	PC8.BamSat.Max_s = PC8.BamSat.Max_s5; // cua song lon nhat cho thang 5, [Cell]
	PC8.BamSat.Min_R = PC8.BamSat.Min_R5; // cu ly bam sat nho nhat cho thang 1, [Cell]

	PC8.BamSat.Start_Screen = 1; // o cell bat dau [Cell]
	PC8.BamSat.Stop_Screen  = 200; // o cell ket thuc [Cell]

	PC8.BamSat._CS_E_Enable = 0;
}

void Cua_Song_Recal_Range(void)
{
	PC8.BamSat.Max_s = PC8.BamSat.Max_s5; // cua song lon nhat cho thang 5, [Cell]
	PC8.BamSat.Min_R = PC8.BamSat.Min_R5; // cu ly bam sat nho nhat cho thang 5, [Cell]
	PC8.BamSat.Limit_H = 10679 - PC8.BamSat.Min_R;
}

void Cua_Song_Cal_New_center(void)
{
	// tinh toan lai vi tri center
	if((PC8.BS_Center_stop_bs > 200)&&(PC8.BS_Center_start_bs <= PC8.BamSat.Safe_Cell)) // Center = 100
	{
		PC8.BamSat.Center_Screen = 100;
	}
	else if((PC8.BS_Center_stop_bs == 200)&&(PC8.BS_Center_start_bs == 0)) // sat tam dai radar
	{
		PC8.BamSat.Center_Screen = PC8.BS_PS17;
	}
	else if(PC8.BS_Center_start_bs > PC8.BamSat.Safe_Cell) // sat tam dai radar
	{
		PC8.BamSat.Center_Screen = PC8.BS_PS17;
	}
}

void Cua_Song_Cal_New_Limit(void)
{
	PC8.BamSat.Max_Cell = PC8.BamSat.Max_Cell5;
	PC8.BamSat.Safe_Cell = PC8.BamSat.Max_Cell - 201;
}

void Cua_Song_Auto_Fix(void)
{
	if(PC8.BS_Center_start >= PC8.BamSat.Min_R)
		{
			PC8.BamSat.Fix_Protect = 0;

			if(PC8.BamSat.Center_Screen >= PC8.BamSat.Max_s)
			{
				PC8.BamSat.Start_Screen = PC8.BamSat.Center_Screen - PC8.BamSat.Max_s + 1; // o cell bat dau [Cell]
			}
			else
			{
				PC8.BamSat.Start_Screen = 1;
			}
		}
		else if(PC8.BS_Center_start < PC8.BamSat.Min_R)
		{
			PC8.BamSat.Fix_Protect = PC8.BamSat.Max_s - PC8.BS_Center_start;

			if(PC8.BamSat.Center_Screen >= PC8.BamSat.Max_s)
			{
				PC8.BamSat.Start_Screen = (PC8.BamSat.Center_Screen - PC8.BamSat.Max_s) + PC8.BamSat.Fix_Protect + 1; // o cell bat dau [Cell]
			}
			else
			{
				PC8.BamSat.Start_Screen = PC8.BamSat.Fix_Protect + 1; // o cell bat dau [Cell]
			}
		}


		PC8.BamSat.High_Screen = PC8.BS_Center_start + PC8.BamSat.Max_s;

		// tinh toan lai : PC8.BamSat.Stop_Screen
		if(PC8.BamSat.High_Screen > PC8.BamSat.Limit_H)
		{
			PC8.BamSat.Fix_ProtectH = PC8.BamSat.High_Screen - PC8.BamSat.Limit_H;
			PC8.BamSat.Stop_Screen  = (PC8.BamSat.Center_Screen + PC8.BamSat.Max_s) - PC8.BamSat.Fix_ProtectH; // o cell ket thuc [Cell]
		}
		else if(PC8.BamSat.High_Screen <= PC8.BamSat.Limit_H)
		{
			PC8.BamSat.Stop_Screen  = PC8.BamSat.Center_Screen + PC8.BamSat.Max_s; // o cell ket thuc [Cell]
		}

		if(PC8.BamSat.Start_Screen > PC8.BamSat.Stop_Screen)
		{
			PC8.BamSat.Buffer_Screen 	= PC8.BamSat.Stop_Screen;
			PC8.BamSat.Stop_Screen 		= PC8.BamSat.Start_Screen;
			PC8.BamSat.Start_Screen 	= PC8.BamSat.Buffer_Screen;
		}
}

void Cua_Song_Cal_target_information(void)
{
	PC8.Delta_Step = 1.0 / 1000.0; // 1220 Hz
	PC8.Target_Radius = (PC8.BS_Center_start_bs + PC8.BS_PS17) * 15.0; // cu ly muc tieu
	PC8.step_counter_Max = 122;

	PC8.Target_Beta  = Erob_Motor.Motor_PhuongVi.Motor_BS_Value / 6000.0;
	PC8.Target_Alpha = Erob_Motor.Motor_GocTa.Motor_BS_Value / 6000.0;
	PC8.Target_Hight = abs(PC8.Target_Radius * sin(PC8.Target_Alpha * 3.1415926535 / 180.0)); // luon duong

	PC8.step_counter0++;
	PC8.Delta_Step = (PC8.step_counter0 * 1.0) / PC8.step_counter_Max;

	if(PC8.step_counter1 >= (PC8.step_counter_Max))
	{
		PC8.Target_Radius1 	= PC8.Target_Radius;
		PC8.Target_Beta1 	= PC8.Target_Beta;
		PC8.Target_Alpha1 	= PC8.Target_Alpha;

		PC8.radian_beta 	= PC8.Target_Beta * 3.1415926535 / 180.0;
		PC8.radian_alpha 	= PC8.Target_Alpha1 * 3.1415926535 / 180.0;

		PC8.x1 				= PC8.Target_Radius * sin(PC8.radian_alpha) * cos(PC8.radian_beta);
		PC8.y1 				= PC8.Target_Radius * sin(PC8.radian_alpha) * sin(PC8.radian_beta);
		PC8.z1 				= PC8.Target_Radius * cos(PC8.radian_beta);

		PC8.step_counter  = PC8.step_counter1;
		PC8.step_counter1 = 0;

		PC8.Delta_time 	  = PC8.step_counter * PC8.Delta_Step;

		PC8.Target_Speedi[3] = PC8.Target_Speedi[2];
		PC8.Target_Speedi[2] = PC8.Target_Speedi[1];
		PC8.Target_Speedi[1] = PC8.Target_Speedi[0];

		if(PC8.Delta_Step >= 2.0)
		{
			PC8.Target_Speedi[0]  = abs(PC8.Target_Radius1 - PC8.Target_Radius0) / 2.0;//(PC8.Delta_time * 2.0);
		}
		else
		{
			PC8.Target_Speedi[0]  = 0.0;
		}

		PC8.Target_Speed = (PC8.Target_Speedi[0] + PC8.Target_Speedi[1] + PC8.Target_Speedi[2] + PC8.Target_Speedi[3]) / 4.0;

		PC8.Target_Radius0 	= PC8.Target_Radius1;
		PC8.Target_Beta0 	= PC8.Target_Beta1;
		PC8.Target_Alpha0 	= PC8.Target_Alpha1;
		PC8.x0 				= PC8.x1;
		PC8.y0 				= PC8.y1;
		PC8.z0 				= PC8.z1;



	}
}
