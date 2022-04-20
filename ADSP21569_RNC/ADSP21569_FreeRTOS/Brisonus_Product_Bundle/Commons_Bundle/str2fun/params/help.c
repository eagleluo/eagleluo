/**
 * @Author: Sheng Zhang <XhxngX2>
 * @Date:   20-Jul-2020
 * @Email:  zhangsheng@brisonus.com
 * @Project: Commom
 * @Last modified by:   Sheng Zhang
 * @Last modified time: 24-Jul-2020
 * @Copyright: Huayan Huisheng (Suzhou) Electronics Technology Co.Ltd,All Rights Reserved.
 */

#include "help.h"

static char cName[] = "help";

static COM_ERROR xHelpInit(struct list_head *phead, str2funParaItemList xMiddle, uint8_t ucParamNum);
static COM_ERROR xHelpParser(struct list_head *phead, uint8_t ucArgc, char **ppcArgv, char *ret);



char __HELP_MSG[] = "Str2Fun debug API: \n"
		"\t\t Change the __PARAM__NAMES__ in the str2fun.c,then use the following command:\n"
		"\t\t\t\t commandList param set|get val[]\n";
char __RET_X[] = "\n";

#define HELP(x)			(strcpy(x, __HELP_MSG))
#define RET(x)			(strcpy(x,__RET_X))
#define IS_HELP(x)		(!strncmp(x,"help",4))

comListItem_t help = {
	.name	= cName,
	.parse	= xHelpParser,
	.parainit = xHelpInit,
};

static COM_ERROR xHelpInit(struct list_head *phead, str2funParaItemList xMiddle, uint8_t ucParamNum)
{
	return COM_NO_ERROR;
}



/**
 * @param  ucArgc   [number of arguments]
 *
 */
static COM_ERROR xHelpParser(struct list_head *phead, uint8_t ucArgc, char **ppcArgv, char *ret)
{

	if(IS_HELP(ppcArgv[0]))
	{
		HELP(ret);
	}
	else
	{
		RET(ret);
	}

	return COM_NO_ERROR;
}
