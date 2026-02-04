/*
 * Erob.c
 *
 *  Created on: Jul 24, 2023
 *      Author: tienn
 */
#include "Erob.h"
#include "MI_505.h"
#include "CAN_Control.h"

char str[80];
char str2[80];

//#define	Radar_Motor_PhuongVi		0x00
//#define	Radar_Motor_GocTa			0x01
//
//#define	Radar_Motor_Running			0x01
//#define	Radar_Motor_Stop			0x00
//
//#define	Radar_Motor_Tien			0x01
//#define	Radar_Motor_Lui				0x00

uint16_t MODBUS_CRC16( const unsigned char *buf, unsigned int len )
{
	uint16_t crc = 0xFFFF;
	unsigned int i = 0;
	char bit = 0;

	for( i = 0; i < len; i++ )
	{
		crc ^= buf[i];

		for( bit = 0; bit < 8; bit++ )
		{
			if( crc & 0x0001 )
			{
				crc >>= 1;
				crc ^= 0xA001;
			}
			else
			{
				crc >>= 1;
			}
		}
	}

	return crc;
}

void Erob_Motor_Control(uint32_t Pulse, uint8_t Dir, uint8_t EN, uint8_t Motor)
{
	if(Motor == 0)
	{
		Motor1_Mode_Freq(Pulse, Dir, EN);
	}
	else if(Motor == 1)
	{
		Motor2_Mode_Freq(Pulse, Dir, EN);
	}
}

void Erob_Motor_Send_Frame(uint8_t Length, uint8_t Motor)
{
	uint8_t i;

	for(i = 0; i < Length; i++)
	{
		Radar.UART_Frame1[i] = CAN_Controls.Erob_Cmd[i];
		Radar.UART_Frame2[i] = CAN_Controls.Erob_Cmd[i];
	}

	if(Motor == 0) UART1_Send_Frame(Length);
	else if(Motor == 1) UART2_Send_Frame(Length);

	// for(i = 0; i < Length; i++)
	// {
	// 	if(Motor == 0) XUartLite_SendByte(XPAR_UARTLITE_1_BASEADDR, CAN_Controls.Erob_Cmd[i]);
	// 	else if(Motor == 1) XUartLite_SendByte(XPAR_UARTLITE_2_BASEADDR, CAN_Controls.Erob_Cmd[i]);
	// }
}

void Erob_Motor_Enable(uint8_t Motor)
{
	CAN_Controls.Erob_Cmd[0] = 0x01;
	CAN_Controls.Erob_Cmd[1] = 0x06;
	CAN_Controls.Erob_Cmd[2] = 0x00;
	CAN_Controls.Erob_Cmd[3] = 0xF0;
	CAN_Controls.Erob_Cmd[4] = 0x00;
	CAN_Controls.Erob_Cmd[5] = 0x01;

	CAN_Controls.Erob_CRC 	= MODBUS_CRC16(CAN_Controls.Erob_Cmd, 6);
	CAN_Controls.Erob_Cmd[6] = (CAN_Controls.Erob_CRC & 0x00ff);
	CAN_Controls.Erob_Cmd[7] = (CAN_Controls.Erob_CRC & 0xff00) >> 8;

	//Erob_Cmd[6] = 0x48;
	//Erob_Cmd[7] = 0x39;
	Erob_Motor_Send_Frame(8, Motor);

	//xil_printf("Erob_Motor_Enable: %4x %02x %02x\n\r", Erob_CRC, (Erob_CRC & 0x00ff), (Erob_CRC & 0xff00) >> 8);


}

void Erob_Motor_Disable(uint8_t Motor)
{
	CAN_Controls.Erob_Cmd[0] = 0x01;
	CAN_Controls.Erob_Cmd[1] = 0x06;
	CAN_Controls.Erob_Cmd[2] = 0x00;
	CAN_Controls.Erob_Cmd[3] = 0xF0;
	CAN_Controls.Erob_Cmd[4] = 0x00;
	CAN_Controls.Erob_Cmd[5] = 0x00;

	CAN_Controls.Erob_CRC = MODBUS_CRC16(CAN_Controls.Erob_Cmd, 6);
	CAN_Controls.Erob_Cmd[6] = (CAN_Controls.Erob_CRC & 0x00ff);
	CAN_Controls.Erob_Cmd[7] = (CAN_Controls.Erob_CRC & 0xff00) >> 8;

	//Erob_Cmd[6] = 0x89;
	//Erob_Cmd[7] = 0xF9;
	Erob_Motor_Send_Frame(8, Motor);

	//xil_printf("Erob_Motor_Disable: %4x %02x %02x\n\r", Erob_CRC, (Erob_CRC & 0x00ff), (Erob_CRC & 0xff00) >> 8);

}

void Erob_Motor_Stop_Motion(uint8_t Motor)
{
	CAN_Controls.Erob_Cmd[0] = 0x01;
	CAN_Controls.Erob_Cmd[1] = 0x06;
	CAN_Controls.Erob_Cmd[2] = 0x00;
	CAN_Controls.Erob_Cmd[3] = 0xF2;
	CAN_Controls.Erob_Cmd[4] = 0x00;
	CAN_Controls.Erob_Cmd[5] = 0x00;

	CAN_Controls.Erob_CRC = MODBUS_CRC16(CAN_Controls.Erob_Cmd, 6);
	CAN_Controls.Erob_Cmd[6] = (CAN_Controls.Erob_CRC & 0x00ff);
	CAN_Controls.Erob_Cmd[7] = (CAN_Controls.Erob_CRC & 0xff00) >> 8;

	//Erob_Cmd[6] = 0x28;
	//Erob_Cmd[7] = 0x39;
	Erob_Motor_Send_Frame(8, Motor);

	//xil_printf("Erob_Motor_Stop_Motion: %4x %02x %02x\n\r", Erob_CRC, (Erob_CRC & 0x00ff), (Erob_CRC & 0xff00) >> 8);

}

void Erob_Motor_Start_to_Move(uint8_t Motor)
{
	CAN_Controls.Erob_Cmd[0] = 0x01;
	CAN_Controls.Erob_Cmd[1] = 0x06;
	CAN_Controls.Erob_Cmd[2] = 0x00;
	CAN_Controls.Erob_Cmd[3] = 0xF1;
	CAN_Controls.Erob_Cmd[4] = 0x00;
	CAN_Controls.Erob_Cmd[5] = 0x01;

	CAN_Controls.Erob_CRC = MODBUS_CRC16(CAN_Controls.Erob_Cmd, 6);
	CAN_Controls.Erob_Cmd[6] = (CAN_Controls.Erob_CRC & 0x00ff);
	CAN_Controls.Erob_Cmd[7] = (CAN_Controls.Erob_CRC & 0xff00) >> 8;

	//Erob_Cmd[6] = 0x19;
	//Erob_Cmd[7] = 0xF9;
	Erob_Motor_Send_Frame(8, Motor);

	//xil_printf("Erob_Motor_Start_to_Move: %4x %02x %02x\n\r", Erob_CRC, (Erob_CRC & 0x00ff), (Erob_CRC & 0xff00) >> 8);

}

void Erob_Motor_Read_Speed(uint8_t Motor)
{
	CAN_Controls.Erob_Cmd[0] = 0x01;
	CAN_Controls.Erob_Cmd[1] = 0x03;
	CAN_Controls.Erob_Cmd[2] = 0x00;
	CAN_Controls.Erob_Cmd[3] = 0x6E;
	CAN_Controls.Erob_Cmd[4] = 0x00;
	CAN_Controls.Erob_Cmd[5] = 0x02;

	CAN_Controls.Erob_CRC = MODBUS_CRC16(CAN_Controls.Erob_Cmd, 6);
	CAN_Controls.Erob_Cmd[6] = (CAN_Controls.Erob_CRC & 0x00ff);
	CAN_Controls.Erob_Cmd[7] = (CAN_Controls.Erob_CRC & 0xff00) >> 8;

	//Erob_Cmd[6] = 0xA5;
	//Erob_Cmd[7] = 0xD6;
	Erob_Motor_Send_Frame(8, Motor);
	//CAN_Send_Control(&Erob_Can0, 8);

	//xil_printf("Erob_Motor_Read_Speed: %4x %02x %02x\n\r", Erob_CRC, (Erob_CRC & 0x00ff), (Erob_CRC & 0xff00) >> 8);

}

void Erob_Motor_Read_Position(uint8_t Motor)
{
	int Status;

	CAN_Controls.Erob_Cmd[0] = 0x01;
	CAN_Controls.Erob_Cmd[1] = 0x03;
	CAN_Controls.Erob_Cmd[2] = 0x00;
	CAN_Controls.Erob_Cmd[3] = 0x66;
	CAN_Controls.Erob_Cmd[4] = 0x00;
	CAN_Controls.Erob_Cmd[5] = 0x02;

	CAN_Controls.Erob_CRC = MODBUS_CRC16(CAN_Controls.Erob_Cmd, 6);
	CAN_Controls.Erob_Cmd[6] = (CAN_Controls.Erob_CRC & 0x00ff);
	CAN_Controls.Erob_Cmd[7] = (CAN_Controls.Erob_CRC & 0xff00) >> 8;

	//Erob_Cmd[6] = 0x24;
	//Erob_Cmd[7] = 0x14;
	Erob_Motor_Send_Frame(8, Motor);
	//Status = CAN_Send_Control(&Erob_Can0, 2);
	//xil_printf("Status: %d\n\r", Status);
	//Status = CAN_RecvFrame(&Erob_Can0);
	//xil_printf("RecvFrame %08x %08x %08x %08x ok\r\n", RxFrame[0],
	//													 RxFrame[1],
	//													 RxFrame[2],
	//													 RxFrame[3]);

	//CAN_Send_Control(&Erob_Can0, 8);
	//xil_printf("Erob_Motor_Read_Position: %4x %02x %02x\n\r", Erob_CRC, (Erob_CRC & 0x00ff), (Erob_CRC & 0xff00) >> 8);

}

void Erob_Motor_Read_Current(uint8_t Motor)
{
	CAN_Controls.Erob_Cmd[0] = 0x01;
	CAN_Controls.Erob_Cmd[1] = 0x03;
	CAN_Controls.Erob_Cmd[2] = 0x00;
	CAN_Controls.Erob_Cmd[3] = 0x72;
	CAN_Controls.Erob_Cmd[4] = 0x00;
	CAN_Controls.Erob_Cmd[5] = 0x02;

	CAN_Controls.Erob_CRC = MODBUS_CRC16(CAN_Controls.Erob_Cmd, 6);
	CAN_Controls.Erob_Cmd[6] = (CAN_Controls.Erob_CRC & 0x00ff);
	CAN_Controls.Erob_Cmd[7] = (CAN_Controls.Erob_CRC & 0xff00) >> 8;

	//Erob_Cmd[6] = 0x24;
	//Erob_Cmd[7] = 0x14;
	Erob_Motor_Send_Frame(8, Motor);
	//CAN_Send_Control(&Erob_Can0, 8);

	//xil_printf("Erob_Motor_Read_Position: %4x %02x %02x\n\r", Erob_CRC, (Erob_CRC & 0x00ff), (Erob_CRC & 0xff00) >> 8);

}

void Erob_Motor_Read_Voltage(uint8_t Motor)
{
	CAN_Controls.Erob_Cmd[0] = 0x01;
	CAN_Controls.Erob_Cmd[1] = 0x03;
	CAN_Controls.Erob_Cmd[2] = 0x00;
	CAN_Controls.Erob_Cmd[3] = 0x76;
	CAN_Controls.Erob_Cmd[4] = 0x00;
	CAN_Controls.Erob_Cmd[5] = 0x02;

	CAN_Controls.Erob_CRC = MODBUS_CRC16(CAN_Controls.Erob_Cmd, 6);
	CAN_Controls.Erob_Cmd[6] = (CAN_Controls.Erob_CRC & 0x00ff);
	CAN_Controls.Erob_Cmd[7] = (CAN_Controls.Erob_CRC & 0xff00) >> 8;

	//Erob_Cmd[6] = 0x24;
	//Erob_Cmd[7] = 0x14;
	Erob_Motor_Send_Frame(8, Motor);
	//CAN_Send_Control(&Erob_Can0, 8);

	//xil_printf("Erob_Motor_Read_Position: %4x %02x %02x\n\r", Erob_CRC, (Erob_CRC & 0x00ff), (Erob_CRC & 0xff00) >> 8);

}

void Erob_Motor_Read_Temerature(uint8_t Motor)
{
	CAN_Controls.Erob_Cmd[0] = 0x01;
	CAN_Controls.Erob_Cmd[1] = 0x03;
	CAN_Controls.Erob_Cmd[2] = 0x00;
	CAN_Controls.Erob_Cmd[3] = 0x78;
	CAN_Controls.Erob_Cmd[4] = 0x00;
	CAN_Controls.Erob_Cmd[5] = 0x02;

	CAN_Controls.Erob_CRC = MODBUS_CRC16(CAN_Controls.Erob_Cmd, 6);
	CAN_Controls.Erob_Cmd[6] = (CAN_Controls.Erob_CRC & 0x00ff);
	CAN_Controls.Erob_Cmd[7] = (CAN_Controls.Erob_CRC & 0xff00) >> 8;

	//Erob_Cmd[6] = 0x24;
	//Erob_Cmd[7] = 0x14;
	Erob_Motor_Send_Frame(8, Motor);
	//CAN_Send_Control(&Erob_Can0, 8);
	//xil_printf("Erob_Motor_Read_Position: %4x %02x %02x\n\r", Erob_CRC, (Erob_CRC & 0x00ff), (Erob_CRC & 0xff00) >> 8);

}

void Erob_Motor_Read_Encoder(uint8_t Motor)
{
	CAN_Controls.Erob_Cmd[0] = 0x01;
	CAN_Controls.Erob_Cmd[1] = 0x03;
	CAN_Controls.Erob_Cmd[2] = 0x00;
	CAN_Controls.Erob_Cmd[3] = 0x79;
	CAN_Controls.Erob_Cmd[4] = 0x00;
	CAN_Controls.Erob_Cmd[5] = 0x02;

	CAN_Controls.Erob_CRC = MODBUS_CRC16(CAN_Controls.Erob_Cmd, 6);
	CAN_Controls.Erob_Cmd[6] = (CAN_Controls.Erob_CRC & 0x00ff);
	CAN_Controls.Erob_Cmd[7] = (CAN_Controls.Erob_CRC & 0xff00) >> 8;

	//Erob_Cmd[6] = 0x24;
	//Erob_Cmd[7] = 0x14;
	Erob_Motor_Send_Frame(8, Motor);
	//CAN_Send_Control(&Erob_Can0, 8);

	//xil_printf("Erob_Motor_Read_Position: %4x %02x %02x\n\r", Erob_CRC, (Erob_CRC & 0x00ff), (Erob_CRC & 0xff00) >> 8);

}

void Erob_Motor_Control_Mode3(uint8_t Motor)
{
	CAN_Controls.Erob_Cmd[0] = 0x01;
	CAN_Controls.Erob_Cmd[1] = 0x06;
	CAN_Controls.Erob_Cmd[2] = 0x00;
	CAN_Controls.Erob_Cmd[3] = 0x40;
	CAN_Controls.Erob_Cmd[4] = 0x00;
	CAN_Controls.Erob_Cmd[5] = 0x03;

	CAN_Controls.Erob_CRC = MODBUS_CRC16(CAN_Controls.Erob_Cmd, 6);
	CAN_Controls.Erob_Cmd[6] = (CAN_Controls.Erob_CRC & 0x00ff);
	CAN_Controls.Erob_Cmd[7] = (CAN_Controls.Erob_CRC & 0xff00) >> 8;

	Erob_Motor_Send_Frame(8, Motor);

	//xil_printf("Erob_Motor_Control_Mode3: %4x %02x %02x\n\r", Erob_CRC, (Erob_CRC & 0x00ff), (Erob_CRC & 0xff00) >> 8);

}

void Erob_Motor_Control_Mode0(uint8_t Motor)
{
	CAN_Controls.Erob_Cmd[0] = 0x01;
	CAN_Controls.Erob_Cmd[1] = 0x06;
	CAN_Controls.Erob_Cmd[2] = 0x00;
	CAN_Controls.Erob_Cmd[3] = 0x41;
	CAN_Controls.Erob_Cmd[4] = 0x00;
	CAN_Controls.Erob_Cmd[5] = 0x00;

	CAN_Controls.Erob_CRC = MODBUS_CRC16(CAN_Controls.Erob_Cmd, 6);
	CAN_Controls.Erob_Cmd[6] = (CAN_Controls.Erob_CRC & 0x00ff);
	CAN_Controls.Erob_Cmd[7] = (CAN_Controls.Erob_CRC & 0xff00) >> 8;

	//Erob_Cmd[6] = 0xD9;
	//Erob_Cmd[7] = 0xDE;
	Erob_Motor_Send_Frame(8, Motor);

	//xil_printf("Erob_Motor_Control_Mode0: %4x %02x %02x\n\r", Erob_CRC, (Erob_CRC & 0x00ff), (Erob_CRC & 0xff00) >> 8);

}

void Erob_Motor_Set_ACC(uint32_t Val, uint8_t Motor)
{
	// 01 10 00 49 00 02 04 00 01 86 A0 04 2D
	CAN_Controls.Erob_Cmd[0] = 0x01;
	CAN_Controls.Erob_Cmd[1] = 0x10;
	CAN_Controls.Erob_Cmd[2] = 0x00;
	CAN_Controls.Erob_Cmd[3] = 0x49;
	CAN_Controls.Erob_Cmd[4] = 0x00;
	CAN_Controls.Erob_Cmd[5] = 0x02;
	CAN_Controls.Erob_Cmd[6] = 0x04;
	CAN_Controls.Erob_Cmd[7] = 0x00;
	CAN_Controls.Erob_Cmd[8] = (Val & 0xff0000) >> 16;
	CAN_Controls.Erob_Cmd[9] = (Val & 0x00ff00) >> 8;
	CAN_Controls.Erob_Cmd[10] =(Val & 0x0000ff);

	CAN_Controls.Erob_CRC = MODBUS_CRC16(CAN_Controls.Erob_Cmd, 11);
	CAN_Controls.Erob_Cmd[11] = (CAN_Controls.Erob_CRC & 0x00ff);
	CAN_Controls.Erob_Cmd[12] = (CAN_Controls.Erob_CRC & 0xff00) >> 8;

	//Erob_Cmd[11] = 0x04;
	//Erob_Cmd[12] = 0x2D;
	Erob_Motor_Send_Frame(13, Motor);

	//xil_printf("Erob_Motor_Set_ACC: %4x %02x %02x\n\r", Erob_CRC, (Erob_CRC & 0x00ff), (Erob_CRC & 0xff00) >> 8);

}

void Erob_Motor_Set_DEC(uint32_t Val, uint8_t Motor)
{
	// 01 10 00 49 00 02 04 00 01 86 A0 04 2D
	CAN_Controls.Erob_Cmd[0] = 0x01;
	CAN_Controls.Erob_Cmd[1] = 0x10;
	CAN_Controls.Erob_Cmd[2] = 0x00;
	CAN_Controls.Erob_Cmd[3] = 0x4B;
	CAN_Controls.Erob_Cmd[4] = 0x00;
	CAN_Controls.Erob_Cmd[5] = 0x02;
	CAN_Controls.Erob_Cmd[6] = 0x04;
	CAN_Controls.Erob_Cmd[7] = 0x00;
	CAN_Controls.Erob_Cmd[8] = (Val & 0xff0000) >> 16;
	CAN_Controls.Erob_Cmd[9] = (Val & 0x00ff00) >> 8;
	CAN_Controls.Erob_Cmd[10] =(Val & 0x0000ff);

	CAN_Controls.Erob_CRC = MODBUS_CRC16(CAN_Controls.Erob_Cmd, 11);
	CAN_Controls.Erob_Cmd[11] = (CAN_Controls.Erob_CRC & 0x00ff);
	CAN_Controls.Erob_Cmd[12] = (CAN_Controls.Erob_CRC & 0xff00) >> 8;

	//Erob_Cmd[11] = 0x04;
	//Erob_Cmd[12] = 0x2D;
	Erob_Motor_Send_Frame(13, Motor);

	//xil_printf("Motor_Set_DEC: %4x %02x %02x\n\r", Erob_CRC, (Erob_CRC & 0x00ff), (Erob_CRC & 0xff00) >> 8);

}

void Erob_Motor_Set_SPEED(uint32_t Val, uint8_t Motor)
{
	// 01 10 00 47 00 02 04 00 01 86 A0 85 A1
	CAN_Controls.Erob_Cmd[0] = 0x01;
	CAN_Controls.Erob_Cmd[1] = 0x10;
	CAN_Controls.Erob_Cmd[2] = 0x00;
	CAN_Controls.Erob_Cmd[3] = 0x47;
	CAN_Controls.Erob_Cmd[4] = 0x00;
	CAN_Controls.Erob_Cmd[5] = 0x02;
	CAN_Controls.Erob_Cmd[6] = 0x04;
	CAN_Controls.Erob_Cmd[7] = 0x00;
	CAN_Controls.Erob_Cmd[8] = (Val & 0xff0000) >> 16;
	CAN_Controls.Erob_Cmd[9] = (Val & 0x00ff00) >> 8;
	CAN_Controls.Erob_Cmd[10] =(Val & 0x0000ff);

	CAN_Controls.Erob_CRC = MODBUS_CRC16(CAN_Controls.Erob_Cmd, 11);
	CAN_Controls.Erob_Cmd[11] = (CAN_Controls.Erob_CRC & 0x00ff);
	CAN_Controls.Erob_Cmd[12] = (CAN_Controls.Erob_CRC & 0xff00) >> 8;

	//Erob_Cmd[11] = 0x85;
	//Erob_Cmd[12] = 0xA1;
	Erob_Motor_Send_Frame(13, Motor);

	//xil_printf("Erob_Motor_Set_SPEED: %4x %02x %02x\n\r", Erob_CRC, (Erob_CRC & 0x00ff), (Erob_CRC & 0xff00) >> 8);

}

void Erob_Motor_Stop(uint8_t Motor)
{
	Erob_Motor_Enable(Motor);
	usleep(100000);
}

void Erob_Read_UART(void)
{
	/*
	Hop4.NguLoi1.Uart_ENA = 0;
	Hop4.NguLoi1.Frame_counter = 0;
	Erob_Motor_Read_Position();
	Hop4.NguLoi1.Uart_ENA = 1;
	usleep(6000);

	Erob_Speed = 0;
	Erob_Position = (int32_t)(Hop4.NguLoi1.Data[11] * 0x01000000 +
							  Hop4.NguLoi1.Data[12] * 0x00010000 +
							  Hop4.NguLoi1.Data[13] * 0x00000100 +
							  Hop4.NguLoi1.Data[14] * 0x00000001);
	*/

	/*
	Hop4.NguLoi1.Uart_ENA = 0;
	Hop4.NguLoi1.Frame_counter = 0;
	Erob_Motor_Read_Speed();
	Hop4.NguLoi1.Uart_ENA = 1;
	usleep(6000);

	Erob_Speed  = (int32_t)(Hop4.NguLoi1.Data[11] * 0x01000000 +
							Hop4.NguLoi1.Data[12] * 0x00010000 +
							Hop4.NguLoi1.Data[13] * 0x00000100 +
							Hop4.NguLoi1.Data[14] * 0x00000001);

	Hop4.NguLoi1.Uart_ENA = 0;
	Hop4.NguLoi1.Frame_counter = 0;
	Erob_Motor_Read_Current();
	Hop4.NguLoi1.Uart_ENA = 1;
	usleep(6000);

	Erob_Current  = (int32_t)(Hop4.NguLoi1.Data[11] * 0x01000000 +
							  Hop4.NguLoi1.Data[12] * 0x00010000 +
							  Hop4.NguLoi1.Data[13] * 0x00000100 +
							  Hop4.NguLoi1.Data[14] * 0x00000001);

	Hop4.NguLoi1.Uart_ENA = 0;
	Hop4.NguLoi1.Frame_counter = 0;
	Erob_Motor_Read_Voltage();
	Hop4.NguLoi1.Uart_ENA = 1;
	usleep(6000);

	Erob_Voltage  = (int32_t)(Hop4.NguLoi1.Data[11] * 0x01000000 +
							  Hop4.NguLoi1.Data[12] * 0x00010000 +
							  Hop4.NguLoi1.Data[13] * 0x00000100 +
							  Hop4.NguLoi1.Data[14] * 0x00000001);

	Hop4.NguLoi1.Uart_ENA = 0;
	Hop4.NguLoi1.Frame_counter = 0;
	Erob_Motor_Read_Temerature();
	Hop4.NguLoi1.Uart_ENA = 1;
	usleep(6000);

	Erob_Temperature  = (Hop4.NguLoi1.Data[11] * 0x0100 + Hop4.NguLoi1.Data[12] * 0x0001);
	*/
}

void Erob_IO_CMD(void)
{
	//Erob_Motor.Erob_Gpio_data 	= XGpio_DiscreteRead(&GPIO_HOP4_INPUT, 1);

	Erob_Motor.Erob_SW1 = 0;// (Erob_Motor.Erob_Gpio_data & 0x8000) ? 1 : 0;
	Erob_Motor.Erob_SW2 = 0;//(Erob_Motor.Erob_Gpio_data & 0x2000) ? 1 : 0;
	Erob_Motor.Erob_SW3 = 0;//(Erob_Motor.Erob_Gpio_data & 0x0800) ? 1 : 0;
	Erob_Motor.Erob_SW4 = 0;//(Erob_Motor.Erob_Gpio_data & 0x0200) ? 1 : 0;

}


void Erob_Innit_Motor_PhuongVi(void)
{
	Erob_Motor.Motor_PhuongVi.UDP_Datas1 = 0;
	Erob_Motor.Motor_PhuongVi.UDP_Datas2 = 0;
	Erob_Motor.Motor_PhuongVi.UDP_Datas3 = 0;
	Erob_Motor.Motor_PhuongVi.UDP_Datas4 = 0;
	Erob_Motor.Motor_PhuongVi.UDP_Angle = 0;
	Erob_Motor.Motor_PhuongVi.UDP_Lygiac = 0;
	Erob_Motor.Motor_PhuongVi.Zynq_Encoder = 0;

	Erob_Motor.Motor_PhuongVi.Erob_Freq = 10000;
	Erob_Motor.Motor_PhuongVi.Erob_Counter = 0;
	Erob_Motor.Motor_PhuongVi.Erob_Speed = 0;
	Erob_Motor.Motor_PhuongVi.Erob_Position = 0;
	Erob_Motor.Motor_PhuongVi.Erob_Current = 0;
	Erob_Motor.Motor_PhuongVi.Erob_Voltage = 0;
	Erob_Motor.Motor_PhuongVi.Erob_Temperature = 0;

	Erob_Motor.Motor_PhuongVi.Erob_Enable = 0; // 3 0-1
	Erob_Motor.Motor_PhuongVi.Erob_DIR = 0;// 7
	Erob_Motor.Motor_PhuongVi.Erob_Calib = 0;// 11
	Erob_Motor.Motor_PhuongVi.Erob_Mode = 0;// 12
	Erob_Motor.Motor_PhuongVi.Erob_Reset = 0;// 13
	Erob_Motor.Motor_PhuongVi.Erob_Update = 0;// 14

	Erob_Motor.Motor_PhuongVi.Pulse_Enable = 0; // 3 0-1
	Erob_Motor.Motor_PhuongVi.Pulse_DIR = 0;// 7
	Erob_Motor.Motor_PhuongVi.Pulse_Freq = 0;
	Erob_Motor.Motor_PhuongVi.Pulse_number = 0;
	Erob_Motor.Motor_PhuongVi.Pulse_done = 0;// 7
	Erob_Motor.Motor_PhuongVi.Pulse_Active = 0;

	Erob_Motor.Motor_PhuongVi.Motor_ENC = 0;		// 4 gia tri encoder dong co ngu loi
	Erob_Motor.Motor_PhuongVi.Motor_ENC_Save = 0;	// 8 gia tri encoder dong co ngu loi
	Erob_Motor.Motor_PhuongVi.Motor_ENC_Save2 = 0;// 12 gia tri encoder dong co ngu loi
	Erob_Motor.Motor_PhuongVi.Motor_Step = 0;		// 16 gia tri encoder dong co ngu loi
	Erob_Motor.Motor_PhuongVi.Request_Encoder = 0;// 20
	Erob_Motor.Motor_PhuongVi.Retry_Motor = 0;    // 21
	Erob_Motor.Motor_PhuongVi.Save_Counter = 0;	// 22
	Erob_Motor.Motor_PhuongVi.Motor_FL_Step = 0;	// 26 so step can di chuyen
	Erob_Motor.Motor_PhuongVi.Motor_Step_done = 0;// 27 da chay het step yeu cau
	Erob_Motor.Motor_PhuongVi.Motor_Angle = 0;	// 29 gia tri encoder dong co ngu loi
	Erob_Motor.Motor_PhuongVi.Absolute_ENC = 0;	// 31 goc encoder tuyet doi gan len truc quay
	Erob_Motor.Motor_PhuongVi.Real_ENC = 0;		// 33 goc encoder cua truc quay ngu loi
	Erob_Motor.Motor_PhuongVi.Delta_Angle = 0;	// 35 goc encoder tuyet doi khi bat nguon cua encoder tuyet doi
	Erob_Motor.Motor_PhuongVi.Angle = 0;			// 37 Goc ngu loi tinh toan duoc (truyen len PC4)
	Erob_Motor.Motor_PhuongVi.Angle_buffer = 0;	// 37 Goc ngu loi tinh toan duoc (truyen len PC4)
	Erob_Motor.Motor_PhuongVi.ADC_buffer = 0;	// 37 Goc ngu loi tinh toan duoc (truyen len PC4)
	Erob_Motor.Motor_PhuongVi.AngleE = 0;
	Erob_Motor.Motor_PhuongVi.Omega = 0;			// 39 Goc ngu loi tu PC4
	Erob_Motor.Motor_PhuongVi.New_Omega = 0;		// 39 Goc ngu loi tu PC4
	Erob_Motor.Motor_PhuongVi.Scale = 0;			// 41 he so chia ngu loi
	Erob_Motor.Motor_PhuongVi.ENC_Scale = 0;		// 43 he so chia ENC ABS
	Erob_Motor.Motor_PhuongVi.Offset = 0;			// 45 bu sai so encoder
	Erob_Motor.Motor_PhuongVi.CMDi = 0;			// 46 lenh thuc thi
	Erob_Motor.Motor_PhuongVi.Running = 0;		// 47 lenh dang thuc thi (lenh dieu khien tu PC4)
	Erob_Motor.Motor_PhuongVi.Run_CMD = 0;		// 48 1: dang quay 0: dung, Idle
	Erob_Motor.Motor_PhuongVi.DIR = 0;			// 49 0: cung chieu kim dong ho; 1: nguoc chieu
	Erob_Motor.Motor_PhuongVi.VE_Speed = 0;		// 50 van toc qua vong/s
	Erob_Motor.Motor_PhuongVi.VE_Update = 0;		// 50 van toc qua vong/s
	Erob_Motor.Motor_PhuongVi.Update_Counter = 0;	// 51 0: chua ket noi	1: ket noi binh thuong
	Erob_Motor.Motor_PhuongVi.ERROR_Counter = 0;	// 52 0: chua ket noi	1: ket noi binh thuong
	Erob_Motor.Motor_PhuongVi.Innit_Done = 0;		// 53 ngu loi 1 da khoi tao xong.
	Erob_Motor.Motor_PhuongVi.Uart_Start = 0;		// 54 0: Idle; 1: dang nhan du lieu
	Erob_Motor.Motor_PhuongVi.Uart_Connect = 0;	// 54 0: Idle; 1: dang nhan du lieu
	//Erob_Motor.Motor_PhuongVi.Data[30];		// 84 0: Idle; 1: dang nhan du lieu
	Erob_Motor.Motor_PhuongVi.Uart_ENA = 0;		// 85 cho phep nhan du lieu uart ngu loi
	Erob_Motor.Motor_PhuongVi.Frame_DIR = 0;		// 86 cho phep nhan du lieu uart ngu loi
	Erob_Motor.Motor_PhuongVi.Frame_Format = 0;	// 87 cho phep nhan du lieu uart ngu loi
	Erob_Motor.Motor_PhuongVi.Frame_counter = 0;	// 88 so byte d�t nhan duoc
	Erob_Motor.Motor_PhuongVi.Max_rotation = 0;	// 90 so vong quay toi da
	Erob_Motor.Motor_PhuongVi.Max_rotation_ENC = 0;// 94 so encoder toi da
	Erob_Motor.Motor_PhuongVi.Max_rotation_Step = 0;// 98 so buoc toi da
	Erob_Motor.Motor_PhuongVi.ENC_Prescal = 0;	//  100 he so chia
	Erob_Motor.Motor_PhuongVi.Raw_ENC = 0;// 98 so buoc toi da
	Erob_Motor.Motor_PhuongVi.Step_Scale = 0;	//  100 he so chia
	Erob_Motor.Motor_PhuongVi.Quay_Trai = 0;  // lenh quay trai
	Erob_Motor.Motor_PhuongVi.Quay_Phai = 0;  // lenh quay trai
	Erob_Motor.Motor_PhuongVi.Be_enable = 0;  // lenh quay trai
	Erob_Motor.Motor_PhuongVi.Calib_enable = 0;  // lenh quay trai
	Erob_Motor.Motor_PhuongVi.Calib_Counter = 0;
	Erob_Motor.Motor_PhuongVi.FreeRun_DIR = 0;
	Erob_Motor.Motor_PhuongVi.ENC_LastPoint = 0;	//  100 he so chia
	Erob_Motor.Motor_PhuongVi.ENC_NewPoint = 0;	//  100 he so chia
	Erob_Motor.Motor_PhuongVi.ENC_OldPoint = 0;	//  100 he so chia
	Erob_Motor.Motor_PhuongVi.ENC_Delta = 0;	//  100 he so chia
	Erob_Motor.Motor_PhuongVi.ENC_Point = 0;	//  100 he so chia
	Erob_Motor.Motor_PhuongVi.ENC_Dx = 0;	//  100 he so chia
	Erob_Motor.Motor_PhuongVi.ENC_Dy = 0;	//  100 he so chia
	Erob_Motor.Motor_PhuongVi.Block_time = 0; // sua loi bai toan tinh
	Erob_Motor.Motor_PhuongVi.Fix_Sensor = 0; // sua loi bai toan tinh
	Erob_Motor.Motor_PhuongVi.Block_Time = 0; // sua loi bai toan tinh
	Erob_Motor.Motor_PhuongVi.Block_Start = 0; // sua loi bai toan tinh
}

void Erob_Innit_Motor_GocTa(void)
{
	Erob_Motor.Motor_GocTa.UDP_Datas1 = 0;
	Erob_Motor.Motor_GocTa.UDP_Datas2 = 0;
	Erob_Motor.Motor_GocTa.UDP_Datas3 = 0;
	Erob_Motor.Motor_GocTa.UDP_Datas4 = 0;
	Erob_Motor.Motor_GocTa.UDP_Angle = 0;
	Erob_Motor.Motor_GocTa.UDP_Lygiac = 0;
	Erob_Motor.Motor_GocTa.Zynq_Encoder = 0;

	Erob_Motor.Motor_GocTa.Erob_Freq = 10000;
	Erob_Motor.Motor_GocTa.Erob_Counter = 0;
	Erob_Motor.Motor_GocTa.Erob_Speed = 0;
	Erob_Motor.Motor_GocTa.Erob_Position = 0;
	Erob_Motor.Motor_GocTa.Erob_Current = 0;
	Erob_Motor.Motor_GocTa.Erob_Voltage = 0;
	Erob_Motor.Motor_GocTa.Erob_Temperature = 0;

	Erob_Motor.Motor_GocTa.Erob_Enable = 0; // 3 0-1
	Erob_Motor.Motor_GocTa.Erob_DIR = 0;// 7
	Erob_Motor.Motor_GocTa.Erob_Calib = 0;// 11
	Erob_Motor.Motor_GocTa.Erob_Mode = 0;// 12
	Erob_Motor.Motor_GocTa.Erob_Reset = 0;// 13
	Erob_Motor.Motor_GocTa.Erob_Update = 0;// 14

	Erob_Motor.Motor_GocTa.Pulse_Enable = 0; // 3 0-1
	Erob_Motor.Motor_GocTa.Pulse_DIR = 0;// 7
	Erob_Motor.Motor_GocTa.Pulse_Freq = 0;
	Erob_Motor.Motor_GocTa.Pulse_number = 0;
	Erob_Motor.Motor_GocTa.Pulse_done = 0;// 7
	Erob_Motor.Motor_GocTa.Pulse_Active = 0;

	Erob_Motor.Motor_GocTa.Motor_ENC = 0;		// 4 gia tri encoder dong co ngu loi
	Erob_Motor.Motor_GocTa.Motor_ENC_Save = 0;	// 8 gia tri encoder dong co ngu loi
	Erob_Motor.Motor_GocTa.Motor_ENC_Save2 = 0;	// 12 gia tri encoder dong co ngu loi
	Erob_Motor.Motor_GocTa.Motor_Step = 0;		// 16 gia tri encoder dong co ngu loi
	Erob_Motor.Motor_GocTa.Request_Encoder = 0;	// 20
	Erob_Motor.Motor_GocTa.Retry_Motor = 0;    	// 21
	Erob_Motor.Motor_GocTa.Save_Counter = 0;	// 22
	Erob_Motor.Motor_GocTa.Motor_FL_Step = 0;	// 26 so step can di chuyen
	Erob_Motor.Motor_GocTa.Motor_Step_done = 0;	// 27 da chay het step yeu cau
	Erob_Motor.Motor_GocTa.Motor_Angle = 0;		// 29 gia tri encoder dong co ngu loi
	Erob_Motor.Motor_GocTa.Absolute_ENC = 0;	// 31 goc encoder tuyet doi gan len truc quay
	Erob_Motor.Motor_GocTa.Real_ENC = 0;		// 33 goc encoder cua truc quay ngu loi
	Erob_Motor.Motor_GocTa.Delta_Angle = 0;		// 35 goc encoder tuyet doi khi bat nguon cua encoder tuyet doi
	Erob_Motor.Motor_GocTa.Angle = 0;			// 37 Goc ngu loi tinh toan duoc (truyen len PC4)
	Erob_Motor.Motor_GocTa.Angle_buffer = 0;	// 37 Goc ngu loi tinh toan duoc (truyen len PC4)
	Erob_Motor.Motor_GocTa.ADC_buffer = 0;		// 37 Goc ngu loi tinh toan duoc (truyen len PC4)
	Erob_Motor.Motor_GocTa.AngleE = 0;
	Erob_Motor.Motor_GocTa.Omega = 0;			// 39 Goc ngu loi tu PC4
	Erob_Motor.Motor_GocTa.New_Omega = 0;		// 39 Goc ngu loi tu PC4
	Erob_Motor.Motor_GocTa.Scale = 0;			// 41 he so chia ngu loi
	Erob_Motor.Motor_GocTa.ENC_Scale = 0;		// 43 he so chia ENC ABS
	Erob_Motor.Motor_GocTa.Offset = 0;			// 45 bu sai so encoder
	Erob_Motor.Motor_GocTa.CMDi = 0;			// 46 lenh thuc thi
	Erob_Motor.Motor_GocTa.Running = 0;			// 47 lenh dang thuc thi (lenh dieu khien tu PC4)
	Erob_Motor.Motor_GocTa.Run_CMD = 0;			// 48 1: dang quay 0: dung, Idle
	Erob_Motor.Motor_GocTa.DIR = 0;				// 49 0: cung chieu kim dong ho; 1: nguoc chieu
	Erob_Motor.Motor_GocTa.VE_Speed = 0;		// 50 van toc qua vong/s
	Erob_Motor.Motor_GocTa.VE_Update = 0;		// 50 van toc qua vong/s
	Erob_Motor.Motor_GocTa.Update_Counter = 0;	// 51 0: chua ket noi	1: ket noi binh thuong
	Erob_Motor.Motor_GocTa.ERROR_Counter = 0;	// 52 0: chua ket noi	1: ket noi binh thuong
	Erob_Motor.Motor_GocTa.Innit_Done = 0;		// 53 ngu loi 1 da khoi tao xong.
	Erob_Motor.Motor_GocTa.Uart_Start = 0;		// 54 0: Idle; 1: dang nhan du lieu
	Erob_Motor.Motor_GocTa.Uart_Connect = 0;	// 54 0: Idle; 1: dang nhan du lieu
	//Erob_Motor.Motor_GocTa.Data[30];			// 84 0: Idle; 1: dang nhan du lieu
	Erob_Motor.Motor_GocTa.Uart_ENA = 0;		// 85 cho phep nhan du lieu uart ngu loi
	Erob_Motor.Motor_GocTa.Frame_DIR = 0;		// 86 cho phep nhan du lieu uart ngu loi
	Erob_Motor.Motor_GocTa.Frame_Format = 0;	// 87 cho phep nhan du lieu uart ngu loi
	Erob_Motor.Motor_GocTa.Frame_counter = 0;	// 88 so byte d�t nhan duoc
	Erob_Motor.Motor_GocTa.Max_rotation = 0;	// 90 so vong quay toi da
	Erob_Motor.Motor_GocTa.Max_rotation_ENC = 0;// 94 so encoder toi da
	Erob_Motor.Motor_GocTa.Max_rotation_Step = 0;// 98 so buoc toi da
	Erob_Motor.Motor_GocTa.ENC_Prescal = 0;		//  100 he so chia
	Erob_Motor.Motor_GocTa.Raw_ENC = 0;			// 98 so buoc toi da
	Erob_Motor.Motor_GocTa.Step_Scale = 0;		//  100 he so chia
	Erob_Motor.Motor_GocTa.Quay_Trai = 0;  		// lenh quay trai
	Erob_Motor.Motor_GocTa.Quay_Phai = 0;  		// lenh quay trai
	Erob_Motor.Motor_GocTa.Be_enable = 0;  		// lenh quay trai
	Erob_Motor.Motor_GocTa.Calib_enable = 0;  	// lenh quay trai
	Erob_Motor.Motor_GocTa.Calib_Counter = 0;
	Erob_Motor.Motor_GocTa.FreeRun_DIR = 0;
	Erob_Motor.Motor_GocTa.ENC_LastPoint = 0;	//  100 he so chia
	Erob_Motor.Motor_GocTa.ENC_NewPoint = 0;	//  100 he so chia
	Erob_Motor.Motor_GocTa.ENC_OldPoint = 0;	//  100 he so chia
	Erob_Motor.Motor_GocTa.ENC_Delta = 0;		//  100 he so chia
	Erob_Motor.Motor_GocTa.ENC_Point = 0;		//  100 he so chia
	Erob_Motor.Motor_GocTa.ENC_Dx = 0;			//  100 he so chia
	Erob_Motor.Motor_GocTa.ENC_Dy = 0;			//  100 he so chia
	Erob_Motor.Motor_GocTa.Block_time = 0; 		// sua loi bai toan tinh
	Erob_Motor.Motor_GocTa.Fix_Sensor = 0; 		// sua loi bai toan tinh
	Erob_Motor.Motor_GocTa.Block_Time = 0; 		// sua loi bai toan tinh
	Erob_Motor.Motor_GocTa.Block_Start = 0; 	// sua loi bai toan tinh
}

void Erob_Data_Innit(void)
{
	Erob_Motor.UDP_counter = 0;
	Erob_Motor.Stop_counter = 0;
	Erob_Motor.Erob_Freq = 100000;
	Erob_Motor.Erob_Freq2 = 100000;
	Erob_Motor.Erob_Gpio_data = 0;
	Erob_Motor.Erob_SW1 = 0;
	Erob_Motor.Erob_SW2 = 0;
	Erob_Motor.Erob_Counter = 0;
	Erob_Motor.Erob_Speed = 0;
	Erob_Motor.Erob_Position = 0;
	Erob_Motor.Erob_Current = 0;
	Erob_Motor.Erob_Voltage = 0;
	Erob_Motor.Erob_Temperature = 0;
	Erob_Motor.Erob_Encoder = 0;
	Erob_Motor.Zynq_Encoder = 0;

	Erob_Innit_Motor_PhuongVi();
	Erob_Innit_Motor_GocTa();
}

void Erob_Motor_Innit()
{
	// cai dat dong co 1
	Erob_Motor_Set_ACC(200000, Radar_Motor_PhuongVi);
	usleep(100000);

	Erob_Motor_Set_DEC(200000, Radar_Motor_PhuongVi);
	usleep(100000);

	Erob_Motor_Set_SPEED(200000, Radar_Motor_PhuongVi);
	usleep(100000);

	Erob_Motor_Enable(0);
	usleep(100000);

	// cai dat dong co 2
	Erob_Motor_Set_ACC(200000, Radar_Motor_GocTa);
	usleep(100000);

	Erob_Motor_Set_DEC(200000, Radar_Motor_GocTa);
	usleep(100000);

	Erob_Motor_Set_SPEED(200000, Radar_Motor_GocTa);
	usleep(100000);

	Erob_Motor_Enable(1);
	usleep(100000);

	Erob_Motor_Control(100000, Radar_Motor_Tien, Radar_Motor_Stop, Radar_Motor_PhuongVi);
	Erob_Motor_Control(100000, Radar_Motor_Tien, Radar_Motor_Stop, Radar_Motor_GocTa);

}


