#include "Erob.h"
#include "MI_505.h"
#include "DK_Maythu.h"
#include "CAN_Control.h"
#define _GNU_SOURCE
#include <math.h>

void _BS_E_Mode_Angle(void)
{
	if (PC8.BamSat.TL_Epxilon_final < 0)
	{
		if(Erob_Motor.Motor_GocTa.Motor_BS_Value > PC8.BamSat.BS_Fix_E)
		{
			Erob_Motor.Motor_GocTa.Motor_BS_Value-= PC8.BamSat.BS_Fix_E;
		}
		else
		{
			Erob_Motor.Motor_GocTa.Motor_BS_Value = 5999;
		}
	}
	else if (PC8.BamSat.TL_Epxilon_final > 0)
	{
		if(Erob_Motor.Motor_GocTa.Motor_BS_Value < (5999 - PC8.BamSat.BS_Fix_E))
		{
			Erob_Motor.Motor_GocTa.Motor_BS_Value += PC8.BamSat.BS_Fix_E;
		}
		else
		{
			Erob_Motor.Motor_GocTa.Motor_BS_Value = 0;
		}
	}
}

void _Sub_E_Mode0(void)
{
	if (PC8.BS_UE0 > PC8.BamSat.Nguong_H_DK_Ue_E)
	{
		if(Erob_Motor.Motor_GocTa.Motor_BS_Value > PC8.BamSat.BS_Fix_E0)
		{
			Erob_Motor.Motor_GocTa.Motor_BS_Value-= PC8.BamSat.BS_Fix_E0;
		}
		else
		{
			Erob_Motor.Motor_GocTa.Motor_BS_Value = 5999;
		}
	}
	else if (PC8.BS_UE0 < PC8.BamSat.Nguong_L_DK_Ue_E)
	{
		if(Erob_Motor.Motor_GocTa.Motor_BS_Value < (5999 - PC8.BamSat.BS_Fix_E0))
		{
			Erob_Motor.Motor_GocTa.Motor_BS_Value+= PC8.BamSat.BS_Fix_E0;
		}
		else
		{
			Erob_Motor.Motor_GocTa.Motor_BS_Value = 0;
		}
	}
}

void _Sub_E_Mode1(void)
{
	if (PC8.BS_UE0 > PC8.BamSat.Nguong_H_DK_Ue_E)
	{
		if(Erob_Motor.Motor_GocTa.Motor_BS_Value < (5999 - PC8.BamSat.BS_Fix_E0))
		{
			Erob_Motor.Motor_GocTa.Motor_BS_Value+= PC8.BamSat.BS_Fix_E0;
		}
		else
		{
			Erob_Motor.Motor_GocTa.Motor_BS_Value = 0;
		}
	}
	else if (PC8.BS_UE0 < PC8.BamSat.Nguong_L_DK_Ue_E)
	{
		if(Erob_Motor.Motor_GocTa.Motor_BS_Value > PC8.BamSat.BS_Fix_E0)
		{
			Erob_Motor.Motor_GocTa.Motor_BS_Value-= PC8.BamSat.BS_Fix_E0;
		}
		else
		{
			Erob_Motor.Motor_GocTa.Motor_BS_Value = 5999;
		}
	}
}

void _Sub_E_over_Center(void)
{
	if (PC8.BamSat.TL_Epxilon_final < 0)
	{
		if(Erob_Motor.Motor_GocTa.Motor_BS_Value > PC8.BamSat.BS_Fix_E)
		{
			Erob_Motor.Motor_GocTa.Motor_BS_Value-= PC8.BamSat.BS_Fix_E;
		}
		else
		{
			Erob_Motor.Motor_GocTa.Motor_BS_Value = 5999;
		}
	}

	else if (PC8.BamSat.TL_Epxilon_final > 0)
	{
		if(Erob_Motor.Motor_GocTa.Motor_BS_Value < (5999 - PC8.BamSat.BS_Fix_E))
		{
			Erob_Motor.Motor_GocTa.Motor_BS_Value += PC8.BamSat.BS_Fix_E;
		}
		else
		{
			Erob_Motor.Motor_GocTa.Motor_BS_Value = 0;
		}
	}
}

void _Sub_E_over_Limmiter(void)
{
	// khong che goc toi da
	if((Erob_Motor.Motor_GocTa.Motor_BS_Value > 1333)&&
	   (Erob_Motor.Motor_GocTa.Motor_BS_Value < 2900))
	{
		Erob_Motor.Motor_GocTa.Motor_BS_Value = 1333;
	}
	else if((Erob_Motor.Motor_GocTa.Motor_BS_Value < 5667)&&
			(Erob_Motor.Motor_GocTa.Motor_BS_Value > 3100))
	{
		Erob_Motor.Motor_GocTa.Motor_BS_Value = 5667;
	}
}

void _Sub_E_Mode_P0(void)
{
   if(((PC8.BamSat.Beam_Max_Val == _Beam17_Max)||
	   (PC8.BamSat.Beam_Max_Val == _Beam6_Max) ||
	   (PC8.BamSat.Beam_Max_Val == _Beam7_Max) ||
	   (PC8.BamSat.Beam_Max_Val == _Beam10_Max)||
	   (PC8.BamSat.Beam_Max_Val == _Beam11_Max))&&
	  ((PC8.S17E_Ue >= PC8.BamSat.Ue_Min_kenh_E)&&(PC8.S17E_Ue <= PC8.BamSat.Ue_Max_kenh_E)))
	{
		if(PC8.Click_E_time >= PC8.E_time)
		{
			PC8.Click_E_time = 0;

			//PC8.BamSat.Process_Epxilon_Ready = 1;
			PC8.BamSat._CS_E_Ready = 1;  // l dieu khien kenh H

			if(PC8.BamSat.Chieu_DK_Bam_E == 0)  // -- dang su dung
			{
				_Sub_E_Mode0();
			}
			else if(PC8.BamSat.Chieu_DK_Bam_E == 1)
			{
				_Sub_E_Mode1();
			}
		}
	}
	else
	{
		PC8.BamSat._CS_E_Ready = 0;  	// l dieu khien kenh H
		_Sub_E_over_Center();			// dieu khien khi goc E nam ngoai center
		_Sub_E_over_Limmiter(); 		// khong che goc dieu khien
	}
}

void _Sub_E_Mode_P1(void)
{
   if(((PC8.BamSat.Beam_Max_Val == _Beam17_Max)||
	   (PC8.BamSat.Beam_Max_Val == _SS2_Max)||
	   (PC8.BamSat.Beam_Max_Val == _SS3_Max)||
	   (PC8.BamSat.Beam_Max_Val == _Beam6_Max) ||
	   (PC8.BamSat.Beam_Max_Val == _Beam7_Max) ||
	   (PC8.BamSat.Beam_Max_Val == _Beam10_Max)||
	   (PC8.BamSat.Beam_Max_Val == _Beam11_Max))&&
	  ((PC8.S17E_Ue >= PC8.BamSat.Ue_Min_kenh_E)&&(PC8.S17E_Ue <= PC8.BamSat.Ue_Max_kenh_E)))
	{
		if(PC8.Click_E_time >= PC8.E_time)
		{
			PC8.Click_E_time = 0;

			//PC8.BamSat.Process_Epxilon_Ready = 1;
			PC8.BamSat._CS_E_Ready = 1;  // l dieu khien kenh E

			if(PC8.BamSat.Chieu_DK_Bam_E == 0)  // -- dang su dung
			{
				_Sub_E_Mode0();
			}
			else if(PC8.BamSat.Chieu_DK_Bam_E == 1)
			{
				_Sub_E_Mode1();
			}
		}
	}
	else
	{
		PC8.BamSat._CS_E_Ready = 0;  	// l dieu khien kenh H
		_Sub_E_over_Center();			// dieu khien khi goc E nam ngoai center
		_Sub_E_over_Limmiter(); 		// khong che goc dieu khien
	}
}

void _BS_Cal_New_E(void)
{
	PC8.Click_E_time++;

	if(PC8.BamSat.BS_Beam_mode == 0)
	{
		_Sub_E_Mode_P0();
	}
	else if(PC8.BamSat.BS_Beam_mode == 1)
	{
		_Sub_E_Mode_P1();
	}
}

void _Sub_BamSat_E_Speed(void)
{
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
		Erob_Motor.Motor_GocTa.Q0_Freq = 11444 / PC8.BamSat.Motor_Speed;// = 11444 / 2;
	}
	else if(Erob_Motor.Motor_GocTa.BS_Delta_Angle <= -400)
	{
		Erob_Motor.Motor_GocTa.Q0_Freq = 11444 / PC8.BamSat.Motor_Speed;// = 11444 / 2;
	}
	else if((Erob_Motor.Motor_GocTa.BS_Delta_Angle < 400)&&
			(Erob_Motor.Motor_GocTa.BS_Delta_Angle > -400))
	{
		if(Erob_Motor.Motor_GocTa.BS_Delta_Angle >= 150)
		{
			Erob_Motor.Motor_GocTa.Q0_Freq  = 17166 / PC8.BamSat.Scan_Speed;// = 9000;
		}
		else if(Erob_Motor.Motor_GocTa.BS_Delta_Angle <= -150)
		{
			Erob_Motor.Motor_GocTa.Q0_Freq = 17166 / PC8.BamSat.Scan_Speed;// = 9000;
		}
		else if((Erob_Motor.Motor_GocTa.BS_Delta_Angle < 150)&&
				(Erob_Motor.Motor_GocTa.BS_Delta_Angle > -150))
		{
			if(Erob_Motor.Motor_GocTa.BS_Delta_Angle >= 33)
			{
				Erob_Motor.Motor_GocTa.Q0_Freq  = 17166 / PC8.BamSat.Bamsat_Speed;// = 9000;
			}
			else if(Erob_Motor.Motor_GocTa.BS_Delta_Angle <= -33)
			{
				Erob_Motor.Motor_GocTa.Q0_Freq = 17166 / PC8.BamSat.Bamsat_Speed;// = 9000;
			}
			else if((Erob_Motor.Motor_GocTa.BS_Delta_Angle < 33)&&
					(Erob_Motor.Motor_GocTa.BS_Delta_Angle > -33))
			{
				//Erob_Motor.Motor_GocTa.Q0_Freq = 28610 / PC8.BamSat.Bamsat_Speed;// = 11444;//(11444 * 3) / 2;
				if(Erob_Motor.Motor_GocTa.BS_Delta_Angle >= 16)
				{
					Erob_Motor.Motor_GocTa.Q0_Freq  = 17166 / PC8.BamSat.Bamsat_Speed;// = 9000;
				}
				else if(Erob_Motor.Motor_GocTa.BS_Delta_Angle <= -16)
				{
					Erob_Motor.Motor_GocTa.Q0_Freq = 17166 / PC8.BamSat.Bamsat_Speed;// = 9000;
				}
				else if((Erob_Motor.Motor_GocTa.BS_Delta_Angle < 16)&&
						(Erob_Motor.Motor_GocTa.BS_Delta_Angle > -16))
				{
					Erob_Motor.Motor_GocTa.Q0_Freq = 17166 / PC8.BamSat.Bamsat_Speed;// = 11444;//(11444 * 3) / 2;
				}
			}
		}
	}
}

void _Sub_BamSat_E_Control_Motor(void)
{
	// dieu khien dong co 2
	if(Erob_Motor.Motor_GocTa.BS_Delta_Angle >= Erob_Motor.Motor_PhuongVi.BS_Max)
	{
		Erob_Motor.Motor_GocTa.Q0_DIR = 1; // 0
		Erob_Motor.Motor_GocTa.Q0_Enable = 1;

		PC8.BamSat._Delta_Epxilon_Enable = 1;
	}
	else if(Erob_Motor.Motor_GocTa.BS_Delta_Angle <= Erob_Motor.Motor_PhuongVi.BS_Min)
	{

		Erob_Motor.Motor_GocTa.Q0_DIR = 0; // 1
		Erob_Motor.Motor_GocTa.Q0_Enable = 1;
		PC8.BamSat._Delta_Epxilon_Enable = 1;
	}
	else
	{
		Erob_Motor.Motor_GocTa.Q0_DIR = 1;
		Erob_Motor.Motor_GocTa.Q0_Enable = 0;

		PC8.BamSat._Delta_Epxilon_Enable = 0;

		// da bam sat xong goc ta

		PC8.BamSat.Process_Epxilon_start = 0;
		PC8.BamSat.Process_BS_done = 1;
	}

	UDP_Frame.Goc_ta_Dir = Erob_Motor.Motor_GocTa.Q0_DIR;	// lenh can thuc thi 2

	Erob_Motor_Control(	Erob_Motor.Motor_GocTa.Q0_Freq,
						Erob_Motor.Motor_GocTa.Q0_DIR,
						Erob_Motor.Motor_GocTa.Q0_Enable,
						Radar_Motor_GocTa);
}

void Erob_Control_Bamsat_PC8_E(void)
{
	if(Erob_Motor.Motor_GocTa.Erob_Enable == 1) // quet quat
	{

		Erob_Motor.Motor_GocTa.BS_Delta_Angle =  (Erob_Motor.Motor_GocTa.BS_Angle -
												  Erob_Motor.Motor_GocTa.Motor_BS_Value);//

		_Sub_BamSat_E_Speed();
		_Sub_BamSat_E_Control_Motor();
	}
	else if(Erob_Motor.Motor_GocTa.Erob_Enable == 0) // quet quat
	{
		Erob_Motor.Motor_GocTa.Q0_DIR 	 = 1;
		Erob_Motor.Motor_GocTa.Q0_Enable = 0;

		Erob_Motor_Control(	Erob_Motor.Motor_GocTa.Erob_Freq,
							Erob_Motor.Motor_GocTa.Erob_DIR,
							Erob_Motor.Motor_GocTa.Erob_Enable,
							Radar_Motor_GocTa);
	}
}

