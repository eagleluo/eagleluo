/*
 * FirmSPORT.h
 *
 *  Created on: Apr 3, 2020
 *      Author: vince
 */

#ifndef BRISONUS_PRODUCT_BUNDLE_FIRMWARE_BUNDLE_FIRMSPORT_H_
#define BRISONUS_PRODUCT_BUNDLE_FIRMWARE_BUNDLE_FIRMSPORT_H_

#include "FreeRTOS.h"
#include "semphr.h"
#include "../../src/projConfig.h"

#define fsportDEVICE_0A                         (0u)            /* SPORT device number */
#define fsportDEVICE_4A                         (4u)            /* SPORT device number */
#define fsportDEVICE_4B                         (4u)            /* SPORT device number */
#define fsportBLOCK_SIZE                        BLOCK_SIZE

#define fsportA2B_SAMPLES						(BLOCK_SIZE*A2B_SLOTS)
#define fsportRX_SAMPLES						(BLOCK_SIZE*RX_SLOTS)
#define fsportTX_SAMPLES						(BLOCK_SIZE*TX_SLOTS)
#define fsportDMA_NUM_DESC                      (2u)

uint32_t ulFSPortInit(int32_t **adaddr,int32_t **daaddr,int32_t **a2baddr,SemaphoreHandle_t semaphore);
uint32_t ulFSPortClose(void);

#endif /* BRISONUS_PRODUCT_BUNDLE_FIRMWARE_BUNDLE_FIRMSPORT_H_ */
