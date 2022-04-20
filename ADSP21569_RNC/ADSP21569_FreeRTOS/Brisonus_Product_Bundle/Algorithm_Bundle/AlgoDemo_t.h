/*
 * algoDemoAlgo.h
 *
 *  Created on: Mar 3, 2020
 *      Author: vince
 */

#ifndef BRISONUS_PRODUCT_BUNDLE_ALGORITHM_BUNDLE_ALGODEMO_T_H_
#define BRISONUS_PRODUCT_BUNDLE_ALGORITHM_BUNDLE_ALGODEMO_T_H_


#include "../../src/projConfig.h"
#include "../Commons_Bundle/str2fun/params/RNC/RNC.h"
#include <stdint.h>

void vaDemoAD(float *floatdata, int32_t *intdata);
void vaDemoAlgo_t(float *fdataout, float *fdatain, float pm filter[][REFERENCE_NUM][TAPS_FILTER_T]);
void vaDemoDA(int32_t *intdata, float *floatdata, int32_t *audio);


#endif /* BRISONUS_PRODUCT_BUNDLE_ALGORITHM_BUNDLE_ALGODEMO_T_H_ */
