#include "Erob.h"
#include "MI_505.h"
#include "DK_Maythu.h"
#include "CAN_Control.h"
#include <math.h>

int TL_Point = 0;

void Erob_BS_Phuong_vi_pulse(void)
{
	// tinh goc delta
	PC8.BamSat._Delta_Phi0 = Erob_Motor.Motor_PhuongVi.UDP_Angle - PC8.BS_Center_anglef;

	if(PC8.BamSat._Delta_Phi0 >= 180)
	{
		PC8.BamSat._Delta_Phi = PC8.BamSat._Delta_Phi0 - 360;
	}
	else if(PC8.BamSat._Delta_Phi0 <= -180)
	{
		PC8.BamSat._Delta_Phi = PC8.BamSat._Delta_Phi0 + 360;
	}
	else if((PC8.BamSat._Delta_Phi0 < 180.0)&&
			(PC8.BamSat._Delta_Phi0 > (-180.0)))
	{
		PC8.BamSat._Delta_Phi = PC8.BamSat._Delta_Phi0;
	}

	// dieu khien quay
	if((PC8.BamSat._Delta_Phi <= 6.0)&&(PC8.BamSat._Delta_Phi > 0.06))
	{
		PC8.BamSat._Delta_Phi_Dir 	= 1;
		PC8.BamSat._Delta_Phi_Speed = 11444 / 12;// (Erob_Motor.Motor_PhuongVi.Erob_Freq) / 4;
		PC8.BamSat._Delta_Phi_Enable = 1;
	}
	else if(PC8.BamSat._Delta_Phi >= 6.0)
	{
		PC8.BamSat._Delta_Phi_Dir 	= 1;
		PC8.BamSat._Delta_Phi_Speed = Erob_Motor.Motor_PhuongVi.Erob_Freq;
		PC8.BamSat._Delta_Phi_Enable = 1;
	}
	else if((PC8.BamSat._Delta_Phi >= -6.0)&&(PC8.BamSat._Delta_Phi < -0.06))
	{
		PC8.BamSat._Delta_Phi_Dir 	= 1;
		PC8.BamSat._Delta_Phi_Speed = (Erob_Motor.Motor_PhuongVi.Erob_Freq) / 4;
		PC8.BamSat._Delta_Phi_Enable = 1;
	}
	else if(PC8.BamSat._Delta_Phi <= -6.0)
	{
		PC8.BamSat._Delta_Phi_Dir 	= 1;
		PC8.BamSat._Delta_Phi_Speed = Erob_Motor.Motor_PhuongVi.Erob_Freq;
		PC8.BamSat._Delta_Phi_Enable = 1;
	}
	else if((PC8.BamSat._Delta_Phi <=  0.06)&&
			(PC8.BamSat._Delta_Phi >= -0.06))
	{
		PC8.BamSat._Delta_Phi_Dir 	= 0;
		PC8.BamSat._Delta_Phi_Speed = Erob_Motor.Motor_PhuongVi.Erob_Freq;
		PC8.BamSat._Delta_Phi_Enable = 0;
	}

	UDP_Frame.Phuong_vi_Dir = PC8.BamSat._Delta_Phi_Dir;	// lenh can thuc thi 2

	Erob_Motor_Control(	PC8.BamSat._Delta_Phi_Speed,
						PC8.BamSat._Delta_Phi_Dir,
						PC8.BamSat._Delta_Phi_Enable,
						Radar_Motor_PhuongVi);

}

void Erob_BS_Goc_ta_pulse(void)
{
		// tinh goc delta
		PC8.BamSat._Delta_Epxilon0 = Erob_Motor.Motor_GocTa.UDP_Angle - PC8.BS_Center_alphaf;

		if(PC8.BamSat._Delta_Epxilon0 >= 180)
		{
			PC8.BamSat._Delta_Epxilon = PC8.BamSat._Delta_Epxilon0 - 360;
		}
		else if(PC8.BamSat._Delta_Epxilon0 <= -180)
		{
			PC8.BamSat._Delta_Epxilon = PC8.BamSat._Delta_Epxilon0 + 360;
		}
		else if((PC8.BamSat._Delta_Epxilon0 < 180.0)&&
				(PC8.BamSat._Delta_Epxilon0 > (-180.0)))
		{
			PC8.BamSat._Delta_Epxilon = PC8.BamSat._Delta_Epxilon0;
		}

		// dieu khien quay
		if((PC8.BamSat._Delta_Epxilon <= 6.0)&&(PC8.BamSat._Delta_Epxilon > 0.06))
		{
			PC8.BamSat._Delta_Epxilon_Dir 	= 0;
			PC8.BamSat._Delta_Epxilon_Speed = (Erob_Motor.Motor_GocTa.Erob_Freq) / 4;
			PC8.BamSat._Delta_Epxilon_Enable = 1;
		}
		else if(PC8.BamSat._Delta_Epxilon >= 6.0)
		{
			PC8.BamSat._Delta_Epxilon_Dir 	= 0;
			PC8.BamSat._Delta_Epxilon_Speed = Erob_Motor.Motor_GocTa.Erob_Freq;
			PC8.BamSat._Delta_Epxilon_Enable = 1;
		}
		else if((PC8.BamSat._Delta_Epxilon >= -6.0)&&(PC8.BamSat._Delta_Epxilon < -0.06))
		{
			PC8.BamSat._Delta_Epxilon_Dir 	= 1;
			PC8.BamSat._Delta_Epxilon_Speed = (Erob_Motor.Motor_GocTa.Erob_Freq) / 4;
			PC8.BamSat._Delta_Epxilon_Enable = 1;
		}
		else if(PC8.BamSat._Delta_Epxilon <= -6.0)
		{
			PC8.BamSat._Delta_Epxilon_Dir 	= 1;
			PC8.BamSat._Delta_Epxilon_Speed = Erob_Motor.Motor_GocTa.Erob_Freq;
			PC8.BamSat._Delta_Epxilon_Enable = 1;
		}
		else if((PC8.BamSat._Delta_Epxilon <=  0.06)&&
				(PC8.BamSat._Delta_Epxilon >= -0.06))
		{
			PC8.BamSat._Delta_Epxilon_Dir 	= 0;
			PC8.BamSat._Delta_Epxilon_Speed = Erob_Motor.Motor_GocTa.Erob_Freq;
			PC8.BamSat._Delta_Epxilon_Enable = 0;
		}

		UDP_Frame.Goc_ta_Dir = PC8.BamSat._Delta_Epxilon_Dir;	// lenh can thuc thi 2

		Erob_Motor_Control(	PC8.BamSat._Delta_Epxilon_Speed,
							PC8.BamSat._Delta_Epxilon_Dir,
							PC8.BamSat._Delta_Epxilon_Enable,
							Radar_Motor_GocTa);

}

void Erob_Motor_PhuongVi_Get_CMD(void)
{
	Erob_Motor.Motor_PhuongVi.Erob_Enable 	= PC8.Beta_Enable;// 	= Hop4.Radar_Config2[3];

	if(PC8.Beta_Speed <= 0) PC8.Beta_Speed = 1; // van toc nho nhat 1v/ phut
	// (60 * 100000000) / (524288 * Erob_Control.Beta_Speed) = 11444 / Erob_Control.Beta_Speed;//
	Erob_Motor.Motor_PhuongVi.Erob_Freq = 11444 / PC8.Beta_Speed;// 	= Hop4.Radar_Config2[17];

	//Erob_Control.Beta_Rotate;// 	= Hop4.Radar_Config2[6] * 256 + Hop4.Radar_Config2[5];
	Erob_Motor.Motor_PhuongVi.Erob_DIR 	 = PC8.Beta_DIR;// 		= Hop4.Radar_Config2[7];
	Erob_Motor.Motor_PhuongVi.Erob_Calib = PC8.Beta_Calib;// 		= Hop4.Radar_Config2[8] * 65536 + Hop4.Radar_Config2[9] * 255 + Hop4.Radar_Config2[10];
	Erob_Motor.Motor_PhuongVi.Erob_Mode  = PC8.Beta_Mode;// 		= Hop4.Radar_Config2[11];
	//Erob_Control.Beta_Reset;// 		= Hop4.Radar_Config2[12];
	//Erob_Control.Beta_Update;// 	= Hop4.Radar_Config2[13];
	Erob_Motor.Motor_PhuongVi.Motor_Angle_Value = PC8.Beta_Val;//		= Hop4.Radar_Config2[14] * 256 + Hop4.Radar_Config2[15];

	Erob_Motor.Motor_PhuongVi.Motor_Angle_P0 = PC8.Anten_P0;
	Erob_Motor.Motor_PhuongVi.Motor_Angle_Min = PC8.Anten_start;
	Erob_Motor.Motor_PhuongVi.Motor_Angle_Max = PC8.Anten_stop;

	Erob_Motor.Motor_PhuongVi.Motor_Angle_buff1 = Erob_Motor.Motor_PhuongVi.Motor_Angle_P0 +
												  Erob_Motor.Motor_PhuongVi.Motor_Angle_Min;

	Erob_Motor.Motor_PhuongVi.Motor_Angle_buff2 = Erob_Motor.Motor_PhuongVi.Motor_Angle_P0 +
												  Erob_Motor.Motor_PhuongVi.Motor_Angle_Max;


	Erob_Motor.Motor_PhuongVi.BS_Max = PC8.Offset1;
	Erob_Motor.Motor_PhuongVi.BS_Min = PC8.Offset1 * (-1);
	Erob_Motor.Motor_PhuongVi.Quat_Freq2 = 11444 / PC8.Offset3;

	PC8.BS_Goto_Center_done = 0;// 20
	PC8.BS_Delta_Angle = 0;// 20

	//Erob_Control.Beta_Step;//		= Hop4.Radar_Config2[32] * 0x010000 + Hop4.Radar_Config2[33] * 0x000100 + Hop4.Radar_Config2[34];
	//Erob_Control.Beta_Angle2;//    	= (Erob_Control.Anten_start + Erob_Control.Anten_stop) / 2;

	//Erob_Control.Calib_B_Value;//	= Hop4.Radar_Config2[46] * 256 + Hop4.Radar_Config2[47];
	//Erob_Control.Calib_dir;//		= Hop4.Radar_Config2[48];
	//Erob_Control.Quat_Error;//		= Hop4.Radar_Config2[49];

}

void Erob_Motor_GocTa_Get_CMD(void)
{
	Erob_Motor.Motor_GocTa.Erob_Enable 	= PC8.Alpha_Enable;// 	= Hop4.Radar_Config2[16];

	if(PC8.Alpha_Speed <= 0) PC8.Alpha_Speed = 1; // van toc nho nhat 1v/ phut
	// (60 * 100000000) / (524288 * PC8.Beta_Speed) = 11444 / PC8.Alpha_Speed;
	Erob_Motor.Motor_GocTa.Erob_Freq 	= 11444 / PC8.Alpha_Speed;// 	= Hop4.Radar_Config2[17];
	Erob_Motor.Motor_GocTa.Erob_Calib = PC8.Alpha_Calib;
	//PC8.Alpha_Rotate;// 	= Hop4.Radar_Config2[18] * 256 + Hop4.Radar_Config2[19];
	Erob_Motor.Motor_GocTa.Erob_DIR 	= PC8.Alpha_DIR;// 		= Hop4.Radar_Config2[20];

	Erob_Motor.Motor_GocTa.Motor_Angle_P0 = PC8.Alpha_P0;
	Erob_Motor.Motor_GocTa.Motor_Angle_Min = PC8.Alpha_Min;
	Erob_Motor.Motor_GocTa.Motor_Angle_Max = PC8.Alpha_Max;

	//Erob_Motor.Motor_GocTa.Erob_DIR = PC8.Alpha_Calib;// 	= Hop4.Radar_Config2[21] * 65536 + Hop4.Radar_Config2[22] * 255 + Hop4.Radar_Config2[23];
	Erob_Motor.Motor_GocTa.Erob_Mode  = PC8.Alpha_Mode;////PC8.Alpha_Mode;// 		= Hop4.Radar_Config2[24];
	//PC8.Alpha_Reset;// 	= Hop4.Radar_Config2[25];
	//PC8.Alpha_Update;// 	= Hop4.Radar_Config2[26];
	//PC8.Alpha_Val;// 		= Hop4.Radar_Config2[27] * 256 + Hop4.Radar_Config2[28];
	Erob_Motor.Motor_GocTa.Motor_Angle_Value = PC8.Alpha_Val;

	Erob_Motor.Motor_GocTa.BS_Max = PC8.Offset2;
	Erob_Motor.Motor_GocTa.BS_Min = PC8.Offset2 * (-1);

}

void Erob_Control_Bamsat1(void)
{
	if(Erob_Motor.Motor_PhuongVi.Erob_Enable == 1) // quet quat
	{

		Erob_Motor.Motor_PhuongVi.BS_Delta_Angle0 =  (Erob_Motor.Motor_PhuongVi.BS_Angle -
													 Erob_Motor.Motor_PhuongVi.Motor_Angle_Value);//

		Erob_Motor.Motor_PhuongVi.Check_time++;

		if(Erob_Motor.Motor_PhuongVi.BS_Delta_Angle0 >= 3000)
		{
			Erob_Motor.Motor_PhuongVi.BS_Delta_Angle = Erob_Motor.Motor_PhuongVi.BS_Delta_Angle0 - 6000;
		}
		else if(Erob_Motor.Motor_PhuongVi.BS_Delta_Angle0 < -3000)
		{
			Erob_Motor.Motor_PhuongVi.BS_Delta_Angle = Erob_Motor.Motor_PhuongVi.BS_Delta_Angle0 + 6000;
		}
		else if((Erob_Motor.Motor_PhuongVi.BS_Delta_Angle0 < 3000)&&
				(Erob_Motor.Motor_PhuongVi.BS_Delta_Angle0 > (-3000)))
		{
			Erob_Motor.Motor_PhuongVi.BS_Delta_Angle = Erob_Motor.Motor_PhuongVi.BS_Delta_Angle0;
		}

		// cai Dat van toc
		if(Erob_Motor.Motor_PhuongVi.BS_Delta_Angle >= 200)
		{
			Erob_Motor.Motor_PhuongVi.Q0_Freq = 11444 / PC8.BamSat.Motor_Speed;
		}
		else if(Erob_Motor.Motor_PhuongVi.BS_Delta_Angle <= -200)
		{
			Erob_Motor.Motor_PhuongVi.Q0_Freq = 11444 / PC8.BamSat.Motor_Speed;
		}
		else if((Erob_Motor.Motor_PhuongVi.BS_Delta_Angle < 200)&&
				(Erob_Motor.Motor_PhuongVi.BS_Delta_Angle > -200))
		{
			if(Erob_Motor.Motor_PhuongVi.BS_Delta_Angle >= 100)
			{
				Erob_Motor.Motor_PhuongVi.Q0_Freq = 17166 / PC8.BamSat.Scan_Speed;//11444;
			}
			else if(Erob_Motor.Motor_PhuongVi.BS_Delta_Angle <= -100)
			{
				Erob_Motor.Motor_PhuongVi.Q0_Freq = 17166 / PC8.BamSat.Scan_Speed;//11444;
			}
			else if((Erob_Motor.Motor_PhuongVi.BS_Delta_Angle < 100)&&
					(Erob_Motor.Motor_PhuongVi.BS_Delta_Angle > -100))
			{
				Erob_Motor.Motor_PhuongVi.Q0_Freq = 22888 / PC8.BamSat.Bamsat_Speed;//(11444 * 3) / 2;
			}

		}


		// dieu khien dong co 2
		if(Erob_Motor.Motor_PhuongVi.BS_Delta_Angle >= Erob_Motor.Motor_PhuongVi.BS_Max)
		{
			Erob_Motor.Motor_PhuongVi.Q0_DIR = 0; // quay lui
			Erob_Motor.Motor_PhuongVi.Q0_Enable = 1;
		}
		else if(Erob_Motor.Motor_PhuongVi.BS_Delta_Angle <= Erob_Motor.Motor_PhuongVi.BS_Min)
		{

			Erob_Motor.Motor_PhuongVi.Q0_DIR = 1; // quay tien
			Erob_Motor.Motor_PhuongVi.Q0_Enable = 1;
		}
		else if((Erob_Motor.Motor_PhuongVi.BS_Delta_Angle < Erob_Motor.Motor_PhuongVi.BS_Max)&&
				(Erob_Motor.Motor_PhuongVi.BS_Delta_Angle > Erob_Motor.Motor_PhuongVi.BS_Min))
		{
			Erob_Motor.Motor_PhuongVi.Q0_DIR = 1;
			Erob_Motor.Motor_PhuongVi.Q0_Enable = 0;
		}

		UDP_Frame.Phuong_vi_Dir = Erob_Motor.Motor_PhuongVi.Q0_DIR;	// lenh can thuc thi 2

		if((Erob_Motor.Motor_PhuongVi.BS_Delta_Angle > 2000)&&
		   (Erob_Motor.Motor_PhuongVi.BS_Delta_Angle < -2000))
		{
			if(Erob_Motor.Motor_PhuongVi.Check_time >= 5)
			{
				Erob_Motor.Motor_PhuongVi.Check_time = 0;
				Erob_Motor_Control(	Erob_Motor.Motor_PhuongVi.Q0_Freq,
									Erob_Motor.Motor_PhuongVi.Q0_DIR,
									Erob_Motor.Motor_PhuongVi.Q0_Enable,
									Radar_Motor_PhuongVi);
			}
		}
		else
		{
			Erob_Motor.Motor_PhuongVi.Check_time = 0;
			Erob_Motor_Control(	Erob_Motor.Motor_PhuongVi.Q0_Freq,
								Erob_Motor.Motor_PhuongVi.Q0_DIR,
								Erob_Motor.Motor_PhuongVi.Q0_Enable,
								Radar_Motor_PhuongVi);
		}

	}
	else if(Erob_Motor.Motor_PhuongVi.Erob_Enable == 0) // quet quat
	{
		Erob_Motor_Control(	Erob_Motor.Motor_PhuongVi.Erob_Freq,
							Erob_Motor.Motor_PhuongVi.Erob_DIR,
							Erob_Motor.Motor_PhuongVi.Erob_Enable,
							Radar_Motor_PhuongVi);
	}
}

void Erob_Control_Bamsat2(void)
{
	if(Erob_Motor.Motor_GocTa.Erob_Enable == 1) // quet quat
	{

		Erob_Motor.Motor_GocTa.BS_Delta_Angle =  (Erob_Motor.Motor_GocTa.BS_Angle -
												  Erob_Motor.Motor_GocTa.Motor_Angle_Value);//

		if(Erob_Motor.Motor_GocTa.BS_Delta_Angle >= 3000)
		{
			Erob_Motor.Motor_GocTa.BS_Delta_Angle = Erob_Motor.Motor_GocTa.BS_Delta_Angle - 6000;
		}
		else if(Erob_Motor.Motor_GocTa.BS_Delta_Angle < -3000)
		{
			Erob_Motor.Motor_GocTa.BS_Delta_Angle = Erob_Motor.Motor_GocTa.BS_Delta_Angle + 6000;
		}

		// cai Dat van toc
		if(Erob_Motor.Motor_GocTa.BS_Delta_Angle >= 400)
		{
			Erob_Motor.Motor_GocTa.Q0_Freq = 11444;
		}
		else if(Erob_Motor.Motor_GocTa.BS_Delta_Angle <= -400)
		{
			Erob_Motor.Motor_GocTa.Q0_Freq = 11444;
		}
		else if((Erob_Motor.Motor_GocTa.BS_Delta_Angle < 400)&&
				(Erob_Motor.Motor_GocTa.BS_Delta_Angle > -400))
		{
			if(Erob_Motor.Motor_GocTa.BS_Delta_Angle >= 300)
			{
				Erob_Motor.Motor_GocTa.Q0_Freq = 11444;
			}
			else if(Erob_Motor.Motor_GocTa.BS_Delta_Angle <= -300)
			{
				Erob_Motor.Motor_GocTa.Q0_Freq = 11444;
			}
			else if((Erob_Motor.Motor_GocTa.BS_Delta_Angle < 300)&&
					(Erob_Motor.Motor_GocTa.BS_Delta_Angle > -300))
			{
				Erob_Motor.Motor_GocTa.Q0_Freq = (11444 * 3) / 2;
			}
		}

		//Erob_Motor.Motor_GocTa.BS_Anglef = Erob_Motor.Motor_GocTa.UDP_Angle;

		// Erob_Motor.Motor_GocTa.UDP_Angle
		// dieu khien dong co 2
		if(Erob_Motor.Motor_GocTa.BS_Delta_Angle >= Erob_Motor.Motor_PhuongVi.BS_Max)
		{
			Erob_Motor.Motor_GocTa.Q0_DIR = 1; // 0
			Erob_Motor.Motor_GocTa.Q0_Enable = 1;
		}
		else if(Erob_Motor.Motor_GocTa.BS_Delta_Angle <= Erob_Motor.Motor_PhuongVi.BS_Min)
		{

			Erob_Motor.Motor_GocTa.Q0_DIR = 0; // 1
			Erob_Motor.Motor_GocTa.Q0_Enable = 1;
		}
		else
		{
			Erob_Motor.Motor_GocTa.Q0_DIR = 1;
			Erob_Motor.Motor_GocTa.Q0_Enable = 0;
		}

		UDP_Frame.Goc_ta_Dir = Erob_Motor.Motor_GocTa.Q0_DIR;	// lenh can thuc thi 2

		Erob_Motor_Control(	Erob_Motor.Motor_GocTa.Q0_Freq,
							Erob_Motor.Motor_GocTa.Q0_DIR,
							Erob_Motor.Motor_GocTa.Q0_Enable,
							Radar_Motor_GocTa);

	}
	else if(Erob_Motor.Motor_GocTa.Erob_Enable == 0) // quet quat
	{
		Erob_Motor.Motor_GocTa.Q0_DIR 	 = 1;
		Erob_Motor.Motor_GocTa.Q0_Enable = 0;

		Erob_Motor_Control(	Erob_Motor.Motor_GocTa.Erob_Freq,
							Erob_Motor.Motor_GocTa.Erob_DIR,
							Erob_Motor.Motor_GocTa.Erob_Enable,
							Radar_Motor_GocTa);

		//Erob_Motor.Motor_GocTa.Motor_Enable = 0; // 3 0-1
		//Erob_Motor.Motor_GocTa.Motor_DIR = 0;// 7
		//Erob_Motor.Motor_GocTa.Quat_DIR = 0;// 7
	}
}

void Erob_Control_Pulse_Mode1(void)
{
	//void Motor1_Mode_Freq(unsigned int Freq, unsigned char Dir, unsigned char Enable)
	Motor1_Mode_Freq(Erob_Motor.Motor_PhuongVi.Pulse_Freq,
					 Erob_Motor.Motor_PhuongVi.Pulse_DIR,
					 0); // tat che do phat xung lien tuc
}

void Erob_Control_Pulse_Mode2(void)
{
	//void Motor1_Mode_Freq(unsigned int Freq, unsigned char Dir, unsigned char Enable)
	Motor2_Mode_Freq(Erob_Motor.Motor_GocTa.Pulse_Freq,
					 Erob_Motor.Motor_GocTa.Pulse_DIR,
					 0); // tat che do phat xung lien tuc

}

void Erob_Control_Motor1_Rotate(void)
{
Erob_Motor_Control(	Erob_Motor.Motor_PhuongVi.Erob_Freq,
								Erob_Motor.Motor_PhuongVi.Erob_DIR,
								Erob_Motor.Motor_PhuongVi.Erob_Enable,
								Radar_Motor_PhuongVi);

			Erob_Motor.Motor_PhuongVi.Motor_Enable = 0; // 3 0-1
			Erob_Motor.Motor_PhuongVi.Motor_DIR = 0;// 7
			Erob_Motor.Motor_PhuongVi.Quat_DIR = 0;// 7
}

void Erob_Control_Motor1(void)
{
	if(Erob_Motor.Motor_PhuongVi.Erob_Enable == 1) // quet quat
	{

		Erob_Motor.Motor_PhuongVi.Quat_Delta0 = Erob_Motor.Motor_PhuongVi.UDP_Angle -
											   Erob_Motor.Motor_PhuongVi.Motor_Angle_P0;

		Erob_Motor.Motor_PhuongVi.Check_time++;

		if(Erob_Motor.Motor_PhuongVi.Quat_Delta0 >= 180)
		{
			Erob_Motor.Motor_PhuongVi.Quat_Delta = Erob_Motor.Motor_PhuongVi.Quat_Delta0 - 360;
		}
		else if(Erob_Motor.Motor_PhuongVi.Quat_Delta0 <= -180)
		{
			Erob_Motor.Motor_PhuongVi.Quat_Delta = Erob_Motor.Motor_PhuongVi.Quat_Delta0 + 360;
		}
		else if((Erob_Motor.Motor_PhuongVi.Quat_Delta < 180.0)&&
				(Erob_Motor.Motor_PhuongVi.Quat_Delta > (-180.0)))
		{
			Erob_Motor.Motor_PhuongVi.Quat_Delta = Erob_Motor.Motor_PhuongVi.Quat_Delta0;
		}


		if(Erob_Motor.Motor_PhuongVi.Quat_Delta < Erob_Motor.Motor_PhuongVi.Motor_Angle_Min)
		{
			Erob_Motor.Motor_PhuongVi.Quat_DIR = 1;
			Erob_Motor.Motor_PhuongVi.Motor_DIR = 1;
			Erob_Motor.Motor_PhuongVi.Quat_Freq = (Erob_Motor.Motor_PhuongVi.Erob_Freq * 7) / 3;
		}
		else if(Erob_Motor.Motor_PhuongVi.Quat_Delta > Erob_Motor.Motor_PhuongVi.Motor_Angle_Max)
		{
			Erob_Motor.Motor_PhuongVi.Quat_DIR = 0;
			Erob_Motor.Motor_PhuongVi.Motor_DIR = 0;
			Erob_Motor.Motor_PhuongVi.Quat_Freq = (Erob_Motor.Motor_PhuongVi.Erob_Freq * 7) / 3;
		}
		else if((Erob_Motor.Motor_PhuongVi.Quat_Delta >= (Erob_Motor.Motor_PhuongVi.Motor_Angle_Min))&&
				(Erob_Motor.Motor_PhuongVi.Quat_Delta <= Erob_Motor.Motor_PhuongVi.Motor_Angle_Max))
		{
			if((Erob_Motor.Motor_PhuongVi.Quat_Delta >= ((Erob_Motor.Motor_PhuongVi.Motor_Angle_Min * 9)/ 10))&&
			   (Erob_Motor.Motor_PhuongVi.Quat_Delta <= ((Erob_Motor.Motor_PhuongVi.Motor_Angle_Max * 9)/ 10)))
			{
				Erob_Motor.Motor_PhuongVi.Quat_Freq = Erob_Motor.Motor_PhuongVi.Erob_Freq;
			}
			else if((Erob_Motor.Motor_PhuongVi.Quat_Delta < ((Erob_Motor.Motor_PhuongVi.Motor_Angle_Min * 9)/ 10))&&
					(Erob_Motor.Motor_PhuongVi.Quat_Delta > ((Erob_Motor.Motor_PhuongVi.Motor_Angle_Max * 9)/ 10)))
					{
						Erob_Motor.Motor_PhuongVi.Quat_Freq = (Erob_Motor.Motor_PhuongVi.Erob_Freq * 7) / 3;
					}

			if(Erob_Motor.Motor_PhuongVi.Quat_DIR == 0)
			{
			Erob_Motor.Motor_PhuongVi.Motor_DIR = 0;
			}
			else if(Erob_Motor.Motor_PhuongVi.Quat_DIR == 1)
			{
			Erob_Motor.Motor_PhuongVi.Motor_DIR = 1;
			}
		}

		UDP_Frame.Phuong_vi_Dir = Erob_Motor.Motor_PhuongVi.Motor_DIR;	// lenh can thuc thi 2

		if((Erob_Motor.Motor_PhuongVi.BS_Delta_Angle > 2000)&&
		   (Erob_Motor.Motor_PhuongVi.BS_Delta_Angle < -2000))
		{
			if(Erob_Motor.Motor_PhuongVi.Check_time >= 5)
			{
				Erob_Motor.Motor_PhuongVi.Check_time = 0;
				Erob_Motor_Control(	Erob_Motor.Motor_PhuongVi.Quat_Freq,
									Erob_Motor.Motor_PhuongVi.Motor_DIR,
									Erob_Motor.Motor_PhuongVi.Erob_Enable,
									Radar_Motor_PhuongVi);
			}
		}
		else
		{
			Erob_Motor.Motor_PhuongVi.Check_time = 0;
			Erob_Motor_Control(	Erob_Motor.Motor_PhuongVi.Quat_Freq,
								Erob_Motor.Motor_PhuongVi.Motor_DIR,
								Erob_Motor.Motor_PhuongVi.Erob_Enable,
								Radar_Motor_PhuongVi);
		}

	}
	else if(Erob_Motor.Motor_PhuongVi.Erob_Enable == 0) // quet quat
	{



		Erob_Motor_Control(	Erob_Motor.Motor_PhuongVi.Erob_Freq,
							Erob_Motor.Motor_PhuongVi.Motor_DIR,
							Erob_Motor.Motor_PhuongVi.Erob_Enable,
							Radar_Motor_PhuongVi);
	}
}

void Erob_Control_Motor2(void)
{
	if(Erob_Motor.Motor_GocTa.Erob_Enable == 1) // quet quat
	{
	Erob_Motor.Motor_GocTa.Quat_Delta = Erob_Motor.Motor_GocTa.UDP_Angle - Erob_Motor.Motor_GocTa.Motor_Angle_P0;

		if(Erob_Motor.Motor_GocTa.Quat_Delta >= 180)
		{
			Erob_Motor.Motor_GocTa.Quat_Delta = Erob_Motor.Motor_GocTa.Quat_Delta - 360;
		}
		else if(Erob_Motor.Motor_GocTa.Quat_Delta <= -180)
		{
			Erob_Motor.Motor_GocTa.Quat_Delta = Erob_Motor.Motor_GocTa.Quat_Delta + 360;
		}

		if(Erob_Motor.Motor_GocTa.Quat_Delta < Erob_Motor.Motor_GocTa.Motor_Angle_Min)
		{
			Erob_Motor.Motor_GocTa.Quat_DIR = 1; // 0
			Erob_Motor.Motor_GocTa.Motor_DIR = 0;
		}
		else if(Erob_Motor.Motor_GocTa.Quat_Delta > Erob_Motor.Motor_GocTa.Motor_Angle_Max)
		{
			Erob_Motor.Motor_GocTa.Quat_DIR = 0; // 1
			Erob_Motor.Motor_GocTa.Motor_DIR = 1;
		}
		else if((Erob_Motor.Motor_GocTa.Quat_Delta >= Erob_Motor.Motor_GocTa.Motor_Angle_Min)&&
				(Erob_Motor.Motor_GocTa.Quat_Delta <= Erob_Motor.Motor_GocTa.Motor_Angle_Max))
		{
			if(Erob_Motor.Motor_GocTa.Quat_DIR == 0)
			{
			Erob_Motor.Motor_GocTa.Motor_DIR = 1; // 1
			}
			else if(Erob_Motor.Motor_GocTa.Quat_DIR == 1)
			{
			Erob_Motor.Motor_GocTa.Motor_DIR = 0; // 0
			}
		}

		UDP_Frame.Goc_ta_Dir = Erob_Motor.Motor_GocTa.Motor_DIR;	// lenh can thuc thi 2

		Erob_Motor_Control(	Erob_Motor.Motor_GocTa.Erob_Freq,
							Erob_Motor.Motor_GocTa.Motor_DIR,
							Erob_Motor.Motor_GocTa.Erob_Enable,
							Radar_Motor_GocTa);
	}
	else if(Erob_Motor.Motor_GocTa.Erob_Enable == 0) // quet quat
	{
		Erob_Motor_Control(	Erob_Motor.Motor_GocTa.Erob_Freq,
							Erob_Motor.Motor_GocTa.Motor_DIR,
							Erob_Motor.Motor_GocTa.Erob_Enable,
							Radar_Motor_GocTa);
	}
}

