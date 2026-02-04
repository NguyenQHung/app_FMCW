/*
 * Noise.c
 *
 *  Created on: Jun 17, 2020
 *      Author: TienNam
 */
#include "DK_Maythu.h"
#include "Noise.h"
extern struct Radar_Struct Radar;
extern struct Frame_Struct Frame;
extern struct PC8_def		PC8;
extern struct PC8_def		PC8s;

extern char*  Frame_CMD;
//
unsigned char		Noise_Map[55656000];
unsigned char		Noise_Map_in[15460];
unsigned char		Noise_Map_out[15460]; //  5120
extern unsigned char Radar_Frame1[Data_length];

void Radar_Noise_Process(void)
{
	uint16_t Ni = 0;
	uint32_t counter=0;
	//unsigned long int counter=0;
	//Radar.SW_RS2=0;
	//if(Radar.Range<=2) Radar.Noise_Sample = Radar.Sample - 100;
	//if(Radar.Range>=3) Radar.Noise_Sample = 2700;

	Radar.Noise_Sample = Radar.Sample - 100;
	//if(Radar.Noise_Sample>=5120) Radar.Noise_Sample=5120;
	// chi xu ly khi co sai khac phuong vi
	if((Radar.Noise_Clear ==1)&&(Radar.Noise_Pvi != PC8.Beta_Angle))
		{
		Radar.Noise_counter++;
		if(Radar.Noise_counter >= 3600)
			{
				Radar.Noise_Clear = 0;
				Radar.Noise_counter=0;
			}
		}

	Radar.Noise_Pvi = Radar.Beta_HW;
	Radar.Noise_Pvi1 = Radar.Beta_HW + 1;
	if(Radar.Noise_Pvi1 >= 3600)Radar.Noise_Pvi1 = 0;

	counter = Radar.Noise_Pvi * 15460;

	/*
	//memcpy(&Noise_Map_in[0], &Radar_Frame1[100], Radar.Noise_Sample);
	counter = Radar.Noise_Pvi * 5120;


	*/

	if(Radar.Noise_Clear==0)
	{
		memcpy(&Noise_Map_in[0], &Radar_Frame1[100], Radar.Noise_Sample);

		for(Ni = 0; Ni < Radar.Noise_Sample; Ni++)
			{
			if(Radar_Frame1[Ni+100]>=Noise_Map[Ni + counter])
				Radar_Frame1[Ni+100] = Radar_Frame1[Ni+100] - Noise_Map[Ni + counter];//Noise_Map[Radar.Noise_Pvi1][Radar.Ni];
			else
				Radar_Frame1[Ni+100] = Noise_Map[Ni + counter] - Radar_Frame1[Ni+100];
			}

		memcpy(&Noise_Map[Radar.Beta_HW * 15460], &Noise_Map_in[0], Radar.Noise_Sample);
	}
	else
	{
		memcpy(&Noise_Map[Radar.Beta_HW * 15460], &Radar_Frame1[100], Radar.Noise_Sample);
	}

}

void Radar_Noise_Innit()
{
	unsigned long int counter=0;
	Radar.Radar_Frame[0] = 0xaa;
	Radar.Noise_counter = 0;
	Radar.Noise_Clear = 0;
	Radar.Noise_Pvi=0;
	Radar.Bando_Nhieu=0;

	for (counter = 0; counter < 15460; counter++)
		{
			Noise_Map_in[counter] = 0x00;//8f

		}

		for (counter = 0; counter < 55656000; counter++)
		{
				Noise_Map[counter] = 0x00;//8f
		}
}

