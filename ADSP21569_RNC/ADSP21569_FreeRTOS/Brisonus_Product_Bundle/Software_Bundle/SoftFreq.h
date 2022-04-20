/*
 * AlgorithmFreq.h
 *
 *  Created on: Apr 2, 2020
 *      Author: vince
 */

#ifndef _ALGORITHMFREQ_H_
#define _ALGORITHMFREQ_H_

#include <stdio.h>
#include <string.h>

#include "../Algorithm_Bundle/AlgoDemo_f.h"
#include "../../src/projConfig.h"

#include "SoftConfigs.h"


#define softfFFT_SIZE           SAMPLES_FFT                  /* FFT signal size global	*/
#define softfBLOCKSIZE          SAMPLES_PER_FFTUPDATE        /* FFT fresh samples global*/
#define softfTaskName           "Frequency domain algorithm"
#define softfFILTER_TAPS        TAPS_FILTER_T                /* Filter taps global		*/
#define softfStackSize          (2000u)
#define softfPriority           (conPRI_ALGO_BASE + 0u)

typedef void  (*algo_freq)(float (*)[REFERENCE_NUM][TAPS_FILTER_T], float (*)[SAMPLES_FFT], uint32_t *);
typedef float (*TimeSigalArray)[SAMPLES_FFT];
typedef float (*FreqFilterArray)[REFERENCE_NUM][TAPS_FILTER_T];


typedef struct {
	TimeSigalArray  signals;

	uint32_t*  		write_poi;
}xAFreqRingBuffer;

typedef struct {
	SemaphoreHandle_t	freq_sem;

	xAFreqRingBuffer	sig_obuffer;

	FreqFilterArray  	freq_filters;

	bool *				fft_finished;

	algo_freq			algorithm;
}xAFreqParams;

void vAFreqTaskFun(xAFreqParams *params);

#endif /* BRISONUS_PRODUCT_BUNDLE_SOFTWARE_BUNDLE_ALGORITHMFREQ_H_ */
