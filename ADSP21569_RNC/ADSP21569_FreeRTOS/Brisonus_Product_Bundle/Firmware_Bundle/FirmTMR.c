#include <stdio.h>
#include <services/tmr/adi_tmr_2156x.h>
#include <services/tmr/adi_tmr_config_2156x.h>
#include "FirmTMR.h"

static uint8_t prvpTimerMemory[ADI_TMR_MEMORY];
static uint32_t prvulTickCount;
static ADI_TMR_HANDLE prvhTimer;


static void prvfTimerHandler(void *pCBParam, uint32_t Event, void *pArg)
{
	prvulTickCount++;
}

static void prvfTimerInitInterrupts(void)
{
	ADI_TMR_RESULT eTmrResult = ADI_TMR_SUCCESS;

	if (prvhTimer) {
		/* Set the IRQ mode to get interrupt after timer counts to Delay + Width */
		if ((eTmrResult = adi_tmr_SetIRQMode(
			     prvhTimer,
			     ADI_TMR_IRQMODE_WIDTH_DELAY)) != ADI_TMR_SUCCESS)
			printf("Failed to set the timer IRQ mode 0x%08X\n", eTmrResult);
	}
	/* Enable the timer*/
	if ((eTmrResult = adi_tmr_Enable(
		     prvhTimer,
		     true)) != ADI_TMR_SUCCESS)
		printf("Failed to enable the timer 0x%08X\n", eTmrResult);
}


void vfTimerInit()
{
	ADI_TMR_RESULT eTmrResult = ADI_TMR_SUCCESS;

	if (!prvhTimer) {
		/* Open the timer */
		if ((eTmrResult = adi_tmr_Open(
			     GP_TIMER_NUM,
			     prvpTimerMemory,
			     ADI_TMR_MEMORY,
			     prvfTimerHandler,
			     NULL,
			     &prvhTimer)) != ADI_TMR_SUCCESS)
			printf("Timer open failed 0x%08X\n", eTmrResult);

		/* Set the mode to PWM OUT */
		if ((eTmrResult = adi_tmr_SetMode(
			     prvhTimer,
			     ADI_TMR_MODE_CONTINUOUS_PWMOUT)) != ADI_TMR_SUCCESS)
			printf("Failed to open timer in PWM out mode 0x%08X\n", eTmrResult);

		/* Set the Period */
		if ((eTmrResult = adi_tmr_SetPeriod(
			     prvhTimer,
			     TIMER_PERIOD)) != ADI_TMR_SUCCESS)
			printf("Failed to set the timer Period 0x%08X\n", eTmrResult);

		/* Set the timer width */
		if ((eTmrResult = adi_tmr_SetWidth(
			     prvhTimer,
			     TIMER_WIDTH)) != ADI_TMR_SUCCESS)
			printf("Failed to set the timer Width 0x%08X\n", eTmrResult);

		/* Set the timer Delay */
		if ((eTmrResult = adi_tmr_SetDelay(
			     prvhTimer,
			     TIMER_DELAY)) != ADI_TMR_SUCCESS)
			printf("Failed to set the timer Delay 0x%08X\n", eTmrResult);

		/* Enable the timer to stop gracefully */
		if ((eTmrResult = adi_tmr_EnableGracefulStop(
			     prvhTimer,
			     true)) != ADI_TMR_SUCCESS)
			printf("Failed to enable the timer to stop gracefully 0x%08X\n", eTmrResult);

		prvfTimerInitInterrupts();
	}
}


