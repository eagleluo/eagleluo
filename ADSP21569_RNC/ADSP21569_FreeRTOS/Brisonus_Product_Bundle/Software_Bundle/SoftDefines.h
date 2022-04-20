/*
 * SoftDefines.h
 *
 *  Created on: Apr 7, 2020
 *      Author: vince
 */

#ifndef BRISONUS_PRODUCT_BUNDLE_SOFTWARE_BUNDLE_SOFTDEFINES_H_
#define BRISONUS_PRODUCT_BUNDLE_SOFTWARE_BUNDLE_SOFTDEFINES_H_

#include "SoftFreq.h"
#include "SoftTime.h"
#include "SoftUart.h"

typedef struct {
	xATimeParams	task_time_params;
	xSUartParams	task_uart_params;
}App_t;


App_t xAppData;


#endif /* BRISONUS_PRODUCT_BUNDLE_SOFTWARE_BUNDLE_SOFTDEFINES_H_ */
