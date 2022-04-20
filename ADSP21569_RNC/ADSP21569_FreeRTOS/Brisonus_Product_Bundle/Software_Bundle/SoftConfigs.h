/*
 * Configs.h
 *
 *  Created on: Apr 2, 2020
 *      Author: vince
 */

#ifndef _CONFIGS_H_
#define _CONFIGS_H_

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define conFREQ_FRESH_SIZE              (0x48)

#define conPRI_SYS_BASE                 (0x40)
#define conPRI_COMM_BASE                (0x30)
#define conPRI_ALGO_BASE                (0x20)
#define conPRI_DIAG_BASE                (0x10)
#define conPRI_UART_BASE                (0x01)


#endif /* BRISONUS_PRODUCT_BUNDLE_SOFTWARE_BUNDLE_CONFIGS_H_ */
