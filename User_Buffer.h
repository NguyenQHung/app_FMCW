extern u8 SendBuffer0[TEST_BUFFER_SIZE];	/* Buffer for Transmitting Data */
extern u8 RecvBuffer0[TEST_BUFFER_SIZE];	/* Buffer for Receiving Data */

extern unsigned char		Radar_Video8[48000];
extern unsigned char		Radar_Video9[48000];
extern unsigned char		Radar_Video10[48000];
extern unsigned char		Radar_Video11[48000];
extern unsigned char		Radar_Window[48000];

extern unsigned char		Beam17i[800];
extern unsigned char		Beam17q[800];
extern unsigned char		Beam17[800];

extern unsigned char		_V1_data[800]; // [-2;0]
extern unsigned char		_V2_data[800]; // [2;0]

extern int32_t				Video_PC8[4096];

extern int16_t				Videop[8];
extern uint32_t				Video_AVG1;
extern uint32_t				Video_AVG2;

extern int32_t				Video_Sub1;
extern int32_t				Video_Sub2;

extern unsigned char		Video_fbx[42720];
extern unsigned char		Video_fb17[42720];

extern unsigned char		Radar_Frame1[Data_length];
extern unsigned char		Video_Frame[21460];
extern unsigned char		Radar_Frames1[Data_length];
extern unsigned char		Radar_Frame_M[Data_length];
extern unsigned char		Radar_Frame_Mp[Data_length];
extern unsigned char		Radar_Video_final[Data_length];
extern unsigned char		Radar_Frame2[2048];
extern unsigned char		PC2_Frame[100];
extern unsigned char		PC8_Frame_in[100];
extern unsigned char		PC8_Frame_out[20000];
extern unsigned char		Fix_Frame[100];
extern unsigned char		PA_Data[Data_length];
extern unsigned char		PA_Data2[Data_length];

extern unsigned int			clear_Counter;

extern unsigned char		Noise_Map_in[15460];
extern unsigned char		Noise_Map_out[15460]; //  5120
