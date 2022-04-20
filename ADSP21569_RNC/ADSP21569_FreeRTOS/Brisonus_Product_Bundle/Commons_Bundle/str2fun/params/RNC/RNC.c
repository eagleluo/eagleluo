#include "RNC.h"

static char cName[] = "RNC";

STR_LIST_HEAD(Str2FunRNCParaHead);

str2funParaItemList str2funRNCList = {
	&RNCStep,
	&RNCLeaky,
	&RNCWeight,
	&RNCGate,
	&RNCLeakyGain1,
	&RNCStepGain1,
	&RNCLeakyGain2,
	&RNCStepGain2,
	&RNCLeakyGain3,
	&RNCStepGain3,
	&RNCWeightGain1,
	&RNCWeightGain2,
	&RNCWeightGain3,
	&RNCSpeakerCheck,
	&RNCSpeakerDisable,
	&RNCReset,
	&RNCSave,
	&RNCPlotNewestTF
};

comListItem_t RNCParams={
	.name = cName,
	.parse = xParser,
	.parainit = xInit,
	.head = &Str2FunRNCParaHead,
	.pparamlist = &str2funRNCList,
	.number = 18
};
