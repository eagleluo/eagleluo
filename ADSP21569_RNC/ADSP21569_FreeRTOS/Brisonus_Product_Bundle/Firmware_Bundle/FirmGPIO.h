/*
 * FirmGPIO.h
 *
 *  Created on: May 8, 2020
 *      Author: vince
 */

#ifndef _FIRMGPIO_H_
#define _FIRMGPIO_H_

#include <stdio.h>
#include <services\gpio\adi_gpio.h>

#define fgpioCALLBACKNUM                 (2u)
#define fgpioGPIO_MEMORY_SIZE 			(ADI_GPIO_CALLBACK_MEM_SIZE)


void vFGPIOInit(void);

#endif /* BRISONUS_PRODUCT_BUNDLE_FIRMWARE_BUNDLE_A2B_STACK_FIRMGPIO_H_ */
