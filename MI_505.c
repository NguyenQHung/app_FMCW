/*
 * MI_505.c
 *
 *  Created on: Apr 7, 2020
 *      Author: TienNam
 */
#include "DK_Maythu.h"
#include "AFC.h"
#include "ADF4159.h"
#include "ADF4360.h"

extern struct Radar_Struct Radar;
extern struct Frame_Struct Frame;
extern struct PC8_def		PC8;
extern struct PC8_def		PC8s;
extern struct AFC_Struct AFC1;
extern struct Radar_Debug_Struct 	Radar_Debug;
extern char* Frame_CMD;

unsigned int Noise_Counter=0;

extern unsigned char Radar_Frame1[Data_length];
extern XGpio  GPIO_ENDFRAME_IRQ;		//	XPAR_ENDFRAME_IRQ_DEVICE_ID
extern XGpio  GPIO_HS1;				//	GPIO_HS1
extern XGpio  GPIO_HS2;				//	GPIO_HS2
extern XGpio  GPIO_HS3;				//	GPIO_HS3

void Radar_Hyper_Output(unsigned int Channel, unsigned int Data)
{
	XGpio_DiscreteWrite(&GPIO_HS1, 1, Channel);
	usleep(5);
	XGpio_DiscreteWrite(&GPIO_HS1, 2, Data);
	usleep(5);
	XGpio_DiscreteWrite(&GPIO_HS1, 1, Channel | 0x0200);
	usleep(5);
	XGpio_DiscreteWrite(&GPIO_HS1, 1, Channel);
	usleep(5);
}

unsigned int Radar_Hyper_Input(unsigned char Channel)
{
	unsigned int Data = 0;
	XGpio_DiscreteWrite(&GPIO_HS2, 1, Channel);
	usleep(5);
	XGpio_DiscreteWrite(&GPIO_HS2, 1, Channel | 0x0100);
	usleep(5);
	Data = XGpio_DiscreteRead(&GPIO_HS2, 2);
	usleep(5);
	XGpio_DiscreteWrite(&GPIO_HS2, 1, Channel);

	return Data;
}

void Motor1_Mode_Freq(unsigned int Freq, unsigned char Dir, unsigned char Enable)
{
	Radar_Hyper_Output(Hyper_Output_Erob_Freq, Freq);
	Radar_Hyper_Output(Hyper_Output_Erob_Dir_Enable, Enable * 2 + Dir);
}

void Motor1_Mode_Pulse(unsigned int Pulse, unsigned char Dir, unsigned char Enable)
{
	Radar_Hyper_Output(Number_Pulse1, Pulse);
	Radar_Hyper_Output(Pulse_Enable1, Enable);
}

void Motor1_Set_Run_Mode(unsigned char Mode)
{
	Radar_Hyper_Output(Erob_mode1, Mode);
}

void Motor1_Set_Mode(unsigned int ENC_Center, unsigned int ENC_Start, unsigned int ENC_Stop)
{
	Radar_Hyper_Output(Erob_start1, ENC_Start);
	Radar_Hyper_Output(Erob_Center1, ENC_Center);
	Radar_Hyper_Output(Erob_stop1, ENC_Stop);
}

void Motor1_Send_ENC(unsigned int ENC)
{
	Radar_Hyper_Output(Erob_Encoder1, ENC);
}


void Motor2_Mode_Freq(unsigned int Freq, unsigned char Dir, unsigned char Enable)
{
	Radar_Hyper_Output(Hyper_Output_Erob_Freq2, Freq);
	Radar_Hyper_Output(Hyper_Output_Erob_Dir_Enable2, Enable * 2 + Dir);
}

void Motor2_Mode_Pulse(unsigned int Pulse, unsigned char Dir, unsigned char Enable)
{
	Radar_Hyper_Output(Number_Pulse2, Pulse);
	Radar_Hyper_Output(Pulse_Enable2, Enable);
}

void Motor2_Set_Run_Mode(unsigned char Mode)
{
	Radar_Hyper_Output(Erob_mode2, Mode);
}

void Motor2_Set_Mode(unsigned int ENC_Center, unsigned int ENC_Start, unsigned int ENC_Stop)
{
	Radar_Hyper_Output(Erob_start2, ENC_Start);
	Radar_Hyper_Output(Erob_Center2, ENC_Center);
	Radar_Hyper_Output(Erob_stop2, ENC_Stop);
}

void Motor2_Send_ENC(unsigned int ENC)
{
	Radar_Hyper_Output(Erob_Encoder2, ENC);
}

void CAN1_Set_Dir(unsigned int Dir)
{
	Radar_Hyper_Output(Hyper_Output_CAN_Dir, Dir);
}

void CAN2_Set_Dir(unsigned int Dir)
{
	Radar_Hyper_Output(Hyper_Output_CAN_Dir2, Dir);
}

void IP_Set_RST(unsigned int Val)
{
	if(Val == 1) Radar_Hyper_Output(IP_rst, 0);
	else if(Val == 0) Radar_Hyper_Output(IP_rst, 1);
}

void IP_Set_Range(unsigned int Val)
{
	Radar_Hyper_Output(IP_Range, Val);
}

void IP_Set_DAC_Mode(unsigned int _IP_SW_mode, unsigned int _IP_target_Mode, float Scale)
{
	Radar_Hyper_Output(IP_SW, _IP_SW_mode);
	Radar_Hyper_Output(IP_target_Mode, _IP_target_Mode);
	IP_Set_amplitude(Scale);
}

void IP_Set_dmt(unsigned int Val)
{
	Radar_Hyper_Output(dmt, Val);
	Radar_Hyper_Output(dmt1, Val + 65536);
}

void IP_Set_amplitude(float Scale)
{
	unsigned int Val = Scale * 32767;
	Radar_Hyper_Output(IP_amplitude, Val);
}

void IP_Set_thang_culy(unsigned int Val)
{
	Radar_Hyper_Output(thang_culy, Val);
}

void IP_Set_Beta_Mode(unsigned int Val)
{
	Radar_Hyper_Output(Beta_mode, Val);
}

void IP_Set_Beta_in(unsigned int Val)
{
	Radar_Hyper_Output(Beta_in, Val);
}

void IP_Set_Beta_start(unsigned int Val)
{
	Radar_Hyper_Output(Beta_start, Val);
}

void IP_Set_Beta_stop(unsigned int Val)
{
	Radar_Hyper_Output(Beta_stop, Val);
}

void System_Set_RS485_DIR1(unsigned int Val)
{
	Radar_Hyper_Output(RS485_DIR1, Val);
}

void System_Set_RS485_DIR2(unsigned int Val)
{
	Radar_Hyper_Output(RS485_DIR2, Val);
}

void System_Set_RF_ENABLE(unsigned int Val)
{
	Radar_Hyper_Output(RF_ON_OFF, Val);
}

void System_Set_RF_ON_OFF(unsigned int Val)
{
	System_Set_RF_ON_OFF2(Val);

	//if(Val == 0) XGpioPs_WritePin(&psgpio, ADF4360_CE, 0);
	//else XGpioPs_WritePin(&psgpio, ADF4360_CE, 1);
}


void System_Set_IQ_View(unsigned int Val)
{
	Radar_Hyper_Output(Mixer_View, Val);
}

void System_Set_Mixer_Sel(unsigned int Val)
{
	if(Val == 0) // txsig
	{
		Radar_Hyper_Output(IP_Mixer_Sel, 0);
	}
	else if((Val >= 1)&&(Val <= 16)) // ADC1 - ADC16
	{
		Radar_Hyper_Output(IP_Mixer_Sel, 2);
	}
	else if(Val == 17) // Mixer
	{
		Radar_Hyper_Output(IP_Mixer_Sel, 1);
	}
	else if(Val == 18) // Mul
	{
		Radar_Hyper_Output(IP_Mixer_Sel, 1);
	}
	else if(Val == 19) // TXSIG
	{
		Radar_Hyper_Output(IP_Mixer_Sel, 3);
	}
}

void System_Set_Trigger_time1(unsigned int Val)
{
	Radar_Hyper_Output(Trigger_time1, Val);
}

void System_Set_Trigger_time2(unsigned int Val)
{
	Radar_Hyper_Output(Trigger_time2, Val);
}

void System_Set_ADC_SEL(unsigned int Val)
{
	Radar_Hyper_Output(ADC_Sel, Val);
}

void System_Set_ADC_SEL2(unsigned int Val)
{
	Radar_Hyper_Output(ADC_Sel2, Val);
}


void System_Set_ATT_Dmax(unsigned int Val)
{
	Radar_Hyper_Output(ATT_Dmax, Val);
}


void System_Set_ATT_TX_Sel(unsigned int Val)
{
	Radar_Hyper_Output(TX_SEL, Val);
}


void System_Set_RX_SEL(unsigned int Val)
{
	//Radar_Hyper_Output(RX_SEL, Val);
}


void System_Set_ADC_Channel(unsigned int Val)
{
	Radar_Hyper_Output(ADC_View_Channel, Val);
}

void System_Set_IQ_Channel1(unsigned int Val)
{
	Radar_Hyper_Output(IQ_Channel_View1, Val);
}


void System_Set_IQ_Channel2(unsigned int Val)
{
	Radar_Hyper_Output(IQ_Channel_View2, Val);
}

void System_Set_IP_code(unsigned int Val)
{
	Radar_Hyper_Output(IP_code, Val);
}


void System_Set_RDY_EN(unsigned int Val)
{
	Radar_Hyper_Output(RDY_EN, Val);
}

void System_Set_Beta_Epxilon_sel(unsigned int Val)
{
	Radar_Hyper_Output(Beta_Epxilon_sel, Val);
}

void System_Set_BS_Enable(unsigned int Val)
{
	Radar_Hyper_Output(BSS_Enable, Val);
}

void System_Set_BS_Angle(unsigned int Val)
{
	Radar_Hyper_Output(BSS_Angle, Val);
}

void System_Set_BS_Start(unsigned int Val)
{
	Radar_Hyper_Output(BSS_Start, Val);
}

void System_Set_BS_Stop(unsigned int Val)
{
	Radar_Hyper_Output(BSS_Stop, Val);
}

void System_Set_BS_Start4(unsigned int Val)
{
	Radar_Hyper_Output(BSS_Start4, Val);
}

void System_Set_BS_Stop4(unsigned int Val)
{
	Radar_Hyper_Output(BSS_Stop4, Val);
}

void System_Set_BS_Size4(unsigned int Val)
{
	Radar_Hyper_Output(BS_Size4, Val);
}

void System_Set_Scope_Delay(unsigned int Val)
{
	Radar_Hyper_Output(Scope_Delay, Val);
}

//#define	Beam_start 						110
//#define	Beam_stop 						111
//#define	Beam_size 						112
//#define	Beam_Max_Cell 					113

void System_Set_Beam_start(unsigned int Val)
{
	Radar_Hyper_Output(Beam_start, Val);
}

void System_Set_Beam_stop(unsigned int Val)
{
	Radar_Hyper_Output(Beam_stop, Val);
}

void System_Set_Beam_size(unsigned int Val)
{
	Radar_Hyper_Output(Beam_size, Val);
}

void System_Set_Beam_Max_Cell(unsigned int Val)
{
	Radar_Hyper_Output(Beam_Max_Cell, Val);
}


void System_Set_Beam17_Bamsat(unsigned int Thang_cu_ly)
{
	unsigned int Delay_CLK = 5120;

	if(Thang_cu_ly == 1)
	{
		Delay_CLK = 0;// 5120 + 9 * 256;
	}
	else if(Thang_cu_ly == 2)
	{
		Delay_CLK = 0;// 20480 + 9 * 1024;
	}
	else if(Thang_cu_ly == 3)
	{
		Delay_CLK = 0;// 40960 + 9 * 2048;
	}
	else if(Thang_cu_ly == 4)
	{
		Delay_CLK = 0;// 81920 + 9 * 4096;
	}
	else if(Thang_cu_ly == 5)
	{
		Delay_CLK = 0;// 163840 + 19 * 4096;
	}

	System_Set_Scope_Delay(Delay_CLK);//Delay_CLK);

}

void System_Set_Beam17_RAM(unsigned int Val)
{
	if(Val == 1)
	{
	System_Set_Beam_Max_Cell(10200); // max = 327755
	}
	else if(Val == 2)
	{
	System_Set_Beam_Max_Cell(41000); // max = 327755
	}
	else if(Val == 3)
	{
	System_Set_Beam_Max_Cell(81000); // max = 327755
	}
	else if(Val == 4)
	{
	System_Set_Beam_Max_Cell(163900); // max = 327755
	}
	else if(Val == 5)
	{
	System_Set_Beam_Max_Cell(327700); // max = 327755
	}
}

void System_Set_BS_Channel(unsigned int Val)
{
	Radar_Hyper_Output(BSS_Channel, Val);
}

void System_Set_BS_Sel(unsigned int Val)
{
	Radar_Hyper_Output(BS_Sel, Val);
}

void System_Set_BS_Size(unsigned int Val)
{
	Radar_Hyper_Output(BS_Size, Val);
}

void System_Set_Beam_Sel(unsigned int Val)
{
	Radar_Hyper_Output(Beam_Sel, Val);
}

void System_Set_Beam_Channel(unsigned int Val)
{
	Radar_Hyper_Output(Beam_Channel, Val);
}

void System_Set_fb_Channel(unsigned int Val)
{
	Radar_Hyper_Output(fb_Channel, Val);
}

void System_Set_Phase_Start_Read(unsigned int Val)
{
	Radar_Hyper_Output(Phase_Start_Read, Val);
}

void System_Set_FIFO_RST(unsigned int Val)
{
	Radar_Hyper_Output(FIFO_EN, Val);
}

void System_Set_IP_B17_Sel(unsigned int Val)
{
	Radar_Hyper_Output(IP_B17_Sel, Val);
}

void System_Set_IP_Bx_Sel(unsigned int Val)
{
	Radar_Hyper_Output(IP_Bx_Sel, Val);
}

void System_Set_IP_code_Sellect(unsigned int Val)
{
	Radar_Hyper_Output(IP_code_Sellect, Val);
}

void System_Set_RF_ON_OFF2(unsigned int Val)
{
	Radar_Hyper_Output(RF_ON_OFF2, Val);
}

void System_Set_IP_Pulse_Width(unsigned int Val)
{
	//Radar_Hyper_Output(IP_PulseWidth, Val + 800);
}

void System_Set_IP_PulseSel(unsigned int Val)
{
	//Radar_Hyper_Output(IP_PulseSel, Val);
}

void System_Set_FFT_Sel(unsigned int Val)
{
	Radar_Hyper_Output(_FFT_Sel, Val);
}

void System_Set_sum_Sel(unsigned int Val)
{
	Radar_Hyper_Output(_Sum_Sel, Val);
}

void System_Set_trigger(unsigned int mode, unsigned int Width, unsigned int Max)
{
	Radar_Hyper_Output(Trigger_Max, Max);
	Radar_Hyper_Output(Trigger_Width, Width);
	Radar_Hyper_Output(Trigger_Mode, mode);
}

void System_Set_ATT_Data(	unsigned int Val1,
							unsigned int Val2,
							unsigned int Val3,
							unsigned int Val4,
							unsigned int Val5,
							unsigned int Val6,
							unsigned int Val7,
							unsigned int Val8,
							unsigned int Val9,
							unsigned int Val10,
							unsigned int Val11,
							unsigned int Val12,
							unsigned int Val13,
							unsigned int Val14,
							unsigned int Val15,
							unsigned int Val16,
							unsigned int Val17)
{
	Radar_Hyper_Output(ATT_DATA1, Val1);
	Radar_Hyper_Output(ATT_DATA2, Val2);
	Radar_Hyper_Output(ATT_DATA3, Val3);
	Radar_Hyper_Output(ATT_DATA4, Val4);
	Radar_Hyper_Output(ATT_DATA5, Val5);
	Radar_Hyper_Output(ATT_DATA6, Val6);
	Radar_Hyper_Output(ATT_DATA7, Val7);
	Radar_Hyper_Output(ATT_DATA8, Val8);
	Radar_Hyper_Output(ATT_DATA9, Val9);
	Radar_Hyper_Output(ATT_DATA10, Val10);
	Radar_Hyper_Output(ATT_DATA11, Val11);
	Radar_Hyper_Output(ATT_DATA12, Val12);
	Radar_Hyper_Output(ATT_DATA13, Val13);
	Radar_Hyper_Output(ATT_DATA14, Val14);
	Radar_Hyper_Output(ATT_DATA15, Val15);
	Radar_Hyper_Output(ATT_DATA16, Val16);
	Radar_Hyper_Output(ATT_DATA17, Val17);
}

void Radar_Cal_ADC_LPF(unsigned char Freq)
{

	if(Freq == 0)
	{
	//Freq = 10MHz;
	Radar_Debug.kL0 = 1;
	Radar_Debug.bL1 = -1.2796324;
	Radar_Debug.bL2 = 0.4775922;
	Radar_Debug.aL0 = 0.04949;
	Radar_Debug.aL1 = 0.0989799;
	Radar_Debug.aL2 = 0.04949;
	}
	else if(Freq == 1)
	{
	//Freq = 8MHz;
	Radar_Debug.kL0 = 1;
	Radar_Debug.bL1 = -1.4189826;
	Radar_Debug.bL2 = 0.5532699;
	Radar_Debug.aL0 = 0.0335718;
	Radar_Debug.aL1 = 0.0671436;
	Radar_Debug.aL2 = 0.0335718;
	}
	else if(Freq == 2)
	{
	//Freq = 6.8MHz;
	xil_printf("LPF = 6.8MHz\n\r");
	Radar_Debug.kL0 = 1;
	Radar_Debug.bL1 = -1.5038889;
	Radar_Debug.bL2 = 0.6045213;
	Radar_Debug.aL0 = 0.0251581;
	Radar_Debug.aL1 = 0.0503162;
	Radar_Debug.aL2 = 0.0251581;
	}
	else if(Freq == 3)
	{
	//Freq = 5.6MHz;
		xil_printf("LPF = 6.8MHz\n\r");
	Radar_Debug.kL0 = 1;
	Radar_Debug.bL1 = -1.5897348;
	Radar_Debug.bL2 = 0.6606105;
	Radar_Debug.aL0 = 0.0177189;
	Radar_Debug.aL1 = 0.0354378;
	Radar_Debug.aL2 = 0.0177189;
	}
	else if(Freq == 4)
	{
	//Freq = 4.7MHz;
		xil_printf("LPF = 4.7MHz\n\r");
	Radar_Debug.kL0 = 1;
	Radar_Debug.bL1 = -1.654702;
	Radar_Debug.bL2 = 0.7061035;
	Radar_Debug.aL0 = 0.0128504;
	Radar_Debug.aL1 = 0.0257007;
	Radar_Debug.aL2 = 0.0128504;
	}
	else if(Freq == 5)
	{
	//Freq = 3.9MHz;
		xil_printf("LPF = 3.9MHz\n\r");
	Radar_Debug.kL0 = 1;
	Radar_Debug.bL1 = -1.7128379;
	Radar_Debug.bL2 = 0.749181;
	Radar_Debug.aL0 = 0.0090858;
	Radar_Debug.aL1 = 0.0181715;
	Radar_Debug.aL2 = 0.0090858;
	}
	else if(Freq == 6)
	{
	//Freq = 3.3MHz;
		xil_printf("LPF = 3.3MHz\n\r");
	Radar_Debug.kL0 = 1;
	Radar_Debug.bL1 = -1.7566578;
	Radar_Debug.bL2 = 0.783211;
	Radar_Debug.aL0 = 0.0066383;
	Radar_Debug.aL1 = 0.0132766;
	Radar_Debug.aL2 = 0.0066383;
	}
	else if(Freq == 7)
	{
	//Freq = 2.7MHz;
		xil_printf("LPF = 2.7MHz\n\r");
	Radar_Debug.kL0 = 1;
	Radar_Debug.bL1 = -1.8006451;
	Radar_Debug.bL2 = 0.8187899;
	Radar_Debug.aL0 = 0.0045362;
	Radar_Debug.aL1 = 0.0090724;
	Radar_Debug.aL2 = 0.0045362;
	}
	else if(Freq == 8)
	{
	//Freq = 2.2MHz;
		xil_printf("LPF = 2.2MHz\n\r");
	Radar_Debug.kL0 = 1;
	Radar_Debug.bL1 = -1.8374137;
	Radar_Debug.bL2 = 0.8496719;
	Radar_Debug.aL0 = 0.0030646;
	Radar_Debug.aL1 = 0.0061291;
	Radar_Debug.aL2 = 0.0030646;
	}
	else if(Freq == 9)
	{
	//Freq = 1.2MHz;
		xil_printf("LPF = 1.2MHz\n\r");
	Radar_Debug.kL0 = 1;
	Radar_Debug.bL1 = -1.9111971;
	Radar_Debug.bL2 = 0.9149758;
	Radar_Debug.aL0 = 0.0009447;
	Radar_Debug.aL1 = 0.0018894;
	Radar_Debug.aL2 = 0.0009447;
	}
	else if(Freq == 10)
	{
	//Freq = 1MHz;
		xil_printf("LPF = 1MHz\n\r");
	Radar_Debug.kL0 = 1;
	Radar_Debug.bL1 = -1.925984;
	Radar_Debug.bL2 = 0.9286271;
	Radar_Debug.aL0 = 0.0006608;
	Radar_Debug.aL1 = 0.0013216;
	Radar_Debug.aL2 = 0.0006608;
	}
	else if(Freq == 11)
	{
	//Freq = 0.82MHz;
		xil_printf("LPF = 0.82MHz\n\r");
	Radar_Debug.kL0 = 1;
	Radar_Debug.bL1 = -1.9392984;
	Radar_Debug.bL2 = 0.9410873;
	Radar_Debug.aL0 = 0.0004472;
	Radar_Debug.aL1 = 0.0008944;
	Radar_Debug.aL2 = 0.0004472;
	}
	else if(Freq == 12)
	{
	//Freq = 0.68MHz;
		xil_printf("LPF = 0.68MHz\n\r");
	Radar_Debug.kL0 = 1;
	Radar_Debug.bL1 = -1.9496576;
	Radar_Debug.bL2 = 0.950894;
	Radar_Debug.aL0 = 0.0003091;
	Radar_Debug.aL1 = 0.0006182;
	Radar_Debug.aL2 = 0.0003091;
	}
	else if(Freq == 13)
	{
	//Freq = 0.56MHz;
		xil_printf("LPF = 0.56MHz\n\r");
	Radar_Debug.kL0 = 1;
	Radar_Debug.bL1 = -1.9585389;
	Radar_Debug.bL2 = 0.9593811;
	Radar_Debug.aL0 = 0.0002106;
	Radar_Debug.aL1 = 0.0004211;
	Radar_Debug.aL2 = 0.0002106;
	}
	else if(Freq == 14)
	{
	//Freq = 0.47MHz;
		xil_printf("LPF = 0.47MHz\n\r");
	Radar_Debug.kL0 = 1;
	Radar_Debug.bL1 = -1.9652008;
	Radar_Debug.bL2 = 0.9657961;
	Radar_Debug.aL0 = 0.0001488;
	Radar_Debug.aL1 = 0.0002976;
	Radar_Debug.aL2 = 0.0001488;
	}
	else if(Freq == 15)
	{
	//Freq = 0.39MHz;
		xil_printf("LPF = 0.39MHz\n\r");
	Radar_Debug.kL0 = 1;
	Radar_Debug.bL1 = -1.9711232;
	Radar_Debug.bL2 = 0.9715343;
	Radar_Debug.aL0 = 0.0001028;
	Radar_Debug.aL1 = 0.0002055;
	Radar_Debug.aL2 = 0.0001028;
	}
	else if(Freq == 16)
	{
	//Freq = 0.33MHz;
	Radar_Debug.kL0 = 1;
	Radar_Debug.bL1 = -1.9755653;
	Radar_Debug.bL2 = 0.9758603;
	Radar_Debug.aL0 = 7.37E-05;
	Radar_Debug.aL1 = 0.0001475;
	Radar_Debug.aL2 = 7.37E-05;
	}

}



void Radar_SET_ADC_LPF_Val(void)
{

}

int IIR_LPF(int32_t v)
{
	Radar_Debug.X1 = (v * Radar_Debug.kL0 - (Radar_Debug.IIZ1 * Radar_Debug.bL2 + Radar_Debug.IIZ0 * Radar_Debug.bL1));
	Radar_Debug.X = (Radar_Debug.IIZ1 * Radar_Debug.aL2 + Radar_Debug.IIZ0 * Radar_Debug.aL1 + Radar_Debug.X1 * Radar_Debug.aL0);
	Radar_Debug.IIZ1 = Radar_Debug.IIZ0;
	Radar_Debug.IIZ0 = Radar_Debug.X1;

	return (int)(Radar_Debug.X);
}


unsigned int gray_to_binary(unsigned int gray)
{
    unsigned int result = gray & 64;
    result |= (gray ^ (result >> 1)) & 32;
    result |= (gray ^ (result >> 1)) & 16;
    result |= (gray ^ (result >> 1)) & 8;
    result |= (gray ^ (result >> 1)) & 4;
    result |= (gray ^ (result >> 1)) & 2;
    result |= (gray ^ (result >> 1)) & 1;
    return result;
}

unsigned int grayToBinary(unsigned int num)
{
unsigned int temp = num ^ (num>>8);
temp ^= (temp>>4);
temp ^= (temp>>2);
temp ^= (temp>>1);
return temp;
}

unsigned int Radar_Read_Beta(void)
{
	unsigned int Read_Input_reg=0;
	//Read_Input_reg=XGpio_DiscreteRead(&GPIO_ENC_Data, 1);
	return Read_Input_reg;
}

unsigned int Radar_Read_Alpha(void)
{
	unsigned int Read_Input_reg=0;
	//Read_Input_reg=XGpio_DiscreteRead(&GPIO_ENC_Data, 2);
	return Read_Input_reg;
}

unsigned int Radar_Read_Input(void)
{
	unsigned int Read_Input_reg=0;
	Read_Input_reg=XGpio_DiscreteRead(&GPIO_ENDFRAME_IRQ, 1);
	return Read_Input_reg;
}

unsigned int Radar_Read_Phuongvi(void)
{
	unsigned int Beta=0;
	Beta = 0;//XGpio_DiscreteRead(&GPIO_BETA, 1);
	return Beta;
}

void Radar_Read_Volt(void)
{
	uint32_t Data_in1 = 0;
	uint32_t Data_in2 = 0;
	uint32_t Data_in3 = 0;
	uint32_t Data_in4 = 0;

	//XGpio_DiscreteWrite(&GPIO_Volt_Mea, 1,0);
	Data_in1 = 0;//XGpio_DiscreteRead(&GPIO_Volt_Mea, 2);
	usleep(1);

	//XGpio_DiscreteWrite(&GPIO_Volt_Mea, 1,1);
	Data_in2 = 0;//XGpio_DiscreteRead(&GPIO_Volt_Mea, 2);
	usleep(1);

	//XGpio_DiscreteWrite(&GPIO_Volt_Mea, 1,2);
	Data_in3 = 0;//XGpio_DiscreteRead(&GPIO_Volt_Mea, 2);
	usleep(1);

	//XGpio_DiscreteWrite(&GPIO_Volt_Mea, 1,3);
	Data_in4 = 0;//XGpio_DiscreteRead(&GPIO_Volt_Mea, 2);
	usleep(1);

	Radar.Volt_Data[0] = (Data_in1 & 0x00000fff);
	Radar.Volt_Data[1] = (Data_in1 & 0x0fff0000) >> 16;

	Radar.Volt_Data[2] = (Data_in2 & 0x00000fff);
	Radar.Volt_Data[3] = (Data_in2 & 0x0fff0000) >> 16;

	Radar.Volt_Data[4] = (Data_in3 & 0x00000fff);
	Radar.Volt_Data[5] = (Data_in3 & 0x0fff0000) >> 16;

	Radar.Volt_Data[6] = (Data_in4 & 0x0000ffff);
	Radar.Volt_Data[7] = (Data_in4 & 0x0fff0000) >> 16;
}

void Radar_Anten_Speed(void)
{
	uint32_t Speed1 = 0;
	Speed1 = 0;//XGpio_DiscreteRead(&GPIO_SPEED, 2);	// 20.	ENCODER: Encoder_Data[11:0]

	xil_printf("d= %d ms\n\r",Speed1/10);

	if(Speed1>0)
	{
		Frame.Radar_Speed = 6000000 / Speed1;
	}
	else
	{
		Frame.Radar_Speed = 10;
	}
}

void Radar_Set_Angle_Mode(	uint16_t Angle_Start,
							uint16_t Angle_Stop,
							uint8_t	 Enable)
{
	//XGpio_DiscreteWrite(&GPIO_TX_G1, 1, Angle_Start);
	//XGpio_DiscreteWrite(&GPIO_TX_G1, 2, Angle_Stop);
	//XGpio_DiscreteWrite(&GPIO_TX_G2, 2, Enable);
}

void Radar_Sellect_Trigger(unsigned char Channel)
{
	//XGpio_DiscreteWrite(&GPIO_TX_G2, 1, Channel);
}

void Radar_Set_Pulse(unsigned int Pulse_width)
{
	//XGpio_DiscreteWrite(&GPIO_TX_G3, 1, Pulse_width);
}

void Radar_SET_Tan_So_Lap(unsigned int Tlap)
{
	PC8.Ts_Val = 120000000 / Tlap;
	//XGpio_DiscreteWrite(&GPIO_TX_G3, 2, PC8.Ts_Val);
}

void Radar_Show_Freq(void)
{

}

void Radar_Update_VCO(void)
{
	ADF4159_Set_Freq(7200);
	ADF4360_Set_Freq(1400);
}
void Radar_Update_AFC(void)
{

	AFC_Control();

	if(Radar.Radar_2_2A == 1) Radar.May2_2A_Buf = 0; else Radar.May2_2A_Buf = 1;
	if(Radar.DK1 == 1) Radar.ATTG_Buf = 0; else Radar.ATTG_Buf = 1;

	if(Radar.TC_TX==0) // 0 :  dieu khien tu xa
		{
			//Radar_Set_ouput(Radar.Phat, Radar.Soi_Dot, AFC1.Led_Tot);
//			Radar_Set_ouput2(Radar.Phat,
//							 May2_2A_Buf,// May 2/2A
//							 ATTG_Buf, // Anten/Tai gia
//							 AFC1.Led_Tot,
//							 Radar.Soi_Dot,
//							 Radar.DK4);
		}
	if(Radar.TC_TX==1) // 1 :  dieu khien tai cho
		{
			//Radar_Set_ouput(0, 0, AFC1.Led_Tot);
//			Radar_Set_ouput2(0, 0, 0, AFC1.Led_Tot, 0, 0);
		}


	if(AFC1.AFC_Active == 1)
	{
		if(Radar.Ngoai_sai0 != AFC1.Control_Value)
		{
			//xil_printf("NS = %d\n\r",Radar.Ngoai_sai/2 + 8950);
			//usleep(200);
			Radar.Ngoai_sai0 = AFC1.Control_Value;
			//ADF_4113_Write(AFC1.Control_Value/2 + 8950, AFC1.Control_Value % 2);
			//ADF4159_Set_Freq(AFC1.Control_Value + 11200);
			ADF4360_Set_Freq(AFC1.Control_Value + ADF4360_Freq0);
			//ADF4360_Set_Freq(ADF4360_Freq0);
			//usleep(2);
		}
	}
}

void Radar_Update_Chong_Nhieu(void)
{
	if(Radar.Chong_Nhieu == 1)
	{
		Radar.Sum2	= 0;
		for(Noise_Counter = 0; Noise_Counter < (Radar.Sample_AVG + 100);Noise_Counter++)
		{
			Radar.Sum2 += Radar_Frame1[Radar.Sample - Noise_Counter];
		}

		Radar.Sum = Radar.Sum +  Radar.Sum2 / (Radar.Sample_AVG + 100);

		Radar.Chong_Nhieu_Counter++;
		if(Radar.Chong_Nhieu_Counter >= 50)
		{
			Radar.Chong_Nhieu_Counter 	= 0;
			Radar.AVG_Value 			= Radar.Sum / 50;
			Radar.Sum 					= 0;
		}
	}
	else
	{
		Radar.AVG_Value 			= 0;
		Radar.Chong_Nhieu_Counter 	= 0;
		Radar.Sum 					= 0;
	}
}
