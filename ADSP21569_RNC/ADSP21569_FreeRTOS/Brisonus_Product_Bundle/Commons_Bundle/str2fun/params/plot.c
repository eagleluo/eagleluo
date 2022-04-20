#include "plot.h"

#include "../../../Algorithm_Bundle/AlgoDemo_f.h"
//#include "../../../Firmware_Bundle/FirmFlashdriver.h"

char _PLOTTF[] = "TF transfer finished\n";



COM_ERROR xPlotTF(uint8_t ucargc, char **ppcargv, char *cret)
{
	uint32_t ulI;

//	vFCommandProcess(24, (float *)(&RNC_TF.fSecPathTransR));

	printf("%d,%d,%d\n", ERROR_NUM, SPEAKER_NUM, TOTAL_FRE_NUM);


	for (ulI = 0; ulI < ERROR_NUM*SPEAKER_NUM*TOTAL_FRE_NUM; ulI++)
	{

			printf("%f\n",RNC_TF.fSecPathTransR[ulI/(SPEAKER_NUM*TOTAL_FRE_NUM)]
											   [ulI%(SPEAKER_NUM*TOTAL_FRE_NUM)/TOTAL_FRE_NUM]
											   [ulI%(SPEAKER_NUM*TOTAL_FRE_NUM)%TOTAL_FRE_NUM]);
	}

	for (ulI = 0; ulI < ERROR_NUM*SPEAKER_NUM*TOTAL_FRE_NUM; ulI++)
	{

			printf("%f\n",RNC_TF.fSecPathTransI[ulI/(SPEAKER_NUM*TOTAL_FRE_NUM)]
											   [ulI%(SPEAKER_NUM*TOTAL_FRE_NUM)/TOTAL_FRE_NUM]
											   [ulI%(SPEAKER_NUM*TOTAL_FRE_NUM)%TOTAL_FRE_NUM]);
	}

	for (ulI = 0; ulI < TOTAL_FRE_NUM; ulI++)
	{
			printf("%f\n",RNC_TF.SpecRadius[ulI]);

	}

	strcpy(cret, _PLOTTF);
	return COM_NO_ERROR;
}
