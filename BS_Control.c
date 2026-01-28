#include "Erob.h"
#include "MI_505.h"
#include "DK_Maythu.h"
#include "CAN_Control.h"
#include <math.h>

void Clear_Check_Max(void)
{
	PC8.S17_amplitude[8] = 0;//31
	PC8.S17_amplitude[7] = 0;//31
	PC8.S17_amplitude[6] = 0;//31
	PC8.S17_amplitude[5] = 0;//31
	PC8.S17_amplitude[4] = 0;//31
	PC8.S17_amplitude[3] = 0;//31
	PC8.S17_amplitude[2] = 0;//31
	PC8.S17_amplitude[1] = 0;//31
	PC8.S17_amplitude[0] = 0;//31

	PC8.S17_Beta[8] = 0;//31
	PC8.S17_Beta[7] = 0;//31
	PC8.S17_Beta[6] = 0;//31
	PC8.S17_Beta[5] = 0;//31
	PC8.S17_Beta[4] = 0;//31
	PC8.S17_Beta[3] = 0;//31
	PC8.S17_Beta[2] = 0;//31
	PC8.S17_Beta[1] = 0;//31
	PC8.S17_Beta[0] = 0;//31

	PC8.S17_Alpha[8] = 0;//31
	PC8.S17_Alpha[7] = 0;//31
	PC8.S17_Alpha[6] = 0;//31
	PC8.S17_Alpha[5] = 0;//31
	PC8.S17_Alpha[4] = 0;//31
	PC8.S17_Alpha[3] = 0;//31
	PC8.S17_Alpha[2] = 0;//31
	PC8.S17_Alpha[1] = 0;//31
	PC8.S17_Alpha[0] = 0;//31

	PC8.S17_Beta_AVG 		= 0;
	PC8.S17_amplitude_H_Dir = 0;
	PC8.S17_Alpha_AVG 		= 0;
	PC8.S17_amplitude_E_Dir = 0;
	PC8.S17_amplitude_MAX 	= 0;//31
	PC8.S17_amplitude_done  = 0;
	PC8.S17_amplitude_Enable_E = 0;

	PC8.Adjust_Mode = 0;
	PC8.Adjust_E_Enable = 0; // 38
	PC8.Adjust_H_Enable = 0; // 38
	PC8.step_counter2 	= 0;
}

void _Process_Amplitude(void)
{
	PC8.step_counter1++;

	if(PC8.step_counter1 >= 10)
	{
		PC8.step_ENA1 = 1;
		PC8.step_ENA2 = 1;
		PC8.step_counter1 = 0;

		PC8.S17_amplitude[8] = PC8.S17_amplitude[7];//31
		PC8.S17_amplitude[7] = PC8.S17_amplitude[6];//31
		PC8.S17_amplitude[6] = PC8.S17_amplitude[5];//31
		PC8.S17_amplitude[5] = PC8.S17_amplitude[4];//31
		PC8.S17_amplitude[4] = PC8.S17_amplitude[3];//31
		PC8.S17_amplitude[3] = PC8.S17_amplitude[2];//31
		PC8.S17_amplitude[2] = PC8.S17_amplitude[1];//31
		PC8.S17_amplitude[1] = PC8.S17_amplitude[0];//31
		PC8.S17_amplitude[0] = PC8.BamSat.Beam_Max;//31

		PC8.S17_Beta[8] = PC8.S17_Beta[7];//31
		PC8.S17_Beta[7] = PC8.S17_Beta[6];//31
		PC8.S17_Beta[6] = PC8.S17_Beta[5];//31
		PC8.S17_Beta[5] = PC8.S17_Beta[4];//31
		PC8.S17_Beta[4] = PC8.S17_Beta[3];//31
		PC8.S17_Beta[3] = PC8.S17_Beta[2];//31
		PC8.S17_Beta[2] = PC8.S17_Beta[1];//31
		PC8.S17_Beta[1] = PC8.S17_Beta[0];//31
		PC8.S17_Beta[0] = Erob_Motor.Motor_PhuongVi.BS_Angle;//31

		PC8.S17_Alpha[8] = PC8.S17_Alpha[7];//31
		PC8.S17_Alpha[7] = PC8.S17_Alpha[6];//31
		PC8.S17_Alpha[6] = PC8.S17_Alpha[5];//31
		PC8.S17_Alpha[5] = PC8.S17_Alpha[4];//31
		PC8.S17_Alpha[4] = PC8.S17_Alpha[3];//31
		PC8.S17_Alpha[3] = PC8.S17_Alpha[2];//31
		PC8.S17_Alpha[2] = PC8.S17_Alpha[1];//31
		PC8.S17_Alpha[1] = PC8.S17_Alpha[0];//31
		PC8.S17_Alpha[0] = Erob_Motor.Motor_GocTa.BS_Angle;//31

		PC8.S17_Beta_AVG = (PC8.S17_Beta[8] +
							PC8.S17_Beta[7] +
							PC8.S17_Beta[6] +
							PC8.S17_Beta[5] +
							PC8.S17_Beta[4] +
							PC8.S17_Beta[3] +
							PC8.S17_Beta[2] +
							PC8.S17_Beta[1]) / 8;

		if(PC8.S17_Beta_AVG > PC8.S17_Beta[0])
		{
			PC8.S17_amplitude_H_Dir = 1; // tien
		}
		else if(PC8.S17_Beta_AVG < PC8.S17_Beta[0])
		{
			PC8.S17_amplitude_H_Dir = 2; // lui
		}
		else
		{
			PC8.S17_amplitude_H_Dir = 0; // dung yen
		}

		PC8.S17_Alpha_AVG =(PC8.S17_Alpha[8] +
							PC8.S17_Alpha[7] +
							PC8.S17_Alpha[6] +
							PC8.S17_Alpha[5] +
							PC8.S17_Alpha[4] +
							PC8.S17_Alpha[3] +
							PC8.S17_Alpha[2] +
							PC8.S17_Alpha[1]) / 8;

		if(PC8.S17_Alpha_AVG > PC8.S17_Alpha[0])
		{
			PC8.S17_amplitude_E_Dir = 1;// tien
		}
		else if(PC8.S17_Alpha_AVG < PC8.S17_Alpha[0])
		{
			PC8.S17_amplitude_E_Dir = 2; // lui
		}
		else
		{
			PC8.S17_amplitude_E_Dir = 0; // dung yen
		}

		if( PC8.S17_amplitude_MAX <= PC8.BamSat.Beam_Max)
		{
			PC8.S17_amplitude_MAX = PC8.BamSat.Beam_Max;// cap nhat bien do beam max lon nhat
			PC8.S17_amplitude_done = (uint32_t)(PC8.BamSat.Beam_Max * 0.75); // nguong cho phep dieu khien goc ta
		}

		// kiem tra co du dieu kien bam sat kenh ta khong
		if(PC8.BamSat.Beam_Max >= PC8.S17_amplitude_done)
		{
			PC8.S17_amplitude_Enable_E = 1;
		}
		else
		{
			PC8.S17_amplitude_Enable_E = 0;
		}
	}

}

void Erob_Process(void)
{

	if(PC8.BS_Start == 0)
	{
		_BS_Work_Off();
		Clear_Check_Max();
		PC8.step_counter1 = 0;
		PC8.step_ENA1 = 0;
		PC8.step_ENA2 = 0;
		PC8.step_counter_All = 0;
		PC8.BamSat.BS_Detect_low_counter = 0;
	}
	else if(PC8.BS_Start == 1)
	{

//		if(PC8.BS_Scan_Start == 0) // sau khi da qua den muc tieu
//		{
//			//_Process_Amplitude(); // xu ly kiem tra nguong tin hieu
//		}

		_BS_Reload_Work(); 	// Xac dinh che do dieu khien
		_BS_Reload_H_E(); 	// dieu khien tuan tu H -> E

		if(PC8.BamSat.Bam_culy_ENA == 1)
		{
			_BS_Culy_Process();
		}
	}
}

void _BS_Reload_H_E(void)
{
	// xu ly: bam sat phuong vi
	if(PC8.BamSat.Process_Phi_start == 1)
	{
		if(Erob_Motor.Motor_PhuongVi.Erob_Enable == 1)
		{
			Erob_Control_Bamsat_PC8_H();
		}
		else
		{
			Erob_Motor_Control(	PC8.BamSat._Delta_Phi_Speed,
								PC8.BamSat._Delta_Phi_Dir,
								0,
								Radar_Motor_PhuongVi);
		}
	}
	// xu ly: bam sat goc ta
	else if(PC8.BamSat.Process_Epxilon_start == 1)
	{

		if(Erob_Motor.Motor_GocTa.Erob_Enable == 1)
		{
			Erob_Control_Bamsat_PC8_E();
		}
		else
		{
			Erob_Motor_Control(	PC8.BamSat._Delta_Epxilon_Speed,
								PC8.BamSat._Delta_Epxilon_Dir,
								0,
								Radar_Motor_GocTa);
		}
	}
}

void Erob_BS_Window(void)
{
	if(PC8.BS_PC1_Click == 1)
	{
		PC8.BS_PC1_Click = 0;

		_BS_First_Click();
	}
	else if(PC8.BS_PC1_Click == 0) // khong co thao tac kich chuot
	{
		_BS_Second_Click();
	}
}

void Erob_Get_CMD(void)
{
	Erob_BS_Window();
	//cap nhat lenh
	Erob_Motor_PhuongVi_Get_CMD();
	Erob_Motor_GocTa_Get_CMD();
}
