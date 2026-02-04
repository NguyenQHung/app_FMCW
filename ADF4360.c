/*
 * ADF4360.c
 *
 *  Created on: Jun 27, 2024
 *      Author: tienn
 */

#include "ADF4360.h"
#include "linux_hal.h"      // Lớp trừu tượng phần cứng (sysfs GPIO)

extern XGpioPs  psgpio;

struct ADF4360_def
{
	unsigned int Update_time;//	= 0;//Frefin / Reference_counter;//
	unsigned int Update_Mode;//	= 0;//Frefin / Reference_counter;//
	unsigned int Bit3[24];
	unsigned int ADF4360_F[3];
	unsigned long int PFD;//	= 0;//Frefin / Reference_counter;//
	unsigned long int N;//		= 0;//(Freq * 1000000) / PFD;// N = (B x P) + A
	unsigned long int Frefin;//= 10000000;// Clk = 10 MHz

	// R0: Control Latch
	// 0: 8/9
	// 1: 16/17
	// 2: 32/33
	// 3: 32/33
	unsigned int RF_Prescaler_Value;// = 1; // 1: 16/17
	unsigned int Prescaler;// = 1; // 1: 16/17

	// CE PD2  PD1
	// 0   X    X : ASYNCHRONOUS POWER-DOWN
	// 1   X    0 : NORMAL OPERATION
	// 1   0    1 : ASYNCHRONOUS POWER-DOWN
	// 1   1    1 : SYNCHRONOUS POWER-DOWN
	unsigned int Power_down;// = 0; // 1   X    0 : NORMAL OPERATION

	// CPI6 CPI5 CPI4 (4.7K): Current_setting2
	//  0    0    0 : Icp = 0.31
	//  0    0    1 : Icp = 0.62
	//  0    1    0 : Icp = 0.93
	//  0    1    1 : Icp = 1.25
	//  1    0    0 : Icp = 1.56
	//  1    0    1 : Icp = 1.87
	//  1    1    0 : Icp = 2.18
	//  1    1    1 : Icp = 2.50
	unsigned int Current_setting2;// = 7; //  1    1    1 : Icp = 2.50

	// CPI3 CPI2 CPI1 (4.7K): Current_setting1
	//  0    0    0 : Icp = 0.31
	//  0    0    1 : Icp = 0.62
	//  0    1    0 : Icp = 0.93
	//  0    1    1 : Icp = 1.25
	//  1    0    0 : Icp = 1.56
	//  1    0    1 : Icp = 1.87
	//  1    1    0 : Icp = 2.18
	//  1    1    1 : Icp = 2.50
	unsigned int Current_setting1;// = 7; //  1    1    1 : Icp = 2.50

	// PL2 PL1 : Current	Output_Power_level (50R)
	//  0   0  :  3.5mA		-14dBm
	//  0   1  :  5.0mA		-11dBm
	//  1   0  :  7.5mA		-8dBm
	//  1   1  :  11.0mA	-5dBm
	unsigned int Output_Power_level;// = 3; // 11mA, -5dBm

	//MTLD 	MUTE-TILL-LOCK DETECT
	// 0 	: DISABLED
	// 1 	: ENABLED
	unsigned int Multil_LD;// = 0;// 0: DISABLED

	// CPG 		CP GAIN
	//  0 	: CURRENT SETTING 1
	//  1 	: CURRENT SETTING 2
	unsigned int CP_Gain1;// = 0;// 0: CURRENT SETTING 1

	// CP 	CHARGE PUMP OUTPUT
	// 0	: NORMAL
	// 1	: THREE-STATE
	unsigned int CP_three_state;//	= 0; // 0	: NORMAL

	//PDP	PHASE DETECTOR POLARITY
	// 0	: NEGATIVE
	// 1	: POSITIVE
	unsigned int Phase_Detector_Polarity;// = 1;// 1: POSITIVE

	// M3 M2 M1 OUTPUT
	// 0  0  0 : THREE-STATE OUTPUT
	// 0  0  1 : DIGITAL LOCK DETECT (ACTIVE HIGH)
	// 0  1  0 : N DIVIDER OUTPUT
	// 0  1  1 : DVDD
	// 1  0  0 : R DIVIDER OUTPUT
	// 1  0  1 : N-CHANNEL OPEN-DRAIN LOCK DETECT
	// 1  1  0 : SERIAL DATA OUTPUT
	// 1  1  1 : DGND
	unsigned int MUX_out;// = 1; // 0  0  1 : DIGITAL LOCK DETECT (ACTIVE HIGH)

	// CR	COUNTER OPERATION
	// 0	: NORMAL
	// 1	: R, A, B COUNTERS HELD IN RESET
	unsigned int Counter_Reset;// = 0; // 0: NORMAL

	// PC2 PC1 CORE POWER LEVEL
	// 0   0   : 5mA
	// 0   1   : 10mA
	// 1   0   : 15mA
	// 1   1   : 20mA
	unsigned int Core_Power_Level;// = 3; // 0   0 : 5mA

	// R1: N Counter Latch

	// DIVSEL	DIVIDE-BY-2 SELECT (PRESCALER INPUT)
	// 0		: FUNDAMENTAL OUTPUT SELECTED
	// 1		: DIVIDE-BY-2 SELECTED
	unsigned int Divide_by_2_select;// = 0; // 0: FUNDAMENTAL OUTPUT SELECTED

	// DIV2		DIVIDE-BY-2
	// 0		: FUNDAMENTAL OUTPUT
	// 1		: DIVIDE-BY-2
	unsigned int Divide_by_2;// = 0; // 0 : FUNDAMENTAL OUTPUT

	// (F4 FASTLOCK ENABLE) CP_GAIN 	OPERATION
	//	0					0			: CHARGE PUMP CURRENT SETTING 1 IS PERMANENTLY USED
	//	0					1			: CHARGE PUMP CURRENT SETTING 2 IS PERMANENTLY USED
	unsigned int CP_Gain2;// = 0;//	0	0: CHARGE PUMP CURRENT SETTING 1

	//	// B COUNTER DIVIDE RATIO: 3 -> 8191
	unsigned int B_Counter;
	//
	//	// A COUNTER DIVIDE RATIO: 0 -> 31
	unsigned int A_counter;

	// N = B x Prescal + A
	// Freq = N x Fref

	// R2: R Counter Latch

	//BSC2 BSC1 BAND SELECT CLOCK DIVIDER
	// 0    0	: /1
	// 0    1	: /2
	// 1    0	: /4
	// 1    1	: /8
	unsigned int Band_Select_Clock;// = 3;// 1    1	: /8

	// 0 : TEST MODE BIT SHOULD BE SET TO 0 FOR NORMAL OPERATION
	unsigned int Test_mode_bit;// = 0;

	// LDP LOCK DETECT PRECISION
	// 0 : THREE CONSECUTIVE CYCLES OF PHASE DELAY LESS THAN 15ns MUST OCCUR BEFORE LOCK DETECT IS SET.
	// 1 : FIVE CONSECUTIVE CYCLES OF PHASE DELAY LESS THAN 15ns MUST OCCUR BEFORE LOCK DETECT IS SET.
	unsigned int Lock_detect_precision;// = 0; // 0 : THREE CONSECUTIVE CYCLES

	// ABP2 ABP1 ANTIBACKLASH PULSE WIDTH
	// 0    0	 : 3.0ns
	// 0    1	 : 1.3ns
	// 1    0	 : 6.0ns
	// 1    1	 : 3.0ns
	unsigned int Anti_Backlash_pulse_width;//	= 0; // 0    0: 3.0ns

	// DIVIDE RATIO : 1 - 16383
	unsigned int Reference_counter;// 	= 50;
};

struct ADF4360_def ADF4360_struct;

void ADF4360_Set_Freq2(unsigned int Freq) // MHz
{
	if(Freq <= 900) Freq = 900;
	if(Freq >= 1500) Freq = 1500;

	ADF4360_struct.PFD	= ADF4360_struct.Frefin / ADF4360_struct.Reference_counter;//
	ADF4360_struct.N	= (Freq * 1000000) / ADF4360_struct.PFD;// N = (B x P) + A

	//#region Calculate A and B
	//Rcounter = REFin * 1000 / PFDFreq;
	//N = (Rcounter * RFout) / REFin;
	//B = (int)N / P;
	//A = (int)N - (B * P);

	ADF4360_struct.B_Counter   = ADF4360_struct.N / ADF4360_struct.Prescaler;
	ADF4360_struct.A_counter   = ADF4360_struct.N - (ADF4360_struct.B_Counter * ADF4360_struct.Prescaler);

	if(ADF4360_struct.B_Counter >= 8191) ADF4360_struct.B_Counter = 8191;

	// control latch
	ADF4360_struct.ADF4360_F[1]	= 	ADF4360_struct.RF_Prescaler_Value 		* 0x400000 +
									ADF4360_struct.Power_down   			* 0x100000 +
									ADF4360_struct.Current_setting2			* 0x020000 +
									ADF4360_struct.Current_setting1			* 0x004000 +
									ADF4360_struct.Output_Power_level		* 0x001000 +
									ADF4360_struct.Multil_LD				* 0x000800 +
									ADF4360_struct.CP_Gain1					* 0x000400 +
									ADF4360_struct.CP_three_state			* 0x000200 +
									ADF4360_struct.Phase_Detector_Polarity	* 0x000100 +
									ADF4360_struct.MUX_out					* 0x000020 +
									ADF4360_struct.Counter_Reset			* 0x000010 +
									ADF4360_struct.Core_Power_Level			* 0x000004 +
															  	  	  	  	  0x000000; // control bit = 00
	// N counter
	ADF4360_struct.ADF4360_F[2]	= 	ADF4360_struct.Divide_by_2_select		* 0x800000 +
									ADF4360_struct.Divide_by_2				* 0x400000 +
									ADF4360_struct.CP_Gain2					* 0x200000 +
									ADF4360_struct.B_Counter				* 0x000100 +
									ADF4360_struct.A_counter				* 0x000004 +
											  	  	  	  	  	  	  	  	  0x000002; // control bit = 10
	// R counter
	ADF4360_struct.ADF4360_F[0]	= 	ADF4360_struct.Band_Select_Clock		 * 0x100000 +
									ADF4360_struct.Test_mode_bit			 * 0x080000 +
									ADF4360_struct.Lock_detect_precision	 * 0x040000 +
									ADF4360_struct.Anti_Backlash_pulse_width * 0x010000 +
									ADF4360_struct.Reference_counter		 * 0x000004 +
											  	   	   	   	   	   	   	   	   0x000001; // control bit = 01

	if(ADF4360_struct.Update_time < 5)
	{
		ADF4360_struct.Update_time++;
		ADF4360_struct.Update_Mode = 0;
	}
	else if(ADF4360_struct.Update_time >= 5)
	{
		//ADF4360_struct.Update_time++;
		ADF4360_struct.Update_Mode = 1;
	}

	ADF4360_Write();
}

void ADF4360_Set_Freq(unsigned int Freq) // MHz
{
	if(Freq <= 900) Freq = 900;
	if(Freq >= 1500) Freq = 1500;

	ADF4360_struct.PFD	= ADF4360_struct.Frefin / ADF4360_struct.Reference_counter;//
	ADF4360_struct.N	= (Freq * 1000000) / ADF4360_struct.PFD;// N = (B x P) + A

	//#region Calculate A and B
	//Rcounter = REFin * 1000 / PFDFreq;
	//N = (Rcounter * RFout) / REFin;
	//B = (int)N / P;
	//A = (int)N - (B * P);

	ADF4360_struct.B_Counter   = ADF4360_struct.N / ADF4360_struct.Prescaler;
	ADF4360_struct.A_counter   = ADF4360_struct.N - (ADF4360_struct.B_Counter * ADF4360_struct.Prescaler);

	if(ADF4360_struct.B_Counter >= 8191) ADF4360_struct.B_Counter = 8191;

	// control latch
	ADF4360_struct.ADF4360_F[1]	= 	ADF4360_struct.RF_Prescaler_Value 		* 0x400000 +
									ADF4360_struct.Power_down   			* 0x100000 +
									ADF4360_struct.Current_setting2			* 0x020000 +
									ADF4360_struct.Current_setting1			* 0x004000 +
									ADF4360_struct.Output_Power_level		* 0x001000 +
									ADF4360_struct.Multil_LD				* 0x000800 +
									ADF4360_struct.CP_Gain1					* 0x000400 +
									ADF4360_struct.CP_three_state			* 0x000200 +
									ADF4360_struct.Phase_Detector_Polarity	* 0x000100 +
									ADF4360_struct.MUX_out					* 0x000020 +
									ADF4360_struct.Counter_Reset			* 0x000010 +
									ADF4360_struct.Core_Power_Level			* 0x000004 +
															  	  	  	  	  0x000000; // control bit = 00
	// N counter
	ADF4360_struct.ADF4360_F[2]	= 	ADF4360_struct.Divide_by_2_select		* 0x800000 +
									ADF4360_struct.Divide_by_2				* 0x400000 +
									ADF4360_struct.CP_Gain2					* 0x200000 +
									ADF4360_struct.B_Counter				* 0x000100 +
									ADF4360_struct.A_counter				* 0x000004 +
											  	  	  	  	  	  	  	  	  0x000002; // control bit = 10
	// R counter
	ADF4360_struct.ADF4360_F[0]	= 	ADF4360_struct.Band_Select_Clock		 * 0x100000 +
									ADF4360_struct.Test_mode_bit			 * 0x080000 +
									ADF4360_struct.Lock_detect_precision	 * 0x040000 +
									ADF4360_struct.Anti_Backlash_pulse_width * 0x010000 +
									ADF4360_struct.Reference_counter		 * 0x000004 +
											  	   	   	   	   	   	   	   	   0x000001; // control bit = 01


	ADF4360_struct.Update_time = 0;
	ADF4360_Write();
}

void ADF4360_Innit(void)
{
	//XGpioPs_WritePin(&psgpio, ADF4360_CE, 1); // cho chip hoat dong
	//XGpioPs_WritePin(&psgpio, ADF4360_CLK, 0);
	//XGpioPs_WritePin(&psgpio, ADF4360_DATA, 0);
	//XGpioPs_WritePin(&psgpio, ADF4360_LE, 0);

	ADF4360_struct.Bit3[0] = 0x800000; // bit 23
	ADF4360_struct.Bit3[1] = 0x400000; // bit 22
	ADF4360_struct.Bit3[2] = 0x200000; // bit 21
	ADF4360_struct.Bit3[3] = 0x100000; // bit 20
	ADF4360_struct.Bit3[4] = 0x080000; // bit 19
	ADF4360_struct.Bit3[5] = 0x040000; // bit 18
	ADF4360_struct.Bit3[6] = 0x020000; // bit 17
	ADF4360_struct.Bit3[7] = 0x010000; // bit 16
	ADF4360_struct.Bit3[8] = 0x008000; // bit 15
	ADF4360_struct.Bit3[9] = 0x004000; // bit 14
	ADF4360_struct.Bit3[10]= 0x002000; // bit 13
	ADF4360_struct.Bit3[11]= 0x001000; // bit 12
	ADF4360_struct.Bit3[12]= 0x000800; // bit 11
	ADF4360_struct.Bit3[13]= 0x000400; // bit 10
	ADF4360_struct.Bit3[14]= 0x000200; // bit 9
	ADF4360_struct.Bit3[15]= 0x000100; // bit 8
	ADF4360_struct.Bit3[16]= 0x000080; // bit 7
	ADF4360_struct.Bit3[17]= 0x000040; // bit 6
	ADF4360_struct.Bit3[18]= 0x000020; // bit 5
	ADF4360_struct.Bit3[19]= 0x000010; // bit 3
	ADF4360_struct.Bit3[20]= 0x000008; // bit 3
	ADF4360_struct.Bit3[21]= 0x000004; // bit 2
	ADF4360_struct.Bit3[22]= 0x000002; // bit 1
	ADF4360_struct.Bit3[23]= 0x000001; // bit 0

	ADF4360_struct.Update_time = 0;
	ADF4360_struct.Update_Mode = 0;
	ADF4360_struct.PFD		= 0;//Frefin / Reference_counter;//
	ADF4360_struct.N		= 0;//(Freq * 1000000) / PFD;// N = (B x P) + A
	ADF4360_struct.Frefin	= 100000000;// Clk = 10 MHz

	// R0: Control Latch
	// 0: 8/9
	// 1: 16/17
	// 2: 32/33
	// 3: 32/33
	ADF4360_struct.RF_Prescaler_Value	 = 0; // 1: 16/17

	if (ADF4360_struct.RF_Prescaler_Value == 0)
	{
		ADF4360_struct.Prescaler	= 8;
	}
	else if (ADF4360_struct.RF_Prescaler_Value == 1)
	{
		ADF4360_struct.Prescaler	= 16;
	}
	else if (ADF4360_struct.RF_Prescaler_Value == 2)
	{
		ADF4360_struct.Prescaler	= 32;
	}
	else if (ADF4360_struct.RF_Prescaler_Value == 3)
	{
		ADF4360_struct.Prescaler	= 32;
	}

	// CE PD2  PD1
	// 0   X    X : ASYNCHRONOUS POWER-DOWN
	// 1   X    0 : NORMAL OPERATION
	// 1   0    1 : ASYNCHRONOUS POWER-DOWN
	// 1   1    1 : SYNCHRONOUS POWER-DOWN
	ADF4360_struct.Power_down	 = 0; // 1   X    0 : NORMAL OPERATION

	// CPI6 CPI5 CPI4 (4.7K): Current_setting2
	//  0    0    0 : Icp = 0.31
	//  0    0    1 : Icp = 0.62
	//  0    1    0 : Icp = 0.93
	//  0    1    1 : Icp = 1.25
	//  1    0    0 : Icp = 1.56
	//  1    0    1 : Icp = 1.87
	//  1    1    0 : Icp = 2.18
	//  1    1    1 : Icp = 2.50
	ADF4360_struct.Current_setting2	 = 7; //  1    1    1 : Icp = 2.50

	// CPI3 CPI2 CPI1 (4.7K): Current_setting1
	//  0    0    0 : Icp = 0.31
	//  0    0    1 : Icp = 0.62
	//  0    1    0 : Icp = 0.93
	//  0    1    1 : Icp = 1.25
	//  1    0    0 : Icp = 1.56
	//  1    0    1 : Icp = 1.87
	//  1    1    0 : Icp = 2.18
	//  1    1    1 : Icp = 2.50
	ADF4360_struct.Current_setting1	 = 7; //  1    1    1 : Icp = 2.50

	// PL2 PL1 : Current	Output_Power_level (50R)
	//  0   0  :  3.5mA		-14dBm
	//  0   1  :  5.0mA		-11dBm
	//  1   0  :  7.5mA		-8dBm
	//  1   1  :  11.0mA	-5dBm
	ADF4360_struct.Output_Power_level	 = 3; // 11mA, -5dBm

	//MTLD 	MUTE-TILL-LOCK DETECT
	// 0 	: DISABLED
	// 1 	: ENABLED
	ADF4360_struct.Multil_LD	 = 0;// 0: DISABLED

	// CPG 		CP GAIN
	//  0 	: CURRENT SETTING 1
	//  1 	: CURRENT SETTING 2
	ADF4360_struct.CP_Gain1	 = 0;// 0: CURRENT SETTING 1

	// CP 	CHARGE PUMP OUTPUT
	// 0	: NORMAL
	// 1	: THREE-STATE
	ADF4360_struct.CP_three_state		= 0; // 0	: NORMAL

	//PDP	PHASE DETECTOR POLARITY
	// 0	: NEGATIVE
	// 1	: POSITIVE
	ADF4360_struct.Phase_Detector_Polarity	 = 1;// 1: POSITIVE

	// M3 M2 M1 OUTPUT
	// 0  0  0 : THREE-STATE OUTPUT
	// 0  0  1 : DIGITAL LOCK DETECT (ACTIVE HIGH)
	// 0  1  0 : N DIVIDER OUTPUT
	// 0  1  1 : DVDD
	// 1  0  0 : R DIVIDER OUTPUT
	// 1  0  1 : N-CHANNEL OPEN-DRAIN LOCK DETECT
	// 1  1  0 : SERIAL DATA OUTPUT
	// 1  1  1 : DGND
	ADF4360_struct.MUX_out	 = 1; // 0  0  1 : DIGITAL LOCK DETECT (ACTIVE HIGH)

	// CR	COUNTER OPERATION
	// 0	: NORMAL
	// 1	: R, A, B COUNTERS HELD IN RESET
	ADF4360_struct.Counter_Reset	 = 0; // 0: NORMAL

	// PC2 PC1 CORE POWER LEVEL
	// 0   0   : 5mA
	// 0   1   : 10mA
	// 1   0   : 15mA
	// 1   1   : 20mA
	ADF4360_struct.Core_Power_Level	 = 3; // 0   0 : 5mA

	// R1: N Counter Latch

	// DIVSEL	DIVIDE-BY-2 SELECT (PRESCALER INPUT)
	// 0		: FUNDAMENTAL OUTPUT SELECTED
	// 1		: DIVIDE-BY-2 SELECTED
	ADF4360_struct.Divide_by_2_select = 0; // 0: FUNDAMENTAL OUTPUT SELECTED

	// DIV2		DIVIDE-BY-2
	// 0		: FUNDAMENTAL OUTPUT
	// 1		: DIVIDE-BY-2
	ADF4360_struct.Divide_by_2	 = 0; // 0 : FUNDAMENTAL OUTPUT

	// (F4 FASTLOCK ENABLE) CP_GAIN 	OPERATION
	//	0					0			: CHARGE PUMP CURRENT SETTING 1 IS PERMANENTLY USED
	//	0					1			: CHARGE PUMP CURRENT SETTING 2 IS PERMANENTLY USED
	ADF4360_struct.CP_Gain2	 = 0;//	0	0: CHARGE PUMP CURRENT SETTING 1

	//	// B COUNTER DIVIDE RATIO: 3 -> 8191
	//	unsigned int B_Counter = 0;
	//
	//	// A COUNTER DIVIDE RATIO: 0 -> 31
	//	unsigned int A_counter = 0;

	// N = B x Prescal + A
	// Freq = N x Fref

	// R2: R Counter Latch

	//BSC2 BSC1 BAND SELECT CLOCK DIVIDER
	// 0    0	: /1
	// 0    1	: /2
	// 1    0	: /4
	// 1    1	: /8
	ADF4360_struct.Band_Select_Clock	 = 3;// 1    1	: /8

	// 0 : TEST MODE BIT SHOULD BE SET TO 0 FOR NORMAL OPERATION
	ADF4360_struct.Test_mode_bit	 = 0;

	// LDP LOCK DETECT PRECISION
	// 0 : THREE CONSECUTIVE CYCLES OF PHASE DELAY LESS THAN 15ns MUST OCCUR BEFORE LOCK DETECT IS SET.
	// 1 : FIVE CONSECUTIVE CYCLES OF PHASE DELAY LESS THAN 15ns MUST OCCUR BEFORE LOCK DETECT IS SET.
	ADF4360_struct.Lock_detect_precision	 = 0; // 0 : THREE CONSECUTIVE CYCLES

	// ABP2 ABP1 ANTIBACKLASH PULSE WIDTH
	// 0    0	 : 3.0ns
	// 0    1	 : 1.3ns
	// 1    0	 : 6.0ns
	// 1    1	 : 3.0ns
	ADF4360_struct.Anti_Backlash_pulse_width	= 0; // 0    0: 3.0ns

	// DIVIDE RATIO : 1 - 16383
	ADF4360_struct.Reference_counter 	= 100;//10;

	//ADF4360_Set_Freq(ADF4360_Freq0);// 12000 MHz = 12.0 Ghz

}

void ADF4360_CE_PIN(unsigned char Val)
{
	if(Val == 0) XGpioPs_WritePin(&psgpio, ADF4360_CE, 0);
	else XGpioPs_WritePin(&psgpio, ADF4360_CE, 1);
}

void ADF4360_Write(void)
{
    unsigned char spi_buf[3]; // 3 bytes = 24 bits
    uint32_t reg_val;
    int row;

    // Đảm bảo chân CE đã được kích hoạt (nếu bạn điều khiển riêng bằng EMIO)
    // XGpioPs_WritePin(&psgpio, ADF4360_CE, 1); 

    if(ADF4360_struct.Update_Mode == 0) // Chế độ nạp toàn bộ 3 thanh ghi
    {
        for(row = 0; row < 3; row++)
        {
            reg_val = ADF4360_struct.ADF4360_F[row];

            /* 
               Chuẩn bị 24 bit dữ liệu. 
               Gửi MSB (Most Significant Bit) trước.
            */
            spi_buf[0] = (unsigned char)((reg_val >> 16) & 0xFF);
            spi_buf[1] = (unsigned char)((reg_val >> 8) & 0xFF);
            spi_buf[2] = (unsigned char)(reg_val & 0xFF);

            /* 
               Gửi qua SPI phần cứng. 
               Driver sẽ tự động: Hạ LE -> Gửi 24 bit -> Kéo LE lên để CHỐT.
            */
		     int Status1 = SPI_Transfer( Pl_Spi3.fd, spi_buf,NULL, 3);
            //Setup_SPI_Processing_Linux(Pl_Spi3.fd, spi_buf, 3);

            // Nghỉ một chút để chip xử lý (thay cho usleep(200) cũ)
            usleep(500); 
        }
        //xil_printf("ADF4360: All 3 registers updated.\r\n");
    }
    else if(ADF4360_struct.Update_Mode == 1) // Chế độ chỉ cập nhật thanh ghi N (Register 2)
    {
        reg_val = ADF4360_struct.ADF4360_F[2];

        spi_buf[0] = (unsigned char)((reg_val >> 16) & 0xFF);
        spi_buf[1] = (unsigned char)((reg_val >> 8) & 0xFF);
        spi_buf[2] = (unsigned char)(reg_val & 0xFF);

		int Status1 = SPI_Transfer(&Pl_Spi3, Pl_Spi3.fd, spi_buf, 3);
        //Setup_SPI_Processing_Linux(Pl_Spi3.fd, spi_buf, 3);
        
        //xil_printf("ADF4360: N-Register (R2) updated.\r\n");
    }
}

// void ADF4360_Write(void)
// {
// 	unsigned char counter=0, row=0;

// 	XGpioPs_WritePin(&psgpio, ADF4360_CLK, 0);
// 	XGpioPs_WritePin(&psgpio, ADF4360_DATA, 0);
// 	XGpioPs_WritePin(&psgpio, ADF4360_LE, 0);
// 	//XGpioPs_WritePin(&psgpio, ADF4360_CE, 1);
// 	usleep(3);

// 	if(ADF4360_struct.Update_Mode == 0) // ghi du 3 thanh ghi
// 	{
// 		// ghi du lieu
// 		for(row = 0; row < 3; row++)
// 		{
// 			//XGpioPs_WritePin(&PMOD, ADF4360_CE, 0);
// 			XGpioPs_WritePin(&psgpio, ADF4360_LE, 0);	// dua chuan LE muc 0
// 			for(counter = 0; counter < 24; counter++)
// 			{
// 				// gui data
// 				if((ADF4360_struct.ADF4360_F[row] & ADF4360_struct.Bit3[counter])==0)
// 					XGpioPs_WritePin(&psgpio, ADF4360_DATA, 0); // data = 0
// 				else
// 					XGpioPs_WritePin(&psgpio, ADF4360_DATA, 1); // data = 1

// 				usleep(5);
// 				XGpioPs_WritePin(&psgpio, ADF4360_CLK, 1); // clk=1
// 				usleep(3);
// 				XGpioPs_WritePin(&psgpio, ADF4360_CLK, 0);	// Clk =0
// 				usleep(7);
// 			}

// 			//XGpioPs_WritePin(&psgpio, ADF4360_DATA, 0);
// 			XGpioPs_WritePin(&psgpio, ADF4360_CLK, 0);
// 			usleep(5);
// 			//XGpioPs_WritePin(&PMOD, ADF4360_CE, 1);
// 			XGpioPs_WritePin(&psgpio, ADF4360_LE, 1); // chot du lieu
// 			usleep(4);
// 			XGpioPs_WritePin(&psgpio, ADF4360_LE, 0);
// 			usleep(200);
// 			//XGpioPs_WritePin(&PMOD, ADF4360_CE, 0);
// 		}
// 	}
// 	else if(ADF4360_struct.Update_Mode == 1) // ghi: N resister
// 	{
// 		//XGpioPs_WritePin(&PMOD, ADF4360_CE, 0);
// 		XGpioPs_WritePin(&psgpio, ADF4360_LE, 0);	// dua chuan LE muc 0
// 		for(counter = 0; counter < 24; counter++)
// 		{
// 			// gui data
// 			if((ADF4360_struct.ADF4360_F[2] & ADF4360_struct.Bit3[counter])==0)
// 				XGpioPs_WritePin(&psgpio, ADF4360_DATA, 0); // data = 0
// 			else
// 				XGpioPs_WritePin(&psgpio, ADF4360_DATA, 1); // data = 1

// 			usleep(5);
// 			XGpioPs_WritePin(&psgpio, ADF4360_CLK, 1); // clk=1
// 			usleep(3);
// 			XGpioPs_WritePin(&psgpio, ADF4360_CLK, 0);	// Clk =0
// 			usleep(7);
// 		}

// 		//XGpioPs_WritePin(&psgpio, ADF4360_DATA, 0);
// 		XGpioPs_WritePin(&psgpio, ADF4360_CLK, 0);
// 		usleep(5);
// 		//XGpioPs_WritePin(&PMOD, ADF4360_CE, 1);
// 		XGpioPs_WritePin(&psgpio, ADF4360_LE, 1); // chot du lieu
// 		usleep(4);
// 		XGpioPs_WritePin(&psgpio, ADF4360_LE, 0);
// 		usleep(4);
// 		//XGpioPs_WritePin(&PMOD, ADF4360_CE, 0);
// 	}


// 	XGpioPs_WritePin(&psgpio, ADF4360_LE, 0);
// 	//XGpioPs_WritePin(&psgpio, ADF4360_DATA, 0);
// 	XGpioPs_WritePin(&psgpio, ADF4360_CLK, 0);
// 	//XGpioPs_WritePin(&psgpio, ADF4360_CE, 1);
// }

