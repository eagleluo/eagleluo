/**
 * @Author: Sheng Zhang <XhxngX2>
 * @Date:   20-Jul-2020
 * @Email:  zhangsheng@brisonus.com
 * @Project: Commom
 * @Last modified by:   Sheng Zhang
 * @Last modified time: 24-Jul-2020
 * @Copyright: Huayan Huisheng (Suzhou) Electronics Technology Co.Ltd,All Rights Reserved.
 */
#include "parser.h"


StrFunParamList_t *xParamItem;




COM_ERROR xInit(struct list_head *phead, str2funParaItemList xMiddle, uint8_t ucParamNum)
{
	for (int i = 0; i < ucParamNum; i++)
	{
		STR_LIST_INIT(xMiddle[i]->parlist);
		list_add_tail((&xMiddle[i]->parlist), phead);
	}

	return COM_NO_ERROR;
}

COM_ERROR xParamProcess(uint8_t ucargc, char **ppcargv, char *cret)
{
	char cTempStr[STR_BUFF_LEN];
	char cTempVal[VAL_BUFF_LEN];
	int i;

	if (PARAM_EQUAL(ppcargv[2], "set", strlen("set")))
	{            											/* If is a setter command	*/
		if (ucargc < 4)
		{
			snprintf(cret, 20, "Parameter invalid.\n");     /* The set value is invalid */
		}
		else
		{
			for (i = 3; i < ucargc; i++)
			{
				*((xParamItem->parvalue) + (i - 3)) = atof(ppcargv[i]);	/* Convert string to floating point */
			}
			memset(cTempVal, 0, VAL_BUFF_LEN);
			strcpy(cTempVal, ppcargv[1]);
			strcat(cTempVal, cTem);
			snprintf(cret, VAL_BUFF_LEN, cTempVal);			/* Combine into string output */
		}
	}
	else if (PARAM_EQUAL(ppcargv[2], "get", strlen("get")))
	{             											/* If is a getter command     */
		memset(cTempStr, 0, STR_BUFF_LEN);
		memset(cTempVal, 0, VAL_BUFF_LEN);
		strcpy(cTempStr, ppcargv[1]);
		strcat(cTempStr, cTemp);
		for (i = 0; i < (xParamItem->parlength); i++)
		{
			snprintf(cTempVal, VAL_BUFF_LEN, "%f ", *((xParamItem->parvalue) + i));	/* Convert floating point number to string */
			strcat(cTempStr, cTempVal);                                             /* Combine into string output */
		}
		snprintf(cret, STR_BUFF_LEN, "%s\n", cTempStr);
	}
	else
	{														/* Not a setter nor a getter       */
		memset(cTempStr, 0, STR_BUFF_LEN);
		memset(cTempVal, 0, VAL_BUFF_LEN);
		snprintf(cret, 20, "Invalid command\n");
	}
	return COM_NO_ERROR;
}


COM_ERROR xParser(struct list_head *phead, uint8_t ucArgc, char **ppcArgv, char *pcret)
{
	list_for_each_entry(xParamItem, phead, StrFunParamList_t, parlist)
	{
		if(strlen(ppcArgv[1]) == strlen(xParamItem->parname))
		{
			if (PARAM_EQUAL(ppcArgv[1], xParamItem->parname, strlen(ppcArgv[1])))      /*if function name matches.*/
			{
				xParamItem->parana(ucArgc, ppcArgv, pcret);
			}
		}
	}
	return COM_NO_ERROR;
}
