/*
 * AlgoDemo_f.h
 *
 *  Created on: Apr 18, 2020
 *      Author: vince
 */

#ifndef BRISONUS_PRODUCT_BUNDLE_ALGORITHM_BUNDLE_ALGODEMO_F_H_
#define BRISONUS_PRODUCT_BUNDLE_ALGORITHM_BUNDLE_ALGODEMO_F_H_

#include <stdint.h>
#include "../../src/projConfig.h"


typedef struct{
	float fSecPathTransR[ERROR_NUM][SPEAKER_NUM][TOTAL_FRE_NUM];
	float fSecPathTransI[ERROR_NUM][SPEAKER_NUM][TOTAL_FRE_NUM];
	float SpecRadius[TOTAL_FRE_NUM];
}ANC_TransFunc_f;


extern ANC_TransFunc_f RNC_TF;

void FreqAlgoInit(void );
void vaDemoAlgo_f(float (*OutputFilters)[REFERENCE_NUM][TAPS_FILTER_T], float (*RingBuffer)[SAMPLES_FFT], uint32_t *poi);

#endif /* BRISONUS_PRODUCT_BUNDLE_ALGORITHM_BUNDLE_ALGODEMO_F_H_ */
