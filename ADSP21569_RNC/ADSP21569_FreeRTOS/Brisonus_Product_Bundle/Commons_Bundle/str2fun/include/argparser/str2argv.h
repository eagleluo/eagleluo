#ifndef STR2ARGV_H
#define STR2ARGV_H

#include <stdint.h>

#define ARGV_MAX_ENTRIES    10
#define ARGV_MAX_TOKEN_LEN  18

typedef enum {
	STR2ARG_NO_ERROR,
	STR2ARG_SOME_ERROR
}STR2ARG_ERROR;

#define bzero(argv, len)         memset(argv, 0, len)
/**
 * [str2argv description]
 * @param  str    [description]
 * @param  argc   [description]
 * @param  argv   [description]
 * @param  errmsg [description]
 * @return        [description]
 */
STR2ARG_ERROR str2argv(char *str, uint32_t *argc, char ***argv, char **errmsg);
/**
 * [argv_free description]
 * @param  argc [description]
 * @param  argv [description]
 * @return      [description]
 */
STR2ARG_ERROR argv_free(uint32_t *argc, char ***argv);


#endif
