#include <stdio.h>

#include "int_util.h"

size_t num_char_int(int n) {
	size_t d = 1;
	while (n >= 10){
		n /= 10;
		d++;
	}
	if (n < 1) {
		d++;
	}
	return d;
}

size_t num_digit(size_t n) {
	size_t d = 1;
	while (n >= 10){
		n /= 10;
		d++;
	}
	return d;
}

size_t num_digit_unsigned_int(unsigned int n) {
	size_t d = 1;
	while (n >= 10){
		n /= 10;
		d++;
	}
	return d;
}

void cpynumstr(char* buf, size_t n, size_t ndigits) {
	buf[ndigits] = '\0';
	while (ndigits) {
		buf[ndigits - 1] = (char)(48 + (n % 10));
		n /= 10;
		ndigits--;
	}
}

void cpynumstr_usi(char* buf, unsigned int n, size_t ndigits) {
	buf[ndigits] = '\0';
	while (ndigits) {
		buf[ndigits - 1] = (char)(48 + (n % 10));
		n /= 10;
		ndigits--;
	}
}

void cpynumstr_int(char* buf, int n, size_t ndigits) {
	buf[ndigits] = '\0';
	while (ndigits) {
		buf[ndigits - 1] = (char)(48 + (n % 10));
		n /= 10;
		ndigits--;
	}
	if (n < 0) {
		*buf = '-';
	}
}

size_t idx_from_str(const char* s) {
	size_t r = 0;
	while (*s >= '0' && *s <= '9') {
		r += (size_t)(*s) - (size_t)'0';
		r *= 10;
		s++;
	}
	return r / 10;
}

int int_from_str(const char* s) {
	int r = 0;
	while (*s >= '0' && *s <= '9') {
		r += (int)(*s) - (int)'0';
		r *= 10;
		s++;
	}
	return r / 10;
}
