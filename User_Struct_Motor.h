
#define	Radar_Motor_PhuongVi		0x00
#define	Radar_Motor_GocTa			0x01

#define	Radar_Motor_Running			0x01
#define	Radar_Motor_Stop			0x00

#define	Radar_Motor_Tien			0x01
#define	Radar_Motor_Lui				0x00

// --- Các hằng số ---
#define TEST_MESSAGE_ID		0x643
#define FRAME_DATA_LENGTH	8

// --- Struct quản lý bus CAN trên Linux ---
typedef struct {
    int socket_fd;          // File descriptor cho CAN socket
    char interface_name[16]; // Tên giao diện (ví dụ: "can0")
    int is_connected;       // Cờ trạng thái
} LinuxCanBus;

struct CAN_Control_def
	{
	uint8_t CAN_CRC_bit[100];
	uint16_t Erob_CRC2;

	uint16_t CAN_Start_of_Frame;// = 0;
	uint16_t CAN_ID;// = 643;
	uint16_t CAN_DLC;// = 8;
	uint16_t CAN_Requ_Remote;// = 0;
	uint16_t CAN_ID_EXt_Bit;// = 0;
	uint16_t CAN_Reserved;// = 0;
	uint16_t CAN_Data[8];
	uint16_t CAN_CRC_Delimiter;//= 1;
	uint16_t CAN_Acknow_Slot_Bit;// = 1;
	uint16_t CAN_ACKnow_Delimiter;// = 0;
	uint16_t CAN_CRC;// = 0x0000;
	uint16_t CAN_EOF;// = 0x7f;
	uint16_t CAN_IFS;// = 0x07;
	uint16_t CAN_Time;// = 100;
	uint32_t CAN_Freq;// = 100000;
	uint16_t CAN_Start;// = 1;

	uint8_t CAN_Bits_Val[100];
	uint32_t CAN_Bits1;// = 0x00000000;
	uint32_t CAN_Bits2;// = 0x00000000;
	uint32_t CAN_Bits3;// = 0x00000000;
	uint32_t CAN_Bits4;// = 0x00000000;
	uint32_t CAN_CMD;//   = 0x00000000;
	uint32_t CAN_DIR2;//  = 1;

	u32 TxFrame[20];//XCANPS_MAX_FRAME_SIZE_IN_WORDS];
	u32 RxFrame[20];//XCANPS_MAX_FRAME_SIZE_IN_WORDS];

	u32 TxFrame2[20];//XCANPS_MAX_FRAME_SIZE_IN_WORDS];
	u32 RxFrame2[20];//XCANPS_MAX_FRAME_SIZE_IN_WORDS];

	uint16_t Erob_CRC;
	uint8_t Erob_Cmd[30];

	uint32_t			FifoFull1_Timeout; // sua loi bai toan tinh
	uint32_t			Motor1_Timeout; // sua loi bai toan tinh
	uint8_t				Motor1_disconect; // sua loi bai toan tinh
	uint8_t				Motor1_Restart; // sua loi bai toan tinh
	uint8_t				Motor1_OK; // sua loi bai toan tinh
	uint32_t			Motor1_Restart_Time; // sua loi bai toan tinh

	uint32_t			FifoFull2_Timeout; // sua loi bai toan tinh
	uint32_t			Motor2_Timeout; // sua loi bai toan tinh
	uint8_t				Motor2_disconect; // sua loi bai toan tinh
	uint8_t				Motor2_Restart; // sua loi bai toan tinh
	uint8_t				Motor2_OK; // sua loi bai toan tinh
	uint32_t			Motor2_Restart_Time; // sua loi bai toan tinh

	};


struct Erob_Struct
{
	uint32_t 			UDP_counter;
	uint32_t 			Stop_counter;
	uint32_t 			Erob_Freq;
	uint32_t 			Erob_Freq2;
	uint32_t 			Erob_Gpio_data;
	uint32_t 			Erob_SW1;
	uint32_t 			Erob_SW2;
	uint32_t 			Erob_SW3;
	uint32_t 			Erob_SW4;
	uint32_t 			Erob_Counter;
	int32_t 			Erob_Speed;
	int32_t 			Erob_Position;
	int32_t 			Erob_Current;
	int32_t 			Erob_Voltage;
	int32_t 			Erob_Temperature;
	uint32_t 			Erob_Encoder;
	int					Zynq_Encoder;
	char 				str[80];
	uint8_t				UDP_Data[100];

	struct Motor_Struct Motor_PhuongVi;
	struct Motor_Struct Motor_GocTa;
};
