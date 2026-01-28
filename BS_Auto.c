#include "Erob.h"
#include "MI_505.h"
#include "DK_Maythu.h"
#include "CAN_Control.h"
#include <math.h>

void _BS_Work_Off(void)
{
	if(Erob_Motor.Motor_PhuongVi.Erob_Enable == 1) // quet quat
	{
		if(Erob_Motor.Motor_PhuongVi.Erob_Mode == 0) Erob_Control_Motor1_Rotate(); 	// quay tron
		else if(Erob_Motor.Motor_PhuongVi.Erob_Mode == 1) Erob_Control_Motor1();	// quet quat
		else if(Erob_Motor.Motor_PhuongVi.Erob_Mode == 2) Erob_Control_Bamsat1();	// Q0
	}
	else // dung quay
	{
		Erob_Motor_Control(	Erob_Motor.Motor_PhuongVi.Quat_Freq,
							Erob_Motor.Motor_PhuongVi.Motor_DIR,
							0,
							Radar_Motor_PhuongVi);
	}

	if(Erob_Motor.Motor_GocTa.Erob_Enable == 1)
	{
		if(Erob_Motor.Motor_GocTa.Erob_Mode == 0) Erob_Control_Bamsat2(); 		// Q0
		else if(Erob_Motor.Motor_GocTa.Erob_Mode == 1) Erob_Control_Motor2(); 	// quet quat
	}
	else // dung quay
	{
		Erob_Motor_Control(	Erob_Motor.Motor_GocTa.Quat_Freq,
							Erob_Motor.Motor_GocTa.Motor_DIR,
							0,
							Radar_Motor_GocTa);
	}
}

void _BS_First_Process(double Fix_Angle)
{
	PC8.BS_Angle_Hb0 = (int16_t)(PC8.BS_Angle_Hz0 + Fix_Angle / 0.06);

	if(PC8.BS_Angle_Hb0 < 0)
	{
		PC8.BS_Angle_H0 = PC8.BS_Angle_Hb0 + 6000;
	}
	else if(PC8.BS_Angle_Hb0 >= 6000)
	{
		PC8.BS_Angle_H0 = PC8.BS_Angle_Hb0 - 6000;
	}
	else PC8.BS_Angle_H0 = (uint16_t)PC8.BS_Angle_Hb0;


	Erob_Motor.Motor_PhuongVi.Motor_BS_Value = PC8.BS_Angle_H0;

	PC8.BamSat.Process_Phi_start 		= 1;// chuyen qua ban sat phuong vi
	PC8.BamSat.Process_Epxilon_start 	= 0;

	PC8.BS_AngleH_AVG[0] = 0.0; // don vi: ly giac
	PC8.BS_AngleH_AVG[1] = 0.0; // don vi: ly giac
	PC8.BS_AngleH_AVG[2] = 0.0; // don vi: ly giac
	PC8.BS_AngleH_AVG[3] = 0.0; // don vi: ly giac
	PC8.BS_AngleH_AVG[4] = 0.0; // don vi: ly giac
	PC8.BS_AngleH_AVG[5] = 0.0; // don vi: ly giac
	PC8.BS_AngleH_AVG[6] = 0.0; // don vi: ly giac
	PC8.BS_AngleH_AVG[7] = 0.0; // don vi: ly giac

	PC8.BS_AngleE_AVG[0] = 0.0; // don vi: ly giac
	PC8.BS_AngleE_AVG[1] = 0.0; // don vi: ly giac
	PC8.BS_AngleE_AVG[2] = 0.0; // don vi: ly giac
	PC8.BS_AngleE_AVG[3] = 0.0; // don vi: ly giac
	PC8.BS_AngleE_AVG[4] = 0.0; // don vi: ly giac
	PC8.BS_AngleE_AVG[5] = 0.0; // don vi: ly giac
	PC8.BS_AngleE_AVG[6] = 0.0; // don vi: ly giac
	PC8.BS_AngleE_AVG[7] = 0.0; // don vi: ly giac

	PC8.BS_AngleH_PID	= 0.0; // don vi: ly giac
	PC8.BS_AngleE_PID	= 0.0; // don vi: ly giac

	PC8.Adjust_E_Enable = 0; // 38
	PC8.Adjust_H_Enable = 0; // 38
	PC8.step_counter2 	= 0;
	PC8.TG_time 		= 0;
	PC8.TG_Stop			= 0;
}

void _BS_Second_AVG(void)
{
	PC8.BS_AngleH_AVG[31]= PC8.BS_AngleH_AVG[30]; // don vi: ly giac
	PC8.BS_AngleH_AVG[30]= PC8.BS_AngleH_AVG[29]; // don vi: ly giac
	PC8.BS_AngleH_AVG[29]= PC8.BS_AngleH_AVG[28]; // don vi: ly giac
	PC8.BS_AngleH_AVG[28]= PC8.BS_AngleH_AVG[27]; // don vi: ly giac
	PC8.BS_AngleH_AVG[27]= PC8.BS_AngleH_AVG[26]; // don vi: ly giac
	PC8.BS_AngleH_AVG[26]= PC8.BS_AngleH_AVG[25]; // don vi: ly giac
	PC8.BS_AngleH_AVG[25]= PC8.BS_AngleH_AVG[24]; // don vi: ly giac
	PC8.BS_AngleH_AVG[24]= PC8.BS_AngleH_AVG[23]; // don vi: ly giac
	PC8.BS_AngleH_AVG[23]= PC8.BS_AngleH_AVG[22]; // don vi: ly giac
	PC8.BS_AngleH_AVG[22]= PC8.BS_AngleH_AVG[21]; // don vi: ly giac
	PC8.BS_AngleH_AVG[21]= PC8.BS_AngleH_AVG[20]; // don vi: ly giac
	PC8.BS_AngleH_AVG[20]= PC8.BS_AngleH_AVG[19]; // don vi: ly giac
	PC8.BS_AngleH_AVG[19]= PC8.BS_AngleH_AVG[18]; // don vi: ly giac
	PC8.BS_AngleH_AVG[18]= PC8.BS_AngleH_AVG[17]; // don vi: ly giac
	PC8.BS_AngleH_AVG[17]= PC8.BS_AngleH_AVG[16]; // don vi: ly giac
	PC8.BS_AngleH_AVG[16]= PC8.BS_AngleH_AVG[15]; // don vi: ly giac
	PC8.BS_AngleH_AVG[15]= PC8.BS_AngleH_AVG[14]; // don vi: ly giac
	PC8.BS_AngleH_AVG[14]= PC8.BS_AngleH_AVG[13]; // don vi: ly giac
	PC8.BS_AngleH_AVG[13]= PC8.BS_AngleH_AVG[12]; // don vi: ly giac
	PC8.BS_AngleH_AVG[12]= PC8.BS_AngleH_AVG[11]; // don vi: ly giac
	PC8.BS_AngleH_AVG[11]= PC8.BS_AngleH_AVG[10]; // don vi: ly giac
	PC8.BS_AngleH_AVG[10]= PC8.BS_AngleH_AVG[9]; // don vi: ly giac
	PC8.BS_AngleH_AVG[9] = PC8.BS_AngleH_AVG[8]; // don vi: ly giac
	PC8.BS_AngleH_AVG[8] = PC8.BS_AngleH_AVG[7]; // don vi: ly giac
	PC8.BS_AngleH_AVG[7] = PC8.BS_AngleH_AVG[6]; // don vi: ly giac
	PC8.BS_AngleH_AVG[6] = PC8.BS_AngleH_AVG[5]; // don vi: ly giac
	PC8.BS_AngleH_AVG[5] = PC8.BS_AngleH_AVG[4]; // don vi: ly giac
	PC8.BS_AngleH_AVG[4] = PC8.BS_AngleH_AVG[3]; // don vi: ly giac
	PC8.BS_AngleH_AVG[3] = PC8.BS_AngleH_AVG[2]; // don vi: ly giac
	PC8.BS_AngleH_AVG[2] = PC8.BS_AngleH_AVG[1]; // don vi: ly giac
	PC8.BS_AngleH_AVG[1] = PC8.BS_AngleH_AVG[0]; // don vi: ly giac
	PC8.BS_AngleH_AVG[0] = PC8.BamSat.TL_Phi_final / 0.06; // don vi: ly giac

	PC8.BS_AngleE_AVG[31]= PC8.BS_AngleE_AVG[30]; // don vi: ly giac
	PC8.BS_AngleE_AVG[30]= PC8.BS_AngleE_AVG[29]; // don vi: ly giac
	PC8.BS_AngleE_AVG[29]= PC8.BS_AngleE_AVG[28]; // don vi: ly giac
	PC8.BS_AngleE_AVG[28]= PC8.BS_AngleE_AVG[27]; // don vi: ly giac
	PC8.BS_AngleE_AVG[27]= PC8.BS_AngleE_AVG[26]; // don vi: ly giac
	PC8.BS_AngleE_AVG[26]= PC8.BS_AngleE_AVG[25]; // don vi: ly giac
	PC8.BS_AngleE_AVG[25]= PC8.BS_AngleE_AVG[24]; // don vi: ly giac
	PC8.BS_AngleE_AVG[24]= PC8.BS_AngleE_AVG[23]; // don vi: ly giac
	PC8.BS_AngleE_AVG[23]= PC8.BS_AngleE_AVG[22]; // don vi: ly giac
	PC8.BS_AngleE_AVG[22]= PC8.BS_AngleE_AVG[21]; // don vi: ly giac
	PC8.BS_AngleE_AVG[21]= PC8.BS_AngleE_AVG[20]; // don vi: ly giac
	PC8.BS_AngleE_AVG[20]= PC8.BS_AngleE_AVG[19]; // don vi: ly giac
	PC8.BS_AngleE_AVG[19]= PC8.BS_AngleE_AVG[18]; // don vi: ly giac
	PC8.BS_AngleE_AVG[18]= PC8.BS_AngleE_AVG[17]; // don vi: ly giac
	PC8.BS_AngleE_AVG[17]= PC8.BS_AngleE_AVG[16]; // don vi: ly giac
	PC8.BS_AngleE_AVG[16]= PC8.BS_AngleE_AVG[15]; // don vi: ly giac
	PC8.BS_AngleE_AVG[15]= PC8.BS_AngleE_AVG[14]; // don vi: ly giac
	PC8.BS_AngleE_AVG[14]= PC8.BS_AngleE_AVG[13]; // don vi: ly giac
	PC8.BS_AngleE_AVG[13]= PC8.BS_AngleE_AVG[12]; // don vi: ly giac
	PC8.BS_AngleE_AVG[12]= PC8.BS_AngleE_AVG[11]; // don vi: ly giac
	PC8.BS_AngleE_AVG[11]= PC8.BS_AngleE_AVG[10]; // don vi: ly giac
	PC8.BS_AngleE_AVG[10]= PC8.BS_AngleE_AVG[9]; // don vi: ly giac
	PC8.BS_AngleE_AVG[9] = PC8.BS_AngleE_AVG[8]; // don vi: ly giac
	PC8.BS_AngleE_AVG[8] = PC8.BS_AngleH_AVG[7]; // don vi: ly giac
	PC8.BS_AngleE_AVG[7] = PC8.BS_AngleE_AVG[6]; // don vi: ly giac
	PC8.BS_AngleE_AVG[6] = PC8.BS_AngleE_AVG[5]; // don vi: ly giac
	PC8.BS_AngleE_AVG[5] = PC8.BS_AngleE_AVG[4]; // don vi: ly giac
	PC8.BS_AngleE_AVG[4] = PC8.BS_AngleE_AVG[3]; // don vi: ly giac
	PC8.BS_AngleE_AVG[3] = PC8.BS_AngleE_AVG[2]; // don vi: ly giac
	PC8.BS_AngleE_AVG[2] = PC8.BS_AngleE_AVG[1]; // don vi: ly giac
	PC8.BS_AngleE_AVG[1] = PC8.BS_AngleE_AVG[0]; // don vi: ly giac
	PC8.BS_AngleE_AVG[0] = PC8.BamSat.TL_Epxilon_final / 0.06; // don vi: ly giac

}

void _BS_Second_Adjust_Mode(void)
{
	if(PC8.Adjust_Mode == 0)
	{
		PC8.BS_AngleH_Kalman = (PC8.BS_AngleH_AVG[7] + // don vi: ly giac
								PC8.BS_AngleH_AVG[6] + // don vi: ly giac
								PC8.BS_AngleH_AVG[5] + // don vi: ly giac
								PC8.BS_AngleH_AVG[4] + // don vi: ly giac
								PC8.BS_AngleH_AVG[3] + // don vi: ly giac
								PC8.BS_AngleH_AVG[2] + // don vi: ly giac
								PC8.BS_AngleH_AVG[1] + // don vi: ly giac
								PC8.BS_AngleH_AVG[0]) / 8.0; // don vi: ly giac; // don vi: ly giac

		PC8.BS_AngleE_Kalman = (PC8.BS_AngleE_AVG[7] + // don vi: ly giac
								PC8.BS_AngleE_AVG[6] + // don vi: ly giac
								PC8.BS_AngleE_AVG[5] + // don vi: ly giac
								PC8.BS_AngleE_AVG[4] + // don vi: ly giac
								PC8.BS_AngleE_AVG[3] + // don vi: ly giac
								PC8.BS_AngleE_AVG[2] + // don vi: ly giac
								PC8.BS_AngleE_AVG[1] + // don vi: ly giac
								PC8.BS_AngleE_AVG[0]) / 8.0; // don vi: ly giac; // don vi: ly giac
	}
	else if(PC8.Adjust_Mode == 1)
	{
		PC8.BS_AngleH_Kalman = (PC8.BS_AngleH_AVG[31] + // don vi: ly giac
								PC8.BS_AngleH_AVG[30] + // don vi: ly giac
								PC8.BS_AngleH_AVG[29] + // don vi: ly giac
								PC8.BS_AngleH_AVG[28] + // don vi: ly giac
								PC8.BS_AngleH_AVG[27] + // don vi: ly giac
								PC8.BS_AngleH_AVG[26] + // don vi: ly giac
								PC8.BS_AngleH_AVG[25] + // don vi: ly giac
								PC8.BS_AngleH_AVG[24] + // don vi: ly giac
								PC8.BS_AngleH_AVG[23] + // don vi: ly giac
								PC8.BS_AngleH_AVG[22] + // don vi: ly giac
								PC8.BS_AngleH_AVG[21] + // don vi: ly giac
								PC8.BS_AngleH_AVG[20] + // don vi: ly giac
								PC8.BS_AngleH_AVG[19] + // don vi: ly giac
								PC8.BS_AngleH_AVG[18] + // don vi: ly giac
								PC8.BS_AngleH_AVG[17] + // don vi: ly giac
								PC8.BS_AngleH_AVG[16] + // don vi: ly giac
								PC8.BS_AngleH_AVG[15] + // don vi: ly giac
								PC8.BS_AngleH_AVG[14] + // don vi: ly giac
								PC8.BS_AngleH_AVG[13] + // don vi: ly giac
								PC8.BS_AngleH_AVG[12] + // don vi: ly giac
								PC8.BS_AngleH_AVG[11] + // don vi: ly giac
								PC8.BS_AngleH_AVG[10] + // don vi: ly giac
								PC8.BS_AngleH_AVG[9] + // don vi: ly giac
								PC8.BS_AngleH_AVG[8] + // don vi: ly giac
								PC8.BS_AngleH_AVG[7] + // don vi: ly giac
								PC8.BS_AngleH_AVG[6] + // don vi: ly giac
								PC8.BS_AngleH_AVG[5] + // don vi: ly giac
								PC8.BS_AngleH_AVG[4] + // don vi: ly giac
								PC8.BS_AngleH_AVG[3] + // don vi: ly giac
								PC8.BS_AngleH_AVG[2] + // don vi: ly giac
								PC8.BS_AngleH_AVG[1] + // don vi: ly giac
								PC8.BS_AngleH_AVG[0]) / 32.0; // don vi: ly giac; // don vi: ly giac

		PC8.BS_AngleE_Kalman = (PC8.BS_AngleE_AVG[31] + // don vi: ly giac
								PC8.BS_AngleE_AVG[30] + // don vi: ly giac
								PC8.BS_AngleE_AVG[29] + // don vi: ly giac
								PC8.BS_AngleE_AVG[28] + // don vi: ly giac
								PC8.BS_AngleE_AVG[27] + // don vi: ly giac
								PC8.BS_AngleE_AVG[26] + // don vi: ly giac
								PC8.BS_AngleE_AVG[25] + // don vi: ly giac
								PC8.BS_AngleE_AVG[24] + // don vi: ly giac
								PC8.BS_AngleE_AVG[23] + // don vi: ly giac
								PC8.BS_AngleE_AVG[22] + // don vi: ly giac
								PC8.BS_AngleE_AVG[21] + // don vi: ly giac
								PC8.BS_AngleE_AVG[20] + // don vi: ly giac
								PC8.BS_AngleE_AVG[19] + // don vi: ly giac
								PC8.BS_AngleE_AVG[18] + // don vi: ly giac
								PC8.BS_AngleE_AVG[17] + // don vi: ly giac
								PC8.BS_AngleE_AVG[16] + // don vi: ly giac
								PC8.BS_AngleE_AVG[15] + // don vi: ly giac
								PC8.BS_AngleE_AVG[14] + // don vi: ly giac
								PC8.BS_AngleE_AVG[13] + // don vi: ly giac
								PC8.BS_AngleE_AVG[12] + // don vi: ly giac
								PC8.BS_AngleE_AVG[11] + // don vi: ly giac
								PC8.BS_AngleE_AVG[10] + // don vi: ly giac
								PC8.BS_AngleE_AVG[9] + // don vi: ly giac
								PC8.BS_AngleE_AVG[8] + // don vi: ly giac
								PC8.BS_AngleE_AVG[7] + // don vi: ly giac
								PC8.BS_AngleE_AVG[6] + // don vi: ly giac
								PC8.BS_AngleE_AVG[5] + // don vi: ly giac
								PC8.BS_AngleE_AVG[4] + // don vi: ly giac
								PC8.BS_AngleE_AVG[3] + // don vi: ly giac
								PC8.BS_AngleE_AVG[2] + // don vi: ly giac
								PC8.BS_AngleE_AVG[1] + // don vi: ly giac
								PC8.BS_AngleE_AVG[0]) / 32.0; // don vi: ly giac; // don vi: ly giac
	}
}

void _BS_Second_TG_Stop(void)
{
	if(PC8.Adjust_Mode == 0)
	{
		// chon gia tri dieu khien
		if(PC8.Bamsat_H_ena == 1)
		{
			PC8.BS_Angle_Hb0 = (int16_t)(PC8.BS_Angle_Hz0 - PC8.BS_AngleH_PID);
		}
		else
		{
			PC8.BS_Angle_Hb0 = (int16_t)(PC8.BS_Angle_Hz0 - kalman_H_cap_nhat(PC8.BS_AngleH_Kalman));
			//PC8.BS_Angle_Hb0 = (int16_t)(PC8.BS_Angle_Hz0 - PC8.BamSat._Current_Vx);
		}

		// cho gia tri dieu khien
		if(PC8.Bamsat_E_ena == 1)
		{
			PC8.BS_Angle_Eb0 = (int16_t)(PC8.BS_Angle_Ez0 + PC8.BS_AngleE_PID);
		}
		else
		{
			PC8.BS_Angle_Eb0 = (int16_t)(PC8.BS_Angle_Ez0 + kalman_E_cap_nhat(PC8.BS_AngleE_Kalman));
		}
	}
	else if(PC8.Adjust_Mode == 1)
	{
		if(PC8.Adjust_H_Enable == 1)
		{
			// chon gia tri dieu khien
			if(PC8.Bamsat_H_ena == 1)
			{
				PC8.BS_Angle_Hb0 = (int16_t)(PC8.BS_Angle_Hz0 - PC8.BS_AngleH_PID);
			}
			else
			{
				PC8.BS_Angle_Hb0 = (int16_t)(PC8.BS_Angle_Hz0 + PC8.Bamsat_K1 * kalman_H_cap_nhat(PC8.BS_AngleH_Kalman));
			}
		}

		if(PC8.Adjust_E_Enable == 1)
		{
			// cho gia tri dieu khien
			if(PC8.Bamsat_E_ena == 1)
			{
				PC8.BS_Angle_Eb0 = (int16_t)(PC8.BS_Angle_Ez0 + PC8.BS_AngleE_PID);
			}
			else
			{
				PC8.BS_Angle_Eb0 = (int16_t)(PC8.BS_Angle_Ez0 + PC8.Bamsat_K1 * kalman_E_cap_nhat(PC8.BS_AngleE_Kalman));
			}
		}
	}
}

void _Sub_TG_Mode_Counter(void)
{
	if(PC8.TG_Mode == 1)
	{
		if(PC8.TG_time <= 80)
		{
			PC8.TG_time++;
			PC8.TG_Stop = 0;
		}
		else
		{
			if(PC8.TG_Stop == 0) // chi xu ly 1 lan duy nhat
			{
				PC8.TG_Stop = 1; // khong dieu khien nua

				// luu du lieu bam sat
				if(PC8.TG_Click_time == 0)
				{
					PC8.TG_Alpha0 = Erob_Motor.Motor_GocTa.BS_Angle; // 38
					PC8.TG_Beta0  = Erob_Motor.Motor_PhuongVi.BS_Angle; // 38
					PC8.TG_Alpha  = PC8.TG_Alpha0; // 38
					PC8.TG_Beta   = PC8.TG_Beta0; // 38
					PC8.TG_Click_time = 1;
				}
				else if(PC8.TG_Click_time >= 1)
				{
					if(PC8.TG_EN == 1)
					{
						// hieu chinh goc
						PC8.TG_Alpha = PC8.TG_Alpha0 + PC8.TG_Alpha_table[(PC8.TG_Click_time - 1) % 16]; // 38
						PC8.TG_Beta  = PC8.TG_Beta0  + PC8.TG_Beta_table[(PC8.TG_Click_time - 1) % 16]; // 38
						PC8.TG_Click_time++;
					}
				}
			}
		}

	}
	else if(PC8.TG_Mode == 0)
	{
		PC8.TG_time++;

		if(PC8.TG_time <= 10)
		{
			PC8.TG_Stop = 0;
		}
		else
		{
			PC8.TG_Stop = 1;
		}

		if(PC8.TG_time >= 20)
		{
			PC8.TG_time = 0;
		}
	}
}

void _Sub_Adjust_Counter(void)
{
	if(PC8.step_counter2 < 5)
	{
		PC8.Adjust_E_Enable = 0; // 38
		PC8.Adjust_H_Enable = 1; // 38
	}
	else if((PC8.step_counter2 >= 5)&&(PC8.step_counter2 < 10))
	{
		PC8.Adjust_E_Enable = 1; // 38
		PC8.Adjust_H_Enable = 0; // 38
	}
	else if((PC8.step_counter2 >= 10))
	{
		PC8.Adjust_E_Enable = 0; // 38
		PC8.Adjust_H_Enable = 0; // 38
	}

	PC8.Adjust_Mode = 1;

	PC8.step_counter2++;
	if(PC8.step_counter2 >= 20) PC8.step_counter2 = 0;
}

void _BS_Second_TG_Beam17(void)
{
	if(PC8.BamSat.Beam_Max_Val == 17) // tam anten ve beam 17
	{
		if(((PC8.BS_AngleH_Kalman <= 10)&&(PC8.BS_AngleH_Kalman >= -10))&& 	// -10 < H < 10
		   ((PC8.BS_AngleE_Kalman <= 10)&&(PC8.BS_AngleE_Kalman >= -10))) 	// -10 < E < 10
		{
			_Sub_TG_Mode_Counter();
			_Sub_Adjust_Counter();
		}
		else
		{
			PC8.Adjust_Mode 	= 0;
			PC8.Adjust_E_Enable = 0; // 38
			PC8.Adjust_H_Enable = 0; // 38
			PC8.step_counter2	= 0;
		}
	}
}

void _BS_Second_kiemtra(void)
{
	// kiem tra vuot nguong kenh: E
	if(PC8.BS_Angle_Eb0 < 0)
	{
		PC8.BS_Angle_E0 = (uint16_t)(PC8.BS_Angle_Eb0 + 6000);
	}
	else if(PC8.BS_Angle_Eb0 >= 6000)
	{
		PC8.BS_Angle_E0 = (uint16_t)(PC8.BS_Angle_Eb0 - 6000);
	}
	else
	{
		PC8.BS_Angle_E0 = (uint16_t)PC8.BS_Angle_Eb0;
	}

	// kiem tra vuot nguong kenh: H
	if(PC8.BS_Angle_Hb0 < 0)
	{
		PC8.BS_Angle_H0 = (uint16_t)(PC8.BS_Angle_Hb0 + 6000);
	}
	else if(PC8.BS_Angle_Hb0 >= 6000)
	{
		PC8.BS_Angle_H0 = (uint16_t)(PC8.BS_Angle_Hb0 - 6000);
	}
	else
	{
		PC8.BS_Angle_H0 = (uint16_t)PC8.BS_Angle_Hb0;
	}

	if(PC8.Save_Z0 == 1) // -- chua su dung
	{
		// cap nhat toa do dieu khien moi, khi ve dung tam
		if ((PC8.BS_UE0 <= PC8.BamSat.Nguong_H_DK_Ue_E)&&(PC8.BS_UE0 >= PC8.BamSat.Nguong_L_DK_Ue_E)&&
			(PC8.BS_UH0 <= PC8.BamSat.Nguong_H_DK_Ue_H)&&(PC8.BS_UH0 >= PC8.BamSat.Nguong_L_DK_Ue_H)&&
			(PC8.BamSat.Beam_Max_Val == 17))
		{
			// xac dinh toa do [E0;H0] moi
			PC8.BS_Angle_Ez0 = PC8.BS_Angle_E0;
			PC8.BS_Angle_Hz0 = PC8.BS_Angle_H0;
		}
	}
}

void _BS_Second_TG_Control(void)
{
	if(PC8.TG_Mode == 1) // che do dieu khien run /stop
	{
		if(PC8.TG_Stop == 0) // run: dieu khien dong co
		{
			if(PC8.Adjust_Mode == 1)
			{
				// tinh toan goc
				if(PC8.Adjust_H_Enable == 1) _BS_Cal_New_H();
				if(PC8.Adjust_E_Enable == 1) _BS_Cal_New_E();
			}
			else if(PC8.Adjust_Mode == 0)
			{
				// tinh toan goc
				_BS_Cal_New_H();
				_BS_Cal_New_E();
			}
		}
		else if(PC8.TG_Stop == 1) // stop: khong dieu khien
		{

		}
	}
	else if(PC8.TG_Mode == 0) // che do dieu khien lien tuc
	{
		if(PC8.Adjust_Mode == 1)
		{
			// tinh toan goc
			if(PC8.Adjust_H_Enable == 1) _BS_Cal_New_H();
			if(PC8.Adjust_E_Enable == 1) _BS_Cal_New_E();
		}
		else if(PC8.Adjust_Mode == 0)
		{
			// tinh toan goc
			_BS_Cal_New_H();
			_BS_Cal_New_E();
		}
	}
}

void _BS_Second_TG_Control2(void)
{
	if(PC8.TG_Stop == 0) // run: dieu khien dong co
	{
		if(PC8.Adjust_Mode == 1)
		{
			// tinh toan goc
			if(PC8.Adjust_H_Enable == 1) _BS_Cal_New_H();
			if(PC8.Adjust_E_Enable == 1) _BS_Cal_New_E();
		}
		else if(PC8.Adjust_Mode == 0)
		{
			// tinh toan goc
			_BS_Cal_New_H();
			_BS_Cal_New_E();
		}
	}
	else if(PC8.TG_Stop == 1) // stop: khong dieu khien
	{
		if(PC8.TG_Click_time >= 1)
		{
			Erob_Motor.Motor_PhuongVi.Motor_BS_Value = PC8.TG_Beta;
			Erob_Motor.Motor_GocTa.Motor_BS_Value = PC8.TG_Alpha;

			PC8.BamSat.Process_Phi_start = 1;// chuyen qua ban sat phuong vi
			PC8.BamSat.Process_Epxilon_start = 1;
		}
	}
}

void _Sub_TG_Mode_Process(void)
{
	// sau 1 thoi gian on dinh, tu dung dong co
	if(PC8.TG_Mode == 1)
	{
		if(PC8.TG_Stop == 0)
		{
			_BS_Second_TG_Stop();
		}
	}
	else if(PC8.TG_Mode == 0)
	{
		_BS_Second_TG_Stop();
	}
}

void _Sub_TG_EN_Process(void)
{
	if(PC8.TG_EN == 0)
	{
		_BS_Second_TG_Control();
	}
	else if(PC8.TG_EN == 1)
	{
		_BS_Second_TG_Control2();
	}
}

void _BS_Second_Process(void)
{
	PC8.BamSat.Process_New_CMD = 0;

	PC8.BamSat.Process_Phi_start = 1;// chuyen qua ban sat phuong vi
	PC8.BamSat.Process_Epxilon_start = 0;

	_BS_Second_AVG();
	_BS_Second_Adjust_Mode();

	PC8.BS_AngleH_PID = PC8.Bamsat_K1 * (PC8.BS_AngleH_AVG[0] - PC8.BS_AngleH_AVG[1]) +
					    PC8.Bamsat_K2 * (PC8.BS_AngleH_AVG[1] - PC8.BS_AngleH_AVG[2]) +
					    PC8.Bamsat_K3 * (PC8.BS_AngleH_AVG[2] - PC8.BS_AngleH_AVG[3]); // don vi: ly giac

	PC8.BS_AngleE_PID = PC8.Bamsat_K1 * (PC8.BS_AngleE_AVG[0] - PC8.BS_AngleE_AVG[1]) +
						PC8.Bamsat_K2 * (PC8.BS_AngleE_AVG[1] - PC8.BS_AngleE_AVG[2]) +
						PC8.Bamsat_K3 * (PC8.BS_AngleE_AVG[2] - PC8.BS_AngleE_AVG[3]); // don vi: ly giac

	PC8.step_counter1++;

	if(PC8.step_counter1 >= 2) // giam toc do xu ly 8 lan
	{
		PC8.step_counter1 = 0;

		_BS_Second_TG_Beam17();
		_Sub_TG_Mode_Process();
		_BS_Second_kiemtra();
		_Sub_TG_EN_Process();

	}
}

void _BS_Mode_Scan_Quat(void)
{
	// sau khi bam sat xong, xu ly chu ki bam sat tiep theo
	if(PC8.BS_Scan_Start == 1)
	{
		_BS_First_Process(10.0);
	}
	else if(PC8.BS_Scan_Start == 2)
	{
		_BS_First_Process(-10.0);
	}
	else if(PC8.BS_Scan_Start == 3)
	{
		PC8.BS_Scan_Start = 0; // da khao sat xong

		// xac dinh dac tuyen
		_Khao_Sat_Get_Max();

		// cap nhat toa do dieu khien
		_BS_First_Process(PC8._KS_Pvi.Center);

		PC8._KS_Pvi.buffer = PC8.BS_Angle_Hz0 + PC8._KS_Pvi.Center / 0.06;

		if(PC8._KS_Pvi.buffer < 0)
		{
			PC8.BS_Angle_Hz0 = PC8.BS_Angle_Hb0 + 6000;
		}
		else if(PC8._KS_Pvi.buffer >= 6000)
		{
			PC8.BS_Angle_Hz0 = PC8.BS_Angle_Hb0 - 6000;
		}
		else
		{
			PC8.BS_Angle_Hz0 = (uint16_t)PC8._KS_Pvi.buffer;
		}
	}
	else if(PC8.BS_Scan_Start == 0)
	{
		_BS_Second_Process();
	}
}

void _BS_Mode_Scan_Normal(void)
{
	// sau khi bam sat xong, xu ly chu ki bam sat tiep theo
	if(PC8.BS_Scan_Start == 1)
	{
		_BS_First_Process(0.0); // quay den vi tri kich chuot
	}
	else if(PC8.BS_Scan_Start == 0)
	{
		_BS_Second_Process();
	}
}

void _BS_Reload_Work(void)
{
	// sau khi quet dac tuyen xong
	if(PC8.BamSat.Process_BS_done == 1)
	{
		PC8.BamSat.Process_BS_done = 0;
		PC8.BS_Counter_time++;

//		if(PC8.BamSat.BS_Quat_mode == 1)
//		{
//			_BS_Mode_Scan_Quat();
//		}
//		else if(PC8.BamSat.BS_Quat_mode == 0)
//		{
			_BS_Mode_Scan_Normal();
//		}
	}

	// tu kiem tra hoan thanh
	if((PC8.BamSat.Process_Epxilon_start == 2)&&
	   (PC8.BamSat.Process_Phi_start 	 == 2)&&
	   (PC8.BamSat.Process_Beta_Mode 	 == 1)) // chuyen qua ban sat goc ta)
	{
		PC8.BamSat.Process_Phi_start 	 = 0;// chuyen qua ban sat phuong vi
		PC8.BamSat.Process_Epxilon_start = 0;
		PC8.BamSat.Process_BS_done 		 = 1;
		PC8.Sync_time 					 = 0;
	}
}

