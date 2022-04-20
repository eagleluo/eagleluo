#include "RNCSpeakerDisable.h"

StrFunParamList_t RNCSpeakerDisable ={
	.parname = "SpeakerDisable",
	.parvalue = RNC_Params.SpeakerDisableValue,
	.parlength = 1,
	.parana = xParamProcess
};
