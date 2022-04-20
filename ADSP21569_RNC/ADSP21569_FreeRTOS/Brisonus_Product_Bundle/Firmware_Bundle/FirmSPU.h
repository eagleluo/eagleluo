/*
 * FirmSPU.h
 *
 *  Created on: Apr 3, 2020
 *      Author: vince
 */

#ifndef BRISONUS_PRODUCT_BUNDLE_FIRMWARE_BUNDLE_FIRMSPU_H_
#define BRISONUS_PRODUCT_BUNDLE_FIRMWARE_BUNDLE_FIRMSPU_H_

#include <stdio.h>
#include <services/spu/adi_spu.h>


#define fspuSPORT_0A_SPU      			(13u)
#define fspuSPORT_4A_SPU      			(21u)
#define fspuSPORT_4B_SPU      			(22u)

#define fspuSPORT_0A_DMA_SPU      		(49u)
#define fspuSPORT_4A_DMA_SPU      		(57u)
#define fspuSPORT_4B_DMA_SPU      		(58u)

#define fspuERROR             			(1u)
#define fspuSUCCESS           			(0u)

uint32_t ulFSPUInit(void);

#endif /* BRISONUS_PRODUCT_BUNDLE_FIRMWARE_BUNDLE_FIRMSPU_H_ */
