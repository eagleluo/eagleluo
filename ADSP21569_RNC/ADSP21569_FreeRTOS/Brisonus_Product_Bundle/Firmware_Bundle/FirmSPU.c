
#include "FirmSPU.h"


/* SPU handle */
static ADI_SPU_HANDLE prvhSPU;
/* Memory required for the SPU operation */
static uint8_t prvpSpuMemory[ADI_SPU_MEMORY_SIZE];

uint32_t ulFSPUInit()
{
	if (adi_spu_Init(0, prvpSpuMemory, NULL, NULL, &prvhSPU) != ADI_SPU_SUCCESS) {
		printf("Failed to initialize SPU service\n");
		return fspuERROR;
	}

	/* Make SPORT 4A to generate secure transactions */
	if(adi_spu_EnableMasterSecure(prvhSPU, fspuSPORT_4A_SPU, true) != ADI_SPU_SUCCESS)
	{
		printf("Failed to enable Master secure for SPORT4A\n");
		return fspuERROR;
	}


	/* Make SPORT 0A to generate secure transactions */
	if (adi_spu_EnableMasterSecure(prvhSPU, fspuSPORT_4A_DMA_SPU, true) != ADI_SPU_SUCCESS) {
		printf("Failed to enable Master secure for SPORT4A\n");
		return fspuERROR;
	}

	/* Make SPORT 0A to generate secure transactions */
	if(adi_spu_EnableMasterSecure(prvhSPU, fspuSPORT_4B_SPU, true) != ADI_SPU_SUCCESS)
	{
		printf("Failed to enable Master secure for SPORT4B\n");
		return fspuERROR;
	}

	/* Make SPORT 0B to generate secure transactions */
	if (adi_spu_EnableMasterSecure(prvhSPU, fspuSPORT_4B_DMA_SPU, true) != ADI_SPU_SUCCESS) {
		printf("Failed to enable Master secure for SPORT4B\n");
		return fspuERROR;
	}

	/* Make SPORT 0A to generate secure transactions */
	if(adi_spu_EnableMasterSecure(prvhSPU, fspuSPORT_0A_SPU, true) != ADI_SPU_SUCCESS)
	{
		printf("Failed to enable Master secure for SPORT0A\n");
		return fspuERROR;
	}

	/* Make SPORT 0A to generate secure transactions */
	if (adi_spu_EnableMasterSecure(prvhSPU, fspuSPORT_0A_DMA_SPU, true) != ADI_SPU_SUCCESS) {
		printf("Failed to enable Master secure for SPORT0A\n");
		return fspuERROR;
	}

	return fspuSUCCESS;
}
