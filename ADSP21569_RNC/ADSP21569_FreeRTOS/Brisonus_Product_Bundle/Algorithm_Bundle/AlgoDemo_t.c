/*
 * algoDemoAlgo.c
 *
 *  Created on: Mar 3, 2020
 *      Author: vince
 */

#include "AlgoDemo_t.h"
#include "../../src/projConfig.h"

#include <string.h>
#include <filter.h>
#include <builtins.h>

/*
 * Design filter for input signals at Fs = 48kHz,BandPass
 * */
#define INPUT_IIRSECTION (2u)
static const float  pm INPUT_BIcoeffs[5*INPUT_IIRSECTION] = {
	#include "InputFilter_VectorVersion.txt"
};
static float dm INPUT_IIR_state[REFERENCE_NUM+ERROR_NUM][2*INPUT_IIRSECTION] = {0};


/* Design filter for output signals at Fs = 48kHz,LowPass
 * */
#define OUTPUT_IIRSECTION (2u)
static const float  pm fOUTPUT_BIcoeffs[5*OUTPUT_IIRSECTION] = {
	#include "OutputFilter_VectorVersion.txt"
};
static float dm fOUTPUT_IIR_state[SPEAKER_NUM][2*OUTPUT_IIRSECTION] = {0};




/* Codes for input channel definition
* Channel 0,1,8,9,10,17 are Mics
* others are Accs
* last bits for safety
******/

/*CS75 A2B*/
//uint32_t InputPoi[20]={2,3,4,5,6,7,11,12,13,14,15,16,0,1,8,9,10,17,0,0};
uint32_t InputPoi[A2B_SLOTS]={0,1,2,3,4,5,9,10,11,12,13,14,6,7,8,15,0,0,0,0};

/*CS75*/
int32_t OutputPoi[TX_SLOTS]={0,1,2,3,4,5,6,7};
uint32_t AudioPoi[RX_SLOTS]={0,2,4,1,3,5};

/*XiaoPeng*/
//int32_t OutputPoi[TX_SLOTS]={0,2,4,5,6,1,3,7};
//uint32_t AudioPoi[RX_SLOTS]={0,1,4,6,2,3,5,7};

static float OutputInterp[SPEAKER_NUM];     /*float array for DA interp*/
/*State for big fir*/
static float dm state[SPEAKER_NUM][REFERENCE_NUM][TAPS_FILTER_T + 1] = { 0 };


/*code for debug*/
static uint32_t SimulationNum = 0;
#define VEC_LEN 	 (512u)
static float OutputCheck[SPEAKER_NUM][VEC_LEN]={0};
static float RefCheck[REFERENCE_NUM][VEC_LEN] = {0};
static float ErrCheck[ERROR_NUM][VEC_LEN] = {0};


extern bool speaker_disable;


// todo: Step 1b, Update Time domain algorithms in this file:
void vaDemoAD(float *floatdata, int32_t *intdata)
{
	uint32_t ChLoop, BlockLoop;
	int32_t Orin_Int;
	float Orin_float;
	float fSignalForFilter[BLOCK_SIZE], fSignalFiltered[BLOCK_SIZE];

	for (ChLoop = 0; ChLoop < REFERENCE_NUM + ERROR_NUM; ChLoop++) {


		for (BlockLoop = 0; BlockLoop < BLOCK_SIZE; BlockLoop++) {

			Orin_Int = *(intdata + RX_DATA_SAMPLES * BlockLoop + InputPoi[ChLoop]);
			fSignalForFilter[BlockLoop] = conv_float_by(Orin_Int, -16);

		}

		// Sample rate = 48k
		biquad(fSignalForFilter,fSignalFiltered,
				INPUT_BIcoeffs,
				INPUT_IIR_state[ChLoop],
				BLOCK_SIZE,
				INPUT_IIRSECTION);

		floatdata[ChLoop] = fSignalFiltered[BLOCK_SIZE-1];

	}

    for(ChLoop = 0 ; ChLoop < REFERENCE_NUM ; ChLoop++){
            RefCheck[ChLoop][SimulationNum] = floatdata[ChLoop];
    }


    for(ChLoop = 0 ; ChLoop < ERROR_NUM ; ChLoop++){
            ErrCheck[ChLoop][SimulationNum] = floatdata[ChLoop + REFERENCE_NUM];
    }

//    speaker_disable = __getSwitchVal();
//    speaker_disable = RNC_Params.SpeakerDisableValue[0];

    if(speaker_disable){

    	for( ChLoop = 0 ; ChLoop < REFERENCE_NUM; ChLoop++){
    		floatdata[ChLoop] = 0;
    	}
    }

}

void vaDemoAlgo_t(float *fdataout, float *fdatain, float pm filter[][REFERENCE_NUM][TAPS_FILTER_T])
{
	int RefLoop, SpecLoop;
	float OutTemp;

	for (SpecLoop = 0; SpecLoop < SPEAKER_NUM; SpecLoop++) {

		fdataout[SpecLoop] = 0;

		for (RefLoop = 0; RefLoop < REFERENCE_NUM; RefLoop++) {

			fir(&fdatain[RefLoop],&OutTemp,
				filter[SpecLoop][RefLoop],
				state[SpecLoop][RefLoop],
				1,
				TAPS_FILTER_T);

			fdataout[SpecLoop] += OutTemp;

		}

		/*Code for debug*/
		OutputCheck[SpecLoop][SimulationNum] = fdataout[SpecLoop];
	}


	SimulationNum = (SimulationNum+1)%VEC_LEN;

}

void vaDemoDA(int32_t *intdata, float *floatdata, int32_t *audio)
{

	float StartValue, StopValue, delta;
	float fSignalForFilter[BLOCK_SIZE],fSignalFiltered[BLOCK_SIZE];
	uint32_t OutCh, BLoop;

	for (OutCh = 0; OutCh < SPEAKER_NUM; OutCh++) {

		/*Code for debug*/
//		OutputCheck[OutCh][SimulationNum] = floatdata[OutCh];

		StartValue = OutputInterp[OutCh];

		StopValue = floatdata[OutCh];

		OutputInterp[OutCh] = StopValue;

		delta = (StopValue - StartValue) / BLOCK_SIZE;

		for (BLoop = 0; BLoop < BLOCK_SIZE; BLoop++) {
			fSignalForFilter[BLoop] = StartValue + (BLoop + 1) * delta;
		}

		biquad(fSignalForFilter,fSignalFiltered,
						fOUTPUT_BIcoeffs,
						fOUTPUT_IIR_state[OutCh],
						BLOCK_SIZE,
						OUTPUT_IIRSECTION);


		for (BLoop = 0; BLoop < BLOCK_SIZE; BLoop++) {
			*(intdata + BLoop * TX_SLOTS + OutputPoi[OutCh]) = conv_fix_by(fSignalFiltered[BLoop], 20);
		}

	}


	for(OutCh = 0; OutCh < AUDIO_NUM; OutCh++){
		if(OutCh < AUDIO_NUM/2){

			for (BLoop = 0; BLoop < BLOCK_SIZE; BLoop++){
				*(intdata + BLoop * TX_SLOTS + AudioPoi[OutCh]) += *(audio + 4 + BLoop * RX_SLOTS);

			}
		}
		else{
			for (BLoop = 0; BLoop < BLOCK_SIZE; BLoop++){
				*(intdata + BLoop * TX_SLOTS + AudioPoi[OutCh]) += *(audio + 5 + BLoop * RX_SLOTS);
			}
		}
	}
}
