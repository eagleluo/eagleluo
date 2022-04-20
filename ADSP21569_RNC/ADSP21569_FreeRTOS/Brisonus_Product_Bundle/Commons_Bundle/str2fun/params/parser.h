#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdlib.h>
#include <string.h>
#include "../include/support/command.h"
#include "../include/support/str2fun_list.h"

#define STR_BUFF_LEN			(60u)
#define VAL_BUFF_LEN			(40u)


COM_ERROR xInit(struct list_head *phead, str2funParaItemList xMiddle, uint8_t ucParamNum);
COM_ERROR xParamProcess(uint8_t ucargc, char **ppcargv, char *cret);
COM_ERROR xParser(struct list_head *phead, uint8_t ucArgc, char **ppcArgv, char *ret);

#endif
