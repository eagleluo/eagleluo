/***
 *
 */

/*****************************************************************************
 * FirmFlashdriver.h
 *****************************************************************************/

#ifndef __FIRMFLASHDRIVER_H__
#define __FIRMFLASHDRIVER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <drivers/spi/adi_spi.h>
#include <services/gpio/adi_gpio.h>

#include "../../src/projConfig.h"
#include "../Software_Bundle/SoftDefines.h"


/* enum of possible errors */
typedef enum
{
    NO_ERR,                 /* No Error */
    POLL_TIMEOUT,           /* Polling toggle bit failed */
    VERIFY_WRITE,           /* Verifying write to flash failed */
    INVALID_SECTOR,         /* Invalid Sector */
    INVALID_BLOCK,          /* Invalid Block */
    UNKNOWN_COMMAND,        /* Unknown Command */
    PROCESS_COMMAND_ERR,    /* Processing command */
    NOT_READ_ERROR,         /* Could not read memory from target */
    DRV_NOTAT_BREAK,        /* The application was not at AFP_BreakReady */
    BUFFER_IS_NULL,         /* Could not allocate storage for the buffer */
    NO_ACCESS_SECTOR,       /* Cannot access the sector( could be locked or something is stored there that should not be touched ) */
    WRITE_ERROR,            /* Error writing to memory */
    NO_MODE_SUPPORT,        /* Not a supported operating mode */
    SETUP_ERROR,            /* Error in setup */
    NUM_ERROR_CODES
}ERROR_CODE;


/* Flash programmer commands */
typedef enum
{
    FLASH_NO_COMMAND,       /* 0 */
    FLASH_GET_CODES,        /* 1 */
    FLASH_RESET,            /* 2 */
    FLASH_WRITE,            /* 3 */
    FLASH_FILL,             /* 4 */
    FLASH_ERASE_ALL,        /* 5 */
    FLASH_ERASE_SECT,       /* 6 */
    FLASH_READ,             /* 7 */
    FLASH_GET_SECTNUM,      /* 8 */
    FLASH_GET_SECSTARTEND,  /* 9 */
}enProgCmds;


/* UART parameters commands */
typedef enum
{
	PARAM_NO_COMMAND,		/* 0 */
	PARAM_WRITE,			/* 1 */
	PARAM_READ,				/* 2 */
}ParamCmds;


/* structure for flash sector information */
typedef struct _SECTORLOCATION{
    unsigned long ulStartOff;
    unsigned long ulEndOff;
}SECTORLOCATION;

extern enProgCmds AFP_Command;


uint32_t ulFlashDriverInit(void);
void vFCommandProcess(int sectornum, float *buffer);
void vfParamsProcess(ParamCmds paramcomd, xSUartParams *pUartParams);
void vFCloseFlashDevice(void);


#endif /* __FIRMFLASHDRIVER_H__ */
