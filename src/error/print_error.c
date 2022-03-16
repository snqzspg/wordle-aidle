#include <stdio.h>

#include "../terminal_helper/cons_graphics.h"

#include "print_error.h"

void print_err_warning_ln(const char* x) {
	pgcg_set_warning_colour_stderr();
	fprintf(stderr, "%s\n", x);
	pgcg_reset_colour_stderr();
}

void print_error_ln(const char* x) {
	pgcg_set_error_colour_stderr();
	fprintf(stderr, "%s\n", x);
	pgcg_reset_colour_stderr();
}
