#include "Erob.h"
#include "MI_505.h"
#include "DK_Maythu.h"
#include "CAN_Control.h"
#define _GNU_SOURCE
#include <math.h>

void _Sub_H_Mode0(void)
{
	if (PC8.BS_UH0 > PC8.BamSat.Nguong_H_DK_Ue_H)
	{
		if(Erob_Motor.Motor_PhuongVi.Motor_BS_Value < (5999 - PC8.BamSat.BS_Fix_H0))
		{
			Erob_Motor.Motor_PhuongVi.Motor_BS_Value += PC8.BamSat.BS_Fix_H0;
		}
		else
		{
			Erob_Motor.Motor_PhuongVi.Motor_BS_Value = 0;
		}
	}
	else if (PC8.BS_UH0 < PC8.BamSat.Nguong_L_DK_Ue_H)
	{
		if(Erob_Motor.Motor_PhuongVi.Motor_BS_Value > PC8.BamSat.BS_Fix_H0)
		{
			Erob_Motor.Motor_PhuongVi.Motor_BS_Value -= PC8.BamSat.BS_Fix_H0;
		}
		else
		{
			Erob_Motor.Motor_PhuongVi.Motor_BS_Value = 5999;
		}
	}
}

void _Sub_H_Mode1(void)
{
	if (PC8.BS_UH0 > PC8.BamSat.Nguong_H_DK_Ue_H)
	{
		if(Erob_Motor.Motor_PhuongVi.Motor_BS_Value > PC8.BamSat.BS_Fix_H0)
		{
			Erob_Motor.Motor_PhuongVi.Motor_BS_Value-= PC8.BamSat.BS_Fix_H0;
		}
		else
		{
			Erob_Motor.Motor_PhuongVi.Motor_BS_Value = 5999;
		}
	}
	else if (PC8.BS_UH0 < PC8.BamSat.Nguong_L_DK_Ue_H)
	{
		if(Erob_Motor.Motor_PhuongVi.Motor_BS_Value < (5999 - PC8.BamSat.BS_Fix_H0))
		{
			Erob_Motor.Motor_PhuongVi.Motor_BS_Value+= PC8.BamSat.BS_Fix_H0;
		}
		else
		{
			Erob_Motor.Motor_PhuongVi.Motor_BS_Value = 0;
		}
	}
}

void _Sub_H_control_EH_Ready(void)
{
	if((PC8.BamSat.Beam_Max_Val == _Beam2_Max)||
	   (PC8.BamSat.Beam_Max_Val == _Beam3_Max)||
	   (PC8.BamSat.Beam_Max_Val == _Beam6_Max)||
	   (PC8.BamSat.Beam_Max_Val == _Beam7_Max)||
	   (PC8.BamSat.Beam_Max_Val == _Beam10_Max)||
	   (PC8.BamSat.Beam_Max_Val == _Beam11_Max)||
	   (PC8.BamSat.Beam_Max_Val == _Beam14_Max)||
	   (PC8.BamSat.Beam_Max_Val == _Beam15_Max))
		{
			PC8.BamSat.Process_Epxilon_Ready = 1; // cho phep dieu khien kenh E
			PC8.BamSat._CS_E_Ready = 1;  // l dieu khien kenh E
			PC8.BamSat._CS_H_Ready = 1;  // l dieu khien kenh H
		}
	else
		{
			PC8.BamSat.Process_Epxilon_Ready = 0; // khong dieu khien kenh E
			PC8.BamSat._CS_E_Ready = 0;  // l dieu khien kenh H
			PC8.BamSat._CS_H_Ready = 0;  // l dieu khien kenh H
		}

}

void _Sub_H_control_EH_Ready1(void)
{
	if((PC8.BamSat.Beam_Max_Val == _Beam17_Max)||
	   (PC8.BamSat.Beam_Max_Val == _SS6_Max)||
	   (PC8.BamSat.Beam_Max_Val == _SS1_Max)||
	   (PC8.BamSat.Beam_Max_Val == _SS7_Max)||
	   (PC8.BamSat.Beam_Max_Val == _SS2_Max)||
	   (PC8.BamSat.Beam_Max_Val == _SS3_Max)||
	   (PC8.BamSat.Beam_Max_Val == _SS8_Max)||
	   (PC8.BamSat.Beam_Max_Val == _SS4_Max)||
	   (PC8.BamSat.Beam_Max_Val == _SS9_Max))
		{
			PC8.BamSat.Process_Epxilon_Ready = 1; // cho phep dieu khien kenh E
			PC8.BamSat._CS_E_Ready = 1;  // l dieu khien kenh E
			PC8.BamSat._CS_H_Ready = 1;  // l dieu khien kenh H
		}
	else
		{
			PC8.BamSat.Process_Epxilon_Ready = 0; // khong dieu khien kenh E
			PC8.BamSat._CS_E_Ready = 0;  // l dieu khien kenh H
			PC8.BamSat._CS_H_Ready = 0;  // l dieu khien kenh H
		}

}

void _Sub_H_over_Center(void)
{
	if (PC8.BamSat.TL_Phi_final > 0)
	{
		if(Erob_Motor.Motor_PhuongVi.Motor_BS_Value > PC8.BamSat.BS_Fix_H)
		{
			Erob_Motor.Motor_PhuongVi.Motor_BS_Value -= PC8.BamSat.BS_Fix_H; // -0.25 do
		}
		else
		{
			Erob_Motor.Motor_PhuongVi.Motor_BS_Value = 5999;
		}
	}
	else if (PC8.BamSat.TL_Phi_final < 0)
	{
		if(Erob_Motor.Motor_PhuongVi.Motor_BS_Value < (5999 - PC8.BamSat.BS_Fix_H))
		{
			Erob_Motor.Motor_PhuongVi.Motor_BS_Value += PC8.BamSat.BS_Fix_H; // +0.25 do
		}
		else
		{
			Erob_Motor.Motor_PhuongVi.Motor_BS_Value = 0;
		}
	}
}

void _Sub_H_Mode_P0(void)
{
   if(((PC8.BamSat.Beam_Max_Val == _Beam17_Max)||
	   (PC8.BamSat.Beam_Max_Val == _Beam6_Max) ||
	   (PC8.BamSat.Beam_Max_Val == _Beam7_Max) ||
	   (PC8.BamSat.Beam_Max_Val == _Beam10_Max)||
	   (PC8.BamSat.Beam_Max_Val == _Beam11_Max))&&
	  ((PC8.S17H_Ue >= PC8.BamSat.Ue_Min_kenh_H)&&(PC8.S17H_Ue <= PC8.BamSat.Ue_Max_kenh_H)))
	{
		// bam theo dac tuyen
		if (PC8.Click_H_time >= PC8.H_time)
		{
			PC8.Click_H_time = 0;

			PC8.BamSat.Process_Epxilon_Ready = 1; // cho phep dieu khien kenh E
			PC8.BamSat._CS_H_Ready = 1;  // l dieu khien kenh H

			if(PC8.BamSat.Chieu_DK_Bam_H == 0) // -- dang su dung
			{
				_Sub_H_Mode0();
			}
			else if(PC8.BamSat.Chieu_DK_Bam_H == 1)
			{
				_Sub_H_Mode1();
			}
		}
	}
	else
	{
		_Sub_H_control_EH_Ready(); 	// cap quyen quay goc E va H
		_Sub_H_over_Center();		// dieu khien khi nam ngoai center
	}
}

void _Sub_H_Mode_P1(void)
{
   if(((PC8.BamSat.Beam_Max_Val == _Beam17_Max)||
	   (PC8.BamSat.Beam_Max_Val == _Beam6_Max) ||
	   (PC8.BamSat.Beam_Max_Val == _Beam7_Max) ||
	   (PC8.BamSat.Beam_Max_Val == _Beam10_Max)||
	   (PC8.BamSat.Beam_Max_Val == _Beam11_Max)||
	   (PC8.BamSat.Beam_Max_Val == _SS2_Max)||
	   (PC8.BamSat.Beam_Max_Val == _SS3_Max)||
	   (PC8.BamSat.Beam_Max_Val == _V1_Max)||
	   (PC8.BamSat.Beam_Max_Val == _V2_Max))&&
	  ((PC8.S17H_Ue >= PC8.BamSat.Ue_Min_kenh_H)&&(PC8.S17H_Ue <= PC8.BamSat.Ue_Max_kenh_H)))
	{
		// bam theo dac tuyen
		if (PC8.Click_H_time >= PC8.H_time)
		{
			PC8.Click_H_time = 0;

			PC8.BamSat.Process_Epxilon_Ready = 1; // cho phep dieu khien kenh E

			PC8.BamSat._CS_H_Ready = 1;  // l dieu khien kenh H

			if(PC8.BamSat.Chieu_DK_Bam_H == 0) // -- dang su dung
			{
				_Sub_H_Mode0();
			}
			else if(PC8.BamSat.Chieu_DK_Bam_H == 1)
			{
				_Sub_H_Mode1();
			}
		}
	}
	else
	{
		_Sub_H_control_EH_Ready1(); 	// cap quyen quay goc E va H
		_Sub_H_over_Center();		// dieu khien khi nam ngoai center
	}
}

void _BS_Cal_New_H(void)
{
	PC8.Click_H_time++;

	if(PC8.BamSat.BS_Beam_mode == 0)
	{
		_Sub_H_Mode_P0();
	}
	else if(PC8.BamSat.BS_Beam_mode == 1)
	{
		_Sub_H_Mode_P1();
	}

}

void _Sub_BamSat_H_Speed(void)
{
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
	if(Erob_Motor.Motor_PhuongVi.BS_Delta_Angle >= 100)
	{
		Erob_Motor.Motor_PhuongVi.Q0_Freq = 11444 / PC8.BamSat.Motor_Speed;
	}
	else if(Erob_Motor.Motor_PhuongVi.BS_Delta_Angle <= -100)
	{
		Erob_Motor.Motor_PhuongVi.Q0_Freq = 11444 / PC8.BamSat.Motor_Speed;
	}
	else if((Erob_Motor.Motor_PhuongVi.BS_Delta_Angle < 100)&&
			(Erob_Motor.Motor_PhuongVi.BS_Delta_Angle > -100))
	{
		if(Erob_Motor.Motor_PhuongVi.BS_Delta_Angle >= 66)
		{
			Erob_Motor.Motor_PhuongVi.Q0_Freq = 17166 / PC8.BamSat.Scan_Speed;
		}
		else if(Erob_Motor.Motor_PhuongVi.BS_Delta_Angle <= -66)
		{
			Erob_Motor.Motor_PhuongVi.Q0_Freq = 17166 / PC8.BamSat.Scan_Speed;
		}
		else if((Erob_Motor.Motor_PhuongVi.BS_Delta_Angle < 66)&&
				(Erob_Motor.Motor_PhuongVi.BS_Delta_Angle > -66))
		{
			if(Erob_Motor.Motor_PhuongVi.BS_Delta_Angle >= 33)
			{
				Erob_Motor.Motor_PhuongVi.Q0_Freq  = 17166 / PC8.BamSat.Bamsat_Speed;// = 9000;
			}
			else if(Erob_Motor.Motor_PhuongVi.BS_Delta_Angle <= -33)
			{
				Erob_Motor.Motor_PhuongVi.Q0_Freq = 17166 / PC8.BamSat.Bamsat_Speed;// = 9000;
			}
			else if((Erob_Motor.Motor_PhuongVi.BS_Delta_Angle < 33)&&
					(Erob_Motor.Motor_PhuongVi.BS_Delta_Angle > -33))
			{
				//Erob_Motor.Motor_PhuongVi.Q0_Freq = 28610 / PC8.BamSat.Bamsat_Speed;// = 11444;//(11444 * 3) / 2;
				if(Erob_Motor.Motor_PhuongVi.BS_Delta_Angle >= 16)
				{
					Erob_Motor.Motor_PhuongVi.Q0_Freq  = 20000 / PC8.BamSat.Bamsat_Speed;// = 9000;
				}
				else if(Erob_Motor.Motor_PhuongVi.BS_Delta_Angle <= -16)
				{
					Erob_Motor.Motor_PhuongVi.Q0_Freq = 20000 / PC8.BamSat.Bamsat_Speed;// = 9000;
				}
				else if((Erob_Motor.Motor_PhuongVi.BS_Delta_Angle < 16)&&
						(Erob_Motor.Motor_PhuongVi.BS_Delta_Angle > -16))
				{
					Erob_Motor.Motor_PhuongVi.Q0_Freq = 20000 / PC8.BamSat.Bamsat_Speed;// = 11444;//(11444 * 3) / 2;
				}

			}
		}
	}

}

void _Sub_BamSat_H_Control_Motor(void)
{
	// dieu khien dong co 2
	if(Erob_Motor.Motor_PhuongVi.BS_Delta_Angle >= Erob_Motor.Motor_PhuongVi.BS_Max)
	{
		Erob_Motor.Motor_PhuongVi.Q0_DIR 	= 0; // quay lui
		Erob_Motor.Motor_PhuongVi.Q0_Enable = 1;
		PC8.BamSat._Delta_Phi_Enable 		= 1;
	}
	else if(Erob_Motor.Motor_PhuongVi.BS_Delta_Angle <= Erob_Motor.Motor_PhuongVi.BS_Min)
	{
		Erob_Motor.Motor_PhuongVi.Q0_DIR 	= 1; // quay tien
		Erob_Motor.Motor_PhuongVi.Q0_Enable = 1;
		PC8.BamSat._Delta_Phi_Enable 		= 1;
	}
	else if((Erob_Motor.Motor_PhuongVi.BS_Delta_Angle < Erob_Motor.Motor_PhuongVi.BS_Max)&&
			(Erob_Motor.Motor_PhuongVi.BS_Delta_Angle > Erob_Motor.Motor_PhuongVi.BS_Min))
	{
		Erob_Motor.Motor_PhuongVi.Q0_DIR 	= 1;
		Erob_Motor.Motor_PhuongVi.Q0_Enable = 0;
		PC8.BamSat._Delta_Phi_Enable 		= 0;

		// da bam sat xong phuong vi
		if(PC8.BS_Scan_Start == 1)
		{
			PC8.BamSat.Process_Phi_start 	 = 0; //
			PC8.BamSat.Process_Epxilon_start = 0; //
			PC8.BamSat.Process_BS_done 		 = 1; //

			PC8.BS_Scan_Start 	= 0;
			PC8._KS_Pvi.Loading = 0; // khong luu dac tuyen

		}
		else if(PC8.BS_Scan_Start == 0)
		{
			PC8.BamSat.Process_Phi_start = 0;

			if(PC8.BamSat.Process_Epxilon_Ready == 1) // co lenh cho phep dieu khien E
			{
				PC8.BamSat.Process_Epxilon_start = 1;
				PC8.BamSat.Process_BS_done = 0;
			}
			else // dieu khien tiep kenh H
			{
				PC8.BamSat.Process_Epxilon_start = 0;
				PC8.BamSat.Process_BS_done = 1;
			}
		}
	}
}

void _Sub_BamSat_H_Control_Fix_Over_Point(void)
{
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

void Erob_Control_Bamsat_PC8_H(void)
{
	if(Erob_Motor.Motor_PhuongVi.Erob_Enable == 1)
	{
		// tinh toan delta
		Erob_Motor.Motor_PhuongVi.BS_Delta_Angle0 =  (Erob_Motor.Motor_PhuongVi.BS_Angle -
													  Erob_Motor.Motor_PhuongVi.Motor_BS_Value);//

		Erob_Motor.Motor_PhuongVi.Check_time++;

		_Sub_BamSat_H_Speed(); 					// cai dat van toc theo delta encoder
		_Sub_BamSat_H_Control_Motor();			// cai dat bam sat dong co kenh H
		_Sub_BamSat_H_Control_Fix_Over_Point();	// xu ly khi encoder di den diem 3000

	}
	else if(Erob_Motor.Motor_PhuongVi.Erob_Enable == 0) // tat dong co
	{
		Erob_Motor_Control(	Erob_Motor.Motor_PhuongVi.Erob_Freq,
							Erob_Motor.Motor_PhuongVi.Erob_DIR,
							Erob_Motor.Motor_PhuongVi.Erob_Enable,
							Radar_Motor_PhuongVi);
	}
}
