# ifndef _RNC_H_
# define _RNC_H_

#include "Ge_Setters/RNCStep.h"
#include "Ge_Setters/RNCLeaky.h"
#include "Ge_Setters/RNCWeight.h"
#include "Ge_Setters/RNCGate.h"
#include "Ge_Setters/RNCLeakyGain1.h"
#include "Ge_Setters/RNCStepGain1.h"
#include "Ge_Setters/RNCLeakyGain2.h"
#include "Ge_Setters/RNCStepGain2.h"
#include "Ge_Setters/RNCLeakyGain3.h"
#include "Ge_Setters/RNCStepGain3.h"
#include "Ge_Setters/RNCWeightGain1.h"
#include "Ge_Setters/RNCWeightGain2.h"
#include "Ge_Setters/RNCWeightGain3.h"
#include "Ge_Setters/RNCSpeakerCheck.h"
#include "Ge_Setters/RNCSpeakerDisable.h"
#include "Actions/RNCReset.h"
#include "Actions/RNCSave.h"
#include "Actions/RNCPlotTF.h"

#include "../parser.h"

GLOBAL_COMMAND RNCParams;

//extern RNC_UartParam_f RNC_Params;

#endif
