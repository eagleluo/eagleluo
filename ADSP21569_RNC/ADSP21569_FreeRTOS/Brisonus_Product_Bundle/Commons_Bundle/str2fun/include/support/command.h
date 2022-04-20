/**
 * @Author: Sheng Zhang <XhxngX2>
 * @Date:   20-Jul-2020
 * @Email:  zhangsheng@brisonus.com
 * @Project: Commom
 * @Last modified by:   Sheng Zhang
 * @Last modified time: 24-Jul-2020
 * @Copyright: Huayan Huisheng (Suzhou) Electronics Technology Co.Ltd,All Rights Reserved.
 */

#ifndef _STR2FUN_INC_COMMAND_H_
#define _STR2FUN_INC_COMMAND_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "str2fun_list.h"

typedef struct list_head comList;

typedef enum {
	COM_NO_ERROR,
	COM_SOME_ERROR
}COM_ERROR;

typedef COM_ERROR (*asqParse)(uint8_t, char **, char *);

typedef struct {
	char *			parname;
	comList			parlist;
	float *         parvalue;
	uint8_t			parlength;
	asqParse     	parana;
}StrFunParamList_t;

typedef StrFunParamList_t *str2funParaItemList[];


typedef COM_ERROR (*comParse)(comList *, uint8_t, char **, char *);
typedef COM_ERROR (*comInit)(struct list_head * , str2funParaItemList, uint8_t );


typedef struct {
	char *		name;
	comParse	parse;
	comInit     parainit;
	comList		list;
	uint8_t		number;

	struct list_head *head;
	str2funParaItemList *pparamlist;
}comListItem_t;

#define GLOBAL_COMMAND extern comListItem_t
#define GLOBAL_PARAMCOM extern StrFunParamList_t

typedef comListItem_t *comItemList[];

#define PARAM_EQUAL(x, y,len)     	(!strncmp(x, y,len))

static char cTem[] = {" set successfully\n"};
static char cTemp[] = {" Value:"};

static char __HELP_MSG_PARAMSET[] = "Stasdfasdf\n";
static char __NA_PARAMSET[] = "Not Applicable\n";

#endif
