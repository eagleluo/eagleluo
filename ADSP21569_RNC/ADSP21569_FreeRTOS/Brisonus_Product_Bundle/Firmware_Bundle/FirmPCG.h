#ifndef __FIRMPCG_H__
#define __FIRMPCG_H__

#include <services/pcg/adi_pcg.h>

/* CLK:31.25MHZ
 * FS:48.828kHZ */
#define fpcgACLK_DIV  				(0x4)
#define fpcgAFS_DIV  				(0xa00)

/* CLK:12.5MHZ
 * FS:48.828kHZ */
#define fpcgBCLK_DIV 				(2u)
#define fpcgBFS_DIV  				(512u)


uint32_t   ulFPCGAInit(void);     	/* Initialize PCG */
uint32_t   ulFPCGAClose(void);

void adi_a2b_InitPCGForCodec(void);
void adi_a2b_DeInitPCGForCodec(void);

#endif
