#include "DK_Maythu.h"
#include "ADF4159.h"
#include "ADF4360.h"
#include "Radar_HW.h"
#include "MI_505.h"
#include "Erob.h"
#include "CAN_Control.h"
#include "linux_hal.h"      // Lớp trừu tượng phần cứng (sysfs GPIO)

/****************************************************************************/
// void Radar_Intr_Handler(void *baseaddr_p)
// {
// 	XGpio *GpioPtr = (XGpio *)baseaddr_p;
// 	/* Clear the Interrupt */
// 	XGpio_InterruptClear(GpioPtr, XGPIO_IR_CH1_MASK);

// 	//(void) XGpio_InterruptClear(&GPIO_ENDFRAME_IRQ, XGPIO_IR_CH1_MASK);
// 	if(Radar.Config_Done == 1)
// 	{
// 		Radar.Read_ok = 1;
// 	}
// }

// /****************************************************************************/
// void Radar_Intr_Handler2(void *baseaddr_p)
// {
// 	XGpio *GpioPtr = (XGpio *)baseaddr_p;
// 	/* Clear the Interrupt */
// 	XGpio_InterruptClear(GpioPtr, XGPIO_IR_CH1_MASK);

// 	//(void) XGpio_InterruptClear(&GPIO_ENDFRAME_IRQ, XGPIO_IR_CH1_MASK);
// 	if(Radar.Config_Done == 1)
// 	{
// 		Radar.Read_ok = 1;
// 	}
// }

// /****************************************************************************/
// void Radar_IP_Intr_Handler(void *baseaddr_p)
// {
// 	//XGpio *GpioPtr = (XGpio *)baseaddr_p;
// 	/* Clear the Interrupt */
// 	//XGpio_InterruptClear(GpioPtr, XGPIO_IR_CH1_MASK);

// 	//(void) XGpio_InterruptClear(&GPIO_ENDFRAME_IRQ, XGPIO_IR_CH1_MASK);
// 	if(Radar.Config_Done == 1)
// 	{
// 		Radar.Read_ok = 1;
// 	}
// }

// /*****************************************************************************/
// /**
// * This static function handles ZDMA Done interrupts.
// *
// * @param	CallBackRef is the callback reference passed from the interrupt
// *		handler, which in our case is a pointer to the driver instance.
// *
// * @return	None.
// *
// * @note		None.
// *
// ******************************************************************************/
// void ZDMA1_DoneHandler(void *CallBackRef)
// {

// 	//ZDMA_Done1 = 1;

// }

// /*****************************************************************************/
// /**
// * This static function handles ZDMA Done interrupts.
// *
// * @param	CallBackRef is the callback reference passed from the interrupt
// *		handler, which in our case is a pointer to the driver instance.
// *
// * @return	None.
// *
// * @note		None.
// *
// ******************************************************************************/
// void ZDMA2_DoneHandler(void *CallBackRef)
// {

// 	//ZDMA_Done2 = 1;

// }

// /*****************************************************************************/
// /**
// * This static function handles ZDMA Done interrupts.
// *
// * @param	CallBackRef is the callback reference passed from the interrupt
// *		handler, which in our case is a pointer to the driver instance.
// *
// * @return	None.
// *
// * @note		None.
// *
// ******************************************************************************/
// void ZDMA3_DoneHandler(void *CallBackRef)
// {

// 	//ZDMA_Done3 = 1;

// }

// /*****************************************************************************/
// /**
// * This static function handles ZDMA Done interrupts.
// *
// * @param	CallBackRef is the callback reference passed from the interrupt
// *		handler, which in our case is a pointer to the driver instance.
// *
// * @return	None.
// *
// * @note		None.
// *
// ******************************************************************************/
// void ZDMA4_DoneHandler(void *CallBackRef)
// {

// 	//ZDMA_Done4 = 1;

// }

// int Radar_InterruptSystemSetup(XScuGic *XScuGicInstancePtr)
// {
// 	// Xil_ExceptionInit();

// 	// XGpio_InterruptEnable(&GPIO_ENDFRAME_IRQ, XGPIO_IR_CH1_MASK);
// 	// XGpio_InterruptGlobalEnable(&GPIO_ENDFRAME_IRQ);

// 	// XGpio_InterruptEnable(&GPIO_ENDFRAME_IRQ1, XGPIO_IR_CH1_MASK);
// 	// XGpio_InterruptGlobalEnable(&GPIO_ENDFRAME_IRQ1);

// 	// Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
// 	// 		(Xil_ExceptionHandler) XScuGic_InterruptHandler,
// 	// 		XScuGicInstancePtr);

// 	// Xil_ExceptionEnable();
// 	return XST_SUCCESS;
// }

int Radar_IntcInitFuntion(	u16 		DeviceId,
							XGpio 		*GpioInstancePtr,
							XGpio 		*GpioInstancePtr2)
{
// 	XScuGic_Config *IntcConfig;
// 	int status;
// 	//https://forum.digilentinc.com/topic/4166-zynq-pl-ps-interrupt-issue/
// 	//interrupt controller initialisation
// 	xil_printf("XScuGic_LookupConfig\r\n");
// 	IntcConfig = XScuGic_LookupConfig(DeviceId);

// 	xil_printf("XScuGic_CfgInitialize\r\n");
// 	status = XScuGic_CfgInitialize(&ZYNQ_INTR, IntcConfig,IntcConfig->CpuBaseAddress);

// 	// Call to interrupt setup
// 	xil_printf("InterruptSystemSetup\r\n");
// 	status = Radar_InterruptSystemSetup(&ZYNQ_INTR);

// 	// connect GPIO interrupt to handler
// 	xil_printf("XScuGic_Connect\r\n");
// 	status = XScuGic_Connect(&ZYNQ_INTR, ENDFRAME_INTERRRUPT_ID, (Xil_ExceptionHandler) Radar_Intr_Handler, (void *) GpioInstancePtr);
// 	status = XScuGic_Connect(&ZYNQ_INTR, ENDFRAME2_INTERRRUPT_ID, (Xil_ExceptionHandler) Radar_Intr_Handler2, (void *) GpioInstancePtr2);
// 	status = XScuGic_Connect(&ZYNQ_INTR, IP_ENDFRAME_INTERRRUPT_ID, (Xil_ExceptionHandler) Radar_IP_Intr_Handler, NULL);

// 	//status = XScuGic_Connect(&ZYNQ_INTR, CAN_INTR_VEC_ID, (Xil_InterruptHandler)XCanPs_IntrHandler, (void *)CanInstancePtr);
// 	//status = XScuGic_Connect(&ZYNQ_INTR, CAN2_INTR_VEC_ID, (Xil_InterruptHandler)XCanPs_IntrHandler, (void *)CanInstancePtr2);
// 	//Status = XScuGic_Connect(&ZYNQ_INTR, ZDMA_INTR_DEVICE_ID1, (Xil_ExceptionHandler) XZDma_IntrHandler1, (void *) InstancePtr1);
// 	//Status = XScuGic_Connect(&ZYNQ_INTR, ZDMA_INTR_DEVICE_ID2, (Xil_ExceptionHandler) XZDma_IntrHandler2, (void *) InstancePtr2);
// 	//Status = XScuGic_Connect(&ZYNQ_INTR, ZDMA_INTR_DEVICE_ID3, (Xil_ExceptionHandler) XZDma_IntrHandler3, (void *) InstancePtr3);
// 	//Status = XScuGic_Connect(&ZYNQ_INTR, ZDMA_INTR_DEVICE_ID4, (Xil_ExceptionHandler) XZDma_IntrHandler4, (void *) InstancePtr4);

// 	//status = XScuGic_Connect(&ZYNQ_INTR, COMPASS_INTERRRUPT_ID, (Xil_ExceptionHandler) COMPASS_Intr_Handler, NULL);

// 	// Enable GPIO Interrupt
// 	xil_printf("XGpio_InterruptEnable\r\n");
// 	XGpio_InterruptEnable(GpioInstancePtr, 1);
// 	XGpio_InterruptGlobalEnable(GpioInstancePtr);
// 	//XGpio_InterruptEnable(GpioInstancePtr2, 1);
// 	//XGpio_InterruptGlobalEnable(GpioInstancePtr2);

// 	//XUartLite_EnableInterrupt(Uart0_Ptr); 	/* Enable the interrupt of the UartLite*/
// 	//XUartLite_EnableInterrupt(Uart1_Ptr); 	/* Enable the interrupt of the UartLite*/

// 	xil_printf("ENDFRAME_INTERRRUPT_ID Enable\r\n");
// 	//XScuGic_Enable(&ZYNQ_INTR, ENDFRAME_INTERRRUPT_ID); // XScuGic *InstancePtr

// //	XZDma_EnableIntr(&ZDma1, XZDMA_IXR_DMA_DONE_MASK);
// //	XZDma_EnableIntr(&ZDma2, XZDMA_IXR_DMA_DONE_MASK);
// //	XZDma_EnableIntr(&ZDma3, XZDMA_IXR_DMA_DONE_MASK);
// //	XZDma_EnableIntr(&ZDma4, XZDMA_IXR_DMA_DONE_MASK);

// 	// cai dat muc do uu tien ngat
// 	xil_printf("XScuGic_Prioty\r\n");
// 	XScuGic_SetPriorityTriggerType(&ZYNQ_INTR, ENDFRAME_INTERRRUPT_ID, 8, 0x3);// XScuGic *InstancePtr, Rising Edge
// 	XScuGic_SetPriorityTriggerType(&ZYNQ_INTR, ENDFRAME2_INTERRRUPT_ID, 8, 0x3);// XScuGic *InstancePtr, Rising Edge
// 	XScuGic_SetPriorityTriggerType(&ZYNQ_INTR, IP_ENDFRAME_INTERRRUPT_ID, 8, 0x3);// XScuGic *InstancePtr, Rising Edge
// 	//XScuGic_SetPriorityTriggerType(&ZYNQ_INTR, CAN_INTR_VEC_ID, 16, 0x3);// XScuGic *InstancePtr, Rising Edge
// 	//XScuGic_SetPriorityTriggerType(&ZYNQ_INTR, CAN2_INTR_VEC_ID, 16, 0x3);// XScuGic *InstancePtr, Rising Edge
// 	//XScuGic_SetPriorityTriggerType(&ZYNQ_INTR, ZDMA_INTR_DEVICE_ID1, 16, 0x3);// XScuGic *InstancePtr, Rising Edge
// 	//XScuGic_SetPriorityTriggerType(&ZYNQ_INTR, ZDMA_INTR_DEVICE_ID2, 16, 0x3);// XScuGic *InstancePtr, Rising Edge
// 	//XScuGic_SetPriorityTriggerType(&ZYNQ_INTR, ZDMA_INTR_DEVICE_ID3, 16, 0x3);// XScuGic *InstancePtr, Rising Edge
// 	//XScuGic_SetPriorityTriggerType(&ZYNQ_INTR, ZDMA_INTR_DEVICE_ID4, 16, 0x3);// XScuGic *InstancePtr, Rising Edge

// 	return XST_SUCCESS;
// }

// void Radar_Innit_Interrupt(void)
// {
// // cai dat ngat cho PS7
// 	int INTC_Status;
// 	xil_printf("CAN_Setup_IRQ ok\r\n");
// 	CAN_Setup_IRQ(&Erob_Can0, &Erob_Can1);
// 	xil_printf("Radar_IntcInitFuntion start\r\n");
// 	INTC_Status = Radar_IntcInitFuntion(INTC_DEVICE_ID,
// 										&GPIO_ENDFRAME_IRQ,
// 										&GPIO_ENDFRAME_IRQ1,
// 										&Erob_Can0,
// 										&Erob_Can1);

// 	if (INTC_Status == XST_SUCCESS) xil_printf("ZYNQ_INTR init ok\r\n");
// 	else xil_printf("ZYNQ_INTR init error\r\n");
}

void Radar_Enable_Interrupt(void)
{
	//XScuGic_Enable(&ZYNQ_INTR, ENDFRAME_INTERRRUPT_ID); // XScuGic *InstancePtr
	//XScuGic_Enable(&ZYNQ_INTR, ENDFRAME2_INTERRRUPT_ID); // XScuGic *InstancePtr

	// XScuGic_Enable(&ZYNQ_INTR, IP_ENDFRAME_INTERRRUPT_ID);

	// XScuGic_Enable(&ZYNQ_INTR, CAN_INTR_VEC_ID);
	// XScuGic_Enable(&ZYNQ_INTR, CAN2_INTR_VEC_ID);
	// XCanPs_IntrEnable(&Erob_Can0, XCANPS_IXR_ALL);
	// XCanPs_IntrEnable(&Erob_Can1, XCANPS_IXR_ALL);

}

void Innit_Radar_struct(void)
{
	unsigned int counter=0;
	Radar.Radar_Frame[0] = 0xaa;
	Radar.Radar_Frame[1] = 0x55; // beta[7:0]
		for (counter = 2; counter < 5100; counter++)
		{
			Radar.Radar_Frame[counter] = 0x00;//8f
		}

	xil_printf("Radar Data innit ok\r\n");

	Radar.Radar_Config[0]=0xaa;// ky tu [I] dinh danh frame IET
	Radar.Radar_Config[1]=0x55;// Uart1_b1
	for (counter = 2; counter < 50; counter++)
			{
				Radar.Radar_Config[counter] = 0x00;//8f
			}

	AFC1.U_LechTrai 	= 600;
	AFC1.U_LechPhai 	= 600;
	Radar.Debug_ENC 	=0;
	Radar.ADC_Cells 	= 4096;

	Radar.UDP_Delay_T1  = 40;
	Radar.UDP_Delay_T2  = 40;
	Radar.UDP_Delay_T3  = 40;
	Radar.UDP_Delay_T4  = 20;
	Radar.UDP_Delay_T5  = 10;

	Radar.UDP_Delay_T1  = 1;
	Radar.UDP_Delay_T2  = 1;
	Radar.UDP_Delay_T3  = 1;
	Radar.UDP_Delay_T4  = 1;
	Radar.UDP_Delay_T5  = 1;

	Radar.Read_Innit_ok = 0;
	Radar.AFC			= 1;
	Radar.ENC_Error1 	= 0;
	Radar.ENC0 			= 0;
	Radar.ENC_Error 	= 0;
	Radar.Error_Counter = 0;
	Radar.Phat			= 0;
	Radar.Soi_Dot		= 0;
	Radar.Sample		= 5220;
	Radar.Config_Done	= 0;
	Radar.Range			= 4;
	Radar.Radar_2_2A	= 1;
	Radar.Ma_Dieu_Che 	= 0;
	Radar.Active		= 0;
	Radar.On			= 0;
	Radar.ADC_Freq0		= 20;
	Radar.Start_Angle	= 0;
	Radar.End_Angle		= 0;
	Radar.Sample_Size	= 1;
	Radar.Test_mode		= 0;
	Radar.Delay_2		= 0;
	Radar.Delay_2A		= 0;
	Radar.TriggerLevel	= 0;
	Radar.ShortTime		= 0;
	Radar.Start_Delay	= 6000;
	Radar.End_Delay		= 6010;
	Radar.Config_Done	= 0;
	for (counter = 0; counter < 200; counter++) Radar.GPS_Frame[counter]=0;
	Radar.GPS_Start		= 0;
	Radar.GPS_Done		= 0;
	Radar.GPS_Detect	= 0;
	Radar.U_Nguong 		= 1900;
	Radar.AFC_Point		= 20;
	Radar.Muc_Nhieu		= 30;
	Radar.Chong_Nhieu_Counter=0;
	Radar.Pulse_Mode	= 0;
	Radar.ENC_Update	= 0;
	Radar.Receiver_Excel= 0;
	for (counter = 0; counter < 10; counter++)  Radar.GPS_Buff[counter]=0;

	for (counter = 0; counter < 33; counter++) Radar.Dutru2[counter]=0;

	Radar.Ngoai_sai 	= 9410;
	Radar.Ngoai_sai0 	= 9410;
	Radar.AFC			= 1;
	Radar.Beta_SW		= 0;
	Radar.IO1			= 0;
	Radar.IO2			= 0;
	//Radar.XC3S200A_Frame[15];
	Radar.Board_speed1	= 0;
	Radar.real_Angle	= 0;
	Radar.Compass1_Connect= 0;
	Radar.Compass2_Connect= 0;

	Radar.Compass_PC2_EN  = 0;
	Radar.Compass_PC2_Val = 0;
	Radar.Compass_PC2_Fix = 0;
	Radar.Radar_Azimuth = 0;
	Radar.AIS_Counter 	= 0;
	Radar.AIS_Connect 	= 0;
	Radar.GPS_Connect	= 0;
	Radar.GPS_Status	= 0;
	Radar.GPS_counter	= 0;
	Radar.Sum			= 0;
	Radar.Sum2			= 0;
	Radar.Compass1_Status= 0;
	Radar.Compass2_Status= 0;
	Radar.Compass_counter= 0;
	Radar.Debug_line		= 0;
	Radar.Angle_mode_ENA	= 0;
	Radar.B1				= 0;
	Radar.B2				= 600;
	Radar.Pulse_Width		= 50;
	Radar.Pulse_Source		= 0;

	Radar.UDP_PC8_Read_video = 0;
	Radar.UDP_PC8_Send_Point = 0;

	Radar.Wind_Speed		= 0;
	Radar.Wind_Angle		= 0;
	Radar.Wind_Connect		= 0;
	Radar.Wind_Connect1		= 0;
	Radar.Wind_Connect2		= 0;

	Radar.PC7_Control		= 0;
	Radar.PC7_ThangCuLy		= 4;
	Radar.PC7_ThangCuLyi 	= 4;
}

void Innit_Frame_Header_Struct(void)
{
	Frame.Radar_Header			=0x55AA;
	Frame.Radar_Frame_size		=Radar.Sample-100;
	Frame.Radar_Beta			=0;
	Frame.Radar_Thang_Culy		=4;
	Frame.Radar_Sample_Size		=1;
	Frame.Radar_On_Off			=1;
	Frame.Radar_2_2A			=1;
	Frame.Radar_ADC_Freq		=40;
	Frame.Radar_Signal_Freq		=1;
	Frame.Radar_Gain			=0;
	Frame.Radar_VCO				=0;
	Frame.Radar_BAPY			=0;
	Frame.Radar_APC				=0;
	Frame.Radar_AFC				=0;
	Frame.Radar_MPV				=0;
	//Frame.Radar_Error_Status	=0;
	Frame.Radar_Speed			=0;
	//Frame.Radar_Din				=0;// bit0: Z0, bit1: 2/2a, bit4: M, bit5: BARKER, bit6:LFM, bit7:PULSE
	//Frame.Radar_Delay_2			=0;
	//Frame.Radar_Delay_2A		=0;
	//Frame.Radar_Dutru			=0;
	Frame.Radar_Number_IFF		=0;
	Frame.Radar_AIS_Valid		=0;
	Frame.Radar_AIS_Status		=0;
	Frame.Radar_AIS_MMSI		=0;
	Frame.Radar_AIS_Longitude	=0; //Latitude
	Frame.Radar_AIS_Latitude	=0;
	Frame.Radar_AIS_Speed		=0;
	Frame.Radar_AIS_Angle		=0;
	Frame.Radar_Azimuth			=0;
	Frame.Radar_Wind_Speed		=0;
	Frame.Radar_Wind_Angle		=0;
	Frame.Radar_GPS_Status		=1;
	Frame.Radar_GPS_Satelites	=5;
	Frame.Radar_GPS_Time		=0;
	Frame.Radar_GPS_Longitude	=0.0;
	Frame.Radar_GPS_Latitude	=0.0;
	Frame.Radar_IFF_Serial1		=1;
	Frame.Radar_IFF_Serial2		=2;
	Frame.Radar_IFF_Serial3		=3;
	Frame.Radar_IFF_Serial4		=4;
	Frame.Radar_IFF_Serial5		=5;
	Frame.Radar_IFF_Serial6		=6;
	Frame.Radar_IFF_Serial7		=7;
}

void Innit_PC8_Struct(void)
{
	// PC8
	PC8.Header = 0x56ab;

	PC8.BS_Center_cell  = 100;
	PC8.BS_Center_start = 0;
	PC8.BS_Center_stop  = 200;
	PC8.BS_Start		= 0;

	PC8.BS_Center_angle = 0;// 20
	PC8.BS_Center_Angle_start = 3520;// 20
	PC8.BS_Center_Angle_stop= 80;// 20
	PC8.BS_Center_angle_point = 0;

	PC8.Beta_Enable 	= 0;
	PC8.Beta_Speed 		= 20;
	PC8.Beta_Rotate 	= 0;
	PC8.Beta_DIR 		= 0;
	PC8.Beta_ENC 		= 0;
	PC8.Beta_Calib 		= 0;
	PC8.Beta_Mode 		= 0;
	PC8.Beta_Reset 		= 0;
	PC8.Beta_Update 	= 0;
	PC8.Gia_Quay_EN 	= 0;
	PC8.epxilon_Channeli = 0;

	PC8.Alpha_Enable 	= 0;
	PC8.Alpha_Speed 	= 20;
	PC8.Alpha_Rotate 	= 0;
	PC8.Alpha_DIR 		= 0;
	PC8.Alpha_ENC 		= 0;
	PC8.Alpha_Calib 	= 0;
	PC8.Alpha_Mode 		= 0;
	PC8.Alpha_Reset 	= 0;
	PC8.Alpha_Update 	= 0;

	PC8.SSI_Speed 		= 0;
	PC8.Encoder_Source 	= 0;

	PC8.Encoder_Format	=1;//26
	PC8.Encoder_Div		=0;//26
	PC8.Encoder_Presscal=90;//26

	PC8.Alpha_Max		= 600;// 20
	PC8.Alpha_Min		= 3500;// 20
	PC8.Alpha_P0		= 1228;// 1228
	PC8.Alpha_Step		= 87500;
	PC8.Beta_Step		= 28280;
	PC8.Update_Q0		= 0;
	PC8.Quay_Quat_Active= 0;
	PC8.Delta_Angle		= 0.0;
	PC8.Alpha_Step1		= 0.0;

	PC8.Tien			= 1.0;
	PC8.Lui				= -1.0;

	PC8.Alpha_Angle		= 0;
	PC8.Beta_Angle		= 0;

	PC8.Beta_A0			= 0;
	PC8.Beta_A1			= 0;

	PC8.Alpha_A0		= 0;
	PC8.Alpha_A1		= 0;
	PC8.Alpha_Stop 		= 1;
	PC8.Beta_Stop 		= 1;
	PC8.Trigger 		= 0;

	PC8.Pulse_MUX 		= 0;
	PC8.Pulse_Width 	= 40;
	PC8.Tan_so_Lap 		= 1200;
	PC8.Ts_Val			= 50000;
	PC8.Auto_Rotate 	= 0;
	PC8.Calib_counter	= 0;
	PC8.Quat_Error		= 10;
	PC8.Trigger_source	= 1;

	PC8.CLK_MUX_IO 		= 0;
	PC8.Test_Enable 	= 0;
	PC8.Clock_Channel 	= 0;
	PC8.SYS_Control 	= 0;
	PC8.CLK_10MHz_Source= 1;

	PC8.Alpha_Vals 		= 0;

	PC8.ATT1.Sau_Bapy 	= 0;
	PC8.ATT1.Rong_Bapy 	= 0;
	PC8.ATT1.KDMT 		= 0;

	PC8.ATT2.Sau_Bapy 	= 0;
	PC8.ATT2.Rong_Bapy 	= 0;
	PC8.ATT2.KDMT 		= 0;

	PC8.ATT3.Sau_Bapy 	= 0;
	PC8.ATT3.Rong_Bapy 	= 0;
	PC8.ATT3.KDMT 		= 0;

	PC8.ATT4.Sau_Bapy 	= 0;
	PC8.ATT4.Rong_Bapy 	= 0;
	PC8.ATT4.KDMT 		= 0;

	PC8.ATT5.Sau_Bapy 	= 0;
	PC8.ATT5.Rong_Bapy 	= 0;
	PC8.ATT5.KDMT 		= 0;

	PC8.ATT6.Sau_Bapy 	= 0;
	PC8.ATT6.Rong_Bapy 	= 0;
	PC8.ATT6.KDMT 		= 0;

	PC8.ATT7.Sau_Bapy 	= 0;
	PC8.ATT7.Rong_Bapy 	= 0;
	PC8.ATT7.KDMT 		= 0;

	PC8.ATT8.Sau_Bapy 	= 0;
	PC8.ATT8.Rong_Bapy 	= 0;
	PC8.ATT8.KDMT 		= 0;

	PC8.ATT9.Sau_Bapy 	= 0;
	PC8.ATT9.Rong_Bapy 	= 0;
	PC8.ATT9.KDMT 		= 0;

	PC8.ATT10.Sau_Bapy 	= 0;
	PC8.ATT10.Rong_Bapy = 0;
	PC8.ATT10.KDMT 		= 0;

	PC8.ATT11.Sau_Bapy 	= 0;
	PC8.ATT11.Rong_Bapy = 0;
	PC8.ATT11.KDMT 		= 0;

	PC8.ATT12.Sau_Bapy 	= 0;
	PC8.ATT12.Rong_Bapy = 0;
	PC8.ATT12.KDMT 		= 0;

	PC8.ATT13.Sau_Bapy 	= 0;
	PC8.ATT13.Rong_Bapy = 0;
	PC8.ATT13.KDMT 		= 0;

	PC8.ATT14.Sau_Bapy 	= 0;
	PC8.ATT14.Rong_Bapy = 0;
	PC8.ATT14.KDMT 		= 0;

	PC8.ATT15.Sau_Bapy 	= 0;
	PC8.ATT15.Rong_Bapy = 0;
	PC8.ATT15.KDMT 		= 0;

	PC8.ATT16.Sau_Bapy 	= 0;
	PC8.ATT16.Rong_Bapy = 0;
	PC8.ATT16.KDMT 		= 0;

	PC8.ATT17.Sau_Bapy 	= 0;
	PC8.ATT17.Rong_Bapy = 0;
	PC8.ATT17.KDMT 		= 255;

	PC8.Thang_Cu_ly 	= 5;
	PC8.Cu_ly_EN	 	= 0;


	PC8.ADC_SEL			= 0;
	PC8.ADC_MAX			= 255;
	PC8.ATT_TX_EN		= 0;

	PC8.ILA_Val			= 0;
	PC8.Video_Val		= 0;
	PC8.ADC_CHx			= 0;
	PC8.Phase_Mode		= 0;

	//unsigned char GPS_Buff[10];
	PC8.beam_ChannelS = 0;
	PC8.fb_ChannelS = 0;
	// PC8s
	PC8s.Header = 0;
	PC8s.Header = 0;

	PC8s.Beta_Enable = 0;
	PC8s.Beta_Speed = 0;
	PC8s.Beta_Rotate = 0;
	PC8s.Beta_DIR = 0;
	PC8s.Beta_ENC = 0;
	PC8s.Beta_Calib = 0;
	PC8s.Beta_Mode = 0;
	PC8s.Beta_Reset = 0;
	PC8s.Beta_Update = 0;

	PC8s.Alpha_Enable = 0;
	PC8s.Alpha_Speed = 20;
	PC8s.Alpha_Rotate = 0;
	PC8s.Alpha_DIR = 0;
	PC8s.Alpha_ENC = 0;
	PC8s.Alpha_Calib = 0;
	PC8s.Alpha_Mode = 0;
	PC8s.Alpha_Reset = 0;
	PC8s.Alpha_Update = 0;

	PC8s.SSI_Speed = 0;
	PC8s.Encoder_Source = 0;

	PC8s.Encoder_Format=1;//26
	PC8s.Encoder_Div=0;//26
	PC8s.Encoder_Presscal=90;//26
}

void Radar_Innit_Frame(void)
{
	Innit_Radar_struct();
	Innit_Frame_Header_Struct();
	Innit_Radar_Debug();
	Innit_PC8_Struct();
	xil_printf("Innit_Radar_Debug ok\r\n");

	Radar.GPS_LAT[0] = 0x00;
	Radar.GPS_LAT[1] = 0x00;
	Radar.GPS_LAT[2] = 0x00;
	Radar.GPS_LAT[3] = 0x00;
	Radar.GPS_LAT[4] = 0x00;
	Radar.GPS_LAT[5] = 0x00;
	Radar.GPS_LAT[6] = 0x00;
	Radar.GPS_LAT[7] = 0x00;

	Radar.GPS_LON[0] = 0x00;
	Radar.GPS_LON[1] = 0x00;
	Radar.GPS_LON[2] = 0x00;
	Radar.GPS_LON[3] = 0x00;
	Radar.GPS_LON[4] = 0x00;
	Radar.GPS_LON[5] = 0x00;
	Radar.GPS_LON[6] = 0x00;
	Radar.GPS_LON[7] = 0x00;

}

void Innit_Radar_Debug(void)
{

	for(Radar.counter=0; Radar.counter<80; Radar.counter++) Radar_Debug.Radar_Config[Radar.counter] = 0;	// du lieu cau hinh XLPC
	for(Radar.counter=0; Radar.counter<150; Radar.counter++) Radar_Debug.Radar_Debug[Radar.counter] = 0;	// du lieu cau hinh XLPC
	for(Radar.counter=0; Radar.counter<44; Radar.counter++) Radar_Debug.GPS_Frame[Radar.counter] = 0;	// du lieu truyen len PC
	for(Radar.counter=0; Radar.counter<24; Radar.counter++) Radar_Debug.Wind_Frame[Radar.counter] = 0;
	for(Radar.counter=0; Radar.counter<80; Radar.counter++) Radar_Debug.AIS_Frame[Radar.counter] = 0;
	for(Radar.counter=0; Radar.counter<8020; Radar.counter++) PA_Data[Radar.counter] = 0;

	Radar_Debug.Update_Debug 		= 0;
	Radar_Debug.Update_Debug2 		= 0;
	Radar_Debug.Update_Enable 		= 0;
	Radar_Debug.Update_Enable2 		= 0;
	Radar_Debug.Update_Wind			= 0;
	Radar_Debug.Update_GPS			= 0;
	Radar_Debug.Update_LPF			= 8;
	Radar_Debug.Update_DDS_Source 	= 0;
	Radar_Debug.Update_DDS_Sine		= 0;
	Radar_Debug.Update_DDS_Data		= 0;
	Radar_Debug.Update_IIR_Mode 	= 1;
	Radar_Debug.Update_Mul_Source	= 0;

	Radar_Debug.Update_Doggy_EN		= 0;
	Radar_Debug.Update_MAX1			= 0;
	Radar_Debug.Update_MAX2			= 0;

	Radar_Debug.Update2_Doggy_EN	= 0;
	Radar_Debug.Update2_MAX1		= 0;
	Radar_Debug.Update2_MAX2		= 0;

	Radar_Debug.kL0 = 16384;
	Radar_Debug.bL1 = -28781;
	Radar_Debug.bL2 = 12832;
	Radar_Debug.aL0 = 109;
	Radar_Debug.aL1 = 218;
	Radar_Debug.aL2 = 109;

	Radar_Debug.Receiver_Status		= 0;
	Radar_Debug.Config_done			= 1;
	Radar_Debug.Config_Start 		= 0;
	Radar_Debug.Fix_Wind_Angle		= 0;
	Radar_Debug.Fix_Wind_Speed		= 0;
	Radar_Debug.GPS_LAT_Size		= 7;
	Radar_Debug.GPS_LON_Size		= 8;
	Radar_Debug.GPS_Speed_Size		= 4;
	Radar_Debug.GPS_Point_LAT1		= 9;
	Radar_Debug.GPS_Point_LAT2		= 10;
	Radar_Debug.GPS_Point_LAT3		= 11;
	Radar_Debug.GPS_Point_LAT4		= 13;
	Radar_Debug.GPS_Point_LAT5		= 14;
	Radar_Debug.GPS_Point_LAT6		= 15;
	Radar_Debug.GPS_Point_LAT7		= 16;
	Radar_Debug.GPS_Point_LAT8		= 17;
	Radar_Debug.GPS_Point_LON1		= 19;
	Radar_Debug.GPS_Point_LON2		= 20;
	Radar_Debug.GPS_Point_LON3		= 21;
	Radar_Debug.GPS_Point_LON4		= 22;
	Radar_Debug.GPS_Point_LON5		= 23;
	Radar_Debug.GPS_Point_LON6		= 25;
	Radar_Debug.GPS_Point_LON7		= 26;
	Radar_Debug.GPS_Point_LON8		= 27;
	Radar_Debug.GPS_Point_Speed1	= 31;
	Radar_Debug.GPS_Point_Speed2	= 32;
	Radar_Debug.GPS_Point_Speed3	= 33;
	Radar_Debug.GPS_Point_Speed4	= 34;
	Radar_Debug.GPS_Point_Speed5	= 36;
	Radar_Debug.Wind_Point_Angle1	= 2;
	Radar_Debug.Wind_Point_Angle2	= 3;
	Radar_Debug.Wind_Point_Angle3	= 4;
	Radar_Debug.Wind_Point_Speed1	= 6;
	Radar_Debug.Wind_Point_Speed2	= 7;
	Radar_Debug.Wind_Point_Speed3	= 8;
	Radar_Debug.Wind_Point_Speed4	= 11;
}

