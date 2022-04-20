/*
 * FirmGPIO.c
 *
 *  Created on: May 8, 2020
 *      Author: Sheng Zhang
 */

#include "FirmGPIO.h"

static uint8_t PrvGPIOMemory[fgpioGPIO_MEMORY_SIZE*fgpioCALLBACKNUM];
static bool prvSpeakerDisable = false;

bool __getSwitchVal(void){
	prvSpeakerDisable = (bool)(*pREG_PORTA_DATA & 0x0200);
	return prvSpeakerDisable;
}

//bool speaker_disable = false;
//
//void __getSwitchVal(void){
//	speaker_disable = (bool)(*pREG_PORTA_DATA & 0x0200);
//}

/*todo:[debug interface,GPIO]:modify this, NOTE:DO NOT ADD TIME CONSUMING ACTIONS like printf and creat task etc.*/
//static void PrvGPIOCallback(ADI_GPIO_PIN_INTERRUPT ePinInt, uint32_t Data, void *pCBParam)
//{
//	speaker_disable = !speaker_disable;
//}

void vFGPIOInit()
{

	adi_pads_Config(FAULTPIN_SEL, true);


	uint32_t gpioMaxCallbacks;

	ADI_GPIO_RESULT result;

	result = adi_gpio_Init((void*)PrvGPIOMemory,sizeof(PrvGPIOMemory),&gpioMaxCallbacks);
	if(result != ADI_GPIO_SUCCESS)
	{
		printf("GPIO Initialization failed \n");

	}


	/*
	result =adi_gpio_PortInit(ADI_GPIO_PORT_B,ADI_GPIO_PIN_3, ADI_GPIO_DIRECTION_INPUT,true);
	 if(result != ADI_GPIO_SUCCESS)
	 {
		 printf("GPIO Initialization failed \n");
	 }

	 result = adi_gpio_RegisterCallback(ADI_GPIO_PIN_INTERRUPT_0,ADI_GPIO_PIN_3,PrvGPIOCallback,(void*)0);
	 if(result != ADI_GPIO_SUCCESS)
	 {
		 printf("GPIO Initialization failed \n");
	 }

	 result =adi_gpio_PinInt(ADI_GPIO_PIN_ASSIGN_PBL_PINT0,ADI_GPIO_PIN_3, ADI_GPIO_PIN_INTERRUPT_0,ADI_GPIO_PIN_ASSIGN_BYTE_0,true,ADI_GPIO_SENSE_LEVEL_HIGH);
	 if(result != ADI_GPIO_SUCCESS)
	 {
		 printf("GPIO Initialization failed \n");
	 }

	 */

	 result = adi_gpio_PortInit(ADI_GPIO_PORT_A,ADI_GPIO_PIN_9, ADI_GPIO_DIRECTION_INPUT,true);
	if(result != ADI_GPIO_SUCCESS)
	{
		printf("GPIO Initialization failed \n");
	}

	*pREG_PADS0_PORTA_PUE = (1<<9);

/*
	result = adi_gpio_Set(ADI_GPIO_PORT_A,ADI_GPIO_PIN_9);

	if(result != ADI_GPIO_SUCCESS)
	{
		printf("GPIO set failed \n");
	}



	result = adi_gpio_RegisterCallback(ADI_GPIO_PIN_INTERRUPT_1,ADI_GPIO_PIN_9,PrvGPIOCallback,(void*)0);
	 if(result != ADI_GPIO_SUCCESS)
	 {
		 printf("GPIO Initialization failed \n");
	 }

	 result =adi_gpio_PinInt(ADI_GPIO_PIN_ASSIGN_PAL_PINT0,ADI_GPIO_PIN_9, ADI_GPIO_PIN_INTERRUPT_1,ADI_GPIO_PIN_ASSIGN_BYTE_0,true,ADI_GPIO_SENSE_LEVEL_HIGH);
	 if(result != ADI_GPIO_SUCCESS)
	 {
		 printf("GPIO Initialization failed \n");
	 }
	 */

	__getSwitchVal();

	 printf("GPIO initialization finished\n");
}

