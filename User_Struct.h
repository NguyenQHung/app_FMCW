//#include "xparameters.h"
//#include "xil_types.h"
#include <math.h>
#include <complex.h>

#include "User_Struct_Radar.h"
#include "User_Struct_PC8.h"
#include "User_Struct_Motor.h"

/**********************************************************************************/
struct AIS_Plot
	{
	uint8_t		Status;		// 0: neo 1: di chuyen
	//uint8_t		Payload[80];// 0: neo 1: di chuyen
	uint8_t		Length;		// 0: neo 1: di chuyen
	uint16_t	Country;	// ma quoc gia
	uint16_t	Board_ID;	// so hieu tau

	uint8_t		Message_Type;		// 0  -   5: Constant: 1-3
	uint8_t		Repeat_Indicator;	// 6  -   7: Message repeat count
	uint32_t	MMSI;				// 8  -  37: 9 decimal digits
	uint8_t		Navigation_Status; 	// 38 -  41: See "Navigation Status"
	uint8_t		Rate_of_Turn;		// 42 -  49:
	uint16_t	Speed_Over_Ground;	// 50 -  59:
	uint8_t		Position_Accuracy;	// 60 -  60:
	uint32_t	Longitude;			// 61 -  88: Minutes/10000
	uint32_t	Latitude;			// 89 - 115: Minutes/10000
	uint16_t	Course_Over_Ground;	// 116- 127: Relative to true north, to 0.1 degree precision
	uint16_t	True_Heading;		// 128- 136: 0 to 359 degrees, 511 = not available.
	uint8_t		Time_Stamp; 		// 137- 142: Second of UTC timestamp
	uint8_t		Maneuver_Indicator; // 143- 144: See "Maneuver Indicator"
	uint8_t		Spare;				// 145- 147:
	uint8_t		RAIM_flag;			// 148- 148:
	uint32_t	Radio_status;		// 149- 167
	};

struct AIS_BASE_STATION
	{
	uint8_t		Status;				// 0: neo 1: di chuyen
	uint16_t	Country;			// ma quoc gia
	uint16_t	Board_ID;			// so hieu tau

	uint8_t		Message_Type;		// 0  -   5: Constant: 1-3
	uint8_t		Repeat_Indicator;	// 6  -   7: Message repeat count
	uint32_t	MMSI;				// 8  -  37: 9 decimal digits
	uint8_t		Year; 				// 38 -  51: UTC, 1-9999, 0 = N/A (default)
	uint8_t		Month;				// 52 -  55: 1-12; 0 = N/A (default)
	uint8_t		Day;				// 56 -  60: 1-31; 0 = N/A
	uint16_t	Hour;				// 61 -  65: 0-23; 24 = N/A
	uint8_t		Minute;				// 66 -  71: 0-59; 60 = N/A
	uint32_t	Second;				// 72 -  77: 0-59; 60 = N/A
	uint32_t	Fix_quality;		// 78 -  78:
	uint16_t	Longitude;			// 79 - 106:
	uint16_t	Latitude;			// 107- 133:
	uint8_t		Type_of_EPFD; 		// 134- 137:
	uint8_t		Spare;				// 138- 147:
	uint8_t		RAIM_flag;			// 148- 148:
	uint32_t	Radio_status;		// 149- 167
	};

struct AIS_Struct
	{
	uint16_t 			AIS_Baudrate;		// du lieu truyen len PC
	uint8_t				AIS_Br_Val;			// gia tri cai dat baudrate
	uint8_t				Data[80];			// 0: neo 1: di chuyen
	uint8_t				Code[80];			// 0: neo 1: di chuyen
	uint8_t				Sixb[80];			// 0: neo 1: di chuyen
	uint8_t				AIS_bit[230];		// 0: neo 1: di chuyen
	uint32_t 			AIS_Data1;			//
	uint32_t 			AIS_Data2;			//
	uint32_t 			AIS_Data3;			//
	uint32_t 			AIS_Data4;			//
	uint8_t				AIS_Line;			//
	uint8_t				AIS_Config;			//
	uint8_t				AIS_Length;			//
	uint8_t				AIS_Length1;		//
	uint8_t				AIS_Sixb;			//
	uint8_t				AIS_Done;			//
	uint8_t				AIS_time;			//
	uint8_t				AIS_number_Plot;	//
	uint8_t				AIS_new_Plot;		//

	uint8_t				AIS_PC2;			//
	uint8_t				AIS_PC1;			//

	uint8_t				Message_Type;		// 0  -   5: Constant: 1-3
	uint8_t				Repeat_Indicator;	// 6  -   7: Message repeat count
	uint32_t			MMSI;				// 8  -  37: 9 decimal digits
	uint8_t				Navigation_Status; 	// 38 -  41: See "Navigation Status"
	uint8_t				Rate_of_Turn;		// 42 -  49:
	uint16_t			Speed_Over_Ground;	// 50 -  59:
	uint8_t				Position_Accuracy;	// 60 -  60:
	uint32_t			Longitude;			// 61 -  88: Minutes/10000
	uint32_t			Latitude;			// 89 - 115: Minutes/10000
	uint16_t			Course_Over_Ground;	// 116- 127: Relative to true north, to 0.1 degree precision
	uint16_t			True_Heading;		// 128- 136: 0 to 359 degrees, 511 = not available.
	uint8_t				Time_Stamp; 		// 137- 142: Second of UTC timestamp
	uint8_t				Maneuver_Indicator; // 143- 144: See "Maneuver Indicator"
	uint8_t				Spare;				// 145- 147:
	uint8_t				RAIM_flag;			// 148- 148:
	uint32_t			Radio_status;		// 149- 167

	//struct AIS_BASE_STATION	Station[10];
	struct AIS_Plot		Plot[200];
	};







