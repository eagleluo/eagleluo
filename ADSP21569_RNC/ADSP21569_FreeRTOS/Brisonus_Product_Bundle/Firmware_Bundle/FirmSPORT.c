#include <sru.h>
#include <stdio.h>
#include <drivers/sport/adi_sport.h>
#include <services/gpio/adi_gpio.h>

#include "semphr.h"
#include "FirmSPORT.h"

#pragma section("seg_l1_block1")
static int32_t prvTxBuffer1[fsportTX_SAMPLES];
#pragma section("seg_l1_block1")
static int32_t prvTxBuffer2[fsportTX_SAMPLES];

#pragma section("seg_l1_block1")
static int32_t prvRxBuffer1[fsportRX_SAMPLES];
#pragma section("seg_l1_block1")
static int32_t prvRxBuffer2[fsportRX_SAMPLES];

#pragma section("seg_l1_block1")
static int32_t prvA2BBuffer1[fsportA2B_SAMPLES];
#pragma section("seg_l1_block1")
static int32_t prvA2BBuffer2[fsportA2B_SAMPLES];

/* Memory required for SPORT */
static uint8_t prvSPORTMemory4A[ADI_SPORT_MEMORY_SIZE];
static uint8_t prvSPORTMemory4B[ADI_SPORT_MEMORY_SIZE];
static uint8_t prvSPORTMemory0A[ADI_SPORT_MEMORY_SIZE];

/* SPORT Handle */
static ADI_SPORT_HANDLE prvSPORTDev4ATx; /*TX*/
static ADI_SPORT_HANDLE prvSPORTDev4BRx; /*RX*/
static ADI_SPORT_HANDLE prvSPORTDev0ARx; /*RX*/

/* Global SPORT Array Handle */
static ADI_SPORT_HANDLE prvSportTxDai1Array[1] = { NULL };
static ADI_SPORT_HANDLE prvSportRxDai1Array[1] = { NULL };

/*GLobal sport group handle */
static ADI_GLOBAL_SPORT_HANDLE prvDai1Group0Tx;
static ADI_GLOBAL_SPORT_HANDLE prvDai1Group1Rx;

/* Destination SPORT PDMA Lists */
static ADI_PDMA_DESC_LIST prvDescList1SP4A;
static ADI_PDMA_DESC_LIST prvDescList2SP4A;

/* Source SPORT PDMA Lists */
static ADI_PDMA_DESC_LIST prvSrcList1SP4B;
static ADI_PDMA_DESC_LIST prvSrcList2SP4B;

/* Source SPORT PDMA Lists */
static ADI_PDMA_DESC_LIST prvSrcList1SP0A;
static ADI_PDMA_DESC_LIST prvSrcList2SP0A;

static int32_t ** prvADAddrPointer;
static int32_t ** prvDAAddrPointer;
static int32_t ** prvA2BAddrPointer;

static SemaphoreHandle_t prvTimeSem;

static uint8_t prvCallbackCount;

static void prvSportCallback(void *pAppHandle, uint32_t	nEvent, void *pArg)
{
	/* CASEOF (event type) */
	switch (nEvent)
	{

		/* CASE (buffer processed) */
		case ADI_SPORT_EVENT_BUFFER_PROCESSED:
	//	case ADI_SPORT_EVENT_RX_BUFFER_PROCESSED:

			prvCallbackCount++;

			if ( prvCallbackCount== 1)
			{
				(*prvADAddrPointer)  = prvRxBuffer1;
				(*prvDAAddrPointer)  = prvTxBuffer1;
				(*prvA2BAddrPointer) = prvA2BBuffer1;
			}

			if ( prvCallbackCount == 2)
			{
				(*prvADAddrPointer)  = prvRxBuffer2;
				(*prvDAAddrPointer)  = prvTxBuffer2;
				(*prvA2BAddrPointer) = prvA2BBuffer2;
				prvCallbackCount = 0;
			}

			xSemaphoreGiveFromISR(prvTimeSem,NULL);

			break;
		default:
			break;
	}
}


static void prvSportPrepareDescriptors(void)
{
	prvDescList1SP4A.pStartAddr = (int32_t *)prvTxBuffer1;
	prvDescList1SP4A.Config     = ENUM_DMA_CFG_XCNT_INT;
	prvDescList1SP4A.XCount     = fsportTX_SAMPLES;
	prvDescList1SP4A.XModify    = 4;
	prvDescList1SP4A.YCount     = 0;
	prvDescList1SP4A.YModify    = 0;
	prvDescList1SP4A.pNxtDscp   = &prvDescList2SP4A;

	prvDescList2SP4A.pStartAddr = (int32_t *)prvTxBuffer2;
	prvDescList2SP4A.Config     = ENUM_DMA_CFG_XCNT_INT;
	prvDescList2SP4A.XCount     = fsportTX_SAMPLES;
	prvDescList2SP4A.XModify    = 4;
	prvDescList2SP4A.YCount     = 0;
	prvDescList2SP4A.YModify    = 0;
	prvDescList2SP4A.pNxtDscp   = &prvDescList1SP4A;

	prvSrcList1SP4B.pStartAddr = (int32_t *)prvRxBuffer1;
	prvSrcList1SP4B.Config     = ENUM_DMA_CFG_XCNT_INT;
	prvSrcList1SP4B.XCount     = fsportRX_SAMPLES;
	prvSrcList1SP4B.XModify    = 4;
	prvSrcList1SP4B.YCount     = 0;
	prvSrcList1SP4B.YModify    = 0;
	prvSrcList1SP4B.pNxtDscp   = &prvSrcList2SP4B;

	prvSrcList2SP4B.pStartAddr = (int32_t *)prvRxBuffer2;
	prvSrcList2SP4B.Config     = ENUM_DMA_CFG_XCNT_INT;
	prvSrcList2SP4B.XCount     = fsportRX_SAMPLES;
	prvSrcList2SP4B.XModify    = 4;
	prvSrcList2SP4B.YCount     = 0;
	prvSrcList2SP4B.YModify    = 0;
	prvSrcList2SP4B.pNxtDscp   = &prvSrcList1SP4B;


	prvSrcList1SP0A.pStartAddr = (int32_t *)prvA2BBuffer1;
	prvSrcList1SP0A.Config     = ENUM_DMA_CFG_XCNT_INT;
	prvSrcList1SP0A.XCount     = fsportA2B_SAMPLES;
	prvSrcList1SP0A.XModify    = 4;
	prvSrcList1SP0A.YCount     = 0;
	prvSrcList1SP0A.YModify    = 0;
	prvSrcList1SP0A.pNxtDscp   = &prvSrcList2SP0A;

	prvSrcList2SP0A.pStartAddr = (int32_t *)prvA2BBuffer2;
	prvSrcList2SP0A.Config     = ENUM_DMA_CFG_XCNT_INT;
	prvSrcList2SP0A.XCount     = fsportA2B_SAMPLES;
	prvSrcList2SP0A.XModify    = 4;
	prvSrcList2SP0A.YCount     = 0;
	prvSrcList2SP0A.YModify    = 0;
	prvSrcList2SP0A.pNxtDscp   = &prvSrcList1SP0A;
}

/**
 * @Function    ulfSPortInit(void)
 * @brief       SPORT initialization
 * @details     SPORT4B direction : RX <- ADAU1979
 * 				SPORT4A direction : TX -> ADAU1962A
 * 				SPORT0A direction : RX <- A2B Upstream
 *				RX Callback
 * @param[in]
 * @return
 **/
uint32_t ulFSPortInit(int32_t **adaddr,int32_t **daaddr,int32_t **a2baddr,SemaphoreHandle_t semaphore)
{
	uint32_t gpioMaxCallbacks;
	ADI_SPORT_RESULT eResult;

	prvADAddrPointer = adaddr;
	prvDAAddrPointer = daaddr;
	prvA2BAddrPointer = a2baddr;
	prvTimeSem = semaphore;

	adi_pads_Config(FAULTPIN_SEL, true);

	eResult = adi_sport_Open(fsportDEVICE_4A, ADI_HALF_SPORT_A, ADI_SPORT_DIR_TX, ADI_SPORT_MC_MODE, prvSPORTMemory4A, ADI_SPORT_MEMORY_SIZE, &prvSPORTDev4ATx);
	eResult = adi_sport_ConfigData(prvSPORTDev4ATx, ADI_SPORT_DTYPE_SIGN_FILL, 31, false, false, false);
	eResult = adi_sport_ConfigClock(prvSPORTDev4ATx, 32, false, false, false);
	eResult = adi_sport_ConfigFrameSync(prvSPORTDev4ATx, 31, false, false, false, true, false, true);
	eResult = adi_sport_ConfigMC(prvSPORTDev4ATx, 1u, 7u, 0u, false);
	eResult = adi_sport_SelectChannel(prvSPORTDev4ATx, 0u, 7u);

	eResult = adi_sport_Open(fsportDEVICE_4B, ADI_HALF_SPORT_B, ADI_SPORT_DIR_RX, ADI_SPORT_MC_MODE, prvSPORTMemory4B, ADI_SPORT_MEMORY_SIZE, &prvSPORTDev4BRx);
	eResult = adi_sport_ConfigData(prvSPORTDev4BRx, ADI_SPORT_DTYPE_SIGN_FILL, 31, false, false, false);
	eResult = adi_sport_ConfigClock(prvSPORTDev4BRx, 32, false, false, false);
	eResult = adi_sport_ConfigFrameSync(prvSPORTDev4BRx, 31, false, false, false, true, false, true);
	eResult = adi_sport_ConfigMC(prvSPORTDev4BRx, 1u, 7u, 0u, false);
	eResult = adi_sport_SelectChannel(prvSPORTDev4BRx, 0u, 7u);

	prvSportTxDai1Array[0] = prvSPORTDev4ATx;
	prvSportRxDai1Array[0] = prvSPORTDev4BRx;

	prvSportPrepareDescriptors();

	eResult = adi_sport_DMATransfer(prvSPORTDev4BRx, &prvSrcList1SP4B, (fsportDMA_NUM_DESC), ADI_PDMA_DESCRIPTOR_LIST, ADI_SPORT_CHANNEL_PRIM);
	eResult = adi_sport_DMATransfer(prvSPORTDev4ATx, &prvDescList1SP4A, (fsportDMA_NUM_DESC), ADI_PDMA_DESCRIPTOR_LIST, ADI_SPORT_CHANNEL_PRIM);

	eResult = adi_sport_CreateGlobalGroup(prvSportTxDai1Array, 1u, &prvDai1Group0Tx, false, true);
	eResult = adi_sport_CreateGlobalGroup(prvSportRxDai1Array, 1u, &prvDai1Group1Rx, false, true);
	eResult = adi_sport_GlobalRegisterCallback(prvDai1Group1Rx, prvSportCallback, NULL);

	/* Open the SPORT Device 0A*/
	eResult = adi_sport_Open(fsportDEVICE_0A,ADI_HALF_SPORT_A,ADI_SPORT_DIR_RX, ADI_SPORT_MC_MODE, prvSPORTMemory0A,ADI_SPORT_MEMORY_SIZE,&prvSPORTDev0ARx);

	/* Configure the data,clock,frame sync and MCTL of SPORT Device 0A*/
	eResult = adi_sport_ConfigData(prvSPORTDev0ARx,ADI_SPORT_DTYPE_SIGN_FILL,31,false,false,false);

	eResult = adi_sport_ConfigClock(prvSPORTDev0ARx,100,false,false,false);

	eResult = adi_sport_ConfigFrameSync(prvSPORTDev0ARx,0,false,false,false,true,false,true);

	eResult = adi_sport_ConfigMC(prvSPORTDev0ARx,1u,19u,0u,false);

	eResult = adi_sport_SelectChannel(prvSPORTDev0ARx,0u,19u);

	/* Submit the first buffer for Rx.  */
	eResult = adi_sport_DMATransfer(prvSPORTDev0ARx, &prvSrcList1SP0A, (fsportDMA_NUM_DESC), ADI_PDMA_DESCRIPTOR_LIST, ADI_SPORT_CHANNEL_PRIM);

	eResult = adi_sport_GlobalEnable(true);

	/*Enable the Sport Device 0A */
	eResult = adi_sport_Enable(prvSPORTDev0ARx,true);

	return eResult;
}

uint32_t ulFSPortClose(void)
{
	/* SPORT return code */
	ADI_SPORT_RESULT xResult;

	/*Stop the DMA transfer of Tx sport group */
	xResult = adi_sport_GlobalStopDMATransfer(prvDai1Group1Rx);

	/*Stop the DMA transfer of Rx sport group*/
	xResult = adi_sport_GlobalStopDMATransfer(prvDai1Group1Rx);

	/* Destroy TX sport group */
	xResult = adi_sport_DestroyGlobalGroup(prvDai1Group0Tx);

	/* Destroy RX sport group */
	xResult = adi_sport_DestroyGlobalGroup(prvDai1Group1Rx);

	/* DeRegister Global callback for RX group*/
	xResult = adi_sport_GlobalRegisterCallback(prvDai1Group1Rx, NULL, NULL);

	/* Global sport Disable */
	xResult = adi_sport_GlobalEnable(false);

	/*Close Sport Device 4B */
	xResult = adi_sport_Close(prvSPORTDev4BRx);

	/*Close Sport Device 4A */
	xResult = adi_sport_Close(prvSPORTDev4ATx);

	xResult = adi_sport_StopDMATransfer(prvSPORTDev0ARx);

	xResult = adi_sport_Close(prvSPORTDev0ARx);

	return xResult;
}
