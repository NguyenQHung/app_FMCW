/*
 * AFC.c
 *
 *  Created on: Apr 10, 2020
 *      Author: TienNam
 */

#include "AFC.h"

extern struct Radar_Struct Radar;
extern struct Frame_Struct Frame;
extern struct PC4_def		PC4;
extern struct PC4_def		PC4s;
extern struct AFC_Struct AFC1;
extern char* Frame_CMD;

extern unsigned char		Radar_Frame2[2048];

void AFC_Xung_Tot(void)
{
	// xu ly khi dang o trung tam
	AFC1.Error_counter  = 0;
	AFC1.Error_counter2  = 0;

	if(Radar.Pulse_Width <35)
		{
			AFC1.Max_Top = 700;
			AFC1.Min_Top = 400;
		}
	else if((Radar.Pulse_Width >=35)&&
			(Radar.Pulse_Width <=38))
	{
		AFC1.Max_Top = 900;
		AFC1.Min_Top = 470;
	}
	else if((Radar.Pulse_Width > 38)&&
			(Radar.Pulse_Width < 45))
	{
		AFC1.Max_Top = 1200;
		AFC1.Min_Top = 540;
	}
	else if(Radar.Pulse_Width >= 45)
	{
		AFC1.Max_Top = 1300;
		AFC1.Min_Top = 600;
	}

	// tang dan nguong
	if((AFC1.Level_Phai > AFC1.Level_Trai)||
	   (AFC1.Level_Trai > AFC1.Level_Phai))
	{
		if(AFC1.U_nguong < AFC1.Max_Top) AFC1.U_nguong += 10;
	}

	// dich tan so
	if((AFC1.Level_Phai > AFC1.Level_Trai)&&
	   (AFC1.Level_Phai > AFC1.U_Lech))
		{
			AFC1.Fix_time2=0;
			AFC1.Fix_time++;

			if(AFC1.Fix_time>5)
			{
				if(AFC1.Control_Value<(AFC1.Control_MAX-1))
				{
					AFC1.Control_Value = AFC1.Control_Value + 1;
				}
				AFC1.Fix_time=0;
			}
		}
	else if((AFC1.Level_Trai >= AFC1.Level_Phai)&&
			(AFC1.Level_Trai >= AFC1.U_Lech))
		{
			AFC1.Fix_time=0;
			AFC1.Fix_time2++;

			if(AFC1.Fix_time2>5)
			{
				if(AFC1.Control_Value> 0)
				{
				AFC1.Control_Value = AFC1.Control_Value - 1;
				}

				AFC1.Fix_time2=0;
			}
		}
	AFC1.Control_Value0 = AFC1.Control_Value;
}

void AFC_Fix_hold(void)
{
	if (Radar.Pulse_Width <= 38)
	{
		if (AFC1.U_nguong > AFC1.Min_Top) AFC1.U_nguong -= 10; else AFC1.U_nguong = AFC1.Min_Top;
		AFC1.U_LechTrai = AFC1.Min_Top - 20;
		AFC1.U_LechPhai = AFC1.Min_Top - 20;
		//AFC1.U_Lech = 7;
	}
	else if ((Radar.Pulse_Width > 38)&&(Radar.Pulse_Width < 45))
	{
		if (AFC1.U_nguong > AFC1.Min_Top) AFC1.U_nguong -= 10; else AFC1.U_nguong = AFC1.Min_Top;
		//AFC1.U_Lech = 9;
		AFC1.U_LechTrai = AFC1.Min_Top - 40;
		AFC1.U_LechPhai = AFC1.Min_Top - 40;
	}
	else if (Radar.Pulse_Width >= 45)
	{
		if (AFC1.U_nguong > AFC1.Min_Top) AFC1.U_nguong -= 10; else AFC1.U_nguong = AFC1.Min_Top;
		//AFC1.U_Lech = 9;
		AFC1.U_LechTrai = AFC1.Min_Top - 50;
		AFC1.U_LechPhai = AFC1.Min_Top - 50;
	}
}

void AFC_Lech_Trai(void)
{
	if(AFC1.AFC_DIR==0) // tang tan so ngoai sai)
	{
		AFC1.Error_counter ++;
		if(AFC1.Error_counter < 6)
		{
			if(AFC1.Control_Value <AFC1.Control_MAX) AFC1.Control_Value += 1;
		}
		else if(AFC1.Error_counter >= 6)
		{
			if(AFC1.Control_Value > 15) AFC1.Control_Value -= 15; // - 60 mhz
			else AFC1.Control_Value = AFC1.Control_MAX;
			AFC1.Error_counter = 0;
		}
	}
	else if(AFC1.AFC_DIR==1) // tang tan so ngoai sai)
	{
		if(AFC1.Control_Value < AFC1.Control_MAX) AFC1.Control_Value += 1;
		else AFC1.Control_Value = 0;
	}
	else if(AFC1.AFC_DIR==2) // tang tan so ngoai sai)
	{
		if(AFC1.Control_Value >= 1) AFC1.Control_Value -= 1;
		else AFC1.Control_Value = AFC1.Control_MAX;
	}
}

void AFC_Lech_Phai(void)
{
	if(AFC1.AFC_DIR==0) // tang tan so ngoai sai)
	{
		AFC1.Error_counter2 ++;
		if(AFC1.Error_counter2 < 6)
		{
			if(AFC1.Control_Value > 1) AFC1.Control_Value -= 1;
			AFC1.Control_Value = AFC1.Control_MAX;
		}
		else if(AFC1.Error_counter2 >= 6)
		{
			if(AFC1.Control_Value > 15) AFC1.Control_Value -= 15;
			else AFC1.Control_Value = AFC1.Control_MAX;// - 60 mhz
			AFC1.Error_counter2 = 0;
			//AFC1.Error_counter = 0;
		}
	}
	else if(AFC1.AFC_DIR==1) // tang tan so ngoai sai)
	{
		if(AFC1.Control_Value > 1) AFC1.Control_Value -= 1;
		else AFC1.Control_Value = AFC1.Control_MAX;
	}
	else if(AFC1.AFC_DIR==2) // tang tan so ngoai sai)
	{
		if(AFC1.Control_Value < AFC1.Control_MAX) AFC1.Control_Value += 1;
		else AFC1.Control_Value =0;
	}
}

void AFC_Mat_Dau(void)
{
	AFC1.Error_counter2 = 0;
	AFC1.Error_counter = 0;

	if(AFC1.AFC_DIR==0)
		{
			if(AFC1.Control_Value <= (AFC1.Control_MAX-5)) AFC1.Control_Value += 5;
			else AFC1.Control_Value = 0;
		}

	else if(AFC1.AFC_DIR==1) // tang tan so ngoai sai
		{
		if(AFC1.Control_Value >= 5) AFC1.Control_Value -= 5;
		else
			{
			AFC1.Control_Value = AFC1.Control_MAX;
			}
		}
	else if(AFC1.AFC_DIR==2)// giam tan so ngoai sai
		{
		if(AFC1.Control_Value <=(AFC1.Control_MAX-5)) AFC1.Control_Value += 5;
		else
			{
			AFC1.Control_Value = 0;
			}
		}
}

void AFC_Control(void)
{
	if(AFC1.Delay_time>0) AFC1.Delay_time--;

	if((AFC1.AFC_Active == 1)&&(AFC1.Delay_time==0))
	{
		if(AFC1.Led_Tot == 1) // dang o vi tri trung tam
		{
			AFC_Xung_Tot();
		}
		else if(AFC1.Led_Tot == 0) // nam vi tri trung tam
		{
			AFC_Fix_hold();

			/*****************************************************************************/
			// LECH PHAI
			if((AFC1.Level_Phai >= AFC1.U_Lech)&&
			   (AFC1.Level_Phai >= AFC1.Level_Trai)&&
			   (AFC1.AFC_done == 0)) // lech phai
			{
				AFC_Lech_Trai();
			}
			/*****************************************************************************/
			// LECH TRAI
			else if((AFC1.Level_Trai >= AFC1.U_Lech)&&
					(AFC1.Level_Trai >= AFC1.Level_Phai)&&
					(AFC1.AFC_done == 0)) // lech trai
			{
				AFC_Lech_Phai();
			}
			/*****************************************************************************/
			// MAT DAU
			else if((AFC1.Level_Phai < AFC1.U_Lech)&&
					(AFC1.Level_Trai < AFC1.U_Lech)&&
					(AFC1.AFC_done == 0)) // mat dau
			{
				AFC_Mat_Dau();
			}
		}
	}
	else if(AFC1.AFC_Active == 0)
	{

	}
}

void AFC_Get_Max(void)
{
	uint16_t i=0;

	AFC1.Max_Xung_Tot	= 0;
	AFC1.Max_Lech_Phai	= 0;
	AFC1.Max_Lech_Trai	= 0;

	for(i = 0; i < 60; i ++)
	{
		AFC1.Lech_Trai[15][i] = AFC1.Lech_Trai[14][i];	// du lieu truyen len PC
		AFC1.Lech_Trai[14][i] = AFC1.Lech_Trai[13][i];	// du lieu truyen len PC
		AFC1.Lech_Trai[13][i] = AFC1.Lech_Trai[12][i];	// du lieu truyen len PC
		AFC1.Lech_Trai[12][i] = AFC1.Lech_Trai[11][i];	// du lieu truyen len PC
		AFC1.Lech_Trai[11][i] = AFC1.Lech_Trai[10][i];	// du lieu truyen len PC
		AFC1.Lech_Trai[10][i] = AFC1.Lech_Trai[9][i];	// du lieu truyen len PC
		AFC1.Lech_Trai[9][i] = AFC1.Lech_Trai[8][i];	// du lieu truyen len PC
		AFC1.Lech_Trai[8][i] = AFC1.Lech_Trai[7][i];	// du lieu truyen len PC
		AFC1.Lech_Trai[7][i] = AFC1.Lech_Trai[6][i];	// du lieu truyen len PC
		AFC1.Lech_Trai[6][i] = AFC1.Lech_Trai[5][i];	// du lieu truyen len PC
		AFC1.Lech_Trai[5][i] = AFC1.Lech_Trai[4][i];	// du lieu truyen len PC
		AFC1.Lech_Trai[4][i] = AFC1.Lech_Trai[3][i];	// du lieu truyen len PC
		AFC1.Lech_Trai[3][i] = AFC1.Lech_Trai[2][i];	// du lieu truyen len PC
		AFC1.Lech_Trai[2][i] = AFC1.Lech_Trai[1][i];	// du lieu truyen len PC
		AFC1.Lech_Trai[1][i] = AFC1.Lech_Trai[0][i];	// du lieu truyen len PC
		AFC1.Lech_Trai[0][i] = Radar_Frame2[i * 4 + 223] * 256 + Radar_Frame2[i * 4 + 222];

		AFC1.Lech_Phai[15][i] = AFC1.Lech_Phai[14][i];	// du lieu truyen len PC
		AFC1.Lech_Phai[14][i] = AFC1.Lech_Phai[13][i];	// du lieu truyen len PC
		AFC1.Lech_Phai[13][i] = AFC1.Lech_Phai[12][i];	// du lieu truyen len PC
		AFC1.Lech_Phai[12][i] = AFC1.Lech_Phai[11][i];	// du lieu truyen len PC
		AFC1.Lech_Phai[11][i] = AFC1.Lech_Phai[10][i];	// du lieu truyen len PC
		AFC1.Lech_Phai[10][i] = AFC1.Lech_Phai[9][i];	// du lieu truyen len PC
		AFC1.Lech_Phai[9][i] = AFC1.Lech_Phai[8][i];	// du lieu truyen len PC
		AFC1.Lech_Phai[8][i] = AFC1.Lech_Phai[7][i];	// du lieu truyen len PC
		AFC1.Lech_Phai[7][i] = AFC1.Lech_Phai[6][i];	// du lieu truyen len PC
		AFC1.Lech_Phai[6][i] = AFC1.Lech_Phai[5][i];	// du lieu truyen len PC
		AFC1.Lech_Phai[5][i] = AFC1.Lech_Phai[4][i];	// du lieu truyen len PC
		AFC1.Lech_Phai[4][i] = AFC1.Lech_Phai[3][i];	// du lieu truyen len PC
		AFC1.Lech_Phai[3][i] = AFC1.Lech_Phai[2][i];	// du lieu truyen len PC
		AFC1.Lech_Phai[2][i] = AFC1.Lech_Phai[1][i];	// du lieu truyen len PC
		AFC1.Lech_Phai[1][i] = AFC1.Lech_Phai[0][i];	// du lieu truyen len PC
		AFC1.Lech_Phai[0][i] = Radar_Frame2[i * 4 + 221] * 256 + Radar_Frame2[i * 4 + 220];

		AFC1.Tot[15][i] 	= AFC1.Tot[14][i];	// du lieu truyen len PC
		AFC1.Tot[14][i] 	= AFC1.Tot[13][i];	// du lieu truyen len PC
		AFC1.Tot[13][i] 	= AFC1.Tot[12][i];	// du lieu truyen len PC
		AFC1.Tot[12][i] 	= AFC1.Tot[11][i];	// du lieu truyen len PC
		AFC1.Tot[11][i] 	= AFC1.Tot[10][i];	// du lieu truyen len PC
		AFC1.Tot[10][i] 	= AFC1.Tot[9][i];	// du lieu truyen len PC
		AFC1.Tot[9][i] 		= AFC1.Tot[8][i];	// du lieu truyen len PC
		AFC1.Tot[8][i] 		= AFC1.Tot[7][i];	// du lieu truyen len PC
		AFC1.Tot[7][i] 		= AFC1.Tot[6][i];	// du lieu truyen len PC
		AFC1.Tot[6][i] 		= AFC1.Tot[5][i];	// du lieu truyen len PC
		AFC1.Tot[5][i] 		= AFC1.Tot[4][i];	// du lieu truyen len PC
		AFC1.Tot[4][i] 		= AFC1.Tot[3][i];	// du lieu truyen len PC
		AFC1.Tot[3][i] 		= AFC1.Tot[2][i];	// du lieu truyen len PC
		AFC1.Tot[2][i] 		= AFC1.Tot[1][i];	// du lieu truyen len PC
		AFC1.Tot[1][i] 		= AFC1.Tot[0][i];	// du lieu truyen len PC
		AFC1.Tot[0][i] 		= Radar_Frame2[i * 2 + 101] * 256 + Radar_Frame2[i * 2 + 100];

		AFC1.Xung_Lech_Phai[i] = (	AFC1.Lech_Phai[0][i] + AFC1.Lech_Phai[1][i] + AFC1.Lech_Phai[2][i] + AFC1.Lech_Phai[3][i] +
									AFC1.Lech_Phai[4][i] + AFC1.Lech_Phai[5][i] + AFC1.Lech_Phai[6][i] + AFC1.Lech_Phai[7][i] +
									AFC1.Lech_Phai[8][i] + AFC1.Lech_Phai[9][i] + AFC1.Lech_Phai[10][i] + AFC1.Lech_Phai[11][i] +
									AFC1.Lech_Phai[12][i] + AFC1.Lech_Phai[13][i] + AFC1.Lech_Phai[14][i] + AFC1.Lech_Phai[15][i])/16;

		AFC1.Xung_Lech_Trai[i] = (	AFC1.Lech_Trai[0][i] + AFC1.Lech_Trai[1][i] + AFC1.Lech_Trai[2][i] + AFC1.Lech_Trai[3][i] +
									AFC1.Lech_Trai[4][i] + AFC1.Lech_Trai[5][i] + AFC1.Lech_Trai[6][i] + AFC1.Lech_Trai[7][i] +
									AFC1.Lech_Trai[8][i] + AFC1.Lech_Trai[9][i] + AFC1.Lech_Trai[10][i] + AFC1.Lech_Trai[11][i] +
									AFC1.Lech_Trai[12][i] + AFC1.Lech_Trai[13][i] + AFC1.Lech_Trai[14][i] + AFC1.Lech_Trai[15][i])/16;

		AFC1.Xung_Tot[i] = (AFC1.Tot[0][i] + AFC1.Tot[1][i] + AFC1.Tot[2][i] + AFC1.Tot[3][i] +
							AFC1.Tot[4][i] + AFC1.Tot[5][i] + AFC1.Tot[6][i] + AFC1.Tot[7][i] +
							AFC1.Tot[8][i] + AFC1.Tot[9][i] + AFC1.Tot[10][i] + AFC1.Tot[11][i] +
							AFC1.Tot[12][i] + AFC1.Tot[13][i] + AFC1.Tot[14][i] + AFC1.Tot[15][i])/16;

		if(AFC1.Xung_Tot[i] >= AFC1.Max_Xung_Tot) AFC1.Max_Xung_Tot = AFC1.Xung_Tot[i];
		if(AFC1.Xung_Lech_Phai[i] >= AFC1.Max_Lech_Phai) AFC1.Max_Lech_Phai = AFC1.Xung_Lech_Phai[i];
		if(AFC1.Xung_Lech_Trai[i] >= AFC1.Max_Lech_Trai) AFC1.Max_Lech_Trai = AFC1.Xung_Lech_Trai[i];
	}

	if(AFC1.Max_Xung_Tot  >= AFC1.U_nguong)
		{
			AFC1.Led_Tot = 1;

			if(AFC1.AFC_done_counter < 5)
				{
				AFC1.AFC_done_counter++;
				AFC1.AFC_done_counter = 0;
				}
			else
				{
				AFC1.AFC_done = 1;
				}
		}
	else
		{
		AFC1.Led_Tot = 0;

		if(AFC1.AFC_done_counter < 5)
			{
			AFC1.AFC_Fail_counter++;
			}
		else
			{
			AFC1.AFC_done = 0;
			AFC1.AFC_done_counter = 0;
			}
		}

	AFC1.Level_Phai = AFC1.Max_Lech_Phai / 16; // 128 level
	AFC1.Level_Trai = AFC1.Max_Lech_Trai / 16; // 128 level

	if(AFC1.Disconect == 1)
	{
		AFC1.AFC_Active = 1;
	}
	//AFC1.AVG_counter++;
	//if(AFC1.AVG_counter>=16) AFC1.AVG_counter	= 0;
}

void AFC_Innit(void)
{
	uint16_t counter = 0;

	for(counter = 0; counter < 60; counter ++)
	{
		AFC1.Xung_Lech_Phai[counter]=0;
		AFC1.Xung_Lech_Trai[counter]=0;
		AFC1.Xung_Tot[counter]=0;

		AFC1.Lech_Trai[0][counter] = 0;	// du lieu truyen len PC
		AFC1.Lech_Trai[1][counter] = 0;	// du lieu truyen len PC
		AFC1.Lech_Trai[2][counter] = 0;	// du lieu truyen len PC
		AFC1.Lech_Trai[3][counter] = 0;	// du lieu truyen len PC
		AFC1.Lech_Trai[4][counter] = 0;	// du lieu truyen len PC
		AFC1.Lech_Trai[5][counter] = 0;	// du lieu truyen len PC
		AFC1.Lech_Trai[6][counter] = 0;	// du lieu truyen len PC
		AFC1.Lech_Trai[7][counter] = 0;	// du lieu truyen len PC
		AFC1.Lech_Trai[8][counter] = 0;	// du lieu truyen len PC
		AFC1.Lech_Trai[9][counter] = 0;	// du lieu truyen len PC
		AFC1.Lech_Trai[10][counter] = 0;	// du lieu truyen len PC
		AFC1.Lech_Trai[11][counter] = 0;	// du lieu truyen len PC
		AFC1.Lech_Trai[12][counter] = 0;	// du lieu truyen len PC
		AFC1.Lech_Trai[13][counter] = 0;	// du lieu truyen len PC
		AFC1.Lech_Trai[14][counter] = 0;	// du lieu truyen len PC
		AFC1.Lech_Trai[15][counter] = 0;	// du lieu truyen len PC

		AFC1.Lech_Phai[0][counter] = 0;	// du lieu truyen len PC
		AFC1.Lech_Phai[1][counter] = 0;	// du lieu truyen len PC
		AFC1.Lech_Phai[2][counter] = 0;	// du lieu truyen len PC
		AFC1.Lech_Phai[3][counter] = 0;	// du lieu truyen len PC
		AFC1.Lech_Phai[4][counter] = 0;	// du lieu truyen len PC
		AFC1.Lech_Phai[5][counter] = 0;	// du lieu truyen len PC
		AFC1.Lech_Phai[6][counter] = 0;	// du lieu truyen len PC
		AFC1.Lech_Phai[7][counter] = 0;	// du lieu truyen len PC
		AFC1.Lech_Phai[8][counter] = 0;	// du lieu truyen len PC
		AFC1.Lech_Phai[9][counter] = 0;	// du lieu truyen len PC
		AFC1.Lech_Phai[10][counter] = 0;	// du lieu truyen len PC
		AFC1.Lech_Phai[11][counter] = 0;	// du lieu truyen len PC
		AFC1.Lech_Phai[12][counter] = 0;	// du lieu truyen len PC
		AFC1.Lech_Phai[13][counter] = 0;	// du lieu truyen len PC
		AFC1.Lech_Phai[14][counter] = 0;	// du lieu truyen len PC
		AFC1.Lech_Phai[15][counter] = 0;	// du lieu truyen len PC

		AFC1.Tot[0][counter] 	   	= 0;	// du lieu truyen len PC
		AFC1.Tot[1][counter] 		= 0;	// du lieu truyen len PC
		AFC1.Tot[2][counter] 		= 0;	// du lieu truyen len PC
		AFC1.Tot[3][counter] 		= 0;	// du lieu truyen len PC
		AFC1.Tot[4][counter] 		= 0;	// du lieu truyen len PC
		AFC1.Tot[5][counter] 		= 0;	// du lieu truyen len PC
		AFC1.Tot[6][counter] 		= 0;	// du lieu truyen len PC
		AFC1.Tot[7][counter] 		= 0;	// du lieu truyen len PC
		AFC1.Tot[8][counter] 		= 0;	// du lieu truyen len PC
		AFC1.Tot[9][counter] 		= 0;	// du lieu truyen len PC
		AFC1.Tot[10][counter] 		= 0;	// du lieu truyen len PC
		AFC1.Tot[11][counter] 		= 0;	// du lieu truyen len PC
		AFC1.Tot[12][counter] 		= 0;	// du lieu truyen len PC
		AFC1.Tot[13][counter] 		= 0;	// du lieu truyen len PC
		AFC1.Tot[14][counter] 		= 0;	// du lieu truyen len PC
		AFC1.Tot[15][counter] 		= 0;	// du lieu truyen len PC
	}

	AFC1.Update_AFC 	= 0;
	AFC1.Fnsai 			= 9380;
	AFC1.Fphat 			= 9410;
	AFC1.Max_Xung_Tot	= 0;
	AFC1.Max_Lech_Phai	= 0;
	AFC1.Max_Lech_Trai	= 0;
	AFC1.U_nguong 		= 540;
	AFC1.U_Lech			= 7;
	AFC1.Control_Value  = 860;
	AFC1.AFC_done_counter= 0;
	AFC1.AFC_Fail_counter= 0;
	AFC1.AFC_done       = 0;
	AFC1.AFC_DIR		= 0;
	AFC1.AFC_counter    = 0;
	AFC1.Error_counter  = 0;
	AFC1.AVG_counter	= 0;
	AFC1.Auto_Counter	= 0;
	AFC1.Auto_Start		= 0;
	AFC1.Disconect 		= 0;
	AFC1.AFC_Active		= 1;
	AFC1.Control_MAX	= 1300;
	AFC1.Max_Top 		= 900;
	AFC1.Min_Top 		= 500;
	AFC1.Delay_time		= 0;
	AFC1.Fix_time		= 0;
	AFC1.Fix_time2		= 0;
	AFC1.Fix_DIR		= 0;
}
