#include "Erob.h"
#include "MI_505.h"
#include "DK_Maythu.h"

#define Culy_Save_Size 	220 // cell
#define Culy_Fix_data 	50  // m
int	buffer_start = 0;
int	buffer_stop = 0;

void _BS_Calib_Point(int Save_EN)
{
	// ghi thong tin bam sat
	System_Set_BS_Enable(PC8.BS_Start); // lenh cho phep bam sat du lieu
	System_Set_BS_Angle(PC8.BS_Center_angle); // goc bam sat

	if(PC8.BS_Center_start >= 16)
	{
		// cau hinh cua song cu ly
		System_Set_BS_Start(PC8.BS_Center_start - 12); // o cu ly bat dau
		System_Set_BS_Stop(PC8.BS_Center_stop 	- 12);// o cu lu ket thuc
		System_Set_BS_Size(Culy_Save_Size);

		// cau hinh cua song phi/epxilon
		System_Set_BS_Start4(PC8.BS_Center_start - 20);
		System_Set_BS_Stop4(PC8.BS_Center_stop 	 - 20);
		System_Set_BS_Size4(Culy_Save_Size);
	}
	else
	{
		// cau hinh cua song cu ly
		System_Set_BS_Start(0); // o cu ly bat dau
		System_Set_BS_Stop(200);// o cu lu ket thuc
		System_Set_BS_Size(Culy_Save_Size);

		// cau hinh cua song phi/epxilon
		System_Set_BS_Start4(0);
		System_Set_BS_Stop4(200);
		System_Set_BS_Size4(Culy_Save_Size);
	}

	// luu lai toa do cua song
	if(Save_EN == 1)
	{
		PC8.BS_Center_start_bs 	= PC8.BS_Center_start;
		PC8.BS_Center_stop_bs 	= PC8.BS_Center_stop;
		PC8.BS_Center_start_bs0 = PC8.BS_Center_start;

		//PC8.BS_Center_start4 = PC8.BS_Center_start;
	}
}

void _BS_First_Click(void)
{
	PC8.Sync_time 						= 0;
	PC8.BS_Counter_time 				= 0;
	PC8.BamSat.BS_Detect_done 			= 0;// 20
	PC8.BamSat.BS_Detect_Fail 			= 0;// 20
	PC8.BamSat.BS_Detect_time 			= 0;// 20
	PC8.BamSat.BS_Detect_Change_Point 	= 0;// 20
	PC8.BamSat.BS_Detect_Fix 			= 0;// 20

	PC8.BamSat.Min_Detect_bamsat = PC8.BamSat.Min_Detect;
	if(PC8.BamSat.Min_Detect_bamsat < 50) PC8.BamSat.Min_Detect_bamsat = 50;

	PC8.BamSat.Beam_SNR2 	= 0.0;
	PC8.BamSat.Beam_SNR2i 	= 0.0;

	PC8.Delta_Click_Start 	= PC8.BS_Center_start - PC8.BS_Center_starti;
	PC8.Delta_Click_Beta 	= PC8.BS_Angle_H - PC8.BS_Angle_Hz0;

	if(PC8.Delta_Click_Beta < -3000) PC8.Delta_Click_Beta = PC8.Delta_Click_Beta + 6000;

	PC8.Target_Radius0 		= 0;
	PC8.Target_Beta0 		= 0;
	PC8.Target_Alpha0 		= 0;
	PC8.x0 					= 0;
	PC8.y0 					= 0;
	PC8.z0 					= 0;

	PC8.Target_Radius1 		= 0;
	PC8.Target_Beta1 		= 0;
	PC8.Target_Alpha1 		= 0;
	PC8.x1 					= 0;
	PC8.y1 					= 0;
	PC8.z1 					= 0;

	PC8.step_counter1 		= 0;
	PC8.step_counter_All 	= 0;
	PC8.Sum_Radius 			= 0;
	PC8.Delta_Step 			= 0;

	PC8.BamSat.BS_Detect_low_counter	= 0;
	PC8.step_counter0 		= 0;
	PC8.Delta_Step 			= 0;

	_BS_Calib_Point(1); // chi luu toa do khi kich chuot

	// luu lai goc kich chuot: ly giac
	Erob_Motor.Motor_GocTa.Motor_BS_Value 	= PC8.BS_Angle_E;
	Erob_Motor.Motor_PhuongVi.Motor_BS_Value = PC8.BS_Angle_H;

	// luu lai goc kich chuot: ly giac
	PC8.BS_Angle_Ez0 	= PC8.BS_Angle_E;
	PC8.BS_Angle_Hz0 	= PC8.BS_Angle_H;

	// luu lai goc kich chuot: ly giac
	PC8.BS_Center_starti= PC8.BS_Center_start;

	// kiem tra: co phai muc tieu moi
	if((PC8.Delta_Click_Start >=  200)|| // +200 cell
	   (PC8.Delta_Click_Start <= -200)|| // -200 cell
	   (PC8.Delta_Click_Beta  <= -100)|| // -100 ly giac
	   (PC8.Delta_Click_Beta  >=  100)|| // +100 ly giac
	   (PC8.BS_K_Point  ==  0)) // chua bam sat lan nao
	{
		PC8.BS_K_Point 	  	= 0;
		PC8.BS_Scan_Start 	= 1;

		//PC8.TG_Click_time   = 0;
		//PC8.TG_Stop 		= 0;
	}
	else
	{
		PC8.BS_Scan_Start 	= 0;
	}

	PC8.Click_E_time 	= 0;
	PC8.Click_H_time 	= 0;

	PC8.BamSat.Process_Phi_start 		= 0; // chuyen qua ban sat phuong vi
	PC8.BamSat.Process_Epxilon_start 	= 0; // chuyen qua ban sat phuong vi
	PC8.BamSat.Process_BS_done 			= 1; // chuyen qua ban sat phuong vi

	// tinh toan du lieu cua song tai lan kich chuot dau tien
	PC8.BamSat.Center_Screen = PC8.BS_Center_cell - PC8.BS_Center_start;
	Cua_Song_Auto_Fix();

}

void _BS_Second_Click(void)
{
	if(PC8.BS_Start == 1) // cap nhat toa do cua song
	{
		if(PC8.BS_K_Point < 1000) PC8.BS_K_Point++; // tang so lan bam sat
	}
	else
	{
		if(PC8.Phase_Mode == 0) // gui goi tin chinh Phase
		{

			if(PC8.BS_Center_start2 != PC8.BS_Center_start)
			{
				PC8.BS_Center_start2 = PC8.BS_Center_start;
				 //xil_printf("BS_start = %d BS_stop = %d\r\n",PC8.BS_Center_start2, PC8.BS_Center_stop);
			}

			_BS_Calib_Point(0);

		}
		else if(PC8.Phase_Mode == 1) // gui goi tin chinh Phase
		{
			_BS_Calib_Point(0);
		}
	}

}

void _BS_Mode_0_Process(void)
{
	//if(PC8.S17max_out >= PC8.BamSat.Min_Detect)
	if(PC8.BamSat.Beam_Max >= PC8.BamSat.Max_channel_AVG)
	{
		PC8.BamSat.BS_Detect_done 			= 1;// 20
		PC8.BamSat.BS_Detect_Fail 			= 0;// 20
		PC8.BamSat.BS_Detect_time 			= 0;// 20
		PC8.BamSat.BS_Detect_Change_Point 	= 0;// 20
		PC8.BamSat.BS_Detect_Fix 			= 0;// 20
		PC8.BamSat.BS_Detect_low_counter	= 0;
	}
	else
	{
		PC8.BamSat.BS_Detect_done 	= 0;// 20
		PC8.BamSat.BS_Detect_Fail 	= 1;// 20

		PC8.BamSat.BS_Detect_low_counter++;

		PC8.BamSat.BS_Detect_time = 0;
		PC8.BamSat.BS_Detect_Change_Point = 1;// 20
		PC8.BamSat.BS_Detect_Fix = 0;// 20

	}
}

// PC8.BamSat.Beam_Maxi
void _BS_Mode_1_Process(void)
{
	if(PC8.BamSat.Beam_Max >= PC8.BamSat.Min_Detect_bamsat)
	{
		PC8.BamSat.Beam_SNR2 = (PC8.BamSat.Beam_Max * 1.0) / PC8.BamSat.Min_Detect_bamsat;

		if(PC8.BamSat.Beam_SNR2 > PC8.BamSat.Beam_SNR2i)
		{
			if(PC8.BamSat.Min_Detect_bamsat < 3000) PC8.BamSat.Min_Detect_bamsat++;
		}
		else if(PC8.BamSat.Beam_SNR2 < PC8.BamSat.Beam_SNR2i)
		{
			if(PC8.BamSat.Min_Detect_bamsat > 50) PC8.BamSat.Min_Detect_bamsat--;
		}

		PC8.BamSat.Beam_SNR2i = PC8.BamSat.Beam_SNR2;

		PC8.BamSat.BS_Detect_done 			= 1;// 20
		PC8.BamSat.BS_Detect_Fail 			= 0;// 20
		PC8.BamSat.BS_Detect_time 			= 0;// 20
		PC8.BamSat.BS_Detect_Change_Point 	= 0;// 20
		PC8.BamSat.BS_Detect_Fix 			= 0;// 20
		PC8.BamSat.BS_Detect_low_counter	= 0;
	}
	else
	{
		if(PC8.BamSat.Min_Detect_bamsat > 50) PC8.BamSat.Min_Detect_bamsat--;

		PC8.BamSat.BS_Detect_done 	= 0;// 20
		PC8.BamSat.BS_Detect_Fail 	= 1;// 20

		PC8.BamSat.BS_Detect_low_counter++;

		PC8.BamSat.BS_Detect_time = 0;
		PC8.BamSat.BS_Detect_Change_Point = 1;// 20
		PC8.BamSat.BS_Detect_Fix = 0;// 20
	}
}

void _BS_Check_Target_in_Window(void)
{
//	if(PC8.BamSat.Min_Detect_mode == 0)
//	{
		_BS_Mode_0_Process();
//	}
//	else if(PC8.BamSat.Min_Detect_mode == 1)
//	{
//		_BS_Mode_1_Process();
//	}
}

void _BS_Get_CS_Point(void)
{
	PC8.BS_Center_start_bs0 = PC8.BS_Center_start_bs;
}

void _Sub_Culy_Mode0(void)
{
	// chi cap nhat khi dang bam sat dung tam muc tieu
	if(((PC8.BamSat._CS_H_Ready == 1)&& // da bam sat H
	   ((PC8.BamSat._CS_E_Ready + PC8.BamSat._CS_E_Enable) >= 1)&& // da bam sat E
	    (PC8.BamSat.BS_Detect_done == 1))|| // co tin hieu muc tieu
		(PC8.BamSat.LoopBack == 1)) // can thiep de kiem tra che do loopback
	{
		if(PC8.BS_PS17 < (PC8.BamSat.Center_Screen - PC8.BamSat.BS_CS_Delta))
		{
			if(PC8.BS_Center_start_bs > 0)
			{
				PC8.BS_Center_start_bs--;
			}
		}
		else if(PC8.BS_PS17 > (PC8.BamSat.Center_Screen + PC8.BamSat.BS_CS_Delta))
		{
			if(PC8.BS_Center_start_bs < 10479) PC8.BS_Center_start_bs++;
		}
		else // bam sat dung center cell
		{

		}

		PC8.BS_Center_stop_bs = PC8.BS_Center_start_bs + 190;// 20

		// cau hinh cua song cu ly
		System_Set_BS_Start(PC8.BS_Center_start_bs); // o cu ly bat dau
		System_Set_BS_Stop(PC8.BS_Center_stop_bs);// o cu lu ket thuc
		System_Set_BS_Size(Culy_Save_Size);

		// cau hinh cua song phi/epxilon
		System_Set_BS_Start4(PC8.BS_Center_start_bs);
		System_Set_BS_Stop4(PC8.BS_Center_stop_bs);
		System_Set_BS_Size4(Culy_Save_Size);

		// dinh vi lai vi tri: PC8.BamSat.Center_Screen
		Cua_Song_Cal_New_center();
		// tinh toan lai cua song sau khi hieu chinh diem center
		Cua_Song_Auto_Fix();

		// tinh toan thong so muc tieu
		Cua_Song_Cal_target_information();

		// doc thong tin muc tieu
		_BS_Get_CS_Point();

	}
}

void _Sub_Culy_Mode1(void)
{
//	// chi cap nhat khi dang bam sat dung tam muc tieu
//	if(((PC8.BamSat._CS_H_Ready 	== 1) && // da bam sat E
//	   ((PC8.BamSat.Beam_Max_Val 	== _Beam17_Max)||
//	    (PC8.BamSat.Beam_Max_Val 	== _Beam6_Max) ||
//	    (PC8.BamSat.Beam_Max_Val 	== _Beam7_Max) ||
//	    (PC8.BamSat.Beam_Max_Val 	== _Beam10_Max)||
//	    (PC8.BamSat.Beam_Max_Val 	== _Beam11_Max)||
//	    (PC8.BamSat.Beam_Max_Val 	== _SS7_Max)||
//	    (PC8.BamSat.Beam_Max_Val 	== _SS2_Max)||
//	    (PC8.BamSat.Beam_Max_Val 	== _SS3_Max)||
//	    (PC8.BamSat.Beam_Max_Val 	== _SS8_Max)||
//	    (PC8.BamSat.Beam_Max_Val 	== _V1_Max)||
//	    (PC8.BamSat.Beam_Max_Val 	== _V2_Max)) &&
//	    (PC8.BamSat.BS_Detect_done 	== 1))|| // co tin hieu muc tieu
//		(PC8.BamSat.LoopBack 		== 1)) // can thiep de kiem tra che do loopback
	// chi cap nhat khi dang bam sat dung tam muc tieu
if(((PC8.BamSat._CS_H_Ready 	== 1) && // da bam sat E
   ((PC8.BamSat.Beam_Max_Val 	== _Beam17_Max)||
	(PC8.BamSat.Beam_Max_Val 	== _Beam2_Max) ||
	(PC8.BamSat.Beam_Max_Val 	== _Beam3_Max) ||
	(PC8.BamSat.Beam_Max_Val 	== _Beam6_Max) ||
	(PC8.BamSat.Beam_Max_Val 	== _Beam7_Max) ||
	(PC8.BamSat.Beam_Max_Val 	== _Beam10_Max)||
	(PC8.BamSat.Beam_Max_Val 	== _Beam11_Max)||
	(PC8.BamSat.Beam_Max_Val 	== _Beam14_Max) ||
	(PC8.BamSat.Beam_Max_Val 	== _Beam15_Max) ||
	(PC8.BamSat.Beam_Max_Val 	== _SS6_Max)||
	(PC8.BamSat.Beam_Max_Val 	== _SS1_Max)||
	(PC8.BamSat.Beam_Max_Val 	== _SS7_Max)||
	(PC8.BamSat.Beam_Max_Val 	== _SS2_Max)||
	(PC8.BamSat.Beam_Max_Val 	== _SS3_Max)||
	(PC8.BamSat.Beam_Max_Val 	== _SS8_Max)||
	(PC8.BamSat.Beam_Max_Val 	== _SS4_Max)||
	(PC8.BamSat.Beam_Max_Val 	== _SS9_Max)||
	(PC8.BamSat.Beam_Max_Val 	== _V1_Max)||
	(PC8.BamSat.Beam_Max_Val 	== _V2_Max)) &&
	(PC8.BamSat.BS_Detect_done 	== 1))|| // co tin hieu muc tieu
	(PC8.BamSat.LoopBack 		== 1)) // can thiep de kiem tra che do loopback
	{
		if(PC8.BS_PS17 < (PC8.BamSat.Center_Screen - PC8.BamSat.BS_CS_Delta))
		{
			if(PC8.BS_Center_start_bs > 0)
			{
				PC8.BS_Center_start_bs--;
			}
		}
		else if(PC8.BS_PS17 > (PC8.BamSat.Center_Screen + PC8.BamSat.BS_CS_Delta))
		{
			if(PC8.BS_Center_start_bs < 10479) PC8.BS_Center_start_bs++;
		}
		else // bam sat dung center cell
		{

		}

		PC8.BS_Center_stop_bs = PC8.BS_Center_start_bs + 190;// 20

		// cau hinh cua song cu ly
		System_Set_BS_Start(PC8.BS_Center_start_bs); // o cu ly bat dau
		System_Set_BS_Stop(PC8.BS_Center_stop_bs);// o cu lu ket thuc
		System_Set_BS_Size(Culy_Save_Size);

		// cau hinh cua song phi/epxilon
		System_Set_BS_Start4(PC8.BS_Center_start_bs);
		System_Set_BS_Stop4(PC8.BS_Center_stop_bs);
		System_Set_BS_Size4(Culy_Save_Size);

		// dinh vi lai vi tri: PC8.BamSat.Center_Screen
		Cua_Song_Cal_New_center();
		// tinh toan lai cua song sau khi hieu chinh diem center
		Cua_Song_Auto_Fix();

		// tinh toan thong so muc tieu
		Cua_Song_Cal_target_information();

		// doc thong tin muc tieu
		_BS_Get_CS_Point();

	}
}

void _BS_Culy_Process(void)
{
	if(PC8.BamSat.BS_Beam_mode == 0)
	{
		_Sub_Culy_Mode0();
	}
	else if(PC8.BamSat.BS_Beam_mode == 1)
	{
		_Sub_Culy_Mode1();
	}
}

