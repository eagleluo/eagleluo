/*
 * AlgorithmFreq.c
 *
 *  Created on: Apr 2, 2020
 *      Author: vince
 */

#include "SoftFreq.h"


void vAFreqTaskFun(xAFreqParams *params)
{
	xAFreqParams *FreqTask = params;

	/*todo:[Tips]Freq task init step 1: Set Algorithm pointers*/
	FreqTask->algorithm = vaDemoAlgo_f;

	/* Frequency algorithm initialize*/
	FreqAlgoInit();

	for ( ;;) {
		xSemaphoreTake(FreqTask->freq_sem, portMAX_DELAY);

		/*[Tips]Freq task runtime step 0: IO indicate Time task begins,LED 9*/

		/*todo:[Tips]Freq task runtime step 1: Algorithm*/
	    (*(FreqTask->algorithm))(FreqTask->freq_filters,
							FreqTask->sig_obuffer.signals,
							FreqTask->sig_obuffer.write_poi);


		/*todo :[Tips]Freq task runtime step 2: Frequency task finished*/
		*(FreqTask->fft_finished) = true;                       /*Set flag*/

		vTaskDelay((TickType_t)5);
	}
}
