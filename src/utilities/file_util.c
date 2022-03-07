#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "int_util.h"
#include "str_util.h"

/**
 * Delete using free
 */
char* create_next_available_filename(const char* fname) {
	const size_t fnlen = strlen(fname);
	int counter = 0;
	size_t digitcount = 1;
	char* retname = malloc(sizeof(char) * (fnlen + 1));
	strcpy(retname, fname);
	FILE* f = fopen(fname, "r");
	while (f) {
		fclose(f);
		counter++;
		digitcount = num_char_int(counter);
		retname = realloc(retname, sizeof(char) * (fnlen + digitcount + 1));
		strcpy(retname, fname);
		insert_int_bef_ext(retname, counter, fnlen + digitcount + 1);
		f = fopen(retname, "r");
	}
	return retname;
}

/**
 * Delete using free
 */
char* create_log_filename(const char* prefix) {
	const size_t pflen = strlen(prefix);
	time_t now = time(NULL);
	struct tm* timeinfo = localtime(&now);
	size_t year_len = num_char_int(timeinfo -> tm_year + 1900);
	char* ret = malloc(sizeof(char) * (pflen + year_len + 25));
	strcpy(ret, prefix);
	ret[pflen] = '-';
	strftime(ret + pflen + 1, year_len + 20, "%Y-%m-%d_%H.%M.%S.log", timeinfo);
	return ret;
}
