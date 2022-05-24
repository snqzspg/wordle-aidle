#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utilities/input-helper.h"
#include "cons_graphics.h"
#include "helper_fxs.h"

void clear_console() {
	#ifdef _WIN32
	if (system("cls") == -1) printf("\n\n------------------------------\n\n");
	#elif defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
	if (system("clear") == -1) printf("\n\n------------------------------\n\n");
	#else
	printf("\n\n------------------------------\n\n");
	#endif // _WIN32
}

void pause_console() {
	#ifdef _WIN32
	system("pause");
	#else
	printf("Press <enter> to continue");
	char* d = ask_user();
	free(d);
	#endif // _WIN32
}

/**
 * Returns the number of chars copied (not including newlines).
 */
static size_t cpy_to_next_eof(char* dest, const char* line, int limit) {
	size_t i;
	for (i = 0; *line != '\n' && *line != '\0' && limit; line++, dest++, limit--, i++) {
		*dest = *line;
	}
	*dest = '\0';
	return i;
}

//static static count_to_next_space(const char* line) {
//	size_t count = 0;
//	for (; strchr(" \t\r\n\v\f", *line) == NULL && *line != '\0'; line++) {
//		count++;
//	}
//	return count;
//}

/**
 * The following macro is not necessary for GCC, but it is a solution for MSVC compilers.
 * https://stackoverflow.com/questions/558223/va-copy-porting-to-visual-c
 */
#ifndef va_copy
#define va_copy(d, s) ((d) = (s))
#endif // va_copy

/**
 * Delete using free()
 */
static char* create_fmttd_line(const char* fmt, va_list args) {
	size_t buffer = strlen(fmt) + 1;
	size_t len = 0;
	va_list stash;
	va_copy(stash, args);
	char* ret = NULL;
	do {
		buffer *= 2;
		ret = realloc(ret, sizeof(char) * buffer);
		vsnprintf(ret, buffer, fmt, args);
		ret[buffer - 1] = '\0';
		len = strlen(ret);
		va_end(args);
		va_copy(args, stash);
	} while (len >= buffer - 1);
	ret = realloc(ret, sizeof(char) * (len + 1));
	va_end(stash);
	return ret;
}

void vfprint_wraped_f(FILE* f, const char* line, int indentation, int indent_size, va_list args) {
	char* fmttd = create_fmttd_line(line, args);
	char* ptr = fmttd;
	indentation *= indent_size;
	const int cols = pgcg_get_console_cols() - 1;
	const int space = cols - indentation;
	char tmpstack[cols + 1];
	tmpstack[cols] = '\0';
	size_t copied = 0;
	for (; *ptr != '\0'; ptr += copied) {
		if (*ptr == '\n') {
			copied = 1;
			fprintf(f, "\n");
			continue;
		}
		for (size_t i = 0; i < indentation; i++) {
			tmpstack[i] = ' ';
		}
		copied = cpy_to_next_eof(&(tmpstack[indentation]), ptr, space);
		fprintf(f, "%s%s", tmpstack, copied < space ? "" : "\n");
	}
	free(fmttd);
}

void vfprint_wraped_linef(FILE* f, const char* line, int indentation, int indent_size, va_list args) {
	vfprint_wraped_f(f, line, indentation, indent_size, args);
	fprintf(f, "\n");
}

void print_wraped_linef(const char* line, int indentation, int indent_size, ...) {
	va_list args;
	va_start(args, indent_size);
	vfprint_wraped_linef(stdout, line, indentation, indent_size, args);
	va_end(args);
}
