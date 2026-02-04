#include "DK_Maythu.h"
#include "ADF4159.h"
#include "ADF4360.h"
#include "CAN_Control.h"
#include "Erob.h"
#include "MI_505.h"
#include "Radar_HW.h"
#include "User_RAM.h"
#include <stdio.h>
#include <stdlib.h>

// khai bao cac struct su dung
struct Radar_Struct Radar;
struct Radar_Debug_Struct Radar_Debug;
struct Frame_Struct Frame;
struct PC8_def PC8;
struct PC8_def PC8s;
struct AFC_Struct AFC1;
struct AIS_Struct AIS_Data;
struct UDP_Struct UDP_Frame;
struct UDP_Phase UDP_Phase1;
struct CMAC_Struct CMAC_Struct1;

u8 SendBuffer0[TEST_BUFFER_SIZE]; /* Buffer for Transmitting Data */
u8 RecvBuffer0[TEST_BUFFER_SIZE]; /* Buffer for Receiving Data */

unsigned char Radar_Video8[48000];
unsigned char Radar_Video9[48000];
unsigned char Radar_Video10[48000];
unsigned char Radar_Video11[48000];
unsigned char Radar_Window[48000];
unsigned char Beam17i[800];
unsigned char Beam17q[800];
unsigned char Beam17[800];

unsigned char _V1_data[800]; // [-2;0]
unsigned char _V2_data[800]; // [2;0]

int16_t Videop[8];
uint32_t Video_AVG1;
uint32_t Video_AVG2;
uint32_t Video_AVG3;
uint32_t Video_AVG4;

int32_t Video_Sub1;
int32_t Video_Sub2;
int32_t Video_Sub3;
int32_t Video_Sub4;

int32_t Video_PC8[4096];

uint16_t point = 0;
uint16_t pointi1 = 0;
uint16_t pointi2 = 0;
uint16_t pointi3 = 0;
uint16_t pointi4 = 0;
uint32_t data_code = 0;
uint16_t Rp = 0;
uint32_t Rv = 0;
uint8_t Ro = 0;

unsigned char Video_fbx[42720];
unsigned char Video_fb17[42720];

uint16_t VP1 = 0;
uint16_t VP2 = 0;
uint16_t VP3 = 0;
uint16_t VP4 = 0;

unsigned char Radar_Frame1[Data_length];
unsigned char Video_Frame[21460];
unsigned char Radar_Frames1[Data_length];
unsigned char Radar_Frame_M[Data_length];
unsigned char Radar_Frame_Mp[Data_length];
unsigned char Radar_Video_final[Data_length];
unsigned char Radar_Frame2[2048];
unsigned char PC2_Frame[100];
unsigned char PC8_Frame_in[100];
unsigned char PC8_Frame_out[20000];
unsigned char Fix_Frame[100];
unsigned char PA_Data[Data_length];
unsigned char PA_Data2[Data_length];

unsigned int clear_Counter = 0;
unsigned int Delay_Bamsat_time = 0;
unsigned int Ri = 0;

void Radar_check_Disconect(void) {
  if (AFC1.Auto_Start == 0)
    AFC1.Auto_Counter++;

  if (AFC1.Auto_Counter > 1500) {
    AFC1.Auto_Start = 1;
    AFC1.Disconect = 1;
  }
}

void Radar_taogia_Encoder(void) {

  Radar.Beta_SW_en++;
  if (Radar.Beta_SW_en >= 4) {
    Radar.Beta_SW_en = 0;
    Radar.Beta_SW++;
    if (Radar.Beta_SW >= 3600)
      Radar.Beta_SW = 0;
  }
}

void Radar_Read_Encoder(void) {

  if ((PC8.Trigger == 1) || (PC8.Gia_Quay_EN == 1)) {
    Radar.Beta_HW = Radar.Beta_SW;
  } else {
    Radar.Beta_HW = (uint16_t)(Erob_Motor.Motor_PhuongVi.UDP_Angle * 10);
  }

  if (Radar.Beta_HW >= 3600)
    Radar.Beta_HW = 0;
  IP_Set_Beta_in(Radar.Beta_HW);

  PC8.Beta_Angle = Radar.Beta_HW;
  if (PC8.Beta_Angle >= 3600)
    PC8.Beta_Angle = 0;

  Radar.Errori = 0;

  if (Radar.ENC0 >= Radar.Beta_HW) {
    Radar.Errori = Radar.ENC0 - Radar.Beta_HW;
  } else if (Radar.ENC0 < Radar.Beta_HW) {
    Radar.Errori = Radar.Beta_HW - Radar.ENC0;
  }

  if (Radar.Errori > 1)
    Radar.ENC_Error++;

  // Radar.Error_Counter++;
  Radar.ENC0 = Radar.Beta_HW;

  if ((Radar.Beta_HW <= 5) && (Radar.Debug_ENC == 0)) {
    // Radar.Error_Counter = 0;
    Radar.Debug_ENC = 1;
    Radar.ENC_Error1 = Radar.ENC_Error;
    Radar.ENC_Error = 0;
  }
  if (Radar.Beta_HW > 10)
    Radar.Debug_ENC = 0;
}

void Radar_Setup_Header(void) {
  if (Radar.Range <= 1) {
    Radar.Sample = 356;
    Radar.ADC_Freq0 = 40;
  } else if (Radar.Range == 2) {
    Radar.Sample = 1124;
    Radar.ADC_Freq0 = 40;
  } else if (Radar.Range == 3) {
    Radar.Sample = 2148;
    Radar.ADC_Freq0 = 40;
  } else if (Radar.Range == 4) {
    Radar.Sample = 4196;
    Radar.ADC_Freq0 = 40;
  } else if (Radar.Range == 5) {
    Radar.Sample = 4196;
    Radar.ADC_Freq0 = 10;
  } else if (Radar.Range == 6) {
    Radar.Sample = 4196;
    Radar.ADC_Freq0 = 10;
  } else if (Radar.Range == 7) {
    Radar.Sample = 4196;
    Radar.ADC_Freq0 = 10;
  }

  Radar.ENC_Compass = 0;   //
  Radar.Radar_Azimuth = 0; //

  if (Frame.Radar_AIS_Valid == 1) {
    AIS_Data.AIS_PC2 = 1; //
    AIS_Data.AIS_PC1 = 1; //
    Frame.Radar_AIS_Valid = 0;
  }

  if (Radar.Compass_PC2_EN == 0) {
    if (Radar.Compass_PC2_Fix < 50) {
      if (Radar.Radar_Azimuth >= (50 - Radar.Compass_PC2_Fix)) {
        Frame.Radar_Azimuth = Radar.Radar_Azimuth + Radar.Compass_PC2_Fix - 50;
      } else if (Radar.Radar_Azimuth < (50 - Radar.Compass_PC2_Fix)) {
        Frame.Radar_Azimuth =
            3550 + Radar.Radar_Azimuth + Radar.Compass_PC2_Fix;
      }
    } else if (Radar.Compass_PC2_Fix >= 50) {
      Frame.Radar_Azimuth = Radar.Radar_Azimuth + Radar.Compass_PC2_Fix - 50;

      if (Frame.Radar_Azimuth >= 3600)
        Frame.Radar_Azimuth = Frame.Radar_Azimuth - 3600;
    }
  } else if (Radar.Compass_PC2_EN == 1) {
    Frame.Radar_Azimuth = Radar.Compass_PC2_Val;
  }

  Frame.Radar_Analog_Compass = Frame.Radar_Azimuth;

  Frame.Radar_Header = 0x55AA;
  // Frame.Radar_Frame_size		=length;
  Frame.Radar_Frame_size = Radar.Sample - 100;
  Frame.Radar_Beta = Radar.Beta_HW;
  Frame.Radar_Thang_Culy = Radar.Range;
  Frame.Radar_Sample_Size = Radar.Sample_Size;
  Frame.Radar_On_Off = Radar.Phat; // 1;
  Frame.Radar_2_2A = Radar.Radar_2_2A;
  Frame.Radar_ADC_Freq = Radar.ADC_Freq0;
  Frame.Radar_Signal_Freq = AFC1.TSP_AFC;
  Frame.Radar_Gain = PC8.KDMT;
  Frame.Radar_VCO = PC8.Rong_Bapy;
  Frame.Radar_BAPY = PC8.Sau_Bapy;
  Frame.Radar_APC = Radar.Chong_Nhieu; // 0;
  Frame.Radar_AFC = Radar.AFC;         // 0;
  Frame.Radar_MPV = Radar.DK4;         // Radar.Vi_Phan;
  Frame.Radar_Error = Radar.Ngoai_sai; // AFC1.Fnsai;
  // Frame.Radar_Speed			=150;
  // Frame.Radar_Din				=0;// bit0: Z0, bit1: 2/2a,
  // bit4: M, bit5: BARKER, bit6:LFM, bit7:PULSE Frame.Radar_Delay_2
  // =0; Frame.Radar_Delay_2A		=0; Frame.Radar_Dutru
  // =0;
  Frame.Radar_Number_IFF = 0;
  // Frame.Radar_AIS_Valid		=0;
  // Frame.Radar_AIS_Status		=0;
  // Frame.Radar_AIS_MMSI		=0;
  // Frame.Radar_AIS_Longitude	=0; //Latitude
  // Frame.Radar_AIS_Latitude	=0;
  Frame.Radar_GPS_Speed = Radar.GPS_Speed;
  // Frame.Radar_Analog_Compass	=0;
  // Frame.Radar_AIS_Azimuth		=340;
  // Frame.Radar_Wind_Speed		=0;
  // Frame.Radar_Wind_Angle		=0;
  Frame.Radar_GPS_Status = 1;
  Frame.Radar_GPS_Satelites = 0;
  Frame.Radar_GPS_Time = 0;
  // Frame.Radar_GPS_Longitude	=103.6789;
  // Frame.Radar_GPS_Latitude	=21.1234;
  Frame.Radar_IFF_Serial1 = 1;
  Frame.Radar_IFF_Serial2 = 2;
  Frame.Radar_IFF_Serial3 = 3;
  Frame.Radar_IFF_Serial4 = 4;
  Frame.Radar_IFF_Serial5 = 5;
  Frame.Radar_IFF_Serial6 = 6;
  Frame.Radar_IFF_Serial7 = 7;

  Fix_Frame[0] = 0xaa;
  Fix_Frame[1] = 0x55;
  Fix_Frame[2] = Frame.Radar_Frame_size % 256;
  Fix_Frame[3] = Frame.Radar_Frame_size / 256;
  Fix_Frame[4] = Frame.Radar_Beta % 256;
  Fix_Frame[5] = Frame.Radar_Beta / 256;
  Fix_Frame[6] = Frame.Radar_Thang_Culy;
  Fix_Frame[7] = Frame.Radar_Sample_Size;
  Fix_Frame[8] = Frame.Radar_On_Off;
  Fix_Frame[9] = Frame.Radar_2_2A;

  Fix_Frame[10] = Frame.Radar_ADC_Freq;
  Fix_Frame[11] = Frame.Radar_Signal_Freq;
  Fix_Frame[12] = Frame.Radar_Gain;
  Fix_Frame[13] = Frame.Radar_VCO;
  Fix_Frame[14] = Frame.Radar_BAPY;
  Fix_Frame[15] = Frame.Radar_APC;
  Fix_Frame[16] = Frame.Radar_AFC;
  Fix_Frame[17] = Frame.Radar_MPV;
  Fix_Frame[18] = Frame.Radar_Error % 256;
  Fix_Frame[19] = Frame.Radar_Error / 256;
  Fix_Frame[20] = Frame.Radar_Speed;

  Fix_Frame[21] = (Frame.Radar_AIS_MMSI & 0x000000ff);
  Fix_Frame[22] = (Frame.Radar_AIS_MMSI & 0x0000ff00) >> 8;
  Fix_Frame[23] = (Frame.Radar_AIS_MMSI & 0x00ff0000) >> 16;
  Fix_Frame[24] = (Frame.Radar_AIS_MMSI & 0xff000000) >> 24;

  Fix_Frame[25] = Frame.Radar_Number_IFF;
  Fix_Frame[26] = AIS_Data.AIS_PC1; // 1 :  co ban tin AIS; 0: khong co du lieu
  Fix_Frame[27] = Frame.Radar_AIS_Status;

  Fix_Frame[28] = Frame.Radar_Analog_Compass % 256;
  Fix_Frame[29] = Frame.Radar_Analog_Compass / 256;

  Fix_Frame[30] = Frame.Radar_AIS_Speed % 256;
  Fix_Frame[31] = Frame.Radar_AIS_Speed / 256;

  Fix_Frame[32] = (Frame.Radar_AIS_Longitude & 0x000000ff);
  Fix_Frame[33] = (Frame.Radar_AIS_Longitude & 0x0000ff00) >> 8;
  Fix_Frame[34] = (Frame.Radar_AIS_Longitude & 0x00ff0000) >> 16;
  Fix_Frame[35] = (Frame.Radar_AIS_Longitude & 0xff000000) >> 24;

  Fix_Frame[36] = (Frame.Radar_AIS_Latitude & 0x000000ff);
  Fix_Frame[37] = (Frame.Radar_AIS_Latitude & 0x0000ff00) >> 8;
  Fix_Frame[38] = (Frame.Radar_AIS_Latitude & 0x00ff0000) >> 16;
  Fix_Frame[39] = (Frame.Radar_AIS_Latitude & 0xff000000) >> 24;

  Fix_Frame[40] = Frame.Radar_GPS_Speed % 256;
  Fix_Frame[41] = Frame.Radar_GPS_Speed / 256;
  Fix_Frame[42] = Frame.Radar_AIS_Angle % 256;
  Fix_Frame[43] = Frame.Radar_AIS_Angle / 256;
  Fix_Frame[44] = Frame.Radar_Azimuth % 256;
  Fix_Frame[45] = Frame.Radar_Azimuth / 256;
  Fix_Frame[46] = Frame.Radar_Wind_Speed % 256;
  Fix_Frame[47] = Frame.Radar_Wind_Speed / 256;
  Fix_Frame[48] = Frame.Radar_Wind_Angle % 256;
  Fix_Frame[49] = Frame.Radar_Wind_Angle / 256;

  Fix_Frame[50] = Frame.Radar_GPS_Status;
  Fix_Frame[51] = Frame.Radar_GPS_Satelites;

  Fix_Frame[52] = (Frame.Radar_GPS_Time & 0x000000ff);
  Fix_Frame[53] = (Frame.Radar_GPS_Time & 0x0000ff00) >> 8;
  Fix_Frame[54] = (Frame.Radar_GPS_Time & 0x00ff0000) >> 16;
  Fix_Frame[55] = (Frame.Radar_GPS_Time & 0xff000000) >> 24;

  memcpy(&Radar.ures, &Frame.Radar_GPS_Longitude, 8);
  Fix_Frame[56] = Radar.ures[0];
  Fix_Frame[57] = Radar.ures[1];
  Fix_Frame[58] = Radar.ures[2];
  Fix_Frame[59] = Radar.ures[3];
  Fix_Frame[60] = Radar.ures[4];
  Fix_Frame[61] = Radar.ures[5];
  Fix_Frame[62] = Radar.ures[6];
  Fix_Frame[63] = Radar.ures[7];

  memcpy(&Radar.ures2, &Frame.Radar_GPS_Latitude, 8);
  Fix_Frame[64] = Radar.ures2[0];
  Fix_Frame[65] = Radar.ures2[1];
  Fix_Frame[66] = Radar.ures2[2];
  Fix_Frame[67] = Radar.ures2[3];
  Fix_Frame[68] = Radar.ures2[4];
  Fix_Frame[69] = Radar.ures2[5];
  Fix_Frame[70] = Radar.ures2[6];
  Fix_Frame[71] = Radar.ures2[7];

  Fix_Frame[72] = (Frame.Radar_IFF_Serial1 & 0x000000ff);
  Fix_Frame[73] = (Frame.Radar_IFF_Serial1 & 0x0000ff00) >> 8;
  Fix_Frame[74] = (Frame.Radar_IFF_Serial1 & 0x00ff0000) >> 16;
  Fix_Frame[75] = (Frame.Radar_IFF_Serial1 & 0xff000000) >> 24;

  Fix_Frame[76] = (Frame.Radar_IFF_Serial2 & 0x000000ff);
  Fix_Frame[77] = (Frame.Radar_IFF_Serial2 & 0x0000ff00) >> 8;
  Fix_Frame[78] = (Frame.Radar_IFF_Serial2 & 0x00ff0000) >> 16;
  Fix_Frame[79] = (Frame.Radar_IFF_Serial2 & 0xff000000) >> 24;

  Fix_Frame[80] = (Frame.Radar_IFF_Serial3 & 0x000000ff);
  Fix_Frame[81] = (Frame.Radar_IFF_Serial3 & 0x0000ff00) >> 8;
  Fix_Frame[82] = (Frame.Radar_IFF_Serial3 & 0x00ff0000) >> 16;
  Fix_Frame[83] = (Frame.Radar_IFF_Serial3 & 0xff000000) >> 24;

  Fix_Frame[84] = (Frame.Radar_IFF_Serial4 & 0x000000ff);
  Fix_Frame[85] = (Frame.Radar_IFF_Serial4 & 0x0000ff00) >> 8;
  Fix_Frame[86] = (Frame.Radar_IFF_Serial4 & 0x00ff0000) >> 16;
  Fix_Frame[87] = (Frame.Radar_IFF_Serial4 & 0xff000000) >> 24;

  Fix_Frame[88] = (Frame.Radar_IFF_Serial5 & 0x000000ff);
  Fix_Frame[89] = (Frame.Radar_IFF_Serial5 & 0x0000ff00) >> 8;
  Fix_Frame[90] = (Frame.Radar_IFF_Serial5 & 0x00ff0000) >> 16;
  Fix_Frame[91] = (Frame.Radar_IFF_Serial5 & 0xff000000) >> 24;

  Fix_Frame[92] = (Frame.Radar_IFF_Serial6 & 0x000000ff);
  Fix_Frame[93] = (Frame.Radar_IFF_Serial6 & 0x0000ff00) >> 8;
  Fix_Frame[94] = (Frame.Radar_IFF_Serial6 & 0x00ff0000) >> 16;
  Fix_Frame[95] = (Frame.Radar_IFF_Serial6 & 0xff000000) >> 24;

  Fix_Frame[96] = (Frame.Radar_IFF_Serial7 & 0x000000ff);
  Fix_Frame[97] = (Frame.Radar_IFF_Serial7 & 0x0000ff00) >> 8;
  Fix_Frame[98] = (Frame.Radar_IFF_Serial7 & 0x00ff0000) >> 16;
  Fix_Frame[99] = (Frame.Radar_IFF_Serial7 & 0xff000000) >> 24;

  AIS_Data.AIS_PC1 = 0;
}

void Radar_Reload_Frame(void) { memcpy(&Radar_Frame1[0], &Fix_Frame, 100); }

// Ham so sanh cho qsort
int compare_ints(const void *a, const void *b) {
  return (*(int *)a - *(int *)b);
}

void median_filter(int *input, int *output, int N, int window_size) {
  //    if (window_size % 2 == 0 || window_size < 1) {
  //        printf("Window size phai la so le duong.\n");
  //        return;
  //    }
  int radius = (window_size - 1) / 2;
  int padded_len = N + 2 * radius;
  int *padded = (int *)malloc(padded_len * sizeof(int));
  int *window = (int *)malloc(window_size * sizeof(int));

  if (!padded || !window) {
    free(padded);
    free(window);
    printf("Loi cap phat bo nho.\n");
    return;
  }

  // Pad dau va cuoi mang
  for (int i = 0; i < radius; i++)
    padded[i] = input[0];
  for (int i = 0; i < N; i++)
    padded[radius + i] = input[i];
  for (int i = 0; i < radius; i++)
    padded[radius + N + i] = input[N - 1];

  // ap dung median filter
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < window_size; j++)
      window[j] = padded[i + j];
    qsort(window, window_size, sizeof(int), compare_ints);
    output[i] = window[radius];
  }

  free(padded);
  free(window);
}

void Radar_Read_Frame(void) {
  Radar_Read_Encoder();
  Radar_Setup_Header();

  //	if(PC8.code_data == 1)
  //	{
  //		if(PC8.Control_View_SS == 9) // Video_Pulse16_Addr //
  // Video_Code163_Addr
  //		{
  //			Linux_memcpy(&Radar_Frame_M[0], Video_Pulse16_Addr,
  // 24000);
  //		}
  //		else // Video_Pulse16_Addr // Video_Code162_Addr
  //		{
  // Linux_memcpy(&Radar_Frame_M[0], Video_Pulse16_Addr, 8000); // 240000
  //		}
  //	}
  //	else if(PC8.code_data == 0)
  //	{
  //		Linux_memcpy(&Radar_Frame_Mp[0], Video_Pulse16_Addr, 21360);
  //	}
  //
  //	if(PC8.BS_Start == 1) // cap nhat toa do cua song
  //	{
  //		//Linux_memcpy(&Video_fb17, Video_fb17_Addr, 21360);
  //	}
  //
  memcpy(&Video_Frame[0], &Fix_Frame, 100);
  memcpy(&Video_Frame[100], &Radar_Frame_M[0], 21360);
  //
  //
  //	if(Radar.IP_Code == 0)
  //	{
  //		if(PC8.code_data == 1)
  //		{
  //			Radar.IP_Data_ready = 1;
  //			memcpy(&Radar_Frame1[3300], &Radar_Frame_M[4216],
  // 18160);
  //		}
  //		else if(PC8.code_data == 0)
  //		{
  //			memcpy(&Radar_Frame1[100], &Radar_Frame_Mp[0], 3200);
  //		}
  //	}
  //	else if(Radar.IP_Code == 1) // barker 5
  //	{
  //		Radar.IP_Data_time = 0;
  //
  //		if(PC8.code_data == 1)
  //		{
  //			Radar.IP_Data_ready = 1;
  //			memcpy(&Radar_Frame1[3300], &Radar_Frame_M[4216],
  // 18160);
  //		}
  //		else if(PC8.code_data == 0)
  //		{
  //			memcpy(&Radar_Frame1[100], &Radar_Frame_Mp[88], 3200);
  //		}
  //	}
  //	else if(Radar.IP_Code == 2) // barker 7
  //	{
  //		Radar.IP_Data_time = 0;
  //
  //		if(PC8.code_data == 1)
  //		{
  //			Radar.IP_Data_ready = 1;
  //			memcpy(&Radar_Frame1[3300], &Radar_Frame_M[4216],
  // 18160);
  //		}
  //		else if(PC8.code_data == 0)
  //		{
  //			memcpy(&Radar_Frame1[100], &Radar_Frame_Mp[120], 3200);
  //		}
  //	}
  //	else if(Radar.IP_Code == 3) // barker 13
  //	{
  //		Radar.IP_Data_time = 0;
  //
  //		if(PC8.code_data == 1)
  //		{
  //			Radar.IP_Data_ready = 1;
  //			memcpy(&Radar_Frame1[3300], &Radar_Frame_M[4216],
  // 18160);
  //		}
  //		else if(PC8.code_data == 0)
  //		{
  //			memcpy(&Radar_Frame1[100], &Radar_Frame_Mp[216], 3200);
  //		}
  //	}
}

void Radar_Innit(void) {
  Radar_Innit_Frame();
  Radar_Innit_Uart();
  Radar_Innit_SPI();
  Radar_Innit_GPIO();
  Erob_Data_Innit();

  ADF4159_VCO_ENable(0);

  Radar.UDP_PC8_Get_New_Screen = 1;
  Radar.Beta_SW_en = 0;
  Radar.Beta_Re_en = 0;
  Radar.Beta_Re_cnt = 0;

  Erob_Motor.Motor_PhuongVi.CAN_Innit_Ok = 1;
  Erob_Motor.Motor_GocTa.CAN_Innit_Ok = 1;

  Erob_Motor.Motor_PhuongVi.CAN_Innit_Time = 0;
  Erob_Motor.Motor_GocTa.CAN_Innit_Time = 0;

  // CAN_Innit_Data();
  CAN_Init_All();

  PC8.CMD_RF_ENABLE = 1;
  PC8.CMD_RF_ON_OFF = 0;

  xil_printf("System_Set_RF_ENABLE \r\n");
  System_Set_RF_ENABLE(PC8.CMD_RF_ENABLE); // ENABLE
  System_Set_RF_ON_OFF(PC8.CMD_RF_ON_OFF); // TAT PHAT

  xil_printf("Radar_Update_Uart_DKSG \r\n");
  Radar_Update_Uart_DKSG();

  xil_printf("System_Set_Phase_Start_Read \r\n");
  System_Set_Phase_Start_Read(0);

  xil_printf("Radar_Innit_Phase \r\n");
  Radar_Innit_Phase();

  xil_printf("Radar_Update_Uart_DKSG \r\n");
  System_Set_RDY_EN(0);
  IP_Set_RST(0); // Reset IP FWMC_Radar
  // CAN1_Set_Dir(1);
  // CAN2_Set_Dir(1);

  Radar.Config_Done = 0;
  UDP_Phase1.AXI_GPIO_IN = 1;
  // UDP_Phase2.AXI_GPIO_IN = 2;

  //	XAdc_Innit(XADC_DEVICE_ID);

  // Radar_Innit_Interrupt();
  xil_printf("PC8_Control_HW \r\n");
  PC8_Control_HW();
  PC8_innit_Bamsat();
  Innit_Beam_data(); // tinh toan beam v1, v2, v3, v4
  Radar_Innit_DAC_Phase();

  PC8.Beam_Switch = 0;
  Radar.Beta_HWi = 0;

  xil_printf("System_Set_Beta_Epxilon_sel \r\n");
  System_Set_Beta_Epxilon_sel(0);
  System_Set_BS_Enable(0);
  System_Set_BS_Angle(0);
  System_Set_BS_Channel(0);
  System_Set_BS_Sel(0);
  System_Set_Beam_Sel(0);

  System_Set_FFT_Sel(PC8.FFT_Sel);
  System_Set_sum_Sel(PC8.Sum_Sel);

  
  xil_printf("System_Set_BS_Start \r\n");
  System_Set_BS_Start(0);
  System_Set_BS_Stop(200);
  System_Set_BS_Size(200);

  System_Set_BS_Start4(0);
  System_Set_BS_Stop4(200);
  System_Set_BS_Size4(200);

  xil_printf("System_Set_Beam17_RAM \r\n");
  System_Set_Beam17_RAM(5);

  System_Set_Beam_Channel(0);
  System_Set_fb_Channel(0);

  
  xil_printf("ADF4159_Innit \r\n");
  ADF4159_Innit();
  // ADF4159_Set_Freq(7700);
  xil_printf("ADF4159_Innit done\r\n");

  ADF4360_Innit();
  // ADF4360_Set_Freq(1400);
  xil_printf("ADF4360_Innit done\r\n");

  System_Set_IP_B17_Sel(0);
  System_Set_IP_Bx_Sel(0);
  System_Set_IP_code_Sellect(0);

  PC8.Screen_Phase = 0;
  System_Set_Beam_start(0);

  Radar.IP_Pulse_Width = 200;
  System_Set_IP_Pulse_Width(Radar.IP_Pulse_Width);
  System_Set_IP_PulseSel(1);
  IP_Set_thang_culy(5);
  Radar.IP_Code = 2;
  System_Set_IP_code(Radar.IP_Code);

  usleep(10);
  System_Set_FIFO_RST(1);
  IP_Set_RST(1); // Run IP FWMC_Radar
  usleep(10000);

  System_Set_FIFO_RST(0);
  System_Set_RDY_EN(1);

  Cua_Song_Innit();

  Erob_Motor_Innit();

  Radar_Setup_Header();
  kalman_Innit(); // khoi tao bo loc kalman
}
