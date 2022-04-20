/*******************************************************************************
Copyright (c) 2014 - Analog Devices Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc.
and its licensors.
******************************************************************************
 * @file:    adi_a2b_codecInit.c
 * @brief:   This is the HAL implementation of SPORT, SPI, PCG, ADAU1979 ADC, ADAU1962A DAC and GPIO
 * @version: $Revision: 6209 $
 * @date:    $Date: 2015-01-07 11:53:06 +0530 (Wed, 07 Jan 2015) $
 * Developed by: Automotive Software and Systems team, Bangalore, India
*****************************************************************************/
/*! \addtogroup ADI_A2B_CODECINIT
 *  @{
 */

/*============= I N C L U D E S =============*/

#include <stdio.h>
#include "adi_a2b_hal.h"         /*!< ADI HAL include file */

#ifndef _TESSY_

/*-------------- G P I O --------------*/
#include <services/gpio/adi_gpio.h>                 /*!< ADI GPIO definitions include file */

#else
#include <stddef.h>
#include "tessy_defines.h"
#endif

/*============= D E F I N E S =============*/

#if !(defined (_TESSY_) || defined (_TESSY_COMPILER_DEPENDENT_))
#define CHECK_RESULT_HAL(hDevice, nExpectedValue, nObtainedValue, nErrorMessage)    \
    do                                                                              \
    {                                                                               \
        if((nObtainedValue) != (nExpectedValue))                                    \
        {                                                                           \
            (hDevice) = NULL;                                                       \
        }                                                                           \
    } while (0)  /* do-while-zero needed for Misra Rule 19.4 */
#else
#define CHECK_RESULT_HAL(hDevice, nExpectedValue, nObtainedValue, nErrorMessage)
#endif	/* _TESSY_ */

/*-------------- G P I O --------------*/

static uint32_t aHalGpioPort[] = {
                            (uint32_t)ADI_GPIO_PORT_A,
                            (uint32_t)ADI_GPIO_PORT_B,
                            (uint32_t)ADI_GPIO_PORT_C,
                          };


static uint32_t aHalGpioPin[] = {
                            ADI_GPIO_PIN_0,
                            ADI_GPIO_PIN_1,
                            ADI_GPIO_PIN_2,
                            ADI_GPIO_PIN_3,
                            ADI_GPIO_PIN_4,
                            ADI_GPIO_PIN_5,
                            ADI_GPIO_PIN_6,
                            ADI_GPIO_PIN_7,
                            ADI_GPIO_PIN_8,
                            ADI_GPIO_PIN_9,
                            ADI_GPIO_PIN_10,
                            ADI_GPIO_PIN_11,
                            ADI_GPIO_PIN_12,
                            ADI_GPIO_PIN_13,
                            ADI_GPIO_PIN_14,
                            ADI_GPIO_PIN_15
                         };


/*-------------- G P I O --------------*/

/********************************************************************************************
    @brief      Sets the given GPIO pin(s) to an input or output direction.
    @param      ePort       The GPIO port.
    @param      ePin        The pin(s) to set as input or output.
    @param      eDirection  The direction to set.
    @return     Return code of ADI_A2B_HAL_GPIO_STATUS type.
                    - #ADI_A2B_HAL_GPIO_STATUS_SUCCESS   : The API call succeeded.
                    - #ADI_A2B_HAL_GPIO_STATUS_FAILED    : The API call failed.

    <b> Global Variables Used: <b>
                None

   @note        This API has to be individually called for each pin to set the direction.
 *********************************************************************************************/

ADI_A2B_HAL_GPIO_STATUS adi_a2b_hal_gpio_SetDirection(ADI_A2B_HAL_GPIO_PORT        ePort,
                                                      ADI_A2B_HAL_GPIO_PIN         ePin,
                                                      ADI_A2B_HAL_GPIO_DIRECTION   eDirection)
{
    ADI_GPIO_RESULT     eGpioResult=ADI_GPIO_FAILURE;
    ADI_GPIO_DIRECTION  eGpioDirection=ADI_GPIO_DIRECTION_OUTPUT;

    switch(eDirection)
    {
        case ADI_A2B_HAL_GPIO_DIRECTION_INPUT:
            eGpioDirection = ADI_GPIO_DIRECTION_INPUT;
            break;
        case ADI_A2B_HAL_GPIO_DIRECTION_OUTPUT:
            eGpioDirection = ADI_GPIO_DIRECTION_OUTPUT;
            break;
        default :
            break;
    }

    eGpioResult = adi_gpio_SetDirection((ADI_GPIO_PORT)aHalGpioPort[ePort], (uint32_t)aHalGpioPin[ePin],eGpioDirection);

    return ((eGpioResult != ADI_GPIO_SUCCESS) ? ADI_A2B_HAL_GPIO_STATUS_FAILED : ADI_A2B_HAL_GPIO_STATUS_SUCCESS);
}

/**********************************************************************************************************
   @brief       Sets the given GPIO pin(s) to a logic high level.
                The logic level of other pins is not changed.
   @param       ePort       The GPIO port.
   @param       ePin        The pin(s) to set to a logic high level.
   @return      Return code of ADI_A2B_HAL_GPIO_STATUS type.
                    - #ADI_A2B_HAL_GPIO_STATUS_SUCCESS   : The API call succeeded.
                    - #ADI_A2B_HAL_GPIO_STATUS_FAILED    : The API call failed.

   <b> Global Variables Used: <b>
                None

  @note         This API has to be individually called for each pin to Set.
************************************************************************************************************/

ADI_A2B_HAL_GPIO_STATUS adi_a2b_hal_gpio_Set(ADI_A2B_HAL_GPIO_PORT ePort, ADI_A2B_HAL_GPIO_PIN ePin)
{
    ADI_GPIO_RESULT eGpioResult;

    eGpioResult = adi_gpio_Set((ADI_GPIO_PORT)aHalGpioPort[ePort], (uint32_t)aHalGpioPin[ePin]);

    return ((eGpioResult != ADI_GPIO_SUCCESS) ? ADI_A2B_HAL_GPIO_STATUS_FAILED : ADI_A2B_HAL_GPIO_STATUS_SUCCESS);
}

/***********************************************************************************************************
   @brief       Sets the given GPIO pin(s) to a logic low level.
                The logic level of other pins is not changed.
   @param       ePort       The GPIO port.
   @param       ePin        The pin(s) to set to a logic low level.
   @return      Return code of ADI_A2B_HAL_GPIO_STATUS type.
                    - #ADI_A2B_HAL_GPIO_STATUS_SUCCESS   : The API call succeeded.
                    - #ADI_A2B_HAL_GPIO_STATUS_FAILED    : The API call failed.

   <b> Global Variables Used: <b>
                None

  @note         This API has to be individually called for each pin to Clear.
************************************************************************************************************/

ADI_A2B_HAL_GPIO_STATUS adi_a2b_hal_gpio_Clear(ADI_A2B_HAL_GPIO_PORT ePort, ADI_A2B_HAL_GPIO_PIN ePin)
{
    ADI_GPIO_RESULT eGpioResult;

    eGpioResult = adi_gpio_Clear((ADI_GPIO_PORT)aHalGpioPort[ePort], (uint32_t)aHalGpioPin[ePin]);

    return ((eGpioResult != ADI_GPIO_SUCCESS) ? ADI_A2B_HAL_GPIO_STATUS_FAILED : ADI_A2B_HAL_GPIO_STATUS_SUCCESS);
}

/***********************************************************************************************************
   @brief       Toggle the logic level of the given GPIO pin(s).
                The logic level of other pins is not changed.
   @param       ePort       The GPIO port.
   @param       ePin        The pin(s) to set to toggle.
   @return      Return code of ADI_A2B_HAL_GPIO_STATUS type.
                    - #ADI_A2B_HAL_GPIO_STATUS_SUCCESS   : The API call succeeded.
                    - #ADI_A2B_HAL_GPIO_STATUS_FAILED    : The API call failed.

   <b> Global Variables Used: <b>
                None

  @note         This API has to be individually called for each pin to Toggle.
************************************************************************************************************/

ADI_A2B_HAL_GPIO_STATUS adi_a2b_hal_gpio_Toggle(ADI_A2B_HAL_GPIO_PORT ePort, ADI_A2B_HAL_GPIO_PIN ePin)
{
    ADI_GPIO_RESULT eGpioResult;

    eGpioResult = adi_gpio_Toggle((ADI_GPIO_PORT)aHalGpioPort[ePort], (uint32_t)aHalGpioPin[ePin]);

    return ((eGpioResult != ADI_GPIO_SUCCESS) ? ADI_A2B_HAL_GPIO_STATUS_FAILED : ADI_A2B_HAL_GPIO_STATUS_SUCCESS);
}

/*
 *  @}
 */

/*
 *
 * EOF:
 *
*/
