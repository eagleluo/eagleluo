/**
 * @Author: Sheng Zhang <XhxngX2>
 * @Date:   20-Jul-2020
 * @Email:  zhangsheng@brisonus.com
 * @Project: Commom
 * @Last modified by:   Sheng Zhang
 * @Last modified time: 24-Jul-2020
 * @Copyright: Huayan Huisheng (Suzhou) Electronics Technology Co.Ltd,All Rights Reserved.
 */

#include <stdio.h>
#include <string.h>

#include "str2fun.h"
#include "com_list.h"


#if defined(_DEBUG)
#define DEBUG_OUT_ERROR(x)      \
	do {                             \
		printf("Error: %s\tIn File:%s, Line: %d ", x,   \
		       __FILE__, __LINE__);     \
	} while (0)
#endif

#define STR_EQUAL(x, y)  (!strncmp(x, y, strlen(y)))

STR_LIST_HEAD(str2funHead);     /*String to function list head.*/
extern comItemList commandList; /*Externally defined commandList.*/

/*str2fun internal definition.*/
typedef struct {
	comItemList *str2funParaList;
} str2fun;

/**
 * [Run String related setter and getter.]
 * @param  pcStr    [Pointer to command string passed in]
 * @param  ucArgc   [number of arguments]
 * @param  pArgv    [lists which carry arguments]
 * @param  pStr2fun [Handle to Str2Fun APi]
 * @return          [STR2FUN_FUNCxx,Please refer to STR2FUN_ERROR]
 */
static STR2FUN_ERROR prvStrRun(char *pcStr, uint8_t ucArgc, char *pArgv[], str2fun *pStr2fun)
{
	comListItem_t *xItem;

	/*iterate from list head to list tail.*/
	list_for_each_entry(xItem, &str2funHead, comListItem_t, list)
	{
		if (STR_EQUAL(pcStr, xItem->name))      /*if function name matches.*/
			goto found;
	}
	return STR2FUN_FUNC_SOME_ERROR; /*return error if none matches*/

/*if found */
found:
	xItem->parse(xItem->head, ucArgc, pArgv, pcStr);
	return STR2FUN_FUNC_NO_ERROR;
}
/**
 * [init str2fun memory and lists]
 * @param  pHandle        [pointer to handle]
 * @param  pusMemory      [pointer to str2fun memory]
 * @param  pulMemroySize  [pointer to memory size]
 * @return                [STR2FUN_FUNCxx,Please refer to STR2FUN_ERROR]
 */
STR2FUN_ERROR xStr2FunInit(Str2FunHandle *pHandle, uint8_t *pusMemory, uint32_t *pulMemroySize)
{
	str2fun *pStr2fun;
	uint8_t uccomNum, i;

	memset(pusMemory, 0, *pulMemroySize);                       /* Clean memory                       */
	pStr2fun = (str2fun *)pusMemory;                            /* Cast handle to object pointer      */

	uccomNum = sizeof(commandList) / sizeof(comListItem_t *);	/* auto determine the command numbers */
	if (!uccomNum)                                              /* command list is empty              */
		return STR2FUN_DEINIT_SOME_ERROR;

	*pulMemroySize = sizeof(str2fun);			/* pass out the memory size  */
	pStr2fun->str2funParaList = &commandList;   /* pass in the command lists */

	*pHandle = pStr2fun;                        /* pass out the handle		 */

	/*initialize the list.*/
	for (i = 0; i < uccomNum; i++) {
		STR_LIST_INIT((*pStr2fun->str2funParaList)[i]->list);
		(*pStr2fun->str2funParaList)[i]->parainit((*pStr2fun->str2funParaList)[i]->head, (*pStr2fun->str2funParaList)[i]->pparamlist, (*pStr2fun->str2funParaList)[i]->number);
		list_add_tail((&(*pStr2fun->str2funParaList)[i]->list), &str2funHead);
	}

	return STR2FUN_DEINIT_NO_ERROR;
}
/**
 * [Main Function parse string and execute function accordingly]
 * @param  pcStr   [The command string]
 * @param  pHandle [Handle to Str2Fun API instance]
 * @return         [STR2FUN_FUNC_XXXX, Please refer to STR2FUN_ERROR
 * description]
 */
STR2FUN_ERROR xStr2Fun(char *pcStr, Str2FunHandle const pHandle)
{
	char *pcErrmsg;         /* Error message */
	char **pArgv;           /* arg lists     */
	uint32_t ulArgc;        /* arg numbers   */

	STR2FUN_ERROR xError = STR2FUN_FUNC_NO_ERROR;

	if (str2argv(pcStr, &ulArgc, &pArgv, &pcErrmsg) != STR2ARG_NO_ERROR)
		return STR2FUN_FUNC_PARSE_ERROR;

	if (!ulArgc)
		return STR2FUN_FUNC_SOME_ERROR;

	xError = prvStrRun(pcStr, ulArgc, pArgv, (str2fun *)pHandle);
	argv_free(&ulArgc,&pArgv);
	return xError;
}
