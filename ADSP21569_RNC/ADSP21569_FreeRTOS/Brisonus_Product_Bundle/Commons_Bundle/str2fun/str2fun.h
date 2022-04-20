/**
 * @Author: Sheng Zhang <XhxngX2>
 * @Date:   20-Jul-2020
 * @Email:  zhangsheng@brisonus.com
 * @Project: Commom
 * @Last modified by:   XhxngX2
 * @Last modified time: 20-Jul-2020
 * @Copyright: Huayan Huisheng (Suzhou) Electronics Technology Co.Ltd,All Rights Reserved.
 */

#ifndef _STR2FUN_H_
#define _STR2FUN_H_


#include "params/RNC/RNC.h"
#include "include/argparser/str2argv.h"


#define str2funMEM_SIZE        (16u)

typedef enum {
	STR2FUN_INIT_NO_ERROR	= 0x10001,
	STR2FUN_INIT_INVALID_MEMORY,
	STR2FUN_INIT_MEM_TOO_SMALL,

	STR2FUN_FUNC_NO_ERROR	= 0x20001,
	STR2FUN_FUNC_SOME_ERROR,
	STR2FUN_FUNC_PARSE_ERROR,

	STR2FUN_DEINIT_NO_ERROR = 0x30001,
	STR2FUN_DEINIT_SOME_ERROR
}STR2FUN_ERROR;

/*Str2Fun Handle*/
typedef void* Str2FunHandle;


/**
 * [Initialze the Str2Fun API isntance]
 * @param  ppHandle        [Pointer to Str2Fun API Memory]
 * @param  pusMemory   	  [Pointer to memory]
 * @param  pulMemroySize  [Addr memory size in Byte,should greater to STR2FUN_MEM_SIZE]
 * @return                [STR2FUN_INIT_XXXX, Please refer to STR2FUN_ERROR description]
 */
STR2FUN_ERROR xStr2FunInit(Str2FunHandle *ppHandle, uint8_t *pusMemory, uint32_t *pulMemroySize);
/**
 * [Main Function parse string and execute function accordingly]
 * @param  pcStr     [The command string]
 * @param  pHandle [Handle to Str2Fun API instance]
 * @return         [STR2FUN_FUNC_XXXX, Please refer to STR2FUN_ERROR description]
 */
STR2FUN_ERROR xStr2Fun(char *pcStr, Str2FunHandle const pHandle);

#endif
