#include "reset.h"
#include "../../../Firmware_Bundle/FirmFlashdriver.h"

char _RESET[] = "Reset set successful\n";

float ResetValue[1];


COM_ERROR xReset(uint8_t ucargc, char ** ppcargv, char* cret)
{
	ResetValue[0] = 1;

	vfParamsProcess(PARAM_READ, &(xAppData.task_uart_params));

	strcpy(cret, _RESET);
	return COM_NO_ERROR;
}
