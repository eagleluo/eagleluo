#include "FirmUART.h"

#define STDIO_UART_PHYSICAL_DEV_ID                              (0u)

static ADI_STDIO_DEVICE_HANDLE prvhSTDIOJTAG = NULL;
static ADI_STDIO_DEVICE_HANDLE prvhSTDIOUART = NULL;
static ADI_UART_HANDLE prvhPhysicalUART = NULL;


void vFUartInit(xSUartParams *puart)
{
	ADI_STDIO_RESULT eResult = ADI_STDIO_RESULT_SUCCESS;

	/* Initialize the STDIO service */
	eResult = adi_stdio_Init(&prvhSTDIOJTAG);

	/* Register UART device type with STDIO service */
	adi_stdio_RegisterUART();

	/* Open UART Device for STDIO*/
	eResult = adi_stdio_OpenDevice(ADI_STDIO_DEVICE_TYPE_UART,      /* Device Type       */
				       STDIO_UART_PHYSICAL_DEV_ID,      /* Device Number     */
				       &prvhSTDIOUART                      /* Pointer to Handle */
				       );

	/* Set character echo for UART device */
	eResult = adi_stdio_ControlDevice(prvhSTDIOUART,                                   /* Device Handle     */
					  ADI_STDIO_COMMAND_ENABLE_CHAR_ECHO,           /* Command Type      */
					  (void *)true                                  /* Command Value     */
					  );
	eResult = adi_stdio_RedirectStream(prvhSTDIOUART,                                  /* Device Handle     */
					   ADI_STDIO_STREAM_ALL_CONSOLE_IO              /* Stream Type       */
					   );


	/* Get physical device handle for UART */
	eResult = adi_stdio_ControlDevice(prvhSTDIOUART,
					  ADI_STDIO_COMMAND_GET_DEVICE_HANDLE,
					  &prvhPhysicalUART
					  );


	/* Set baud rate for UART device */
	eResult = adi_stdio_ControlDevice(prvhSTDIOUART,                                   /* Device Handle     */
					  ADI_STDIO_COMMAND_SET_UART_BAUD_RATE,         /* Command Type      */
					  (void *)fuartBAUD_RATE                        /* Command Value     */
					  );

	puart->Start_Addr  = 0x190000;
	puart->ParamsNum   = RNCParams.number - 5;
	puart->Buffer_Addr = (int32_t *)(&RNC_Params);
}
