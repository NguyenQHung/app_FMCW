/*
 * adcDisableCoeffOvrd.h
 *
 *  Created on: Aug 10, 2019
 *      Author: jlantz
 */

#ifndef ADCDISABLECALCOEFFOVRD_H_
#define ADCDISABLECALCOEFFOVRD_H_


/***************************** Include Files *********************************/
//#include "xil_types.h"

#include "linux_hal.h"      // Lớp trừu tượng phần cứng (sysfs GPIO)

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/


void adcDisableCoeffOvrd(u32 *cmdVals);
void cli_adcDisableCoeffOvrd_init(void);


/************************** Variable Definitions *****************************/








#endif /* ADCDISABLECALCOEFFOVRD_H_ */
