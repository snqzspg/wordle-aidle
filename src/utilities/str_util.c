#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_util.h"
#include "int_util.h"
#include "str_util.h"

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

void lowercase(char* s) {
	if (s == NULL) {
		return;
	}
	for (; *s != '\0'; s++) {
		*s = (char) tolower(*s);
	}
}

void uppercase(char* s) {
	if (s == NULL) {
		return;
	}
	for (; *s != '\0'; s++) {
		*s = (char) toupper(*s);
	}
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

long hash_str(const char* s) {
	return hash_bytes(s, strlen(s) * sizeof(char));
}

/**
 * http://www.cse.yorku.ca/~oz/hash.html
 */
long djb2_hash_str(const char* s) {
	return djb2_hash_bytes(s, strlen(s) * sizeof(char));
}

/**
 * http://www.cse.yorku.ca/~oz/hash.html
 */
long sdbm_hash_str(const char* s) {
	return sdbm_hash_bytes(s, strlen(s) * sizeof(char));
}
