/*
 * FirmUART.h
 *
 *  Created on: Apr 3, 2020
 *      Author: vince
 */

#ifndef BRISONUS_PRODUCT_BUNDLE_FIRMWARE_BUNDLE_FIRMUART_H_
#define BRISONUS_PRODUCT_BUNDLE_FIRMWARE_BUNDLE_FIRMUART_H_


#include <stdio.h>
#include <drivers/uart/adi_uart.h>
#include <services/stdio/adi_stdio.h>

#include "../Commons_Bundle/str2fun/str2fun.h"


#define fuartBAUD_RATE                  (115200u)

typedef struct {

	char*		UartBuf;
	uint32_t 	Start_Addr;
	uint32_t 	ParamsNum;
	int32_t *	Buffer_Addr;

}xSUartParams;


void vFUartInit(xSUartParams *puart);

#endif /* BRISONUS_PRODUCT_BUNDLE_FIRMWARE_BUNDLE_FIRMUART_H_ */
