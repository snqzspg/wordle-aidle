#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../error/print_error.h"
#include "../terminal_helper/cons_graphics.h"
#include "input-helper.h"

static char have_newline(char* str, size_t len) {
	while (len) {
		len--;
		if (str[len] == 10) {
			return 1;
		}
	}
	return 0;
}

static void cut_str_at_newline(char* str, size_t len) {
	while (len) {
		len--;
		if (str[len] == 10) {
			str[len] = 0;
			break;
		}
	}
}

/**
 *  A dynamic version that gives malloc (Yes you have to free the pointer)
 */
char *ask_user() {
	const size_t blockl = 4;
	size_t l = blockl + 2;
	char* buf = NULL;
	int count = 0;
	char tmp[blockl + 2];
	for (size_t i = 0; i < blockl + 2; i++) {
		tmp[i] = '\0';
	}
	do {
		if (fgets(tmp, blockl + 1, stdin) == NULL) {
		// https://www.tutorialspoint.com/c_standard_library/c_function_fgets.htm
		// Stops when either (n-1) characters are read, the newline character is read, or the end-of-file is reached, whichever comes first.
			free(buf);
			return NULL;
		}
		if (buf == NULL) {
			buf = malloc(l * sizeof(char));
			for (size_t i = 0; i < l; i++) {
				buf[i] = '\0';
			}
			if (buf == NULL) {
				print_error_ln("Error: Memory allocation failed (ask_user function).");
				exit(1);
			}
			strcpy(buf, tmp);
		} else {
			l += blockl;
			buf = realloc(buf, l * sizeof(char));
			if (buf == NULL) {
				print_error_ln("Error: Memory allocation failed (ask_user function).");
				exit(1);
			}
			strcat(buf, tmp);
		}
		count++;
	} while (!have_newline(tmp, blockl));
	cut_str_at_newline(buf, l);
	if (*buf == 0) {
		free(buf);
		return NULL;
	}
	return buf;
}
