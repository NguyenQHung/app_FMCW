#include "LMK_display.h" // Giữ nếu không phụ thuộc Xilinx (kiểm tra code)
#include "LMX_display.h" // Giữ tạm, nhưng có thể cần port
// #include "LMX2820.h"      // Comment nếu dùng SPI Xilinx
/**********************************************************************************/
extern struct Radar_Struct Radar;
extern struct Radar_Debug_Struct Radar_Debug;
extern struct Frame_Struct Frame;
extern struct PC8_def PC8;
extern struct PC8_def PC8s;
extern struct AFC_Struct AFC1;
extern struct AIS_Struct AIS_Data;
extern struct Erob_Struct Erob_Motor;
extern struct Erob_Control_def Erob_Control;
extern struct AIS_Struct AIS_Data;
extern struct UDP_Struct UDP_Frame;
extern struct CAN_Control_def CAN_Controls;
extern struct UDP_Phase UDP_Phase1;
extern struct CMAC_Struct CMAC_Struct1;

extern int cmdExitVal;
/******************** Constant Definitions **********************************/
#define ENABLE_METAL_PRINTS

#define URAM_PLAY_BASE XPAR_HIER_PLAY_AXI_BRAM_CTRL_0_S_AXI_BASEADDR

// PLL debug defines. Will print all calculated values
#undef LMK_DEBUG
#undef LMX_DEBUG

extern lmk_config_t lmkConfig;
extern lmx_config_t lmxConfig;

/*
 * Device instance definitions
 */
// XRFdc RFdcInst;      /* RFdc driver instance */
//
// lmk_config_t lmkConfig;
// lmx_config_t lmxConfig;
extern XRFdc RFdcInst; /* RFdc driver instance */
/******************** Constant Definitions **********************************/

#define _Epxilon_Up 2
#define _Epxilon_Down 1
#define _Epxilon_Ok 0

// Necessary to use this define when using jtagterminal but not SDK jtaguart
// console
// #define STRIP_CHAR_CR

// RFDC defines
#define RFDC_DEVICE_ID 0     // XPAR_XRFDC_0_DEVICE_ID
#define RFDC_BASE 0xA0200000 // XPAR_XRFDC_0_BASEADDR

// Number of Tiles and Blocks in device
#define NUM_TILES 4
#define NUM_BLOCKS 4

#define _S17I_AVG_ch 0 // 38
#define _S17Q_AVG_ch 1 // 38
#define _V1I_AVG_ch 2  // 38
#define _V1Q_AVG_ch 3  // 38
#define _V2I_AVG_ch 4  // 38
#define _V2Q_AVG_ch 5  // 38
#define _V3I_AVG_ch 6  // 38
#define _V3Q_AVG_ch 7  // 38
#define _V4I_AVG_ch 8  // 38
#define _V4Q_AVG_ch 9  // 38

#define ADF4360_Freq0 1400
