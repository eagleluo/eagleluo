#include "str2argv.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


STR2ARG_ERROR argv_init(uint32_t *argc, char ***argv)
{
	*argv = (char **)calloc(ARGV_MAX_ENTRIES, sizeof(char *));

	(*argv)[0] = (char *)calloc(ARGV_MAX_TOKEN_LEN, sizeof(char));

	bzero((*argv)[0], ARGV_MAX_TOKEN_LEN * sizeof(char));

	*argc = 0;

	return STR2ARG_NO_ERROR;
}
/**
 * [argv_free lists]
 * @param  argc [argument count for free]
 * @param  argv [argument list fo clean]
 * @return      [Pass or Fail]
 */
STR2ARG_ERROR argv_free(uint32_t *argc, char ***argv)
{
	int i;

	for (i = 0; i <= ARGV_MAX_ENTRIES; i++)
	{
		free((*argv)[i]);
	}

	free(*argv);
	*argc = 0;

	return STR2ARG_NO_ERROR;
}

STR2ARG_ERROR argv_addch(uint32_t argc, char **argv, uint32_t c)
{
	uint32_t n;

	n = strlen(argv[argc]);
	if (ARGV_MAX_TOKEN_LEN - 1 == n)
		return STR2ARG_SOME_ERROR;
	argv[argc][n] = c;
	return STR2ARG_NO_ERROR;
}

STR2ARG_ERROR argv_finish_token(uint32_t *argc, char ***argv)
{
	if (ARGV_MAX_ENTRIES - 1 == (*argc))
	{
		return STR2ARG_SOME_ERROR;
	}

	if (0 == strlen((*argv)[*argc]))
	{
		return STR2ARG_NO_ERROR;
	}

	(*argc) = (*argc) + 1;
	if (NULL == ((*argv)[*argc] = (char *)calloc(ARGV_MAX_TOKEN_LEN, sizeof(char))))
		return STR2ARG_SOME_ERROR;

	bzero((*argv)[*argc], ARGV_MAX_TOKEN_LEN * sizeof(uint8_t));
	return STR2ARG_NO_ERROR;
}


STR2ARG_ERROR str2argv(char *str, uint32_t *argc, char ***argv, char **errmsg)
{
	bool in_token;
	bool in_container;
	bool escaped;
	uint8_t container_start;
	char c;
	uint32_t len;
	uint32_t i;

	char *ERRORS[2] = {
		"Unmatched quotes",
		"Unused/Dangling escape sequence"
	};

	if (NULL == argc || NULL == argv || NULL == errmsg)
		return STR2ARG_SOME_ERROR;

	*errmsg = NULL;

	container_start = 0;
	in_token = false;
	in_container = false;
	escaped = false;

	len = strlen(str);

	argv_init(argc, argv);
	for (i = 0; i < len; i++) {
		c = str[i];

		switch (c) {
		/* handle whitespace */
		case ' ':
		case '\t':
		case '\n':
		case '\r':
			if (!in_token)
				continue;

			if (in_container) {
				argv_addch(*argc, *argv, c);
				continue;
			}

			if (escaped) {
				escaped = false;
				argv_addch(*argc, *argv, c);
				continue;
			}

			/* if reached here, we're at end of token */
			in_token = false;
			argv_finish_token(argc, argv);
			break;

		/* handle quotes */
		case '\'':
		case '\"':

			if (escaped) {
				argv_addch(*argc, *argv, c);
				escaped = false;
				continue;
			}

			if (!in_token) {
				in_token = true;
				in_container = true;
				container_start = c;
				continue;
			}

			if (in_token && !in_container) {
				in_container = true;
				container_start = c;
				continue;
			}

			if (in_container) {
				if (c == container_start) {
					in_container = false;
					in_token = false;
					argv_finish_token(argc, argv);
					continue;
				} else {
					argv_addch(*argc, *argv, c);
					continue;
				}
			}

			*errmsg = ERRORS[0];
			argv_free(argc, argv);
			return STR2ARG_SOME_ERROR;;

		case '\\':
			if (in_container && str[i + 1] != container_start) {
				argv_addch(*argc, *argv, c);
				continue;
			}

			if (escaped) {
				escaped = false;
				argv_addch(*argc, *argv, c);
				continue;
			}

			escaped = true;
			break;

		default:
			if (!in_token)
				in_token = true;

			if (escaped)
				escaped = false;

			argv_addch(*argc, *argv, c);
		}
	}
	argv_finish_token(argc, argv);

	if (in_container) {
		argv_free(argc, argv);
		*errmsg = ERRORS[0];
		return STR2ARG_SOME_ERROR;;
	}

	if (escaped) {
		argv_free(argc, argv);
		*errmsg = ERRORS[1];
		return STR2ARG_SOME_ERROR;;
	}

//	(*argv)[*argc] = NULL;

	return STR2ARG_NO_ERROR;;
}


