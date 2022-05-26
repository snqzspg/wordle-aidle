#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

//const char querty_key_seq[36] = "123weasdzxc456rtyfghvbn789uiojklm0p";
//
///**
// * Note that first option starts from 0
// * Returns the length of the corresponding option key string of i
// */
//size_t get_option_key_len(size_t i) {
//	return i / 35 + 1;
//}
//
///**
// * Note that first option starts from 0
// * Copies the corresponding option key of i into opt
// */
//void cpy_option_key(size_t i, char* optbuf) {
//	while (i >= 35) {
//		*optbuf = querty_key_seq[i % 35];
//		optbuf++;
//		i /= 35;
//	}
//	*optbuf = querty_key_seq[i % 35];
//	optbuf++;
//	*optbuf = '\0';
//}

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
	register size_t len;
	register long x;

	len = strlen(s);
	x = *s << 7;
	while (--len >= 10) {
		x = (1000003 * x) ^ (*s + 1);
	}
	x ^= strlen(s);
	if (x == -1) {
		x = -2;
	}
	return x;
}

/**
 * http://www.cse.yorku.ca/~oz/hash.html
 */
long djb2_hash_str(const char* s) {
	if (*s == '\0') {
		return 0;
	}
	long hash = 5381;
	int c = *s;
	while (c) {
		hash = ((hash << 5) + hash) + c;
		c = *s;
		s++;
	}
	return hash;
}

/**
 * http://www.cse.yorku.ca/~oz/hash.html
 */
long sdbm_hash_str(const char* s) {
	long hash = 0;
	int c = *s;
	while (c) {
		hash = c + (hash << 6) + (hash << 16) - hash;
		c = *s;
		s++;
	}
	return hash;
}
