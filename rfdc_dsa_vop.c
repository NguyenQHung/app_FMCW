/*
 * rfdc_dsa_vop.c
 *
 *  Created on: 06/23/2020
 *      Author: jlantz
 */
/***************************** Include Files *********************************/
#include <stdio.h>
#include <stdlib.h>
#include "linux_hal.h"      // Lớp trừu tượng phần cứng (sysfs GPIO)
//#include "xil_io.h"
//#include "xil_types.h"
#include "cli.h"
//#include "xparameters.h"
#include "xrfdc.h"
#include "main.h"
#include "rfdc_dsa_vop.h"


/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/
float mygetline(void);

/************************** Variable Definitions *****************************/


/************************** Function Definitions ******************************/


/*****************************************************************************/
/**
*
* cli_cmd_func_dac_init Add functions from this file to CLI
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/

void cli_rfdcDSAVOP_init(void)
{
	static CMDSTRUCT cliCmds[] = {
		{"################ VOP and DSA commands ##################" , " " , 0, *cmdComment   },
		{"dacSetVOP"    , "<tile> <dac> <setting(uA)> - Set DAC VOP for Gen3", 3, *dacSetVOP},
		{"adcGetDSA"    , "- Get the ADC DSA for Gen3"      , 0, *adcGetDSA},
		{"adcSetDSA"    , "<tile> <adc> - Set the ADC DSA for Gen3"      , 2, *adcSetDSA},
		{" "            , " "                                  , 0, *cmdComment   },
	};

	cli_addCmds(cliCmds, sizeof(cliCmds)/sizeof(cliCmds[0]));
}

/*****************************************************************************/
/**
*
* mygetline
*
* @param	get line from console - Float Value
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
float mygetlinef()
{
	char line[10];
	int i=0;
	char mychar;
	float finalVal;

	while(1) {
		 mychar=inbyte();
		 if (mychar == 0x0a) continue;
		 if (mychar == 0x0d) break;
		 if(i>=8) break;
		 xil_printf("%c", mychar);
		 line[i++]=mychar;
	}

	line[i] = 0;
	finalVal = atof(&line[0]);

	return finalVal;
}


/*****************************************************************************/
/**
*
* Set the DAC VOP via API
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void dacSetVOP(u32 *cmdVals)
{

	u32 Tile_Id;
	u32 Block_Id;
	u32 uACurrent;
	XRFdc_IPStatus ipStatus;
	XRFdc* RFdcInstPtr = &RFdcInst;
	u32 Status;
	u32 OutputCurr;

	Tile_Id = cmdVals[0];
	Block_Id = cmdVals[1];
	uACurrent = cmdVals[2];

	// Calling this function gets the status of the IP
	XRFdc_GetIPStatus(RFdcInstPtr, &ipStatus);
	//xil_printf("\n\r###############################################\r\n");

	if (RFdcInstPtr->RFdc_Config.IPType >= XRFDC_GEN3) {

		if(XRFdc_IsDACBlockEnabled(RFdcInstPtr, Tile_Id, Block_Id)) {

			// DAC Set Output Power
			Status = XRFdc_SetDACVOP(RFdcInstPtr,  Tile_Id, Block_Id, uACurrent);
			if (Status != XST_SUCCESS) {
				//xil_printf("XRFdc_SetDACVOP() for DAC Tile%d ch%d failed.\n\r",Tile_Id,Block_Id);
				return;
			}
			else{
				Status = XRFdc_GetOutputCurr(RFdcInstPtr, Tile_Id, Block_Id, &OutputCurr);
				xil_printf("DAC[%d,%d]= %d uA\n\r",Tile_Id,Block_Id,OutputCurr);
				//xil_printf("   DAC Tile%d ch%d VOP set to %d uA.\n\r",Tile_Id,Block_Id,OutputCurr);
			}
		}
		else{
			//xil_printf("DAC Tile%d ch%d is not available.\n\r", Tile_Id, Block_Id);
		}
		}
	else{
		xil_printf("This command is for RFSoC Gen3 devices only.\n\r");
	}

		//xil_printf("###############################################\r\n");
		return;
}

/**
 * @brief       Reads the VOP (Output Current in uA) for all 16 possible DAC channels.
 * @param[out]  cmdVals: A pointer to an array of 16 u32 elements to store the results.
 *              The results are stored in the format: cmdVals[Tile_Id * 4 + Block_Id].
 *              A value of 0 indicates the channel is not available or an error occurred.
 * @return      None.
 */
void dacGetAllVOP(u32 *cmdVals)
{
	u32 Tile_Id;
	u32 Block_Id;
	u32 Status;
	u32 OutputCurrent; // Variable to store the read VOP value in uA
	XRFdc_IPStatus ipStatus;
	XRFdc* RFdcInstPtr = &RFdcInst;

	// Get the overall status of the RFdc IP
	XRFdc_GetIPStatus(RFdcInstPtr, &ipStatus);

	// Loop through all possible Tiles (0-3)
	for (Tile_Id = 0; Tile_Id <= 3; Tile_Id++)
	{
		// Loop through all possible Blocks within a Tile (0-3)
		for (Block_Id = 0; Block_Id <= 3; Block_Id++)
		{
			// Set a default/error value. If any check fails, this value will be kept.
			cmdVals[Tile_Id * 4 + Block_Id] = 0;

			// Proceed only if the Tile and Block are enabled and the device is Gen3+
			if ((ipStatus.DACTileStatus[Tile_Id].IsEnabled == 1) &&
			    (XRFdc_IsDACBlockEnabled(RFdcInstPtr, Tile_Id, Block_Id)) &&
			    (RFdcInstPtr->RFdc_Config.IPType >= XRFDC_GEN3))
			{
				// Attempt to read the output current (VOP)
				Status = XRFdc_GetOutputCurr(RFdcInstPtr, Tile_Id, Block_Id, &OutputCurrent);

				// If the read was successful, update the array with the actual value
				if (Status == XST_SUCCESS)
				{
					cmdVals[Tile_Id * 4 + Block_Id] = OutputCurrent;
				}
				// If Status is not XST_SUCCESS, the value remains 0 (the default error value)
			}
		}
	}

	return;
}

/*****************************************************************************/
/**
*
* Set the ADC DSA via API
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void adcSetDSA(u32 *cmdVals)
{

	u32 Tile_Id;
	u32 Block_Id;
	u32 Status;
	float Attenuation;
	float Attenuation_get;
	XRFdc_IPStatus ipStatus;
	XRFdc* RFdcInstPtr = &RFdcInst;
	XRFdc_DSA_Settings DSA_Settings;

	Tile_Id = cmdVals[0];
	Block_Id = cmdVals[1];

	xil_printf("Set Attenuation in dB to : ");
	Attenuation = mygetlinef();

	// Calling this function gets the status of the IP
	XRFdc_GetIPStatus(RFdcInstPtr, &ipStatus);
	xil_printf("\n\r###############################################\r\n");

	if (RFdcInstPtr->RFdc_Config.IPType >= XRFDC_GEN3) {

		if(XRFdc_IsADCBlockEnabled(RFdcInstPtr, Tile_Id, Block_Id)) {
			DSA_Settings.Attenuation = Attenuation;
			// ADC Set Attenuation
			Status = XRFdc_SetDSA(RFdcInstPtr,  Tile_Id, Block_Id, &DSA_Settings);
			if (Status != XST_SUCCESS) {
				xil_printf("XRFdc_SetDSA() for ADC Tile%d ch%d failed.\n\r",Tile_Id,Block_Id);
				return;
			}
			else{
				Status = XRFdc_GetDSA(RFdcInstPtr,  Tile_Id, Block_Id, &DSA_Settings);
				Attenuation_get = DSA_Settings.Attenuation;

				printf("   ADC Tile%d ch%d DSA set to %.1f dB.\n\r",Tile_Id,Block_Id,Attenuation_get);
			}
		}
		else{
			xil_printf("ADC Tile%d ch%d is not available.\n\r", Tile_Id, Block_Id);
		}
		}  // Gen1 or Gen2
	else{
		xil_printf("This command is for RFSoC Gen3 devices only.\n\r");
	}
		xil_printf("###############################################\r\n");
		return;
}


/*****************************************************************************/
/**
*
* Set the ADC DSA via API
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void adcSetDSA2(u32 Tile, u32 Block, float Att)
{

	u32 Tile_Id;
	u32 Block_Id;
	u32 Status;
	float Attenuation;
	float Attenuation_get;
	XRFdc_IPStatus ipStatus;
	XRFdc* RFdcInstPtr = &RFdcInst;
	XRFdc_DSA_Settings DSA_Settings;

	Tile_Id = Tile;//cmdVals[0];
	Block_Id = Block;//cmdVals[1];
	Attenuation = Att;// cmdVals[2];

	xil_printf("Set Attenuation in dB to :  %1.1f", Attenuation);
	//Attenuation = mygetlinef();

	// Calling this function gets the status of the IP
	XRFdc_GetIPStatus(RFdcInstPtr, &ipStatus);
	xil_printf("\n\r###############################################\r\n");

	if (RFdcInstPtr->RFdc_Config.IPType >= XRFDC_GEN3) {

		if(XRFdc_IsADCBlockEnabled(RFdcInstPtr, Tile_Id, Block_Id)) {
			DSA_Settings.Attenuation = Attenuation;
			// ADC Set Attenuation
			Status = XRFdc_SetDSA(RFdcInstPtr,  Tile_Id, Block_Id, &DSA_Settings);
			if (Status != XST_SUCCESS) {
				xil_printf("XRFdc_SetDSA() for ADC Tile%d ch%d failed.\n\r",Tile_Id,Block_Id);
				return;
			}
			else{
				Status = XRFdc_GetDSA(RFdcInstPtr,  Tile_Id, Block_Id, &DSA_Settings);
				Attenuation_get = DSA_Settings.Attenuation;

				printf("   ADC Tile%d ch%d DSA set to %.1f dB.\n\r",Tile_Id,Block_Id,Attenuation_get);
			}
		}
		else{
			xil_printf("ADC Tile%d ch%d is not available.\n\r", Tile_Id, Block_Id);
		}
		}  // Gen1 or Gen2
	else{
		xil_printf("This command is for RFSoC Gen3 devices only.\n\r");
	}
		xil_printf("###############################################\r\n");
		return;
}

/*****************************************************************************/
/**
*
* Set the ADC DSA via API
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void adcSetDSA3(u32 Tile, u32 Block, int Att)
{

	u32 Tile_Id;
	u32 Block_Id;
	u32 Status;
	float Attenuation;
	float Attenuation_get;
	XRFdc_IPStatus ipStatus;
	XRFdc* RFdcInstPtr = &RFdcInst;
	XRFdc_DSA_Settings DSA_Settings;

	Tile_Id = Tile;//cmdVals[0];
	Block_Id = Block;//cmdVals[1];
	Attenuation = Att * 1.0;// cmdVals[2];

	//xil_printf("Set Attenuation in dB to :  %1.1f", Attenuation);
	//Attenuation = mygetlinef();

	// Calling this function gets the status of the IP
	XRFdc_GetIPStatus(RFdcInstPtr, &ipStatus);
	//xil_printf("\n\r###############################################\r\n");

	if (RFdcInstPtr->RFdc_Config.IPType >= XRFDC_GEN3) {

		if(XRFdc_IsADCBlockEnabled(RFdcInstPtr, Tile_Id, Block_Id)) {
			DSA_Settings.Attenuation = Attenuation;
			// ADC Set Attenuation
			Status = XRFdc_SetDSA(RFdcInstPtr,  Tile_Id, Block_Id, &DSA_Settings);
			if (Status != XST_SUCCESS) {
				//xil_printf("XRFdc_SetDSA() for ADC Tile%d ch%d failed.\n\r",Tile_Id,Block_Id);
				return;
			}
			else{
				Status = XRFdc_GetDSA(RFdcInstPtr,  Tile_Id, Block_Id, &DSA_Settings);
				Attenuation_get = DSA_Settings.Attenuation;
				printf("ADC[%d:%d] = %.1f dB.\n\r",Tile_Id,Block_Id,Attenuation_get);

				//printf("   ADC Tile%d ch%d DSA set to %.1f dB.\n\r",Tile_Id,Block_Id,Attenuation_get);
			}
		}
		else{
			//xil_printf("ADC Tile%d ch%d is not available.\n\r", Tile_Id, Block_Id);
		}
		}  // Gen1 or Gen2
	else{
		//xil_printf("This command is for RFSoC Gen3 devices only.\n\r");
	}
		//xil_printf("###############################################\r\n");
		return;
}



/*****************************************************************************/
/**
*
* Get the ADC DSA via API
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void adcGetDSA(u32 *cmdVals)
{
	u32 Tile_Id;
	u32 Block_Id;
	u32 Status;
	float Attenuation2;
	XRFdc_IPStatus ipStatus;
	XRFdc* RFdcInstPtr = &RFdcInst;

	XRFdc_DSA_Settings DSA_Settings;

	// Calling this function gets the status of the IP
	XRFdc_GetIPStatus(RFdcInstPtr, &ipStatus);

	xil_printf("\r\n###############################################\r\n");
	xil_printf("=== ADC DSA Report ===\n\r");

	for ( Tile_Id=0; Tile_Id<=3; Tile_Id++) {
    	if (ipStatus.ADCTileStatus[Tile_Id].IsEnabled == 1) {
			for ( Block_Id=0; Block_Id<=3; Block_Id++) {
				if (XRFdc_IsADCBlockEnabled(RFdcInstPtr, Tile_Id, Block_Id) != 0U) {
					if (RFdcInstPtr->RFdc_Config.IPType >= XRFDC_GEN3) {

						if(XRFdc_IsADCBlockEnabled(RFdcInstPtr, Tile_Id, Block_Id)) {

							// ADC Set Attenuation
							Status = XRFdc_GetDSA(RFdcInstPtr,  Tile_Id, Block_Id, &DSA_Settings);
							Attenuation2 = DSA_Settings.Attenuation;
							if (Status != XST_SUCCESS) {
								xil_printf("XRFdc_SetDSA() for ADC Tile%d ch%d failed.\n\r",Tile_Id,Block_Id);
								return;
							}
							else{
								printf("   ADC Tile%d ch%d DSA set to %.1f dB.\n\r",Tile_Id,Block_Id,Attenuation2);
							}
						}
						else{
							xil_printf("ADC Tile%d ch%d is not available.\n\r", Tile_Id, Block_Id);
						}
						}
					else{
						xil_printf("This command is for RFSoC Gen3 devices only.\n\r");
					}
    			}
    		}
		}
	}
	xil_printf("###############################################\r\n");

	return;
}


/*****************************************************************************/
/**
*
* Get the ADC DSA via API and store the results for 16 channels.
*
* @param	cmdVals: Pointer to an array to store the attenuation values.
*
* @return	None
*
* @note		The attenuation values are stored as floating-point numbers,
*               but will be cast to u32 when stored in cmdVals.
*
******************************************************************************/
void adcGetDSA2(u32 *cmdVals)
{
	u32 Tile_Id;
	u32 Block_Id;
	u32 Status;
	float Attenuation2;
	XRFdc_IPStatus ipStatus;
	XRFdc* RFdcInstPtr = &RFdcInst;
	u32 channel_index = 0; // Index to store values in cmdVals

	XRFdc_DSA_Settings DSA_Settings;

	// Calling this function gets the status of the IP
	XRFdc_GetIPStatus(RFdcInstPtr, &ipStatus);

	for (Tile_Id = 0; Tile_Id <= 3; Tile_Id++)
	{
		if (ipStatus.ADCTileStatus[Tile_Id].IsEnabled == 1)
		{
			for (Block_Id = 0; Block_Id <= 3; Block_Id++)
			{
				if (XRFdc_IsADCBlockEnabled(RFdcInstPtr, Tile_Id, Block_Id) != 0U)
				{
					if (RFdcInstPtr->RFdc_Config.IPType >= XRFDC_GEN3)
					{
						if(XRFdc_IsADCBlockEnabled(RFdcInstPtr, Tile_Id, Block_Id))
						{
							// ADC Get Attenuation
							Status = XRFdc_GetDSA(RFdcInstPtr, Tile_Id, Block_Id, &DSA_Settings);
							if (Status != XST_SUCCESS)
							{
								//xil_printf("XRFdc_GetDSA() for ADC Tile%d ch%d failed.\n\r", Tile_Id, Block_Id);
								cmdVals[Tile_Id * 4 + Block_Id] = 28; //
								return;
							}
							else
							{
								Attenuation2 = DSA_Settings.Attenuation;
								//printf("   ADC Tile%d ch%d DSA is %.1f dB.\n\r", Tile_Id, Block_Id, Attenuation2);
								cmdVals[Tile_Id * 4 + Block_Id] = (u32)(Attenuation2 * 1.0f); // Example: store 24.5 as 245
							}
						}
						else
						{
							cmdVals[Tile_Id * 4 + Block_Id] = 28; //
							//xil_printf("ADC Tile%d ch%d is not available.\n\r", Tile_Id, Block_Id);
						}
					}
					else
					{
						cmdVals[Tile_Id * 4 + Block_Id] = 28; //
						//xil_printf("This command is for RFSoC Gen3 devices only.\n\r");
					}
				}
			}
		}
	}
	//xil_printf("###############################################\r\n");

	return;
}
