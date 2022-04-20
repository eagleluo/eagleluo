/*
 * SoftUart.c
 *
 *  Created on: May 7, 2020
 *      Author: Sheng Zhang
 */


#include "SoftUart.h"




static Str2FunHandle prvStr2FunHandle;	/*str2fun handle*/
static uint32_t ulMemSize = str2funMEM_SIZE+1;
static uint8_t pStr2FunMem[str2funMEM_SIZE];
static char prvInput[200];



void vSUartTaskFunc(xSUartParams *params)
{
	/*These three params are updated in audio related firmware */

	STR2FUN_ERROR error;
	xSUartParams *UartTask = params;

	UartTask->UartBuf = prvInput;

	error = xStr2FunInit(&prvStr2FunHandle, pStr2FunMem, &ulMemSize);

	if(error != STR2FUN_DEINIT_NO_ERROR)
	{
		printf("Str2Fun initialize failed with error code:%d\n",error);
		TaskHandle_t xHandleThisTask = xTaskGetCurrentTaskHandle();
		vTaskDelete(xHandleThisTask);
	}

	for ( ;;)
	{
		gets(UartTask->UartBuf);
		xStr2Fun(UartTask->UartBuf,prvStr2FunHandle);
		printf(UartTask->UartBuf);
		vTaskDelay((TickType_t)10);
	}
}

