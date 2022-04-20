#include "FirmPCG.h"


static uint8_t prvPCGAMemory[ADI_PCG_MEMORY_SIZE];                   /* PCG - A */
static ADI_PCG_HANDLE prvhPCGA;


/*
 * Opens and initializes PCG Device.
 *
 * Parameters
 *  None
 *
 * Returns
 *  0 if success, other values for error
 *
 */
uint32_t ulFPCGAInit(void)
{
    uint32_t Result = 0u;

    /* Open PCG */
    if( adi_pcg_Open(0u, &prvPCGAMemory[0], ADI_PCG_MEMORY_SIZE, &prvhPCGA) != ADI_PCG_SUCCESS)
    {
        Result = 1u;
    }

    /* Select FS and clock outputs for PCG */
    if((uint32_t)adi_pcg_SelectOutput(prvhPCGA, ADI_PCG_OUT_CLK_FS) != 0u)
    {
        /* return error */
        return 1u;
    }

    /* Set the PCG input clock source to external*/
    if((uint32_t)adi_pcg_EnableClkPll(prvhPCGA, true) != 0u)
    {
        /* return error */
        return 1u;
    }

    /* Set the PCG input fs to external */
    if((uint32_t)adi_pcg_EnableFsPll(prvhPCGA, true) != 0u)
    {
        /* return error */
        return 1u;
    }

    /* Clock A 36.864 MHz */
    if((uint32_t)adi_pcg_ClockDivide(prvhPCGA, fpcgACLK_DIV) != 0u)
    {
        /* return error */
        return 1u;
    }

    /* FS A 48.828 kHz */
    if((uint32_t)adi_pcg_FrameSyncDivide(prvhPCGA, fpcgAFS_DIV) != 0u)
    {
        /* return error */
        return 1u;
    }

    Result = (uint32_t)adi_pcg_Enable(prvhPCGA, true);

    return Result;
}

uint32_t ulFPCGAClose(void)
{
	uint32_t Result = 0u;

	Result = (uint32_t)adi_pcg_Enable(prvhPCGA, false);
	Result = (uint32_t)adi_pcg_Close(prvhPCGA);

	return Result;
}


/********************************************************************************/
/*!
@brief This function does the PCG initialization for CODEC 1961 BCLK, MCLK and SYNC

@param [in] none

@return     None

Note: Current configuration - 12.288MHz, SYNC -48K

*/
/***********************************************************************************/

void adi_a2b_InitPCGForCodec(void)
{
  	*pREG_PCG0_CTLC1 |= (uint32_t)fpcgBCLK_DIV | (2U<<20U);
	*pREG_PCG0_CTLC0 |= (uint32_t)fpcgBFS_DIV | (BITM_PCG_CTLA0_CLKEN) | (uint32_t)(BITM_PCG_CTLA0_FSEN);

}

/********************************************************************************/
/*!
@brief This function does the PCG initialization for CODEC 1961 BCLK, MCLK and SYNC

@param [in] none

@return     None

Note: Current configuration - 3.072MHz, 12.288MHz, SYNC -48K

*/
/***********************************************************************************/

void adi_a2b_DeInitPCGForCodec(void)
{
	*pREG_PCG0_CTLA0 = 0u;
  	*pREG_PCG0_CTLA1 = 0u;

	*pREG_PCG0_CTLB0 = 0u;
	*pREG_PCG0_CTLB1 = 0u;

}

