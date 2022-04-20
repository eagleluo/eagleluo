# ifndef _RNCINCLUDE_H_
# define _RNCINCLUDE_H_

typedef struct{
    float StepValue[1];
    float LeakyValue[1];
    float WeightValue[1];
    float GateValue[1];
    float LeakyGain1Value[1];
    float StepGain1Value[1];
    float LeakyGain2Value[1];
    float StepGain2Value[1];
    float LeakyGain3Value[1];
    float StepGain3Value[1];
    float WeightGain1Value[1];
    float WeightGain2Value[1];
    float WeightGain3Value[1];
    float SpeakerCheckValue[1];
    float SpeakerDisableValue[1];
}RNC_UartParam_f;

RNC_UartParam_f RNC_Params;


#endif

