#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "int_util.h"

#define bool char
#define true 1
#define false 0

bool str_islen(const char *s, size_t len) {
	for (size_t i = 0; i < len; i++) {
		if (s[i] == '\0') {
			return false;
		}
	}
	return s[len] == '\0';
}

const char querty_key_seq[36] = "123weasdzxc456rtyfghvbn789uiojklm0p";

/**
 * Note that first option starts from 0
 */
size_t get_option_key_len(size_t i) {
	return i / 35;
}


/**
 * Note that first option starts from 0
 * Copies the corresponding option key of i into opt
 */
void cpy_option_key(size_t i, char* optbuf) {
	while (i >= 35) {
		*optbuf = querty_key_seq[i % 35];
		optbuf++;
		i /= 35;
	}
	*optbuf = querty_key_seq[i % 35];
	optbuf++;
	*optbuf = '\0';
}

void insert_int_bef_ext(char* fname, const int n, const size_t buflen) {
	const size_t nlen = num_char_int(n);
	const size_t slen = strlen(fname);
	if (buflen < slen + nlen + 1) {
		return;
	}
	char* ext = strrchr(fname, '.');
	char tmp[strlen(ext) + 1];
	strcpy(tmp, ext);
	cpynumstr_int(ext, n, nlen);
	ext += nlen;
	strcpy(ext, tmp);
}
