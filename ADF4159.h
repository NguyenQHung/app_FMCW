/*
 * ADF_4159.h
 *
 *  Created on: Mar 18, 2024
 *      Author: tienn
 */

#ifndef SRC_ADF4159_H_
#define SRC_ADF4159_H_

#include "main.h"

/* adi,muxout-select */
#define MUXOUT_THREE_STATE_OUTPUT			0
#define MUXOUT_DVDD							1
#define MUXOUT_DGND							2
#define MUXOUT_R_DIVIDER_OUTPUT				3
#define MUXOUT_N_DIVIDER_OUTPUT				4
#define MUXOUT_DIGITAL_LOCK_DETECT			6
#define MUXOUT_SERIAL_DATA_OUTPUT			7
#define MUXOUT_CLK_DIVIDER_OUTPUT			10
#define MUXOUT_R_DIVIDER_DIV2				13
#define MUXOUT_N_DIVIDER_DIV2				14
#define MUXOUT_READBACK_TO_MUXOUT			15

/* adi,clk-div-mode */
#define CLK_DIV_MODE_OFF					0
#define CLK_DIV_MODE_FAST_LOCK				1
#define CLK_DIV_MODE_RAMP					3

/* adi,ramp-mode-select */
#define RAMP_MODE_CONTINUOUS_SAWTOOTH		0
#define RAMP_MODE_CONTINUOUS_TRIANGULAR		1
#define RAMP_MODE_SINGLE_SAWTOOTH_BURST		2
#define RAMP_MODE_SINGLE_RAMP_BURST			3

/* adi,ramp-status-mode */
#define RAMP_STATUS_NORMAL_OPERATION		0
#define RAMP_STATUS_READBACK_TO_MUXOUT		2
#define RAMP_STATUS_RAMP_COMPLETE_TO_MUXOUT	3
#define RAMP_STATUS_CHARGE_PUMP_UP			16
#define RAMP_STATUS_CHARGE_PUMP_DOWN		17

void delay2();
void ADF4159_Innit(void);
void ADF4159_Write(void);
void ADF4159_Set_Freq(unsigned int Freq);
void ADF4159_VCO_ENable(unsigned int val);

#endif /* SRC_ADF4159_H_ */
