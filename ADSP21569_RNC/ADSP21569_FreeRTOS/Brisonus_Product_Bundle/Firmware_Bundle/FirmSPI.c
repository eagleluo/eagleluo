#include "FirmSPI.h"
#include <services/int/adi_int.h>

unsigned char prvTxBUFF[fspiBUFF_SIZE] ;
unsigned char prvRxBUFF[fspiBUFF_SIZE] ;


KEA128_SPI_DATA *prvData;
KEA128_SPI_DATA prvDataTmp;


static void prvInitSPIRx(void);
static void prvSPIInterrupts(void);
static void prvResubmitBuf(void);
static void SPI1_RxDMA_Handler (uint32_t iid, void *handlerArg);
static unsigned char prvGetCheckSum(const char byte[],const char length);
static KEA128_COM_ERROR prvParseData(void);


static void	prvInitSPIRx(void){

	*pREG_DMA25_CFG			= (ENUM_DMA_CFG_WRITE|ENUM_DMA_CFG_STOP| ENUM_DMA_CFG_STOP |((fspiSPI_SIZE << BITP_DMA_CFG_PSIZE) & BITM_DMA_CFG_PSIZE) | ENUM_DMA_CFG_MSIZE01 | ENUM_DMA_CFG_XCNT_INT);
	*pREG_DMA25_ADDRSTART	= prvRxBUFF+0x28000000;
	*pREG_DMA25_XCNT	  = fspiBUFF_SIZE;
	*pREG_DMA25_XMOD = 1;
	*pREG_DMA25_CFG		   |= ENUM_DMA_CFG_EN ;
}


static 	void prvSPIInterrupts(void){

	adi_int_InstallHandler (INTR_SPI1_RXDMA, SPI1_RxDMA_Handler, 0, true);

}

static void prvResubmitBuf(void){

	*pREG_SPI1_RWC = fspiBUFF_SIZE;
	*pREG_SPI1_TWC = fspiBUFF_SIZE;
	*pREG_DMA25_XCNT = fspiBUFF_SIZE;
	*pREG_DMA25_CFG |= ENUM_DMA_CFG_EN ;

}

static void SPI1_RxDMA_Handler (uint32_t iid, void *handlerArg)
{
	*pREG_DMA25_STAT = ENUM_DMA_STAT_IRQDONE ;
	prvParseData();
	prvResubmitBuf();
}

static unsigned char prvGetCheckSum(const char byte[],const char length)
{

		char i,tCheckSum;
		tCheckSum = 0;
		for(i=0;i<length;i++){
				tCheckSum += byte[i];
		}

		tCheckSum = tCheckSum & 0xff;

		return tCheckSum;
}


static KEA128_COM_ERROR prvParseData(void){

	uint32_t i;

	for(i=0;i<fspiBUFF_SIZE-1;i++){

		if( prvRxBUFF[i]==KEA128_BYTE_START1 && prvRxBUFF[i+1]==KEA128_BYTE_START2 )
		{
			int j;

			for(j=0;j<KEA128_SPI_LEN_OUT;j++){

				prvDataTmp.data[j] = prvRxBUFF[i+j];

			}

			uint8_t ucCheckSum = prvGetCheckSum(prvDataTmp.data,KEA128_SPI_LEN_OUT-1);

			if(ucCheckSum == prvDataTmp.data[KEA128_SPI_LEN_OUT-1]){
				memcpy(prvData,&prvDataTmp,sizeof(KEA128_SPI_DATA));
				return NO_ERROR;
			}else{
				return TRIED_BUT_ERROR;
			}
		}
	}

	return TRIED_BUT_ERROR;

}


void vFSPI_Init(KEA128_SPI_DATA * data){

	prvData = data;

	*pREG_SPI1_DLY	= (	((1 << BITP_SPI_DLY_STOP) & BITM_SPI_DLY_STOP) 	|
						((1 << BITP_SPI_DLY_LEADX)& BITM_SPI_DLY_LEADX) |
						((1 << BITP_SPI_DLY_LAGX) & BITM_SPI_DLY_LAGX));

	*pREG_SPI1_CTL 	= (	((0	<< BITP_SPI_CTL_MSTR) & BITM_SPI_CTL_MSTR)  |
						((fspiCPHA		<< BITP_SPI_CTL_CPHA) & BITM_SPI_CTL_CPHA ) |
						((fspiCPOL		<< BITP_SPI_CTL_CPOL) & BITM_SPI_CTL_CPOL ) |
						((fspiSPI_SIZE	<< BITP_SPI_CTL_SIZE) & BITM_SPI_CTL_SIZE ) |
						((fspiLSBF		<< BITP_SPI_CTL_LSBF) & BITM_SPI_CTL_LSBF ) |
						BITM_SPI_CTL_ASSEL | BITM_SPI_CTL_SELST  );

	*pREG_SPI1_RXCTL = (ENUM_SPI_RXCTL_RWC_EN | ENUM_SPI_RXCTL_RDR_NE );

	*pREG_SPI1_RWC	= fspiBUFF_SIZE;
	*pREG_SPI1_RWCR	= 0x00;

	*pREG_SPI1_SLVSEL = (ENUM_SPI_SLVSEL_SSEL1_EN);

	prvInitSPIRx();
	prvSPIInterrupts();


	*pREG_SPI1_RXCTL |= BITM_SPI_RXCTL_RTI | BITM_SPI_RXCTL_REN;
	*pREG_SPI1_CTL 	 |= ENUM_SPI_CTL_EN;

}
