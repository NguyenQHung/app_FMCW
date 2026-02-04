#include "xrfclk.h"

extern const u32 LMK_CKin[LMK_FREQ_NUM][LMK_COUNT];
extern const u32 LMX2594[][LMX2594_COUNT];

/************************** Variable Definitions ****************************/

#define   LMK_RESET_IO        	82 // bit 4 : 78 + 4   = 82
#define   LMK_CLKIN_SEL0_IO   	83 // bit 5 : 78 + 5   = 83
#define   LMK_CLKIN_SEL1_IO   	84 // bit 6 : 78 + 6   = 84

#define   LMX_MUTE1_IO        	90 // bit 12 : 78 + 12 = 90
#define   LMX_CE1_IO          	91 // bit 13 : 78 + 13 = 91
#define   LMX_MUTE2_IO        	92 // bit 14 : 78 + 14 = 92
#define   LMX_CE2_IO         	93 // bit 15 : 78 + 15 = 93

#define   SFP_LPMODE         	98 // bit 15 : 78 + 15 = 93
#define   SFP_MODE_SEL         	99 // bit 15 : 78 + 15 = 93
#define   SFP_RESET         	100 // bit 15 : 78 + 15 = 93

#define	ADF4360_CE				85 // bit 8  : 78 + 8  = 86 // muon chan
// #define	ADF4360_CLK			87 // bit 9  : 78 + 9  = 87
// #define	ADF4360_DATA		88 // bit 10 : 78 + 10 = 88
// #define	ADF4360_LE			89 // bit 11 : 78 + 11 = 89

#define	ADF4159_CE				81 // bit 3  : 78 + 3  = 81
// #define	ADF4159_CLK			80 // bit 2  : 78 + 2  = 80
// #define	ADF4159_DATA		79 // bit 1  : 78 + 1  = 79
// #define	ADF4159_LE			86 // bit 7  : 78 + 7  = 85

//#define	PHY_Reset_Pin			24 // bit 10 : 78 + 7  = 85

// chua su dung
#define	ADF4113_CE				13 // PMOD D0
#define	ADF4113_CLK				11 // PMOD D2
#define	ADF4113_DATA			0  // PMOD D4
#define	ADF4113_LE				14 // PMOD D6
