
struct ATT_def
	{
	uint8_t		Phase;
	uint8_t		Sau_Bapy;
	uint8_t		Rong_Bapy;
	uint8_t		KDMT;
	};

struct Excel_def
	{
	uint8_t		Loading_E_Angle_done;
	uint8_t		Loading_E_Ue_done;
	uint8_t		Loading_H_Angle_done;
	uint8_t		Loading_H_Ue_done;
	uint32_t	Loading_done;
	uint32_t	Loading_Reset;
	uint16_t	Point;
	uint16_t	Load_Point;
	float		AngleE[432];
	float		Ue_E[432];
	float		AngleH[432];
	float		Ue_H[432];
	};

// Dinh nghia cau truc de luu tru cac trang thai cua bo loc Kalman
struct BoLocKalman{
    double q; // Nhieu qua trinh (Process noise covariance)
    double r; // Nhieu do luong (Measurement noise covariance)
    double x; // Gia tri uoc tinh (gia tri da duoc loc)
    double p; // Sai so hiep phuong sai cua uoc tinh (estimate error covariance)
    double k; // Kalman gain
};


struct Khao_Sat_def
	{
	uint16_t	Loading;
	uint16_t	Point;
	uint16_t	Max_Point;
	double		Max_V1;
	double		Max_V2;
	double		Max_S17;
	double		Max_Ue;
	double		Center;
	double		ly_giac;
	double		buffer;
	int16_t		Number;
	int32_t		Delta_Angle;
	double		V1[334];
	double		V2[334];
	double		S17[334];
	double		Ue[334];
	};

struct BamSat_def
	{
	uint32_t	Beam_Max; 		// bien do tin hieu lon nhat
	uint32_t	Beam_Max_Val; 	// beam co gia tri lon nhat
	uint32_t	Beam_Maxi;
	uint32_t	Beam_Max_Vali;
	uint32_t	Value_Max[27];
	double		Value_SUM[27];
	double		Value_AVG[27];
	uint32_t	Max_channel_AVG;
	float		Beam_SNR;
	float		Beam_SNR2;
	float		Beam_SNR2i;
	float		Beam_Scale;
	float		Beam_Scalei;
	float		Beam_Scalei2;

	uint32_t	fb_Val[27][200];
	uint32_t	_SS2[200];
	uint32_t	_SS3[200];

	uint32_t	fb_cnt[27];
	uint32_t	fb_Pass[27][200];
	uint32_t	fb_Point[27][200];
	double		fb_Sum[27];
	double		fb_AVG[27];
	uint32_t	fb_Size[27];

	uint32_t	fb_Start;
	uint32_t	fb_Stop;
	uint32_t	fb_center;
	uint32_t	fb_cells;
	uint32_t	fb_Beam_Max;
	float		fb_SNR_Scale;
	float		fb_SNR_Scalei;

	float		_Delta_Phi0;
	float		_Delta_Phi;
	uint32_t	_Delta_Phi_Dir;
	uint32_t	_Delta_Phi_Speed;
	uint32_t	_Delta_Phi_Enable;

	float		_Delta_Epxilon0;
	float		_Delta_Epxilon;
	uint32_t	_Delta_Epxilon_Dir;
	uint32_t	_Delta_Epxilon_Speed;
	uint32_t	_Delta_Epxilon_Enable;

	float		Nguong_H_DK_Ue_H;
	float		Nguong_L_DK_Ue_H;
	float		Nguong_H_DK_Ue_E;
	float		Nguong_L_DK_Ue_E;

	float		Ue_Min_kenh_H;
	float		Ue_Max_kenh_H;
	float		Ue_Min_kenh_E;
	float		Ue_Max_kenh_E;

	float 		_Current_beta;
	float 		_Current_Heading; 	// huong muc tieu
	float 		_Current_V;			// van toc muc tieu
	float 		_Current_Vx;		// van toc dich ngang (phuong vi)
	float 		_Current_Vy;		// van toc dich doc (cu ly)
	uint16_t 	_Current_Start;		// muc tieu san sang

	uint32_t	Giam_mau_xuly;
	uint32_t	Chieu_DK_Bam_H;
	uint32_t	Chieu_DK_Bam_E;

	int32_t		Delta_beta;
	int32_t		Delta_alpha;

	uint16_t	BS_Fix_H;// 20
	uint16_t	BS_Fix_E;// 20

	uint16_t	BS_Fix_H0;// 20
	uint16_t	BS_Fix_E0;// 20

	uint16_t	BS_Fix_Epxilon;// 20
	uint16_t	BS_Fix_Dir;// 20

	uint32_t	BS_Detect_low_counter;// 20
	uint16_t	BS_Detect_done;// 20
	uint16_t	BS_Detect_Fail;// 20
	uint16_t	BS_Detect_time;// 20
	uint16_t	BS_Detect_Change_Point;// 20
	uint16_t	BS_Detect_Fix;// 20
	uint16_t	BS_CS_Delta;// 20
	uint32_t 	Min_Detect;
	uint32_t 	Min_Detect_bamsat;
	uint32_t 	Min_Detect_mode;
	uint32_t 	Bam_culy_ENA;

	uint32_t	phi[17];
	uint32_t	epxilon[17];

	double		TL_Phi[256];
	double		TL_Epxilon[256];

	double		TL_Phi_final0;
	double		TL_Epxilon_final0;

	double		TL_Phi_final;
	double		TL_Epxilon_final;

	double		TL_Phi_sum;
	double		TL_Epxilon_sum;

	uint32_t	Process_Phi_start;
	uint32_t	Process_Epxilon_start;
	uint32_t	Process_Epxilon_Ready;  // l dieu khien kenh E
	uint32_t	_CS_H_Ready;  // l dieu khien kenh H
	uint32_t	_CS_E_Ready;  // l dieu khien kenh E
	uint32_t	_CS_E_Enable;
	uint32_t	Process_New_CMD;
	uint32_t	Process_BS_done;
	uint32_t	Process_BS_done2;

	uint32_t	Process_Beta_Mode;
	uint32_t	Process_Alpha_Mode;

	uint8_t	BS_Quat_mode;
	uint8_t	BS_Beam_mode;

	uint8_t	Motor_Speed;
	uint8_t	Scan_Speed;
	uint8_t	Bamsat_Speed;

	uint16_t	Max_s1; // cua song lon nhat cho thang 1, [Cell]
	uint16_t	Max_s2; // cua song lon nhat cho thang 2, [Cell]
	uint16_t	Max_s3; // cua song lon nhat cho thang 3, [Cell]
	uint16_t	Max_s4; // cua song lon nhat cho thang 4, [Cell]
	uint16_t	Max_s5; // cua song lon nhat cho thang 5, [Cell]

	uint16_t	Min_R1; // cu ly bam sat nho nhat cho thang 1, [Cell]
	uint16_t	Min_R2; // cu ly bam sat nho nhat cho thang 2, [Cell]
	uint16_t	Min_R3; // cu ly bam sat nho nhat cho thang 3, [Cell]
	uint16_t	Min_R4; // cu ly bam sat nho nhat cho thang 4, [Cell]
	uint16_t	Min_R5; // cu ly bam sat nho nhat cho thang 5, [Cell]

	uint16_t	Max_s; // cua song lon nhat cho thang 5, [Cell]
	uint16_t	Min_R; // cu ly bam sat nho nhat cho thang 1, [Cell]

	uint16_t	Cell_Screen; 	// o cell bat dau [Cell]
	uint16_t	Start_Screen; 	// o cell bat dau [Cell]
	uint16_t	Stop_Screen; 	// o cell ket thuc [Cell]
	uint16_t	Center_Screen; 	// o cell trung tam [Cell]
	uint16_t	High_Screen; 	// o cell trung tam [Cell]
	uint16_t	Buffer_Screen; 	// o cell trung tam [Cell]
	uint16_t	Fix_Protect; 	// o cell trung tam [Cell]
	uint16_t	Fix_ProtectH; 	// o cell trung tam [Cell]
	uint16_t	Limit_H; 		// o cell trung tam [Cell]

	uint16_t	LoopBack;

	uint16_t	Max_Cell1;
	uint16_t	Max_Cell2;
	uint16_t	Max_Cell3;
	uint16_t	Max_Cell4;
	uint16_t	Max_Cell5;
	uint16_t	Max_Cell;
	uint16_t	Safe_Cell;


	};

struct Beam_def
	{
	uint16_t    Innit_done;
	float		H_Angle;
	float		E_Angle;
	float		H_Anglei;
	float		E_Anglei;
	double 		tau[16];
	double 		Angle[16];
	_Complex 	A[16];
	uint16_t	Image[16];
	uint16_t	Real[16];
	double 		_Image;
	double 		_Real;
	};

struct PC8_def
	{
	uint16_t	Header;	// 0 1
	uint8_t 	Length; // 2

	uint8_t 	Beta_Enable; // 3 0-1
	uint8_t 	Beta_Speed; // 4 0-1
	uint16_t 	Beta_Rotate; // 5 6 0-1
	uint8_t		Beta_DIR;// 7
	uint16_t	Beta_ENC;// 8 - 9
	uint32_t	Beta_Calib;// 11
	uint8_t		Beta_Mode;// 12
	uint8_t		Beta_Reset;// 13
	uint8_t		Beta_Update;// 14
	uint16_t	Beta_Val;// 25-26

	uint8_t 	Alpha_Enable; // 15 0-1
	uint8_t 	Alpha_Speed; // 16 0-1
	uint16_t 	Alpha_Rotate; // 18  0-1
	uint8_t		Alpha_DIR;// 19
	uint16_t	Alpha_ENC;// 20
	uint32_t	Alpha_Calib;// 21
	uint8_t		Alpha_Mode;//22
	uint8_t		Alpha_Reset;//23
	uint8_t		Alpha_Update;//24
	uint16_t	Alpha_Val;// 25-26

	uint8_t		SSI_Speed;//27
	uint8_t		Encoder_Source;//28

	uint8_t		Encoder_Format;//29
	uint8_t		Encoder_Div;//30
	uint8_t		Encoder_Presscal;//31

	uint32_t	Alpha_Step;	// 32 - 34
	uint32_t	Beta_Step;	// 35 - 37

	uint16_t	Anten_P0;// Goc Start che do quay quat
	uint8_t		Alpha_Stop;	// 38
	uint8_t		Beta_Stop;	// 38

	uint8_t		S17_Max_point; // 38
	uint32_t	S17_Max; // 38

	uint32_t	S17_i1; // 38
	uint32_t	S17_i2; // 38
	uint32_t	S17_i3; // 38
	uint32_t	S17_q1; // 38
	uint32_t	S17_q2; // 38
	uint32_t	S17_q3; // 38

	uint32_t	V1I; // 38
	uint32_t	V1Q; // 38
	uint32_t	V2I; // 38
	uint32_t	V2Q; // 38
	uint32_t	V3I; // 38
	uint32_t	V3Q; // 38
	uint32_t	V4I; // 38
	uint32_t	V4Q; // 38

	uint32_t	S17Im; // 38
	uint32_t	S17Qm; // 38
	uint32_t	V1Im; // 38
	uint32_t	V1Qm; // 38
	uint32_t	V2Im; // 38
	uint32_t	V2Qm; // 38
	uint32_t	V3Im; // 38
	uint32_t	V3Qm; // 38
	uint32_t	V4Im; // 38
	uint32_t	V4Qm; // 38

	uint32_t	SS1_MAX; // 38
	uint32_t	SS2_MAX; // 38
	uint32_t	SS3_MAX; // 38
	uint32_t	SS4_MAX; // 38

	double	S17Im_Sum; // 38
	double	S17Qm_Sum; // 38
	double	V1Im_Sum; // 38
	double	V1Qm_Sum; // 38
	double	V2Im_Sum; // 38
	double	V2Qm_Sum; // 38
	double	V3Im_Sum; // 38
	double	V3Qm_Sum; // 38
	double	V4Im_Sum; // 38
	double	V4Qm_Sum; // 38

	double	S17ImA_Sum; // 38
	double	V1ImA_Sum; // 38
	double	V2ImA_Sum; // 38
	double	V3ImA_Sum; // 38
	double	V4ImA_Sum; // 38

	double	S17Im_[200]; // 38
	double	S17Qm_[200]; // 38
	double	V1Im_[200]; // 38
	double	V1Qm_[200]; // 38
	double	V2Im_[200]; // 38
	double	V2Qm_[200]; // 38
	double	V3Im_[200]; // 38
	double	V3Qm_[200]; // 38
	double	V4Im_[200]; // 38
	double	V4Qm_[200]; // 38

	double	S17ImA[200]; // 38
	double	V1ImA[200]; // 38
	double	V2ImA[200]; // 38
	double	V3ImA[200]; // 38
	double	V4ImA[200]; // 38

	double	S17max_out; // 38
	double	S17max; // 38
	double	V1max; // 38
	double	V2max; // 38
	double	V3max; // 38
	double	V4max; // 38

	uint8_t		ADC_ATTi;// 20
	uint8_t		ADC_ATTiv;// 20

	uint8_t		FFT_Sel;// 20
	uint8_t		FFT_Seli;// 20

	uint8_t		Sum_Sel;// 20
	uint8_t		Sum_Seli;// 20

	uint8_t		DAC_VOP;// 20
	uint8_t		DAC_VOPi;// 20

	double	S17max_AVG[8]; // 38
	double	V1max_AVG[8]; // 38
	double	V2max_AVG[8]; // 38
	double	V3max_AVG[8]; // 38
	double	V4max_AVG[8]; // 38

	double	_S17I_AVG[8]; // 38
	double	_S17Q_AVG[8]; // 38
	double	_V1I_AVG[8]; // 38
	double	_V1Q_AVG[8]; // 38
	double	_V2I_AVG[8]; // 38
	double	_V2Q_AVG[8]; // 38
	double	_V3I_AVG[8]; // 38
	double	_V3Q_AVG[8]; // 38
	double	_V4I_AVG[8]; // 38
	double	_V4Q_AVG[8]; // 38

	uint32_t	BS_PS17; // 38
	uint32_t	BS_PV1; // 38
	uint32_t	BS_PV2; // 38
	uint32_t	BS_PV3; // 38
	uint32_t	BS_PV4; // 38

	double	_S17I; // 38
	double	_S17Q; // 38
	double	_V1I; // 38
	double	_V1Q; // 38
	double	_V2I; // 38
	double	_V2Q; // 38
	double	_V3I; // 38
	double	_V3Q; // 38
	double	_V4I; // 38
	double	_V4Q; // 38

	float 	radian_beta;
	float 	radian_alpha;

	float 	Target_Radius0;
	float 	Target_Beta0;
	float 	Target_Alpha0;
	float 	x0;
	float 	y0;
	float 	z0;

	float 	Target_Radius1;
	float 	Target_Beta1;
	float 	Target_Alpha1;
	float 	x1;
	float 	y1;
	float 	z1;

	uint32_t	step_counter; // 38
	uint32_t	step_counter_All; // 38
	uint32_t	step_counter_Max; // 38
	uint32_t	step_ENA1; // 38
	uint32_t	step_ENA2; // 38
	uint32_t	Adjust_Mode; // 38
	uint32_t	Adjust_E_Enable; // 38
	uint32_t	Adjust_H_Enable; // 38
	uint32_t	step_counter2; // 38
	uint32_t	step_counter1; // 38
	uint32_t	step_counter0; // 38

	uint32_t	TG_Stop; // 38
	uint32_t	TG_time; // 38
	uint32_t	TG_Reset; // 38
	uint32_t	TG_EN; // 38
	uint32_t	TG_ENi; // 38
	uint32_t	TG_Click_time; // 38
	uint32_t	TG_Mode; // 38

	uint32_t	Beam17_MAX_Mode; // 38

	int32_t		TG_Alpha0; // 38
	int32_t		TG_Beta0; // 38

	int32_t		TG_Alpha; // 38
	int32_t		TG_Beta; // 38

	int32_t		TG_Alpha_table[16]; // 38
	int32_t		TG_Beta_table[16]; // 38

	float 	Delta_Step;
	float 	Delta_time;
	float 	Sum_Radius;

	float 	Target_Radius;
	float 	Target_Beta;
	float 	Target_Alpha;
	float 	Target_Hight;
	float 	Target_Speedi[8];
	float 	Target_Speed;

	double 		Sum17;
	double 		S17_Amplitude;
	double		S17i;//31
	double		S17q;//31

	double		a3;//31
	double		b3;//31
	double		Ha4;//31
	double		Hb4;//31
	double		Ha5;//31
	double		Hb5;//31

	double		Ea4;//31
	double		Eb4;//31
	double		Ea5;//31
	double		Eb5;//31

	double		Delta_V12;//31
	double		Delta_V34;//31

	double 		SumE17;
	double 		Amplitude_V1;
	double 		Amplitude_V2;
	double 		S17E_Amplitude;
	double 		SumEre;
	double 		SumEim;
	double		S17Ei;//31
	double		S17Eq;//31
	double		S17Ei2;//31
	double		S17Eq2;//31
	double		S17E_Cos_buf;//31
	double		S17E_Cos;//31
	double		S17E_Ue;//31
	double		S17E_Uei;//31
	double		S17E_Goc;//31

	uint32_t	S17_amplitude[9];//31
	int32_t		S17_Beta[9];//31
	int32_t		S17_Alpha[9];//31
	int32_t		S17_Beta_AVG;//31
	int32_t		S17_Alpha_AVG;//31
	uint32_t	S17_amplitude_MAX;//31
	uint32_t	S17_amplitude_done;//31
	uint16_t	S17_amplitude_H_Dir;
	uint16_t	S17_amplitude_E_Dir;
	uint16_t	S17_amplitude_Enable_E;


	double 		SumH17;
	double 		Amplitude_V3;
	double 		Amplitude_V4;
	double 		S17H_Amplitude;
	double 		SumHre;
	double 		SumHim;
	double		S17Hi;//31
	double		S17Hq;//31
	double		S17Hi2;//31
	double		S17Hq2;//31
	double		S17H_Cos_buf;//31
	double		S17H_Cos;//31
	double		S17H_Ue;//31
	double		S17H_Uei;//31
	double		S17H_Goc;//31

	double		S17H_Ueb[32];//31
	double		S17E_Ueb[32];//31

	uint16_t	AVG_H_Enable;
	uint16_t	AVG_E_Enable;

	double		Ue_Min;//31
	double		Ue_Max;//31

	uint16_t	real_Angle;

	struct BoLocKalman _E_Kalman;
	struct BoLocKalman _H_Kalman;

	struct ATT_def	ATT1;
	struct ATT_def	ATT2;
	struct ATT_def	ATT3;
	struct ATT_def	ATT4;
	struct ATT_def	ATT5;
	struct ATT_def	ATT6;
	struct ATT_def	ATT7;
	struct ATT_def	ATT8;
	struct ATT_def	ATT9;
	struct ATT_def	ATT10;
	struct ATT_def	ATT11;
	struct ATT_def	ATT12;
	struct ATT_def	ATT13;
	struct ATT_def	ATT14;
	struct ATT_def	ATT15;
	struct ATT_def	ATT16;
	struct ATT_def	ATT17;
	struct BamSat_def 	BamSat;
	struct Excel_def 	Excel;

	uint8_t Update_Phase_start;

	double C;
	double Fc;
	double Lamda;
	double dA;
	double dB;
	double xPos[16];
	double yPos[16];
	double zPos[16];

	struct Beam_def		V1;
	struct Beam_def		V2;
	struct Beam_def		V3;
	struct Beam_def		V4;

	struct Beam_def		DAC;

	struct Khao_Sat_def	_KS_Pvi;
	struct Khao_Sat_def	_KS_Ta;

	uint16_t	VCO_Val1;
	uint16_t	VCO_Val2;
	uint16_t	VCO_Val1i;
	uint16_t	VCO_Val2i;
	uint16_t	VCO_time;
	uint16_t	VCO_time2;

	uint16_t	ATT_Mode;
	uint16_t	ATT_Delay;
	uint16_t	ATT_Step;
	uint16_t	ATT_Delta;
	uint16_t	ATT_Part;
	uint16_t	ATT_Data_Mode;

	uint8_t		Start_BeamV;
	uint32_t	ATT_Val[16];	// 35 - 37
	uint32_t	VOP_Val[16];	// 35 - 37

	uint8_t		VCO_EN;
	uint8_t		ADC_SEL;
	uint16_t	ADC_MAX;
	uint16_t	IP_dmt;
	uint16_t	IP_dmt2;
	uint32_t	IP_dmt_Val;	// 35 - 37
	uint32_t	IP_dmt_Val2;	// 35 - 37
	float		IP_amplitudei;	// 35 - 37
	uint8_t		ATT_TX_EN;
	uint8_t		ILA_Val;
	uint8_t		Beam_Max_View;
	uint8_t		Video_Val;
	uint8_t		Alpha_Vals;
	uint8_t		Alpha_Vals2;
	uint8_t		Video_Vali;
	uint8_t		ADC_CHx;
	uint8_t		ADC_raw_sel;
	uint8_t		Trigger_Val;
	uint8_t		Phase_Mode;
	uint8_t		IQ_Channel1;
	uint8_t		IQ_Channel2;
	uint16_t	Sine_offset[16];
	uint16_t	Cos_offset[16];
	int32_t		Phase_Value;	// 35 - 37
	int32_t		Phase_Valuei;	// 35 - 37

	uint8_t		IQ_Channel1i;
	uint8_t		IQ_Channel2i;
	uint8_t		beam_Channeli;
	uint8_t		fb_Channeli;
	uint8_t		beam_Channeli2;
	uint8_t		fb_Channeli2;
	uint8_t		epxilon_Channeli;
	uint8_t		fb_ChannelS;
	uint8_t		beam_ChannelS;

	uint16_t	ADC_Data[12];

	uint32_t	code_data;
	uint32_t	code_data2;
	uint32_t	Screen_Phase;
	uint8_t 	IQ_View_Sel;
	uint8_t 	_Set_Beam_Max_Point;
	uint8_t 	_Set_Beam_Max_Pointi;

	uint8_t 	DAC_KS_Enable;
	uint8_t 	DAC_KS_Fix;
	uint16_t 	DAC_H_E_Val;
	uint16_t 	DAC_Center_Val;

	uint8_t		Thang_Cu_ly;
	uint8_t		Thang_Cu_lyi;
	uint8_t		IP_BW_SEL;
	uint8_t		IP_BW_SELi;
	uint8_t		Cu_ly_EN;
	uint8_t		Gia_Quay_EN;
	uint8_t		Gia_Quay_ENi;
	uint8_t		AVG_EN;
	uint8_t		Loopback_Mode;

	uint8_t		Speed_Val[8];
	uint8_t		Beta_Speeds;	// 35 - 37
	int32_t		Beta_Delta_ENC;	// 35 - 37
	int32_t		Alpha_Delta_ENC;	// 35 - 37
	uint8_t		Beam_Switch;

	uint8_t		Trigger; // 0: Encoder tu anten; 1: tao gia encoder
	uint8_t		Sau_Bapy;
	uint8_t		Rong_Bapy;
	uint8_t		KDMT;
	uint8_t		CSP;
	uint8_t		BoLoc;

	uint8_t		Anten_mode;	// 0: Quay tron 1: Quay Quat
	uint16_t	Anten_start;// Goc Start che do quay quat
	uint16_t	Anten_stop; // goc stop che do quay quat
	uint16_t	Anten_angle;

	uint16_t	Alpha_Max;	// Goc ta lon nhat
	uint16_t	Alpha_Min;	// Goc ta nho nhat
	uint16_t	Alpha_P0;	// Vi tri Q0


	uint8_t		Bat_Phat;	// Lenh cap nhat Q0
	uint8_t		Bat_Phat2;	// Lenh cap nhat Q0

	uint8_t		Pulse_MUX;	// Lenh cap nhat Q0
	uint8_t		Pulse_Width;// Lenh cap nhat Q0
	uint16_t	Tan_so_Lap;// Lenh cap nhat Q0
	uint32_t	Ts_Val;// Lenh cap nhat Q0

	uint8_t		Update_Q0;			// Lenh cap nhat Q0
	uint8_t		Quay_Quat_Active;	// Lenh cap nhat Q0
	uint8_t		Auto_Rotate;	// Lenh cap nhat Q0

	double		Delta_Angle;
	double		Delta_Angle0;
	double		Alpha_Step1;

	double		Tien;
	double		Lui;

	uint16_t	Alpha_Angle;

	uint16_t	Beta_Angle;
	uint16_t	Beta_Angle2;
	uint8_t		RSV[24];

	uint16_t	Alpha_A0;// 20
	uint16_t	Alpha_A1;// 20

	uint16_t	Beta_A0;// 20
	uint16_t	Beta_A1;// 20

	uint16_t	Calib_B_Value;// 20
	uint16_t	Calib_counter;// 20
	uint8_t		Calib_dir;// 20
	uint8_t		Quat_Error;// 20
	uint8_t		Trigger_source;// 20
	uint8_t 	CLK_MUX_IO;
	uint8_t 	Test_Enable;
	uint8_t 	Clock_Channel;
	uint8_t 	SYS_Control;
	uint8_t 	CLK_10MHz_Source;
	uint8_t 	ADC_Channel;

	uint8_t		Offset1;// 20
	uint8_t		Offset2;// 20
	uint8_t		Offset3;// 20
	uint8_t		Tao_Gia;// 20

	uint8_t		Pulse_Sel;// 20
	uint8_t		code_Sel;// 20

	uint16_t	BS_Center_cell;// o cell trung tam ban sat muc tieu
	uint16_t	BS_Center_start;// o cell bat dau bam sat
	uint16_t	BS_Center_start2;// o cell bat dau bam sat
	uint16_t	BS_Center_starti;//
	uint16_t	BS_Center_stop;//  o cell ket thuc bam sat
	uint16_t	BS_Center_start4;// o cell bat dau bam sat
	uint16_t	BS_Center_stop4;// o cell bat dau bam sat
	uint16_t	BS_Center_start_bs;// 20
	uint16_t	BS_Center_start_bs0;// 20
	uint16_t	BS_Center_stop_bs;// 20
	uint8_t		BS_Start;// 20
	uint8_t		BS_Steps;// 20
	uint16_t	BS_Center_angle_point;// 20
	uint16_t	BS_Center_angle;// 20
	float		BS_Center_anglef;// 20
	float		BS_Center_anglef0;// 20
	uint16_t	BS_Center_Angle_start;// 20
	uint16_t	BS_Center_Angle_stop;// 20
	uint16_t	BS_Center_alpha;// 20
	float		BS_Center_alphaf;// 20
	float		BS_Center_alphaf0;// 20
	uint16_t	BS_Goto_Center_done;// 20
	int16_t		BS_Delta_Angle;// 20
	uint8_t		BS_PC1_Click;// 20
	uint16_t 	BS_Motor_Speed;
	uint16_t 	BS_Scan_Speed;
	uint16_t 	BS_Bamsat_Speed;
	uint16_t 	Khaosat_Speed;
	uint16_t 	Khaosat_Start;
	uint16_t 	Khaosat_center;
	uint16_t 	Khaosat_Stop;
	uint32_t 	Khaosat_Encoder;
	uint16_t 	Khaosat_Enable;
	uint16_t 	Khaosat_Enablei;
	uint16_t 	Khaosat_Dir;
	uint16_t 	HW_Dir;
	uint16_t 	HW_Dir0;
	double 		Khaosat_angle;

	uint8_t		Pvi_dir;// 20
	uint8_t		Gocta_dir;// 20
	uint8_t		Control_View_SS;// 20
	uint8_t		CMD_RF_ON_OFF;// 20
	uint8_t		CMD_RF_ENABLE;// 20
	uint8_t		CMD_Mixer_Txsig;// 20
	uint8_t		CMD_Mixer_Sel;// 20
	uint8_t		Phase_Reading;// 20

	uint16_t 	BS_Angle_E; // don vi: ly giac
	uint16_t 	BS_Angle_H; // don vi: ly giac

	uint16_t 	BS_Angle_E0; // don vi: ly giac
	uint16_t 	BS_Angle_H0; // don vi: ly giac

	uint16_t 	BS_Angle_Ez0; // don vi: ly giac
	uint16_t 	BS_Angle_Hz0; // don vi: ly giac

	double 		BS_AngleH_AVG[32]; // don vi: ly giac
	double 		BS_AngleE_AVG[32]; // don vi: ly giac

	double 		BS_AngleH_Kalman; // don vi: ly giac
	double 		BS_AngleE_Kalman; // don vi: ly giac

	double 		BS_pH_Kalman; // don vi: ly giac
	double 		BS_pE_Kalman; // don vi: ly giac

	double 		BS_AngleH_PID; // don vi: ly giac
	double 		BS_AngleE_PID; // don vi: ly giac

	float 		Bamsat_K1;
	float 		Bamsat_K2;
	float 		Bamsat_K3;
	uint8_t 	Bamsat_H_ena;
	uint8_t 	Bamsat_E_ena;

	uint32_t 	BS_Counter_time; // don vi: ly giac


	int16_t 	Delta_Click_Beta;
	int16_t 	Delta_Click_Start;
	uint16_t 	BS_Angle_Last_Ez0;
	uint16_t 	BS_Angle_Last_Hz0;
	uint16_t 	BS_K_Point;
	uint16_t 	BS_Scan_Start;

	int16_t 	BS_Angle_Eb0;
	int16_t 	BS_Angle_Hb0;

	uint16_t 	BS_Read_time;

	double 		BS_UE0_AVG[16];
	double 		BS_UH0_AVG[16];

	double 		BS_UE0;
	double 		BS_UH0;
	uint16_t 	Click_E_time;
	uint16_t 	Click_H_time;
	uint16_t 	E_time;
	uint16_t 	H_time;
	uint8_t 	Save_Z0;

	int 		Frame_counters;
	int 		Read_ENC_start;
	int 		Read_ENC_Time;
	int 		Read_Bamsat_start;
	int 		Read_Bamsat_time;
	int 		Sync_time;
	uint 		Phase_Step;
	uint 		Phase_Step0;

	};

struct Motor_Struct
{
	uint8_t				UDP_Datas1;
	uint8_t				UDP_Datas2;
	uint8_t				UDP_Datas3;
	uint8_t				UDP_Datas4;
	int32_t				UDP_Angle0;
	uint32_t			UDP_Anglei;
	float				UDP_Angle;
	float				UDP_Lygiac;
	uint32_t			Zynq_Encoder;
	int					Zynq_Encoder0;
	float				Quat_Delta;
	float				Quat_Delta0;

	int32_t 			BS_Angle;		//
	double 				BS_Anglef;		//
	int32_t 			BS_Set_Angle;	//
	int32_t 			BS_Delta_Angle;	//
	int32_t 			BS_Delta_Angle0;//
	uint32_t 			BS_Offset;		//
	int32_t 			BS_Delta;		//
	int32_t 			BS_Max;		//
	int32_t 			BS_Min;		//
	int32_t 			BS_Stop;		//
	int32_t 			BS_Stop_time;		//


	int16_t 			Check_time;		//

	uint16_t			Motor_BS_Value;
	uint16_t			Motor_Angle_Value;
	int16_t				Motor_Angle_P0;
	int16_t				Motor_Angle_Min;
	int16_t				Motor_Angle_Max;

	int16_t				Motor_Angle_buff1;
	int16_t				Motor_Angle_buff2;

	int16_t				Motor_Angle_Low;
	int16_t				Motor_Angle_High;

	uint8_t 			Motor_Enable; // 3 0-1
	uint8_t				Motor_DIR;// 7
	uint8_t				Quat_DIR;// 7
	uint32_t 			Quat_Freq;
	uint32_t 			Quat_Freq2;

	uint8_t 			Pulse_Enable; // 3 0-1
	uint8_t				Pulse_DIR;// 7
	uint32_t 			Pulse_Freq;
	uint32_t 			Pulse_number;
	uint32_t 			Pulse_Point;
	uint8_t				Pulse_done;// 7
	uint8_t				Pulse_Active;// 7

	uint32_t 			Erob_Freq;
	uint32_t 			Q0_Freq;
	uint8_t				Q0_DIR;// 7
	uint8_t				Q0_Enable;// 7
	uint32_t 			Erob_Counter;
	int32_t 			Erob_Speed;
	int32_t 			Erob_Position;
	int32_t 			Erob_Current;
	int32_t 			Erob_Voltage;
	int32_t 			Erob_Temperature;

	uint8_t 			Erob_Enable; // 3 0-1
	uint8_t				Erob_DIR;// 7
	uint32_t			Erob_Calib;// 11
	uint8_t				Erob_Mode;// 12
	uint8_t				Erob_Reset;// 13
	uint8_t				Erob_Update;// 14

	uint32_t			Motor_ENC;		// 4 gia tri encoder dong co ngu loi
	uint32_t			Motor_ENC_Save;	// 8 gia tri encoder dong co ngu loi
	uint32_t			Motor_ENC_Save2;// 12 gia tri encoder dong co ngu loi
	uint32_t			Motor_Step;		// 16 gia tri encoder dong co ngu loi
	uint32_t			Request_Encoder;// 20
	uint8_t				Retry_Motor;    // 21
	uint8_t				Save_Counter;	// 22
	uint32_t			Motor_FL_Step;	// 26 so step can di chuyen
	uint8_t				Motor_Step_done;// 27 da chay het step yeu cau
	uint16_t			Motor_Angle;	// 29 gia tri encoder dong co ngu loi
	uint32_t			Absolute_ENC;	// 31 goc encoder tuyet doi gan len truc quay
	uint16_t			Real_ENC;		// 33 goc encoder cua truc quay ngu loi
	uint16_t			Delta_Angle;	// 35 goc encoder tuyet doi khi bat nguon cua encoder tuyet doi
	uint16_t			Angle;			// 37 Goc ngu loi tinh toan duoc (truyen len PC4)
	uint16_t			Angle_buffer;	// 37 Goc ngu loi tinh toan duoc (truyen len PC4)
	uint16_t			ADC_buffer;	// 37 Goc ngu loi tinh toan duoc (truyen len PC4)
	uint16_t			AngleE;
	uint16_t			Omega;			// 39 Goc ngu loi tu PC4
	uint16_t			New_Omega;		// 39 Goc ngu loi tu PC4
	uint16_t			Scale;			// 41 he so chia ngu loi
	uint16_t			ENC_Scale;		// 43 he so chia ENC ABS
	uint16_t			Offset;			// 45 bu sai so encoder
	uint8_t				CMDi;			// 46 lenh thuc thi
	uint8_t				Running;		// 47 lenh dang thuc thi (lenh dieu khien tu PC4)
	uint8_t				Run_CMD;		// 48 1: dang quay 0: dung, Idle
	uint8_t				DIR;			// 49 0: cung chieu kim dong ho; 1: nguoc chieu
	uint8_t				VE_Speed;		// 50 van toc qua vong/s
	uint8_t				VE_Update;		// 50 van toc qua vong/s
	uint8_t				Update_Counter;	// 51 0: chua ket noi	1: ket noi binh thuong
	uint8_t				ERROR_Counter;	// 52 0: chua ket noi	1: ket noi binh thuong
	uint8_t				Innit_Done;		// 53 ngu loi 1 da khoi tao xong.
	uint8_t				Uart_Start;		// 54 0: Idle; 1: dang nhan du lieu
	uint8_t				Uart_Connect;	// 54 0: Idle; 1: dang nhan du lieu
	uint8_t				Data[30];		// 84 0: Idle; 1: dang nhan du lieu
	uint8_t 			Uart_ENA;		// 85 cho phep nhan du lieu uart ngu loi
	uint8_t 			Frame_DIR;		// 86 cho phep nhan du lieu uart ngu loi
	uint8_t 			Frame_Format;	// 87 cho phep nhan du lieu uart ngu loi
	uint8_t 			Frame_counter;	// 88 so byte d�t nhan duoc
	uint16_t			Max_rotation;	// 90 so vong quay toi da
	uint32_t			Max_rotation_ENC;// 94 so encoder toi da
	uint32_t			Max_rotation_Step;// 98 so buoc toi da
	uint16_t			ENC_Prescal;	//  100 he so chia
	uint32_t			Raw_ENC;// 98 so buoc toi da
	uint16_t			Step_Scale;	//  100 he so chia
	uint8_t 			Quay_Trai;  // lenh quay trai
	uint8_t 			Quay_Phai;  // lenh quay trai
	uint8_t 			Be_enable;  // lenh quay trai
	uint8_t 			Calib_enable;  // lenh quay trai
	uint8_t 			Calib_Counter;
	uint8_t 			FreeRun_DIR;
	uint16_t			ENC_LastPoint;	//  100 he so chia
	uint16_t			ENC_NewPoint;	//  100 he so chia
	uint16_t			ENC_OldPoint;	//  100 he so chia
	uint16_t			ENC_Delta;	//  100 he so chia
	uint16_t			ENC_Point;	//  100 he so chia
	uint16_t			ENC_Dx;	//  100 he so chia
	uint16_t			ENC_Dy;	//  100 he so chia
	uint8_t				Block_time; // sua loi bai toan tinh
	uint8_t				Fix_Sensor; // sua loi bai toan tinh
	uint32_t			Block_Time; // sua loi bai toan tinh
	uint8_t				Block_Start; // sua loi bai toan tinh

	uint32_t			FifoFull_Timeout; // sua loi bai toan tinh
	uint8_t				Motor_disconect; // sua loi bai toan tinh

	uint8_t				CAN_Innit_Ok; // sua loi bai toan tinh
	uint16_t			CAN_Innit_Time;	//  100 he so chia

};
