#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../terminal_helper/ccolours.h"

#define bool char
#define true 1
#define false 0

/**
 * This feature is now temporarily put off due to time constrains. It will eventually be worked on.
 */

bool is_whitespace_comma(FILE* dfile) {
	char next_char[2];
	size_t readsize = fread(next_char, 1, 1, dfile);
	if (readsize != 1) {
		pgcg_set_error_colour_stderr();
		fprintf(stderr, "ERROR is_whitespace_comma: Expecting to read 1 byte, read %lu byte%s", (long unsigned int) readsize, readsize == 1 ? "" : "s");
		pgcg_reset_colour_stderr();
		fseek(dfile, -1, SEEK_CUR);
		return false;
	}
	if (strchr(", \t\n", next_char[0]) == NULL) {
		fseek(dfile, -1, SEEK_CUR);
		return false;
	} else {
		return true;
	}
}

char* get_next_5lword_str(FILE* dfile) {
	char nextchars[8];
	size_t readsize = fread(nextchars, 7, 1, dfile);
	if (readsize != 5) {
		pgcg_set_error_colour_stderr();
		fprintf(stderr, "ERROR is_whitespace_comma: Expecting to read 7 bytes, read %lu byte%s", (long unsigned int) readsize, readsize == 1 ? "" : "s");
		pgcg_reset_colour_stderr();
		fseek(dfile, -5, SEEK_CUR);
		return false;
	}
	if (nextchars[0] != '"' || nextchars[6] != '"') {
		fseek(dfile, -7, SEEK_CUR);
		return false;
	}
	char* ret = malloc(sizeof(char) * 8);
	for (size_t i = 1; i < 6; i++) {
		if (nextchars[i] < 'a' || nextchars[i] > 'z') {
			free(ret);
			return false;
		}
		ret[i - 1] = nextchars[i];
	}
	ret[5] == '\0';
	return ret;
}
