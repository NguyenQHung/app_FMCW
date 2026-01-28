#include "main.h"

struct Radar_Struct
	{
	unsigned char 		Read_ok;
	unsigned char 		Read_DMA1_ok;
	unsigned char 		Read_Innit_ok;
	unsigned char		Radar_Frame[Data_length];	// du lieu truyen len PC
	unsigned char		Radar_Config[Config_length];	// du lieu cau hinh XLPC
	unsigned char		Radar_Config1[Config_length];	// du lieu cau hinh XLPC
	unsigned char		PC8_Config[320];	// du lieu cau hinh XLPC
	unsigned char		PC8_Excel[828];	// du lieu cau hinh XLPC
	unsigned char		Radar_Config2[80];	// du lieu cau hinh XLPC
	unsigned char		Radar_Config3[80];	// du lieu cau hinh XLPC
	unsigned char		Radar_UART_Frame[100];	// du lieu truyen len PC
	unsigned char		Radar_UART_Frame2[10];	// du lieu truyen len PC
	unsigned char		UART_Frame0[50];	//
	unsigned char		UART_Frame1[50];	//
	unsigned char		UART_Frame2[50];	//
	unsigned char		UART_Frame3[50];	//
	unsigned char		UART_Frame4[50];	//
	unsigned char		Radar_DKSG[80];	// du lieu cau hinh XLPC
	unsigned char		Uart_Counter2;
	unsigned char		Uart_Counter;
	unsigned char		Uart_Start;
	unsigned char		Uart_Status;
	unsigned char		Uart_Complete;
	unsigned int		UDP_counter1;
	unsigned int		UDP_counter2;
	unsigned int		UDP_counter3;
	unsigned char		Receiver_Status;
	unsigned char		Receiver_Status2;
	unsigned char		Receiver_Status3;
	unsigned char		Receiver_Status4;
	unsigned char		Receiver_Excel;
	unsigned char		Receiver_CAN0;
	unsigned char		Receiver_CAN1;
	unsigned char		Receiver_Speed;
	unsigned char 		Config1;
	unsigned char 		Config2;
	unsigned char 		Led_Status;
	unsigned char 		Radar_2_2A;
	unsigned char 		Radar_2_2Ai;
	unsigned char 		Ma_Dieu_Che;
	unsigned char 		Active;
	unsigned char 		On;
	unsigned char 		Start;
	unsigned char 		Range;
	unsigned char 		IP_culy;
	unsigned char 		IP_Code;
	unsigned int 		IP_Pulse_Widthi;
	unsigned int 		IP_Pulse_Width;
	unsigned char 		IP_Data_time;
	unsigned char 		IP_Data_time2;
	unsigned char 		IP_Data_ready;

	unsigned char 		Range2;
	uint16_t 			ADC_Cells;
	uint32_t 			ADC_Cells2;
	uint16_t 			ADC_Time_us;
	uint16_t 			ADC_Num_CLK;
	uint16_t 			ADC_Pulse_CLK;

	unsigned char 		ADC_Freq0;
	uint16_t 			Start_Angle;
	uint16_t 			End_Angle;
	uint16_t 			Angle_Start;
	uint16_t 			Angle_Stop;
	unsigned char 		Angle_mode_ENA;
	unsigned char 		Trigger_MUX;
	unsigned char 		Pulse_Source;
	uint16_t 			Pulse_Width;
	unsigned char 		Sample_Size;
	unsigned char 		Test_mode;
	unsigned char 		Delay_2;
	unsigned char 		Delay_2A;
	unsigned char 		TriggerLevel;
	unsigned char 		ShortTime;
	unsigned char 		Dutru2[34];
	unsigned int 		Sample;
	unsigned int 		Start_Delay;
	unsigned int 		End_Delay;
	unsigned char 		Config_Done;
	unsigned char 		GPS_Frame[200];
	unsigned char 		GPS_Start;
	unsigned char 		GPS_Done;
	unsigned char 		COMPASS_Done;
	unsigned char 		COMPASS_A_Done;
	unsigned char 		ANTEN_Done;
	unsigned int 		AIS_Counter;
	unsigned char 		AIS_Connect;
	unsigned char 		GPS_Counter;
	unsigned char 		GPS_Detect;
	unsigned int		GPS_Speed;
	unsigned int 		Beta_SW;
	unsigned int 		Beta_SW_en;
	unsigned int 		Beta_Re_en;
	unsigned int 		Beta_Re_cnt;
	unsigned char 		IO1;
	unsigned char 		IO2;
	unsigned char 		XC3S200A_Frame[15];
	unsigned int  		Board_speed1;
	unsigned char 		real_Angle;
	unsigned char 		Compass1_Connect;
	unsigned char 		Compass2_Connect;
	unsigned char 		GPS_Connect;
	unsigned char 		GPS_Status;
	unsigned char 		GPS_counter;
	unsigned char 		Compass1_Status;
	unsigned char 		Compass2_Status;
	unsigned char 		Compass_counter;
	unsigned char 		Compass_PC2_EN;
	uint16_t 			Compass_PC2_Val;
	uint16_t 			Compass_PC2_Fix;
	uint16_t			Radar_Azimuth;
	unsigned char 		GPS_Buff[10];
	unsigned char 		GPS_Buff1;
	unsigned char 		bit[8];
	uint16_t 			Beta_HW;
	uint16_t 			Beta_HWi;
	uint16_t 			Ngoai_sai;
	uint16_t 			Ngoai_sai0;
	unsigned char 		AFC;
	unsigned char 		AFC_Point;
	uint16_t 			U_Nguong;
	unsigned char 		Phat;
	unsigned char 		Soi_Dot;
	unsigned char 		GPIO;
	unsigned char 		DK1;
	unsigned char 		DK2;
	unsigned char 		DK4;
	unsigned char 		Muc_Nhieu;
	unsigned char 		Chong_Nhieu;
	uint16_t 			Chong_Nhieu_Counter;
	uint16_t 			Start_Byte;
	uint16_t 			Sample_AVG;
	unsigned long int 	Sum;
	unsigned long int 	Sum2;
	unsigned char		AVG_Value;
	unsigned char		AVG_Value0;
	unsigned char 		Vi_Phan;
	unsigned char 		TC_TX;
	unsigned char 		Phat_TC;
	unsigned char 		SoiDot_TC;
	unsigned char 		Dong_SoiDot1;
	unsigned char 		Dong_SoiDot2;
	unsigned char 		Scope_Enable;
	unsigned char 		ENC_Update;
	unsigned char 		ENC_Update2;
	uint16_t 			Errori;
	uint16_t 			ENC0;
	uint16_t 			ENC_Error;
	uint16_t 			ENC_Error1;
	uint16_t 			Error_Counter;
	uint16_t 			B1;
	uint16_t 			B2;
	unsigned char 		Delta;
	unsigned char 		buffer;
	unsigned int 		Base_Counter_In;
	uint16_t 			Volt_Data[8];
	uint8_t				Volt_Channel;
	unsigned char 		Bando_Nhieu;
	unsigned char 		Noise_Active;
	unsigned char 		Noise_Clear;
	uint16_t 			Noise_counter;
	uint16_t 			Ni;
	uint16_t 			Noise_Sample;
	uint16_t 			Noise_Pvi;
	uint16_t 			Noise_Pvi1;
	uint16_t 			Wind_Speed;
	uint16_t 			Wind_Angle;
	unsigned char 		Wind_Connect;
	unsigned char 		Wind_Connect1;
	unsigned char 		Wind_Connect2;
	unsigned int 		Temp_val;
	unsigned int 		Temp_min;
	unsigned int 		Temp_max;
	unsigned char 		Pulse_Mode;
	unsigned char 		ENC_Filter_time;
	unsigned char 		ENC_Filter_Ena;
	unsigned char 		Debug_line;
	unsigned char 		Debug_ENC;
	unsigned char 		PC7_Control;
	unsigned char 		PC7_ThangCuLy;
	unsigned char 		PC7_ThangCuLyi;
	unsigned char 		ADC_Send_Start;
	unsigned char 		SWITCH;
	unsigned char 		GPS_Setup;
	unsigned char 		CSP_Updatei;

	char* 				Frame_CMD;
	char* 				Frame_CMD1;

	uint8_t 			ures_float[4];
	uint8_t 			ures[8];
	uint8_t 			ures2[8];
	uint16_t 			ENC_Compass;
	uint16_t 			DDS_Counter;
	uint8_t 			GPS_LAT[8];
	uint8_t 			GPS_LON[8];
	uint16_t 			counter;
	uint16_t 			Digital_Mode_ena;
	uint8_t 			May2_2A_Buf;
	uint8_t 			ATTG_Buf;

	// Global Variables to store results and handle data flow
	int				UDP_counter;
	int				Udp_counters;
	int				VCO_Done;
	int				UDP_Step;
	int				UDP_Step2;
	int				UDP_Step3;
	int				UDP_Speed_Video;
	int				UDP_Read_counter;
	int				UDP_Delay_T1;
	int				UDP_Delay_T2;
	int				UDP_Delay_T3;
	int				UDP_Delay_T4;
	int				UDP_Delay_T5;
	int				UDP_Delay_V1;
	int				UDP_Delay_V2;
	int				UDP_Delay_V3;
	int				UDP_Delay_V4;
	int				UDP_Delay_V5;
	int				Screen_Mode;
	int				Screen_Modei;
	int				UDP_Read_Start;
	int				UDP_PC2_Start;
	int				UDP_PC8_Start;
	int				UDP_PC8_Start2;
	int				UDP_PC1_Start;
	int				UDP_PC8_Read_video;
	int				UDP_PC8_Read_A1;
	int				UDP_PC8_Read_A2;
	int				UDP_PC8_Read_A3;
	int				UDP_PC8_Read_A4;
	int				UDP_PC8_Read_fb;
	int				UDP_PC8_Read_Full;
	int				UDP_PC8_Read_Point;
	int				UDP_PC8_Get_New_Screen;
	int				UDP_PC8_Send_Point;
	int				UDP_PC8_Screen_Point;
	int				UDP_PC8_Read_fb_done;
	int				UDP_PC8_Read_fb1;
	int				UDP_PC8_Read_fb2;
	int				UDP_PC8_Read_fb3;
	int				UDP_PC8_Read_fb4;
	int				UDP_PC8_Read_fb5;
	int				UDP_PC8_Read_fb6;
	int				UDP_PC8_Read_fb7;
	int				UDP_PC8_Read_fb8;
	int				UDP_PC8_Read_fb9;
	int				UDP_PC8_Read_fb10;
	int				UDP_PC8_Read_fb11;
	int				UDP_PC8_Read_fb12;
	int				UDP_PC8_Read_fb13;
	int				UDP_PC8_Read_fb14;
	int				UDP_PC8_Read_fb15;
	int				UDP_PC8_Read_fb16;
	int				UDP_PC8_Read_fb17;
	int				UDP_KDMT;
	};

struct Radar_Debug_Struct
	{
	unsigned char		Radar_Config[80];	// du lieu cau hinh XLPC
	unsigned char		Radar_Debug[150];	// du lieu cau hinh XLPC
	unsigned char		GPS_Frame[44];	// du lieu truyen len PC
	unsigned char		Wind_Frame[24];
	unsigned char		AIS_Frame[80];
	int32_t				DDS_Datai[20000];
	int32_t				DDS_Datao[20000];
	int32_t				DDS_IIRi[20000];
	unsigned char		Update_Debug;
	unsigned char		Update_Debug2;
	unsigned char		Send_Debug2;
	unsigned char		Update_Phase_Amp;
	unsigned char		Update_Wind;
	unsigned char		Update_LPF;
	unsigned char		Update_DDS_Source;
	unsigned char		Update_DDS_Sine;
	unsigned char		Update_DDS_Data;
	unsigned char		Update_GPS;
	unsigned char		Update_IIR_Mode;
	unsigned char		Update_Mul_Source;
	unsigned char		Update_Enable;
	unsigned char		Update_Enable2;
	unsigned char		Update_Offset;
	unsigned char		Update_Ngan_Dai_View;
	unsigned char		Update_Ngan_Dai_Mode;
	unsigned char		Update_Doggy_EN;
	uint32_t			Update_MAX1;
	uint32_t 			Update_MAX2;
	int32_t 			Update_MIN2;
	unsigned char		Update2_Doggy_EN;
	uint32_t			Update2_MAX1;
	uint32_t 			Update2_MAX2;
	double				kL0;
	double				bL1;
	double				bL2;
	double				aL0;
	double				aL1;
	double				aL2;
	double				IIZ0;
	double				IIZ1;
	double 				X1;
	double 				X;
	unsigned char		Receiver_Status;
	unsigned char		Config_done;
	unsigned char		Config_Start;
	unsigned int		Fix_Wind_Angle;
	unsigned int		Fix_Wind_Speed;
	unsigned char		GPS_LAT_Size;
	unsigned char		GPS_LON_Size;
	unsigned char		GPS_Speed_Size;
	unsigned char		GPS_Point_LAT1;
	unsigned char		GPS_Point_LAT2;
	unsigned char		GPS_Point_LAT3;
	unsigned char		GPS_Point_LAT4;
	unsigned char		GPS_Point_LAT5;
	unsigned char		GPS_Point_LAT6;
	unsigned char		GPS_Point_LAT7;
	unsigned char		GPS_Point_LAT8;
	unsigned char		GPS_Point_LON1;
	unsigned char		GPS_Point_LON2;
	unsigned char		GPS_Point_LON3;
	unsigned char		GPS_Point_LON4;
	unsigned char		GPS_Point_LON5;
	unsigned char		GPS_Point_LON6;
	unsigned char		GPS_Point_LON7;
	unsigned char		GPS_Point_LON8;
	unsigned char		GPS_Point_Speed1;
	unsigned char		GPS_Point_Speed2;
	unsigned char		GPS_Point_Speed3;
	unsigned char		GPS_Point_Speed4;
	unsigned char		GPS_Point_Speed5;
	unsigned char		Wind_Point_Angle1;
	unsigned char		Wind_Point_Angle2;
	unsigned char		Wind_Point_Angle3;
	unsigned char		Wind_Point_Speed1;
	unsigned char		Wind_Point_Speed2;
	unsigned char		Wind_Point_Speed3;
	unsigned char		Wind_Point_Speed4;

	};

struct Frame_Struct
	{
	uint16_t 			Radar_Header; 			// 0-1
	uint16_t 			Radar_Frame_size;		//2-3
	uint16_t 			Radar_Beta;				//4-5
	uint8_t 			Radar_Thang_Culy;		//6
	uint8_t 			Radar_Sample_Size;		//7
	uint8_t 			Radar_On_Off;			//8
	uint8_t 			Radar_2_2A;				//9
	uint8_t 			Radar_ADC_Freq;			//10
	uint8_t 			Radar_Signal_Freq;		//11
	uint8_t 			Radar_Gain;				//12
	uint8_t 			Radar_VCO;				//13
	uint8_t 			Radar_BAPY;				//14
	uint8_t 			Radar_APC;				//15
	uint8_t 			Radar_AFC;				//16
	uint8_t 			Radar_MPV;				//17
	uint16_t  			Radar_Error;			//18-19
	uint8_t 			Radar_Speed;			//20
	uint32_t  			Radar_AIS_MMSI;			//21-24
	uint8_t 			Radar_Number_IFF;		//25
	uint8_t 			Radar_AIS_Valid;		//26
	uint8_t				Radar_AIS_Status;		//27
	uint16_t 			Radar_Analog_Compass;	//28 - 29
	uint16_t 			Radar_AIS_Speed;		//30-31 van toc tau muc tieu (AIS)
	uint32_t  			Radar_AIS_Longitude; 	//32-35 Latitude
	uint32_t  			Radar_AIS_Latitude;		//36-39
	uint16_t	 		Radar_GPS_Speed;		//40-41 van toc tau ta
	uint16_t	 		Radar_AIS_Angle;		//42-43
	uint16_t	 		Radar_Azimuth;			//44-45
	uint16_t	 		Radar_Wind_Speed;		//46-47
	uint16_t	 		Radar_Wind_Angle;		//48-49
	uint8_t	 			Radar_GPS_Status;		//50
	uint8_t	 			Radar_GPS_Satelites;	//51
	uint32_t 			Radar_GPS_Time;			//52-55
	double				Radar_GPS_Longitude;	//56-63
	double				Radar_GPS_Latitude;		//64 - 71
	uint32_t 			Radar_IFF_Serial1;		// 72 - 75
	uint32_t 			Radar_IFF_Serial2;		// 76 - 79
	uint32_t 			Radar_IFF_Serial3;		// 80 - 83
	uint32_t 			Radar_IFF_Serial4;		// 84 - 87
	uint32_t 			Radar_IFF_Serial5;		// 88 - 91
	uint32_t			Radar_IFF_Serial6;		// 92 - 95
	uint32_t 			Radar_IFF_Serial7;		// 96 - 99
	};

struct UDP_Struct
	{
	uint32_t 			AXI_GPIO_IN;	// 0xAA55
	uint32_t 			Goc_Omega1;		// goc omega cho be phong 1 (0 - 3600). 1 don vi = 0.1 do
	uint32_t 			Goc_Omega2;		// goc omega cho be phong 2 (0 - 3600). 1 don vi = 0.1 do
	uint32_t 			Motor1_ENA;		// cho phep dieu khien be phong 1
	uint32_t 			Motor2_ENA;		// cho phep dieu khien be phong 2
	uint32_t			ENC1_Value;		// he so chia be phong 1, mac dinh 2880
	uint32_t			ENC2_Value;  	// he so chia be phong 2, mac dinh 2880
	uint32_t			Kiemtra1_ENA;	// he so chia enc be phong 1, mac dinh 2865/2880
	uint32_t			Kiemtra2_ENA;	// he so chia enc be phong 2, mac dinh 2865/2880
	uint32_t			Goc_Kiemtra1;	// bu sai so ENC tuyet doi cho be phong 1, mac dinh 2865-2880 = 15
	uint32_t			Goc_Kiemtra2;	// bu sai so ENC tuyet doi cho be phong 2, mac dinh 2865-2880 = 15
	uint32_t			ENC_CMD1;		// lenh can thuc thi 1
	uint32_t			ENC_Status1;	// lenh can thuc thi 1
	uint32_t			ENC_CMD2;		// lenh can thuc thi 2
	uint32_t			ENC_Status2;	// lenh can thuc thi 2
	uint32_t			Calib1_Point;	// gia tri encoder khi dia co khi be 1 = 0
	uint32_t			Calib2_Point;	// gia tri encoder khi dia co khi be 1 = 0
	uint32_t			Motor_Rotate1;	// gia tri encoder khi dia co khi be 1 = 0
	uint32_t			Motor_Rotate2;	// gia tri encoder khi dia co khi be 1 = 0
	float				Motor1_Angle;
	float				Motor1_Lygiac;
	float				Motor2_Angle;
	float				Motor2_Lygiac;
    int 				BS_Delta_Angle1;    //
    int 				BS_Delta_Angle2;    //
    float 				Quat_Delta1;    //
    float 				Quat_Delta2;	//
    uint32_t 			Phuong_vi_Dir;    //
    uint32_t 			Goc_ta_Dir;	//
	uint32_t 			VCO_Val1;		//
	uint32_t 			VCO_Val2;		//
	uint32_t 			ADC_CH0;		//
	uint32_t 			ADC_CH1;		//
	uint32_t 			ADC_CH2;		//
	uint32_t 			ADC_CH3;		//
	uint32_t 			ADC_CH4;		//
	uint32_t 			ADC_CH5;		//
	uint32_t 			ADC_CH6;		//
	uint32_t 			ADC_CH7;		//
	uint32_t 			ADC_CH8;		//
	uint32_t 			ADC_CH9;		//
	uint32_t 			ADC_CH10;		//
	uint32_t 			ADC_CH11;		//
	uint32_t 			Frame_Lengh;	//
	uint8_t				Buffer[21360];
	uint8_t				Buffer3[5600];
	uint8_t				Buffer4[3200];
	uint8_t				Buffer5[272];
	float 				BS_S17;	//
	float 				BS_V1;	//
	float 				BS_V2;	//
	float 				BS_V3;	//
	float 				BS_V4;	//
	float 				BS_V1I;	//
	float 				BS_V2I;	//
	float 				BS_V3I;	//
	float 				BS_V4I;	//
	float 				BS_V1Q;	//
	float 				BS_V2Q;	//
	float 				BS_V3Q;	//
	float 				BS_V4Q;	//
    float 				BS_UeH;	//
    float 				BS_GocH;//
    float 				BS_UeE;	//
    float 				BS_GocE;//
    float 				Phi_final;	//
    float 				Epxilon_final;//
    float 				BS_S17I;	//
    float 				BS_S17Q;	//
	uint32_t 			BS_PS17;	//
	uint32_t 			BS_PV1;		//
	uint32_t 			BS_PV2;		//
	uint32_t 			BS_PV3;		//
	uint32_t 			BS_PV4;		//
	uint32_t 			BS_Angle_E;	//
	uint32_t 			BS_Angle_H;	//
	uint32_t 			BS_Status;	//
	int32_t 			BS_Angle_Eb0;
	int32_t 			BS_Angle_Hb0;
	uint32_t 			BS_Angle_Ez0;
	uint32_t 			BS_Angle_Hz0;
	uint32_t 			Motor_BS_ValueE;
	uint32_t 			Motor_BS_ValueH;
	float 				Delta_V12;
	float 				Delta_V34;
	uint32_t 			SS1_MAX;
	uint32_t 			SS2_MAX;
	uint32_t 			SS3_MAX;
	uint32_t 			SS4_MAX;
	uint32_t 			Radar_Range;

	float 				Target_Radius0;
	float 				Target_Beta0;
	float 				Target_Alpha0;
	float 				x0;
	float 				y0;
	float 				z0;

	float 				Target_Radius1;
	float 				Target_Beta1;
	float 				Target_Alpha1;
	float 				x1;
	float 				y1;
	float 				z1;

	uint32_t			step_counter; // 38
	float 				Delta_Step;
	float 				Delta_time;

	float 				Target_Radius;
	float 				Target_Beta;
	float 				Target_Alpha;
	float 				Target_Hight;
	float 				Target_Speed;
//	uint32_t			ATT_Val1;
//	uint32_t			ATT_Val2;
//	uint32_t			ATT_Val3;
//	uint32_t			ATT_Val4;
//	uint32_t			ATT_Val5;
//	uint32_t			ATT_Val6;
//	uint32_t			ATT_Val7;
//	uint32_t			ATT_Val8;
//	uint32_t			ATT_Val9;
//	uint32_t			ATT_Val10;
//	uint32_t			ATT_Val11;
//	uint32_t			ATT_Val12;
//	uint32_t			ATT_Val13;
//	uint32_t			ATT_Val14;
//	uint32_t			ATT_Val15;
//	uint32_t			ATT_Val16;
	};

struct UDP_Phase
	{
	uint32_t 			AXI_GPIO_IN;	// 0xAA55
	uint32_t 			Goc_Omega1;		// goc omega cho be phong 1 (0 - 3600). 1 don vi = 0.1 do
	uint32_t 			Goc_Omega2;		// goc omega cho be phong 2 (0 - 3600). 1 don vi = 0.1 do
	uint32_t 			Motor1_ENA;		// cho phep dieu khien be phong 1
	uint32_t 			Motor2_ENA;		// cho phep dieu khien be phong 2
	uint32_t			ENC1_Value;		// he so chia be phong 1, mac dinh 2880
	uint32_t			ENC2_Value;  	// he so chia be phong 2, mac dinh 2880
	uint32_t			Kiemtra1_ENA;	// he so chia enc be phong 1, mac dinh 2865/2880
	uint32_t			Kiemtra2_ENA;	// he so chia enc be phong 2, mac dinh 2865/2880
	uint32_t			Goc_Kiemtra1;	// bu sai so ENC tuyet doi cho be phong 1, mac dinh 2865-2880 = 15
	uint32_t			Goc_Kiemtra2;	// bu sai so ENC tuyet doi cho be phong 2, mac dinh 2865-2880 = 15
	uint32_t			ENC_CMD1;		// lenh can thuc thi 1
	uint32_t			ENC_Status1;	// lenh can thuc thi 1
	uint32_t			ENC_CMD2;		// lenh can thuc thi 2
	uint32_t			ENC_Status2;	// lenh can thuc thi 2
	uint32_t			Calib1_Point;	// gia tri encoder khi dia co khi be 1 = 0
	uint32_t			Calib2_Point;	// gia tri encoder khi dia co khi be 1 = 0
	uint32_t			Motor_Rotate1;	// gia tri encoder khi dia co khi be 1 = 0
	uint32_t			Motor_Rotate2;	// gia tri encoder khi dia co khi be 1 = 0
	float				Motor1_Angle;
	float				Motor1_Lygiac;
	float				Motor2_Angle;
	float				Motor2_Lygiac;
    int 				BS_Delta_Angle1;    //
    int 				BS_Delta_Angle2;    //
    float 				Quat_Delta1;    //
    float 				Quat_Delta2;	//
    uint32_t 			Phuong_vi_Dir;    //
    uint32_t 			Goc_ta_Dir;	//
	uint32_t 			VCO_Val1;		//
	uint32_t 			VCO_Val2;		//
	uint32_t 			ADC_CH0;		//
	uint32_t 			ADC_CH1;		//
	uint32_t 			ADC_CH2;		//
	uint32_t 			ADC_CH3;		//
	uint32_t 			ADC_CH4;		//
	uint32_t 			ADC_CH5;		//
	uint32_t 			ADC_CH6;		//
	uint32_t 			ADC_CH7;		//
	uint32_t 			ADC_CH8;		//
	uint32_t 			ADC_CH9;		//
	uint32_t 			ADC_CH10;		//
	uint32_t 			ADC_CH11;		//
	uint32_t 			Frame_Lengh;	//
	uint8_t				Buffer[48000];
	};

struct AFC_Struct
	{
	uint16_t 			Xung_Tot[60];		// du lieu truyen len PC
	uint16_t			Xung_Lech_Phai[60];	// du lieu truyen len PC
	uint16_t			Xung_Lech_Trai[60];	// du lieu truyen len PC
	uint16_t			Lech_Trai[16][60];	// du lieu truyen len PC
	uint16_t			Lech_Phai[16][60];	// du lieu truyen len PC
	uint16_t			Tot[16][60];	// du lieu truyen len PC
	uint16_t			AVG_counter;
	uint16_t 			Max_Xung_Tot;		// gia tri max cua xung tot
	uint16_t			Max_Lech_Phai;
	uint16_t			Max_Lech_Trai;
	uint16_t			Max_Top;
	uint16_t			Min_Top;
	uint8_t				Update_AFC;
	uint8_t				AFC_done_counter;
	uint8_t				AFC_Fail_counter;
	uint8_t				AFC_done;
	uint8_t				TSP_AFC;
	uint16_t			Fphat;
	uint16_t			Fnsai;
	uint16_t			U_nguong;
	uint16_t			U_nguong_PC;
	uint16_t			U_Lech;
	uint16_t			U_LechPhai;
	uint16_t			U_LechTrai;
	uint16_t			Control_Value;
	uint16_t			Control_Value0;
	uint16_t			Control_MAX;
	unsigned char 		Disconect;
	unsigned char 		Auto_Start;
	uint16_t 			Auto_Counter;
	unsigned char 		AFC_Active;
	unsigned char 		Led_Tot;
	unsigned char 		Level_Trai;
	unsigned char 		Level_Phai;
	unsigned char 		AFC_DIR;
	unsigned char 		AFC_counter;
	unsigned char 		Error_counter;
	unsigned char 		Error_counter2;
	unsigned char 		Bam_Sat;
	unsigned char 		Delay_time;
	unsigned char 		Fix_time;
	unsigned char 		Fix_time2;
	unsigned char 		Fix_DIR;
	};
