/*
 * FirmTMR.h
 *
 *  Created on: Apr 3, 2020
 *      Author: vince
 */

#ifndef BRISONUS_PRODUCT_BUNDLE_FIRMWARE_BUNDLE_FIRMTMR_H_
#define BRISONUS_PRODUCT_BUNDLE_FIRMWARE_BUNDLE_FIRMTMR_H_

#define TIMER_WIDTH             (10000u)
#define TIMER_DELAY             (1000u)
#define TIMER_PERIOD            (20000u)
#define GP_TIMER_NUM            (1u)

void vfTimerInit(void);

#endif /* BRISONUS_PRODUCT_BUNDLE_FIRMWARE_BUNDLE_FIRMTMR_H_ */
