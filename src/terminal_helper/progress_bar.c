#include <stdio.h>
#include <stdlib.h>

#include "cons_graphics.h"

void print_progress_bar_u(unsigned int progress, unsigned int total, size_t length) {
	size_t col_length = (size_t) progress * length / (size_t) total;
	printf("[");
	pgcg_set_progress_bar_colour();
	for (size_t i = 0; i < col_length; i++) {
		printf("#");
	}
	pgcg_reset_colour();
	for (size_t i = col_length; i < length; i++) {
		printf(" ");
	}
	printf("]");
}
