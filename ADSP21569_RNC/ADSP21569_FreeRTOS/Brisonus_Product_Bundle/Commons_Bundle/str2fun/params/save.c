#include "save.h"
#include "../../../Firmware_Bundle/FirmFlashdriver.h"

char _SAVE[] = "Save flash successful\n";


COM_ERROR xSave(uint8_t ucargc, char ** ppcargv, char* cret)
{

	vfParamsProcess(PARAM_WRITE, &(xAppData.task_uart_params));
	vFCloseFlashDevice();

	strcpy(cret, _SAVE);
	return COM_NO_ERROR;
}
