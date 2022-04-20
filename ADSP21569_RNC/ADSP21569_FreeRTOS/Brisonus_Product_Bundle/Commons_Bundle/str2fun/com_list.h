#ifndef _PARAM_LIST_H_
#define _PARAM_LIST_H_

#include "params/help.h"
#include "params/RNC/RNC.h"


/*todo:[debug interface,UART]:Step1 update command lookup list.*/
comItemList commandList = {
	&help,
	&RNCParams,
};

#endif
