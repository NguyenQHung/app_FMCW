/*
 * ADF4360.h
 *
 *  Created on: Jun 27, 2024
 *      Author: tienn
 */

#ifndef ADF4360_7_H_
#define ADF4360_7_H_

#include "main.h"

//#define	ADF4360_Freq0 1400

void ADF4360_Set_Freq2(unsigned int Freq);
void ADF4360_Set_Freq(unsigned int Freq);
void ADF4360_CE_PIN(unsigned char Val);
void ADF4360_Innit(void);
void ADF4360_Write(void);

#endif /* ADF4360_7_H_ */
