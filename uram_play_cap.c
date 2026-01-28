


/***************************** Include Files *********************************/
#include <stdio.h>
#include <stdlib.h>
#include "linux_hal.h"
//#include "xil_io.h"
//#include "xil_types.h"
#include "cli.h"
//#include "xparameters.h"
#include "main.h"
#include "uram_play_cap.h"


/************************** Constant Definitions *****************************/


/**************************** Type Definitions *******************************/
#if URAM_PLAY_EN == 1
#define GPIO_DAC_BASE 			0 //XPAR_DAC_GPIO_BASEADDR

#define GPIO_DAC_PLAY_EN_MSK		0x1
#endif

#if URAM_CAP_EN == 1
#define GPIO_CAP_BASE 			1//XPAR_ADC_GPIO_0_BASEADDR

#define GPIO_CLK_SYNC 			0x00A00C0000 //XPAR_CLK_PHASE_SYNC_BASEADDR
#define GPIO_CLK_SYNC_EN_MSK		0x1
#define GPIO_CAP_EN_MSK		0x1
#define GPIO_CAP_MASTER_MSK		0x2
#endif

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

/************************** Function Definitions ******************************/


/*****************************************************************************/
/**
*
* cli_uram_play_cap_init Add functions from this file to CLI
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void cli_uram_play_cap_init(void)
{
	static CMDSTRUCT cliCmds[] = {
		//000000000011111111112222    000000000011111111112222222222333333333
		//012345678901234567890123    012345678901234567890123456789012345678
		{"################## play/cap commands ###################" , " " 	, 0, *cmdComment   },
#if URAM_PLAY_EN == 1
		{"dacMemPlay" 	   , "- start the DAC URAM/BRAM player" 			   		, 0, *dacMemPlay},
		{"dacMemStop"	   , "- stop the DAC URAM/BRAM player"						, 0, *dacMemStop},
//		{"clkSync"	   , "- Sync Clock Modules"						, 0, *clkSync},

#endif
#if URAM_CAP_EN == 1
		{"adcMemCap" 	   , "- Capture all ADC waveforms to URAM/BRAM"	   		    , 0, *adcMemCap},

#if MULTI_BOARD == 1
//		{"capMaster" 	   , "<1 or 0> - 1 = multi board master enable"		, 1, *capMaster},
#endif

#endif
		{" "                       , " "                                  	, 0, *cmdComment   },

	};

	cli_addCmds(cliCmds, sizeof(cliCmds)/sizeof(cliCmds[0]));
}


/*****************************************************************************/
/**
*
* play: Enables the URAM player block
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
#if URAM_PLAY_EN == 1
void dacMemPlay (u32 *cmdVals)
{
	u32 tmpVal;

	//tmpVal = Xil_In32(GPIO_DAC_BASE);
	//tmpVal = tmpVal | GPIO_DAC_PLAY_EN_MSK;		//Set uram play enable
	//Xil_Out32(GPIO_DAC_BASE, tmpVal);
	xil_printf("DAC BRAM player running. \n\r");

	return;
}

/*****************************************************************************/
/**
*
* stop: Disables the URAM player block
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void dacMemStop (u32 *cmdVals)
{
	u32 tmpVal;

	//tmpVal = Xil_In32(GPIO_DAC_BASE);
	//tmpVal = tmpVal & ~GPIO_DAC_PLAY_EN_MSK;		//Set uram play enable
	//Xil_Out32(GPIO_DAC_BASE, tmpVal);
	xil_printf("DAC BRAM player stopped. \n\r");
	return;
}
#endif


/*****************************************************************************/
/**
*
* uramCap: Creates pulse to URAM capture block enables
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
#if URAM_CAP_EN == 1
void adcMemCap (u32 *cmdVals)
{
	u32 tmpVal;
	int cntr = 0;
	int cntr_flag = 0;

	tmpVal = 0;//Xil_In32(GPIO_CAP_BASE);
	//Xil_Out32(GPIO_CAP_BASE, tmpVal | GPIO_CAP_EN_MSK); //Set uram capture signal high
//	Xil_Out32(GPIO_CAP_BASE, tmpVal | GPIO_CAP_EN_MSK); //Set uram capture signal high
//	Xil_Out32(GPIO_CAP_BASE, tmpVal | GPIO_CAP_EN_MSK); //Set uram capture signal high
// Wait function to generate a ADC capture pulse at least one sysref long
	for (int i=0; i < 11; i++)  // JL - was 10
		{
			cntr = cntr + 1;
			if (cntr == 10) {   // jl was 10
				cntr_flag = 1;
			}
		}
		if (cntr_flag == 1) {
			//Xil_Out32(GPIO_CAP_BASE, tmpVal & ~GPIO_CAP_EN_MSK); //Set uram capture signal low
	}
	xil_printf("ADC captures complete. \n\r");
	xil_printf("Use XSCT to transfer the captures from memory to the computer. \n\r");
	return;
}

/*****************************************************************************/
/**
*
* uramCap: Creates pulse to URAM capture block enables
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void clkSync (u32 *cmdVals)
{
	u32 tmpVal;
	int cntr;
	int cntr_flag = 0;

	tmpVal = Xil_In32(GPIO_CLK_SYNC);
	Xil_Out32(GPIO_CLK_SYNC, tmpVal | GPIO_CLK_SYNC_EN_MSK); //Set uram capture signal high
// Wait function to generate a Clock Sync pulse at least one sysref long
	for (int i=0; i < 10; i++)
		{
			cntr = cntr + 1;
			if (cntr == 10) {
				cntr_flag = 1;
			}
		}
		if (cntr_flag == 1) {
			Xil_Out32(GPIO_CLK_SYNC, tmpVal & ~GPIO_CLK_SYNC_EN_MSK); //Set uram capture signal low
	}
	xil_printf("Clock Sync Pulse issued. \n\r");
	return;
}


/*****************************************************************************/
/**
*
* capMaster: set or clear master mode for multi board sync
*
* @param	cmdVals[0] set to 1 or 0 to enable/disable master mode
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
#if MULTI_BOARD == 1
void capMaster (u32 *cmdVals)
{
	u32 tmpVal;
	u32 enable;

	enable = cmdVals[0];

	tmpVal = 0;//Xil_In32(GPIO_CAP_BASE);
	if(!enable) {
		//Xil_Out32(GPIO_CAP_BASE, tmpVal & ~GPIO_CAP_MASTER_MSK); //Set multi board master mode low
		xil_printf("Multi-board master mode disabled\n\r");
	} else {
		//Xil_Out32(GPIO_CAP_BASE, tmpVal | GPIO_CAP_MASTER_MSK); //Set multi board master mode high
		xil_printf("Multi-board slave mode enabled\n\r");
	}

	return;

}

#endif

#endif

