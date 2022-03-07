#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char obfs_char(const char c, const size_t i) {
	return c - 'A' + i;
}

char deobfs_char(const char c, const size_t i) {
	return c + 'A' - i;
}

void obfs_str(char* s, size_t i) {
	i &= 7;
	while (*s) {
		*s = obfs_char(*s, i);
		s++;
	}
}

void deobfs_str(char* s, size_t i) {
	i &= 7;
	while (*s) {
		*s = deobfs_char(*s, i);
		s++;
	}
}
