/*
 * SoftUart.H
 *
 *  Created on: May 7, 2020
 *      Author: vince
 */

#ifndef _SOFTUART_H_
#define _SOFTUART_H_

#include <stdio.h>

#include "../Firmware_Bundle/FirmUART.h"
#include "SoftConfigs.h"




#define softuTASK_NAME          "Uart the debugger"
#define softuSTACK_SIZE         (2000u)
#define softuPRIORTITY          (conPRI_UART_BASE + 5u)



void vSUartTaskFunc(xSUartParams *puart);


#endif /* BRISONUS_PRODUCT_BUNDLE_SOFTWARE_BUNDLE_SOFTUART_H_ */
