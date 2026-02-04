/*
 * ADF_4159.c
 *
 *  Created on: Mar 18, 2024
 *      Author: tienn
 */

#include "ADF4159.h"
#include "User_VCO_Pin.h"
#include "linux_hal.h"      // Lớp trừu tượng phần cứng (sysfs GPIO)

extern XGpioPs  psgpio;

///* REG0 Bit Definitions */
//#define ADF4159_REG0_INT(x)				(((x) & 0xFFF) << 15)
//#define ADF4159_REG0_FRACT_MSB(x)		(((x) & 0xFFF) << 3)
//#define ADF4159_REG0_MUXOUT(x)			(((x) & 0xF) << 27)
//#define ADF4159_REG0_RAMP_ON(x)			(((x) & 0x1) << 31)
//
///* REG1 Bit Definitions */
//#define ADF4159_REG1_FRACT_LSB(x)			(((x) & 0x1FFF) << 15)
//#define ADF4159_REG1_PHASE(x)				(((x) & 0xFFF) << 3)
//#define ADF4159_REG1_PHASE_ADJ_EN(x)		(((x) & 0x1) << 28)
//
///* REG2 Bit Definitions */
//#define ADF4159_REG2_CLK1_DIV(x)			(((x) & 0xFFF) << 3)
//#define ADF4159_REG2_R_CNT(x)				(((x) & 0x1F) << 15)
//#define ADF4159_REG2_REF_DOUBLER_EN(x)		(((x) & 0x1) << 20)
//#define ADF4159_REG2_REF_DIV2_EN(x)			(((x) & 0x1) << 21)
//#define ADF4159_REG2_PRESCALER_89_EN(x)		(((x) & 0x1) << 22)
//#define ADF4159_REG2_CP_CURRENT(x)			(((x) & 0xF) << 24)
//#define ADF4159_REG2_CSR_EN(x)				(((x) & 0x1) << 28)
//
///* REG3 Bit Definitions */
//#define ADF4159_REG3_CNT_RESET_EN(x)		(((x) & 0x1) << 3)
//#define ADF4159_REG3_CP_THREE_STATE_EN(x)	(((x) & 0x1) << 4)
//#define ADF4159_REG3_POWER_DOWN_EN(x)		(((x) & 0x1) << 5)
//#define ADF4159_REG3_PD_POLARITY_POS_EN(x)	(((x) & 0x1) << 6)
//#define ADF4159_REG3_LDP_6NS_EN(x)			(((x) & 0x1) << 7)
//#define ADF4159_REG3_FSK_EN(x)				(((x) & 0x1) << 8)
//#define ADF4159_REG3_PSK_EN(x)				(((x) & 0x1) << 9)
//#define ADF4159_REG3_RAMP_MODE(x)			(((x) & 0x3) << 10)
//#define ADF4159_REG3_SD_RESET_DIS(x)		(((x) & 0x1) << 14)
//#define ADF4159_REG3_LOL_DIS(x)				(((x) & 0x1) << 16)
//#define ADF4159_REG3_NEG_BLEED_EN(x)		(((x) & 0x1) << 21)
//#define ADF4159_REG3_NEG_BLEED_CURR(x)		(((x) & 0x7) << 22)
//
///* REG4 Bit Definitions */
//#define ADF4159_REG4_CLK_DIV2_SEL(x)		(((x) & 0x1) << 6)
//#define ADF4159_REG4_CLK2_DIV(x)			(((x) & 0xFFF) << 7)
//#define ADF4159_REG4_CLK_DIV_MODE(x)		(((x) & 0x3) << 19)
//#define ADF4159_REG4_RAMP_STATUS(x)			(((x) & 0x1F) << 21)
//#define ADF4159_REG4_SD_MOD_MODE(x)			(((x) & 0x1F) << 26)
//#define ADF4159_REG4_LE_SEL_SYNC_REFIN_EN(x)(((x) & 0x1) << 31)
//
///* REG5 Bit Definitions */
//#define ADF4159_REG5_DEVIATION(x)			(((x) & 0xFFFF) << 3)
//#define ADF4159_REG5_DEVIATION_OFFSET(x)	(((x) & 0xF) << 19)
//#define ADF4159_REG5_DEV_SEL(x)				(((x) & 0x1) << 23)
//#define ADF4159_REG5_DUAL_RAMP_EN(x)		(((x) & 0x1) << 24)
//#define ADF4159_REG5_FSK_RAMP_EN(x)			(((x) & 0x1) << 25)
//#define ADF4159_REG5_INTERRUPT_MODE(x)		(((x) & 0x3) << 26)
//#define ADF4159_REG5_PARABOLIC_RAMP_EN(x)	(((x) & 0x1) << 28)
//#define ADF4159_REG5_TXDATA_RAMP_CLK(x)		(((x) & 0x1) << 29)
//#define ADF4159_REG5_TXDATA_INVERT(x)		(((x) & 0x1) << 30)
//
///* REG6 Bit Definitions */
//#define ADF4159_REG6_STEP(x)				(((x) & 0xFFFFF) << 3)
//#define ADF4159_REG6_STEP_SEL(x)			(((x) & 0x1) << 23)
//
///* REG7 Bit Definitions */
//#define ADF4159_REG7_DELAY_START(x)			(((x) & 0xFFF) << 3)
//#define ADF4159_REG7_DELAY_START_EN(x)		(((x) & 0x1) << 15)
//#define ADF4159_REG7_DEL_CLK_SEL(x)			(((x) & 0x1) << 16)
//#define ADF4159_REG7_RAMP_DEL_EN(x)			(((x) & 0x1) << 17)
//#define ADF4159_REG7_RAMP_DEL_FL_EN(x)		(((x) & 0x1) << 18)
//#define ADF4159_REG7_FAST_RAMP_EN(x)		(((x) & 0x1) << 19)
//#define ADF4159_REG7_TXDATA_TRIG_EN(x)		(((x) & 0x1) << 20)
//#define ADF4159_REG7_SING_FULL_TRI_EN(x)	(((x) & 0x1) << 21)
//#define ADF4159_REG7_TRI_DELAY_EN(x)		(((x) & 0x1) << 22)
//#define ADF4159_REG7_TXDATA_TRIG_DELAY_EN(x)(((x) & 0x1) << 23)
//
///* Specifications */
//#define ADF4159_MIN_FREQ				500000000ULL /* Hz */
//#define ADF4159_MAX_FREQ				13000000000ULL /* Hz */
//#define ADF4159_MAX_FREQ_PFD			110000000UL /* Hz */
//
///* Specifications */
//#define ADF4169_MIN_FREQ				500000000ULL /* Hz */
//#define ADF4169_MAX_FREQ				13500000000ULL /* Hz */
//#define ADF4169_MAX_FREQ_PFD			130000000UL /* Hz */
//
//#define ADF4159_MAX_FREQ_REFIN			260000000UL /* 260 MHz */
//#define ADF4159_MAX_R_CNT				32
//#define ADF4159_MAX_INT					4095
//#define ADF4159_MIN_INT					23
//#define ADF4159_MIN_INT_PRESCALER_89	75
//#define ADF4159_MODULUS					33554432ULL
//#define ADF4159_MAX_FRAC				(ADF4159_MODULUS - 1)
//#define ADF4159_MAX_FREQ_PRESCALER_45	8000000000ULL /* 8 GHz */
//#define ADF4159_MIN_CLKIN_DOUB_FREQ		10000000ULL /* Hz */
//#define ADF4159_MAX_CLKIN_DOUB_FREQ		50000000ULL /* Hz */
//
///* Registers */
//enum adf4159_reg {
//	ADF4159_REG0,
//	ADF4159_REG1,
//	ADF4159_REG2,
//	ADF4159_REG3,
//	ADF4159_REG4,
//	ADF4159_REG5,
//	ADF4159_REG6,
//	ADF4159_REG7,
//	ADF4159_REG4_SEL1,
//	ADF4159_REG5_SEL1,
//	ADF4159_REG6_SEL1,
//	ADF4159_REG_NUM,
//};
//
//enum {
//	ADF4159_FREQ,
//	ADF4159_PWRDOWN,
//	ADF4159_FREQ_DEV_STEP,
//	ADF4159_FREQ_DEV_RANGE,
//	ADF4159_FREQ_DEV_TIME,
//};


unsigned int Bit2[32];
unsigned int ADF4159_F[11];

void delay2()
{
	unsigned int counter=0;
	for(counter=0;counter<50;counter++);
}

void ADF4159_Set_Freq(unsigned int Freq) // MHz
{
	// 12 GHz
	unsigned long int reg1 		= 0;
	unsigned long int reg2 		= 0;
	//unsigned long int reg3 		= 0;
	unsigned long int PFD 		= 0;
	//unsigned long int Data 		= 0;

	unsigned int MUX_out  		= MUXOUT_DIGITAL_LOCK_DETECT;
	unsigned int N_PLL 	  		= 0;
	unsigned int MSB_FRAC 		= 0;
	unsigned int LSB_FRAC 		= 0;
	unsigned int Phase 			= 0;
	unsigned int Phase_adjust	= 0;


	unsigned int REF_Frequency 	= 100;
	unsigned int CLK_Divider 	= 1;
	unsigned int R_counter 		= 20;
	unsigned int REF_double 	= 0;
	unsigned int RDIV2_DBB 		= 0;
	unsigned int Prescaler 		= 0; // mode 4/5
	unsigned int CP_Current		= 15;
	unsigned int Neg_Current	= 4;
	unsigned int CSR			= 0;

	PFD 						= REF_Frequency / R_counter;

//	reg1		= (Freq / 2) / 5;
//	reg2		= (((Freq / 2) % 5) * 33554432) / 5;
//
//	N_PLL		= reg1;
//	MSB_FRAC	= reg2 / 0x00004000;
//	LSB_FRAC	= reg2 % 0x00004000;

	// do su dung dau phan hoi RFOUT/2
	reg1		= ((Freq / 2) / PFD);
	reg2		= (((Freq / 2) % PFD) * 33554432) / PFD;

	N_PLL		= reg1;
	MSB_FRAC	= reg2 / 0x00004000;
	LSB_FRAC	= reg2 % 0x00004000;


	//ADF4159_F[10]	= 0x301E0000; // R0
	ADF4159_F[10]	= MUX_out 		* 0x08000000 +
					  N_PLL   		* 0x00008000 +
					  MSB_FRAC		* 0x00000008 +
					  	  	  	  	  0x00000000; // control bit = 000

	ADF4159_F[9]	= Phase_adjust 	* 0x10000000 +
					  LSB_FRAC  	* 0x00008000 +
					  Phase			* 0x00000008 +
								  	  0x00000001; // control bit = 001
	//	clk_div = 1
	//ADF4159_F[8]	= 0x070A000A; // R2       0x0700800A; // R2
	ADF4159_F[8]  = CSR			* 0x10000000 +
					CP_Current  * 0x01000000 +
					Prescaler  	* 0x00400000 +
					RDIV2_DBB  	* 0x00200000 +
					REF_double  * 0x00100000 +
					R_counter  	* 0x00008000 +
					CLK_Divider	* 0x00000008 +
								  0x00000002; // control bit = 001

	//ADF4159_F[7]	= 0x01030083;// & 0xFE0FFFFF) | (CP_Current * 0x0400000); // control bit = 001
	ADF4159_F[7]	= (0x01030083 & 0xFE0FFFFF) | (Neg_Current * 0x0400000); // control bit = 001

	ADF4159_F[6]	= 0x00180104; // R4 - Ramp 1
	ADF4159_F[5]	= 0x00180144; // R4 - Ramp 2
	ADF4159_F[4]	= 0x00000005; // R5 - Ramp 1
	ADF4159_F[3]	= 0x00800005; // R5 - Ramp 2
	ADF4159_F[2]	= 0x00000006; // R6 - Ramp 1
	ADF4159_F[1]	= 0x00800006; // R6 - Ramp 2
	ADF4159_F[0]	= 0x00000007; // R7

	ADF4159_Write();
}

void ADF4159_VCO_ENable(unsigned int val)
{
	XGpioPs_WritePin(&psgpio, ADF4159_CE, val); // cho chip hoat dong
}

void ADF4159_Innit(void)
{
	//XGpioPs_WritePin(&psgpio, ADF4159_CE, 1); // cho chip hoat dong
	//XGpioPs_WritePin(&psgpio, ADF4159_CLK, 0);
	//XGpioPs_WritePin(&psgpio, ADF4159_DATA, 0);
	//XGpioPs_WritePin(&psgpio, ADF4159_LE, 0);

	Bit2[0]	= 0x80000000; // bit 31
	Bit2[1]	= 0x40000000; // bit 30
	Bit2[2]	= 0x20000000; // bit 29
	Bit2[3]	= 0x10000000; // bit 28
	Bit2[4]	= 0x08000000; // bit 27
	Bit2[5]	= 0x04000000; // bit 26
	Bit2[6]	= 0x02000000; // bit 25
	Bit2[7]	= 0x01000000; // bit 24
	Bit2[8]	= 0x00800000; // bit 23
	Bit2[9]	= 0x00400000; // bit 22
	Bit2[10]= 0x00200000; // bit 21
	Bit2[11]= 0x00100000; // bit 20
	Bit2[12]= 0x00080000; // bit 19
	Bit2[13]= 0x00040000; // bit 18
	Bit2[14]= 0x00020000; // bit 17
	Bit2[15]= 0x00010000; // bit 16
	Bit2[16]= 0x00008000; // bit 15
	Bit2[17]= 0x00004000; // bit 14
	Bit2[18]= 0x00002000; // bit 13
	Bit2[19]= 0x00001000; // bit 12
	Bit2[20]= 0x00000800; // bit 11
	Bit2[21]= 0x00000400; // bit 10
	Bit2[22]= 0x00000200; // bit 9
	Bit2[23]= 0x00000100; // bit 8
	Bit2[24]= 0x00000080; // bit 7
	Bit2[25]= 0x00000040; // bit 6
	Bit2[26]= 0x00000020; // bit 5
	Bit2[27]= 0x00000010; // bit 3
	Bit2[28]= 0x00000008; // bit 3
	Bit2[29]= 0x00000004; // bit 2
	Bit2[30]= 0x00000002; // bit 1
	Bit2[31]= 0x00000001; // bit 0

	//ADF4159_Set_Freq(7700);// 12000 MHz = 12.0 Ghz
}

void ADF4159_Write(void)
{
    unsigned char row = 0;
    unsigned char spi_buf[4]; // 4 bytes = 32 bits
    uint32_t reg_data;

    //xil_printf("ADF4159: Starting configuration via Hardware SPI...\r\n");

    for(row = 0; row < 11; row++)
    {
        // Lấy giá trị thanh ghi từ mảng dữ liệu của bạn (ví dụ thanh ghi 32-bit)
        reg_data = ADF4159_F[10 - row];

        /* 
           Phân tách 32-bit thành 4 byte để gửi qua SPI.
           Chip ADF4159 yêu cầu gửi MSB (bit cao) trước.
        */
        spi_buf[0] = (unsigned char)((reg_data >> 24) & 0xFF);
        spi_buf[1] = (unsigned char)((reg_data >> 16) & 0xFF);
        spi_buf[2] = (unsigned char)((reg_data >> 8) & 0xFF);
        spi_buf[3] = (unsigned char)(reg_data & 0xFF);

        /* 
           Gửi dữ liệu qua SPI phần cứng (Pl_Spi4.fd tương ứng /dev/spidev5.0).
           Hàm này sẽ tự động hạ chân SS (LE) xuống và kéo lên sau khi gửi xong 4 byte.
        */
	   //int Status1 = SPI_Transfer(&Pl_Spi3, Pl_Spi2.fd, spi_buf, 3);
        if (SPI_Transfer( Pl_Spi4.fd, spi_buf,NULL, 4) != 0) {
            //xil_printf("ADF4159: Error writing register %d\n", 10-row);
        }

        /* 
           Nghỉ một khoảng ngắn để PLL ổn định hoặc chip xử lý. 
           Bạn có thể để usleep(1000) (1ms) cho an toàn.
        */
        usleep(1000); 
    }

    //xil_printf("ADF4159: Done. 11 registers latched.\r\n");
}

