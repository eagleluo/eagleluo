/*
 * projConfig.h
 *
 *  Created on: Mar 3, 2020
 *      Author: vince
 */

#ifndef SRC_PROJCONFIG_H_
#define SRC_PROJCONFIG_H_

#define BLOCK_SIZE                      (24u)
#define A2B_SLOTS                       (20u)
#define RX_SLOTS                        (8u)
#define TX_SLOTS                        (8u)

#define SAMPLES_PER_FFTUPDATE           (70u)
#define SAMPLES_FFT                     (512u)
#define TAPS_FILTER_T                   (256u)
#define REFERENCE_NUM           		(12u)
#define SPEAKER_NUM                     (6u)
#define AUDIO_NUM						(6u)
#define ERROR_NUM                       (4u)
#define STOP_FRE_NUM                    (80u)
#define START_FRE_NUM                   (12u)
#define TOTAL_FRE_NUM                   (80u)   //same as TF
#define WINDOW_LEN                      (7u)     // should be odd number
#define PI                              (3.1415926f)
/**
 * todo:[Add]AUDIO_DATA_SAMPLES: Vehicle audio signal channel
 */
#define AUDIO_DATA_SAMPLES      		RX_SLOTS

/**
 * todo:[Flash enable]Use flash flag
 * 		USE_FLASH flag set 1 : use flash
 * 		USE_FLASH flag set 0 : do not use flash
 */
#define USE_FLASH                       (1u)

/**
 * todo:[Unchange]RX_DATA_SAMPLES: A2B signal channel
 */
#define RX_DATA_SAMPLES         		A2B_SLOTS

#define TX_DATA_SAMPLES         		TX_SLOTS

#define RX_DATA_BYTE            		(BLOCK_SIZE * RX_SLOTS * 4)
#define TX_DATA_BYTE            		(BLOCK_SIZE * TX_SLOTS * 4)

#endif /* SRC_PROJCONFIG_H_ */
