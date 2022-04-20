#include <math.h>
#include <filter.h>
#include <string.h>
#include <complex.h>

#include "AlgoDemo_f.h"
#include "../Commons_Bundle/str2fun/params/RNC/RNC.h"


/* Parameter for dual signal fft */

static complex_float dm input[SAMPLES_FFT];
static complex_float dm temp[SAMPLES_FFT];
static complex_float dm output[SAMPLES_FFT];
static complex_float pm twiddle[SAMPLES_FFT/2];

/* Parameter for frequency domain update*/
static float AdapFilterR[SPEAKER_NUM][REFERENCE_NUM][TOTAL_FRE_NUM];
static float AdapFilterI[SPEAKER_NUM][REFERENCE_NUM][TOTAL_FRE_NUM];


/*Parameters for Fredomain Algorithm*/
static float FFTsignals_shadow[REFERENCE_NUM+ERROR_NUM][SAMPLES_FFT];

extern float ResetValue[1];





static float Alpha;        // normalized step
static float Gamma;        // leaky factor
static float Eta;          // error power weight
static float Delta;        // Gate value
static float LeakyGain1;   // Leaky Gain of 1st Fre band
static float StepGain1;    // Step Gain of 1st Fre band
static float WeightGain1;
static float LeakyGain2;   // Leaky Gain of 2nd Fre band
static float StepGain2;    // Step Gain of 2nd Fre band
static float WeightGain2;
static float LeakyGain3;   // Leaky Gain of 3st Fre band
static float StepGain3;    // Step Gain of 3st Fre band
static float WeightGain3;

static uint32_t HalfErrNum;
static uint32_t HalfRefNum;


static float GradR1[SAMPLES_FFT];
static float GradI1[SAMPLES_FFT];
static float GradR2[SAMPLES_FFT];
static float GradI2[SAMPLES_FFT];
static float *GradR[2] = { GradR1, GradR2 };
static float *GradI[2] = { GradI1, GradI2 };


static float REAL[SAMPLES_FFT], IMAG[SAMPLES_FFT];  /* for dual fft and ifft only*/

static float Real1[SAMPLES_FFT];
static float Imag1[SAMPLES_FFT];
static float Real2[SAMPLES_FFT];
static float Imag2[SAMPLES_FFT];
static float *XR[2] = { Real1, Real2 };
static float *XI[2] = { Imag1, Imag2 };

static float TimeSignal1[SAMPLES_FFT];
static float TimeSignal2[SAMPLES_FFT];
static float Temp1[SAMPLES_FFT];
static float Temp2[SAMPLES_FFT];

static float ErrDataR[ERROR_NUM][TOTAL_FRE_NUM];               // Frequency domain error data, REAL part
static float ErrDataI[ERROR_NUM][TOTAL_FRE_NUM];               // Frequency domain error data, imaginary part

static uint32_t SpecLoop, ErrLoop, RefLoop, FreLoop, PairLoop,WinLoop;
static float ErrPow[TOTAL_FRE_NUM];
static float RealS, ImagS;
static float RealF, ImagF;
static float Miu, fRefPow;
static float xPow[TOTAL_FRE_NUM];



/***************************************
 *  imported parameters
 *   */
/*TransFunc*/


#if(USE_FLASH)
	ANC_TransFunc_f RNC_TF = {
			.fSecPathTransR = {0},
			.fSecPathTransI = {0},
			.SpecRadius = {0}
	};
#else
	ANC_TransFunc_f RNC_TF = {
			.fSecPathTransR = {
				#include "TranFuncReal.txt"
			},
			.fSecPathTransI = {
				#include "TranFuncImag.txt"
			},
			.SpecRadius = {
				#include "SpectralRadius.txt"
			}
	};
#endif

///* Spectrum Radius*/
//const static float SpecRadius[TOTAL_FRE_NUM] = {
//	#include "SpectralRadius.txt"
//};
//
///* secondary path, real part*/
//const static float	SecPathTransR[ERROR_NUM][SPEAKER_NUM][TOTAL_FRE_NUM] = {
//	#include "TranFuncReal.txt"
//};
//
///* secondary path, imaginary part*/
//const static float	SecPathTransI[ERROR_NUM][SPEAKER_NUM][TOTAL_FRE_NUM] = {
//	#include "TranFuncImag.txt"
//};

/* Half cosine window for high frequency*/
static float HalfWindow[WINDOW_LEN];

/* ***********************
 * Self defined functions
 * */
static void ControlParametersInit(void );
static void ControlParametersCheck(float (*)[REFERENCE_NUM][TAPS_FILTER_T]);
static void VecTVec(float A[], float B[], uint32_t VLength);
static void FFTDualSignal(float A[], float B[], float Ar[], float Ai[], float Br[], float Bi[]);
static void IFFTDualSignal(float A[], float B[], float Ar[], float Ai[], float Br[], float Bi[]);
static void CopyErrDatato1D(float A[][SAMPLES_FFT], float B[], uint32_t Row, uint32_t pointer);
static void CopyRefDatato1D(float A[][SAMPLES_FFT], float B[], uint32_t Row, uint32_t pointer);
static void CopyData1Dto2D(float A[][TOTAL_FRE_NUM], float B[], uint32_t Row);
static void StoreAdaptiveFilters(float A[][REFERENCE_NUM][TAPS_FILTER_T], float B[],uint32_t Spec, uint32_t Ref);
static void UpdateAdaptiveFilters(float A[][REFERENCE_NUM][TAPS_FILTER_T], float B[], uint32_t Spec, uint32_t Ref, float Step, float Leaky);


/* Frequency domain function initial*/
void FreqAlgoInit(void ){

	uint32_t Remainder;
	Remainder = ERROR_NUM % 2;
	HalfErrNum = (ERROR_NUM + Remainder)/2;

	Remainder = REFERENCE_NUM%2;
	HalfRefNum = (REFERENCE_NUM + Remainder)/2;

	twidfft(twiddle, SAMPLES_FFT);

	/*Parameters initialize*/
	ControlParametersInit();

	/*Initialize half window*/
	int i;
	float N;
	N = (WINDOW_LEN-1)*2;

	for( i=0 ; i<WINDOW_LEN ; i++){

		HalfWindow[i] = 0.5*cosf(2*PI*i/N)+0.5;

	}
}



// todo: Step 1a, Update Frequency domain algorithm here:
void vaDemoAlgo_f(float (*OutputFilters)[REFERENCE_NUM][TAPS_FILTER_T], float (*RingBuffer)[SAMPLES_FFT], uint32_t *poi){


	/*Change Parameters by UART*/
	ControlParametersCheck(OutputFilters);

	/* Store RingBuffer Data*/
	uint32_t Pointer_poi = *poi;

	memcpy(FFTsignals_shadow, RingBuffer, (REFERENCE_NUM+ERROR_NUM) * SAMPLES_FFT * sizeof(float));

	/* Err signals:Time-domain to frequency-domain */
	for (ErrLoop = 0; ErrLoop < HalfErrNum; ErrLoop++) {

		CopyErrDatato1D(FFTsignals_shadow, TimeSignal1, ErrLoop, Pointer_poi);
		if( ErrLoop + HalfErrNum < ERROR_NUM )
			CopyErrDatato1D(FFTsignals_shadow, TimeSignal2, ErrLoop + HalfErrNum, Pointer_poi);

		FFTDualSignal(TimeSignal1, TimeSignal2, Real1, Imag1, Real2, Imag2);

		CopyData1Dto2D(ErrDataR, Real1, ErrLoop);
		CopyData1Dto2D(ErrDataI, Imag1, ErrLoop);

		if( ErrLoop + HalfErrNum < ERROR_NUM ){
			CopyData1Dto2D(ErrDataR, Real2, ErrLoop + HalfErrNum);
			CopyData1Dto2D(ErrDataI, Imag2, ErrLoop + HalfErrNum);
		}

	}

	/* Calculate the power of Error signals*/
	for (FreLoop = START_FRE_NUM; FreLoop < TOTAL_FRE_NUM; FreLoop++) {

		ErrPow[FreLoop] = 0;

		for (ErrLoop = 0; ErrLoop < ERROR_NUM; ErrLoop++) {

			RealS = ErrDataR[ErrLoop][FreLoop];
			ImagS = ErrDataI[ErrLoop][FreLoop];
			ErrPow[FreLoop] += (RealS * RealS + ImagS * ImagS)/ERROR_NUM;

		}
	}

	/* Calculation in frequency domain, reference by reference */
	for (RefLoop = 0; RefLoop < HalfRefNum; RefLoop++) {

		/* Transfer reference from time domain to frequency domain */
		CopyRefDatato1D(FFTsignals_shadow, TimeSignal1, RefLoop, Pointer_poi);
		if(RefLoop + HalfRefNum < REFERENCE_NUM)
			CopyRefDatato1D(FFTsignals_shadow, TimeSignal2, RefLoop + HalfRefNum, Pointer_poi);

		FFTDualSignal(TimeSignal1, TimeSignal2, Real1, Imag1, Real2, Imag2);

		/* Second path filtering in frequency domain */
		for (SpecLoop = 0; SpecLoop < SPEAKER_NUM; SpecLoop++) {


			 /*Filter of the 1st band*/
			for (FreLoop = START_FRE_NUM; FreLoop < TOTAL_FRE_NUM; FreLoop++){

				for (PairLoop = 0; PairLoop < 2; PairLoop++) {

					GradR[PairLoop][FreLoop] = 0;
					GradI[PairLoop][FreLoop] = 0;

					/* Calculate the power of reference */
					fRefPow = XR[PairLoop][FreLoop] * XR[PairLoop][FreLoop] +
									XI[PairLoop][FreLoop] * XI[PairLoop][FreLoop];

					/* Check power */
					xPow[FreLoop] = fRefPow;

					for (ErrLoop = 0; ErrLoop < ERROR_NUM; ErrLoop++) {
						/* Second path filtering in frequency domain */
						RealS = RNC_TF.fSecPathTransR[ErrLoop][SpecLoop][FreLoop];
						ImagS = RNC_TF.fSecPathTransI[ErrLoop][SpecLoop][FreLoop];

						RealF = XR[PairLoop][FreLoop] * RealS - XI[PairLoop][FreLoop] * ImagS;
						ImagF = XI[PairLoop][FreLoop] * RealS + XR[PairLoop][FreLoop] * ImagS;


						/* Calculate normalized step factor */
						Miu = 1 / (fRefPow + WeightGain1 * Eta * ErrPow[FreLoop] + Delta)/(RNC_TF.SpecRadius[FreLoop]+5);  // need to be adjust

						/* Calculate gradient */
						GradR[PairLoop][FreLoop] += Miu * (RealF * ErrDataR[ErrLoop][FreLoop] +
														   ImagF * ErrDataI[ErrLoop][FreLoop]);
						GradI[PairLoop][FreLoop] += Miu * (RealF * ErrDataI[ErrLoop][FreLoop] -
														   ImagF * ErrDataR[ErrLoop][FreLoop]);
					}

//					if(53< FreLoop < 57){
//						/*update in frequency domain*/
//						AdapFilterR[SpecLoop][RefLoop+PairLoop*HalfRefNum][FreLoop] =
//								(1-LeakyGain3*Gamma)*AdapFilterR[SpecLoop][RefLoop+PairLoop*HalfRefNum][FreLoop] - StepGain1*Alpha*GradR[PairLoop][FreLoop];
//						AdapFilterI[SpecLoop][RefLoop+PairLoop*HalfRefNum][FreLoop] =
//								(1-LeakyGain3*Gamma)*AdapFilterI[SpecLoop][RefLoop+PairLoop*HalfRefNum][FreLoop] - StepGain1*Alpha*GradI[PairLoop][FreLoop];
//					}
//					else{
						/*update in frequency domain*/
						AdapFilterR[SpecLoop][RefLoop+PairLoop*HalfRefNum][FreLoop] =
								(1-LeakyGain1*Gamma)*AdapFilterR[SpecLoop][RefLoop+PairLoop*HalfRefNum][FreLoop] - StepGain1*Alpha*GradR[PairLoop][FreLoop];
						AdapFilterI[SpecLoop][RefLoop+PairLoop*HalfRefNum][FreLoop] =
								(1-LeakyGain1*Gamma)*AdapFilterI[SpecLoop][RefLoop+PairLoop*HalfRefNum][FreLoop] - StepGain1*Alpha*GradI[PairLoop][FreLoop];
//					}

				}
			}


			/*Filter of the 2nd band*/
//			for (FreLoop =STOP_FRE_NUM1 ; FreLoop < STOP_FRE_NUM; FreLoop++){
//
//				for (PairLoop = 0; PairLoop < 2; PairLoop++) {
//
//					GradR[PairLoop][FreLoop] = 0;
//					GradI[PairLoop][FreLoop] = 0;
//
//					/* Calculate the power of reference */
//					fRefPow = XR[PairLoop][FreLoop] * XR[PairLoop][FreLoop] +
//					XI[PairLoop][FreLoop] * XI[PairLoop][FreLoop];
//
//					/* Check power */
//					xPow[FreLoop] = fRefPow;
//
//					for (ErrLoop = 0; ErrLoop < ERROR_NUM; ErrLoop++) {
//						/* Second path filtering in frequency domain */
//						RealS = RNC_TF.fSecPathTransR[ErrLoop][SpecLoop][FreLoop];
//						ImagS = RNC_TF.fSecPathTransI[ErrLoop][SpecLoop][FreLoop];
//
//						RealF = XR[PairLoop][FreLoop] * RealS - XI[PairLoop][FreLoop] * ImagS;
//						ImagF = XI[PairLoop][FreLoop] * RealS + XR[PairLoop][FreLoop] * ImagS;
//
//
//						/* Calculate normalized step factor */
//						Miu = 1 / (fRefPow + WeightGain2* Eta * ErrPow[FreLoop] + Delta)/(RNC_TF.SpecRadius[FreLoop]+5);  // need to be adjust
//
//						/* Calculate gradient */
//						GradR[PairLoop][FreLoop] += Miu * (RealF * ErrDataR[ErrLoop][FreLoop] +
//												   ImagF * ErrDataI[ErrLoop][FreLoop]);
//						GradI[PairLoop][FreLoop] += Miu * (RealF * ErrDataI[ErrLoop][FreLoop] -
//												   ImagF * ErrDataR[ErrLoop][FreLoop]);
//					}
//
//					/*update in frequency domain*/
//					AdapFilterR[SpecLoop][RefLoop+PairLoop*HalfRefNum][FreLoop] =
//					(1-LeakyGain2*Gamma)*AdapFilterR[SpecLoop][RefLoop+PairLoop*HalfRefNum][FreLoop] - StepGain2*Alpha*GradR[PairLoop][FreLoop];
//					AdapFilterI[SpecLoop][RefLoop+PairLoop*HalfRefNum][FreLoop] =
//					(1-LeakyGain2*Gamma)*AdapFilterI[SpecLoop][RefLoop+PairLoop*HalfRefNum][FreLoop] - StepGain2*Alpha*GradI[PairLoop][FreLoop];
//
//
//				}
//			}

//
			/*Filter of the 3th band*/
//			for (FreLoop = STOP_FRE_NUM; FreLoop < TOTAL_FRE_NUM; FreLoop++){
//
//				for (PairLoop = 0; PairLoop < 2; PairLoop++) {
//
//					GradR[PairLoop][FreLoop] = 0;
//					GradI[PairLoop][FreLoop] = 0;
//
//					/* Calculate the power of reference */
//					fRefPow = XR[PairLoop][FreLoop] * XR[PairLoop][FreLoop] +
//					XI[PairLoop][FreLoop] * XI[PairLoop][FreLoop];
//
//					/* Check power */
//					xPow[FreLoop] = fRefPow;
//
//					for (ErrLoop = 0; ErrLoop < ERROR_NUM; ErrLoop++) {
//						/* Second path filtering in frequency domain */
//						RealS = RNC_TF.fSecPathTransR[ErrLoop][SpecLoop][FreLoop];
//						ImagS = RNC_TF.fSecPathTransI[ErrLoop][SpecLoop][FreLoop];
//
//						RealF = XR[PairLoop][FreLoop] * RealS - XI[PairLoop][FreLoop] * ImagS;
//						ImagF = XI[PairLoop][FreLoop] * RealS + XR[PairLoop][FreLoop] * ImagS;
//
//
//						/* Calculate normalized step factor */
//						Miu = 1 / (fRefPow + WeightGain3*Eta * ErrPow[FreLoop] + Delta)/(RNC_TF.SpecRadius[FreLoop]+5);  // need to be adjust
//
//						/* Calculate gradient */
//						GradR[PairLoop][FreLoop] += Miu * (RealF * ErrDataR[ErrLoop][FreLoop] +
//												   ImagF * ErrDataI[ErrLoop][FreLoop]);
//						GradI[PairLoop][FreLoop] += Miu * (RealF * ErrDataI[ErrLoop][FreLoop] -
//												   ImagF * ErrDataR[ErrLoop][FreLoop]);
//					}
//
//				/*update in frequency domain*/
//				AdapFilterR[SpecLoop][RefLoop+PairLoop*HalfRefNum][FreLoop] =
//				(1-LeakyGain3*Gamma)*AdapFilterR[SpecLoop][RefLoop+PairLoop*HalfRefNum][FreLoop] - StepGain3*Alpha*GradR[PairLoop][FreLoop];
//				AdapFilterI[SpecLoop][RefLoop+PairLoop*HalfRefNum][FreLoop] =
//				(1-LeakyGain3*Gamma)*AdapFilterI[SpecLoop][RefLoop+PairLoop*HalfRefNum][FreLoop] - StepGain3*Alpha*GradI[PairLoop][FreLoop];
//
//				}
//			}


//			/* Input windowed added */
//			for (FreLoop = START_FRE_NUM; FreLoop < START_FRE_NUM + WINDOW_LEN; FreLoop++)
//			{
//
//				WinLoop = WINDOW_LEN - (FreLoop - START_FRE_NUM) - 1;
//
//				for (PairLoop = 0; PairLoop < 2; PairLoop++)
//				{
//
//					/*Windowed in frequency domain*/
//					AdapFilterR[SpecLoop][RefLoop + PairLoop * HalfRefNum][FreLoop] =
//						HalfWindow[WinLoop] * AdapFilterR[SpecLoop][RefLoop + PairLoop * HalfRefNum][FreLoop];
//					AdapFilterI[SpecLoop][RefLoop + PairLoop * HalfRefNum][FreLoop] =
//						HalfWindow[WinLoop] * AdapFilterI[SpecLoop][RefLoop + PairLoop * HalfRefNum][FreLoop];
//				}
//			}
//
//			/* Output windowed added */
//			for (FreLoop = STOP_FRE_NUM - WINDOW_LEN ; FreLoop < STOP_FRE_NUM; FreLoop++){
//
//				WinLoop = FreLoop - (STOP_FRE_NUM - WINDOW_LEN);
//
//				for (PairLoop = 0; PairLoop < 2; PairLoop++) {
//
//					/*Windowed in frequency domain*/
//					AdapFilterR[SpecLoop][RefLoop+PairLoop*HalfRefNum][FreLoop] =
//							HalfWindow[WinLoop]*AdapFilterR[SpecLoop][RefLoop+PairLoop*HalfRefNum][FreLoop];
//					AdapFilterI[SpecLoop][RefLoop+PairLoop*HalfRefNum][FreLoop] =
//							HalfWindow[WinLoop]*AdapFilterI[SpecLoop][RefLoop+PairLoop*HalfRefNum][FreLoop];
//
//
//				}
//			}


			/*Construct frequency domain vectors, Grad is not really gradient*/
			for (FreLoop = START_FRE_NUM; FreLoop < TOTAL_FRE_NUM; FreLoop++){

				for (PairLoop = 0; PairLoop < 2; PairLoop++) {

					GradR[PairLoop][FreLoop] = AdapFilterR[SpecLoop][RefLoop+PairLoop*HalfRefNum][FreLoop];
					GradI[PairLoop][FreLoop] = AdapFilterI[SpecLoop][RefLoop+PairLoop*HalfRefNum][FreLoop];

					/* Mirorr spectrum construction*/
					GradR[PairLoop][SAMPLES_FFT - FreLoop] = GradR[PairLoop][FreLoop];
					GradI[PairLoop][SAMPLES_FFT - FreLoop] = -GradI[PairLoop][FreLoop];

				}
			}

			/* Transfer back to time domain */
			IFFTDualSignal(Temp1, Temp2,
					   GradR[0], GradI[0],
					   GradR[1], GradI[1]);

			/* Copy adaptive filters in reverse order */
			StoreAdaptiveFilters(OutputFilters, Temp1, SpecLoop, RefLoop);
			if(RefLoop + HalfRefNum < REFERENCE_NUM)
				StoreAdaptiveFilters(OutputFilters, Temp2, SpecLoop, RefLoop + HalfRefNum);

		}
	}
}


/*
* copy Error data from a 2D vector to a 1D vector
* circular buffer should be considered
******************************************************************************/
static void CopyErrDatato1D(float A[][SAMPLES_FFT], float B[], uint32_t Row, uint32_t pointer)
{
	memset(B, 0, SAMPLES_FFT/2*sizeof(float));

	/* formmer half of the vector should be ZERO */
	uint32_t i;
	for (i = SAMPLES_FFT / 2; i < SAMPLES_FFT; i++) {
		if (pointer + i < SAMPLES_FFT)
			B[i] = A[Row + REFERENCE_NUM][pointer + i];
		else
			B[i] = A[Row + REFERENCE_NUM][pointer + i - SAMPLES_FFT];
	}
}


/*
* copy reference data from a 2D vector to a 1D vector
* circular buffer should be considered
******************************************************************************/
static void CopyRefDatato1D(float A[][SAMPLES_FFT], float B[], uint32_t Row, uint32_t pointer)
{
	uint32_t i;

	for (i = 0; i < SAMPLES_FFT; i++) {
		if (pointer + i < SAMPLES_FFT)
			B[i] = A[Row][pointer + i];
		else
			B[i] = A[Row][pointer + i - SAMPLES_FFT];

	}
}

//*****************************************************************************
// copy data from a 1D vector to a 2D vector
//*****************************************************************************
static void CopyData1Dto2D(float A[][TOTAL_FRE_NUM], float B[], uint32_t Row)
{

//	memcpy(A[Row],B,TOTAL_FRE_NUM*sizeof(float));

	uint32_t i;
	for (i = 0; i < TOTAL_FRE_NUM; i++)
		A[Row][i] = B[i];

}


//*****************************************************************************
//  * Copy vector B to A
//*****************************************************************************
static void VecTVec(float A[], float B[], uint32_t VLength)
{
	uint32_t i;

	for (i = 0; i < VLength; i++)
		A[i] = B[i];

}


//*****************************************************************************
// Store adaptive filters in a reverse order
//*****************************************************************************
static void StoreAdaptiveFilters(float A[][REFERENCE_NUM][TAPS_FILTER_T], float B[],
				  uint32_t Spec, uint32_t Ref)
{
	uint32_t i;

	for (i = 0; i < TAPS_FILTER_T; i++)
		A[Spec][Ref][TAPS_FILTER_T - 1 - i] = B[i];


}




//*****************************************************************************
// update adaptive filters in a reverse order
//*****************************************************************************
static void UpdateAdaptiveFilters(float A[][REFERENCE_NUM][TAPS_FILTER_T], float B[],
				  uint32_t Spec, uint32_t Ref, float Step, float Leaky)
{
	uint32_t i;

	for (i = 0; i < TAPS_FILTER_T; i++)
		A[Spec][Ref][TAPS_FILTER_T - 1 - i] = (1 - Leaky) * A[Spec][Ref][TAPS_FILTER_T - 1 - i] - Step * B[i];


}


/**********************************
 * Name: FFTDualSignal
 * Function: Transform two time sigal to frequency domain uses just one FFT
 * The output magnitude is half normalized (0.5 * origin)
 *************************************/
static void FFTDualSignal(float A[], float B[], float Ar[], float Ai[], float Br[], float Bi[])
{	

	uint32_t i;
	for(i = 0 ; i<SAMPLES_FFT; i++){
		input[i].re = A[i];
		input[i].im = B[i];
	}

	cfft(input, temp, output, twiddle, 1, SAMPLES_FFT);

	Ar[0] = output[0].re;
	Ai[0] = 0;
	Br[0] = output[0].im;
	Bi[0] = 0;

	for (i = 0; i < SAMPLES_FFT / 2; i++) {
		// Real(A)
		Ar[i + 1] = 0.5 * (output[i + 1].re + output[SAMPLES_FFT - 1 - i].re);
		Ar[i + SAMPLES_FFT / 2] = 0.5 * (output[SAMPLES_FFT / 2 + i].re + output[SAMPLES_FFT / 2 - i].re);
		// Imag(A)
		Ai[i + 1] = 0.5 * (output[i + 1].im - output[SAMPLES_FFT - 1 - i].im);
		Ai[i + SAMPLES_FFT / 2] = 0.5 * (output[SAMPLES_FFT / 2 + i].im - output[SAMPLES_FFT / 2 - i].im);

		// Real(B)
		Br[i + 1] = 0.5 * (output[i + 1].im + output[SAMPLES_FFT - 1 - i].im);
		Br[i + SAMPLES_FFT / 2] = 0.5 * (output[SAMPLES_FFT / 2 + i].im + output[SAMPLES_FFT / 2 - i].im);
		// Imag(B)
		Bi[i + 1] = -0.5 * (output[i + 1].re - output[SAMPLES_FFT - 1 - i].re);
		Bi[i + SAMPLES_FFT / 2] = -0.5 * (output[SAMPLES_FFT / 2 + i].re - output[SAMPLES_FFT / 2 - i].re);
	}
}

/**********************************
 * Name: iFFTDualSignal
 * Function: Transform two complex signal to time domain uses just one IFFT
 * Use half normalized magnitude in frequency, the out put gets the oringinal magnitude
 *************************************/
static void IFFTDualSignal(float A[], float B[], float Ar[], float Ai[], float Br[], float Bi[])
{
	uint32_t i;

	for (i = 0; i < SAMPLES_FFT; i++) {
		input[i].re = Ar[i] - Bi[i];
		input[i].im = Ai[i] + Br[i];
	}

	ifft(input, temp, output, twiddle, 1, SAMPLES_FFT);

	for(i = 0; i < SAMPLES_FFT; i++){
		A[i] = output[i].re;
        B[i] = output[i].im;
	}

}


/*****************************************************************************
*Control parameters initialization
*****************************************************************************/
static void ControlParametersInit(void)
{

	/* Control parameters in frequency.c */
	/*global control*/
	Alpha = 0.0001;        // normalized step, Step->param1,
	Gamma = 1e-5;       // leaky factor, Leaky -> param2
	Eta = 1;        // error power weight, Weight -> param3
	Delta = 100;         // Gate value, Gate -> param4

	StepGain1 = 1;
	LeakyGain1 = 1;
	WeightGain1 = 1;

	StepGain2= 1;
	LeakyGain2 = 1;
	WeightGain2 = 1;


	StepGain3 = 1;
	LeakyGain3 = 1 ;
	WeightGain3 = 1;



	/* Control parameters from UART*/
//	RNC_Params.StepValue[0] = 0;
//	RNC_Params.LeakyValue[0] = 0;
//	RNC_Params.WeightValue[0] = 0;
//	RNC_Params.GateValue[0] = 0;
	ResetValue[0] = 0;

//	RNC_Params.LeakyGain1Value[0] = 0;
//	RNC_Params.StepGain1Value[0] = 0;
//	RNC_Params.WeightGain1Value[0] = 0;
//	RNC_Params.LeakyGain2Value[0] = 0;
//	RNC_Params.StepGain2Value[0] = 0;
//	RNC_Params.WeightGain2Value[0] = 0;
//	RNC_Params.LeakyGain3Value[0] = 0;
//	RNC_Params.StepGain3Value[0] = 0;
//	RNC_Params.WeightGain3Value[0] = 0;
}




/*****************************************************************************
* Control parameters Check
*****************************************************************************/
static void ControlParametersCheck(float (*OutputFilters)[REFERENCE_NUM][TAPS_FILTER_T])
{

	if (ResetValue[0] == 1){

		ControlParametersInit();

	    memset(AdapFilterR, 0, SPEAKER_NUM*REFERENCE_NUM*TOTAL_FRE_NUM*sizeof(float));
	    memset(AdapFilterI, 0, SPEAKER_NUM*REFERENCE_NUM*TOTAL_FRE_NUM*sizeof(float));
	}

	if(RNC_Params.StepValue[0] != 0)
		Alpha = RNC_Params.StepValue[0];            // normalized step

	if(RNC_Params.LeakyValue[0] != 0)
		Gamma = RNC_Params.LeakyValue[0];            // leaky factor

	if(RNC_Params.WeightValue[0] != 0)
		Eta = RNC_Params.WeightValue[0];             // error power weight

	if(RNC_Params.GateValue[0] != 0)
		Delta = RNC_Params.GateValue[0];             // Gate value


	if(RNC_Params.LeakyGain1Value[0] != 0)
		LeakyGain1 = RNC_Params.LeakyGain1Value[0];   // Leak Gian 1
	if(RNC_Params.StepGain1Value[0] != 0)
		StepGain1 = RNC_Params.StepGain1Value[0];     // Step Gian 1
	if (RNC_Params.WeightGain1Value[0] != 0)
		WeightGain1 = RNC_Params.WeightGain1Value[0];

	if(RNC_Params.LeakyGain2Value[0] != 0)
		LeakyGain2 = RNC_Params.LeakyGain2Value[0];   // Leak Gian 2
	if(RNC_Params.StepGain2Value[0] != 0)
		StepGain2 = RNC_Params.StepGain2Value[0];     // Step Gian 2
	if (RNC_Params.WeightGain2Value[0] != 0)
		WeightGain2 = RNC_Params.WeightGain2Value[0]; // Step Gian 2

	if(RNC_Params.LeakyGain3Value[0] != 0)
		LeakyGain3 = RNC_Params.LeakyGain3Value[0];   // Leak Gian 3
	if(RNC_Params.StepGain3Value[0] != 0)
		StepGain3 = RNC_Params.StepGain3Value[0];     // Step Gian 3
	if (RNC_Params.WeightGain3Value[0] != 0)
		WeightGain3 = RNC_Params.WeightGain3Value[0]; // Step Gian 2

}

