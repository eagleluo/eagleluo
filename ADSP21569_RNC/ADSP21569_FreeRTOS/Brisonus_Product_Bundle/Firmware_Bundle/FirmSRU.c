
#include "FirmSRU.h"


void vFSRUInit()
{
	*pREG_PADS0_DAI0_IE = 0x1fffff;
	*pREG_PADS0_DAI1_IE = 0x1fffff;


    SRU2(HIGH,DAI1_PBEN08_I);        /* DAI1_PBEN05 set as output */
    SRU2(HIGH,DAI1_PBEN07_I);        /* DAI1_PBEN04 set as output */
    SRU2(DAI1_PB08_O,SPT4_ACLK_I);   /* DAC clock to SPORT 4A     */
    SRU2(DAI1_PB07_O,SPT4_AFS_I);    /* DAC FS to SPORT 4A        */

    SRU2(SPT4_AD0_O,DAI1_PB09_I);    /* SPORT 4A to DAC           */
    SRU2(HIGH,DAI1_PBEN09_I);        /* DAI1_PBEN01 set as output */

    SRU2(DAI1_PB08_O,SPT4_BCLK_I);   /* DAC clock to SPORT 4A     */
    SRU2(DAI1_PB07_O,SPT4_BFS_I);    /* DAC FS to SPORT 4B        */

	SRU2(DAI1_PB01_O, SPT4_BD0_I);	 /* ADC digital to SPORT 4B   */
	SRU2(LOW, DAI1_PBEN01_I);

	SRU(HIGH,DAI0_PBEN06_I);         /* DAI0_PBEN05 set as output */
	SRU(HIGH,DAI0_PBEN05_I);         /* DAI0_PBEN06 set as output */
    SRU(DAI0_PB06_O, SPT0_ACLK_I);   /* PCGA to SPORT0 CLK (CLK)  */
	SRU(DAI0_PB05_O, SPT0_AFS_I);    /* PCGA to SPORT0 FS (FS)    */

	SRU(DAI0_PB04_O,SPT0_AD0_I);     /* A2B digital to SPORT 0A   */
    SRU(LOW,DAI0_PBEN04_I);

}
