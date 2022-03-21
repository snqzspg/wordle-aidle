#include <stdio.h>
#include <stdlib.h>

#include "../error/print_error.h"

#include "letter_counter.h"

static size_t index_from_letter(const char c) {
	return (size_t)(c - 'a');
}

char lcounter_is_logged(const lcounter_item* item) {
	return item -> logged;
}

void lcounter_set_logged(lcounter_item* item, char x) {
	item -> logged = x;
}

void lcounter_reset_item(lcounter_item* item) {
	item -> letter = 0;
	item -> count = 0;
	item -> logged = 0;
}

/**
 * Produces inaccurate value if not logged.
 */
size_t lcounter_get_by_litem(const lcounter_item* item) {
	return item -> count;
}

/**
 * Only copies if it is logged. Returns 1 if logged and 0 if not.
 */
char lcounter_cpy_by_litem(size_t* __restrict__ dest, const lcounter_item* __restrict__ item) {
	if (!lcounter_is_logged(item)) return 0;
	*dest = lcounter_get_by_litem(item);
	return 1;
}

void lcounter_set(lcounter* __restrict__ map, const char c, const size_t count) {
	size_t index = index_from_letter(c);
	if (!lcounter_is_logged(&(map -> data[index]))) {
		map -> logged_indices[map -> logged_indices_len] = index;
		map -> logged_indices_len++;
	}
	lcounter_set_logged(&(map -> data[index]), 1);
	map -> data[index].count = count;
	map -> data[index].letter = c;
}

/**
 * Only copies if it is logged. Returns 1 if logged and 0 if not.
 */
char lcounter_cpy(lcounter* map, size_t* dest, const char c) {
	size_t index = index_from_letter(c);
	return lcounter_cpy_by_litem(dest, &(map -> data[index]));
}

void lcounter_reset_to_default(lcounter* __restrict__ map) {
	if (map -> data == NULL) {
		print_err_warning_ln("lcounter make sure map -> data is allocated before running lcounter_reset_to_default.");
		return;
	}
	if (map -> logged_indices == NULL) {
		print_err_warning_ln("lcounter make sure map -> logged_indices is allocated before running lcounter_reset_to_default.");
		return;
	}
	for (size_t i = 0; i < map -> allocated_length; i++) {
		lcounter_reset_item(&(map -> data[i]));
	}
	map -> logged_indices_len = 0;
}

lcounter* lcounter_create(const size_t fixedlength) {
	lcounter* map = malloc(sizeof(lcounter));
	if (map == NULL) {
		print_error_ln("lcounter out of memory to create lcounter.");
		return NULL;
	}
	map -> data = malloc(sizeof(lcounter_item) * fixedlength);
	if (map -> data == NULL) {
		free(map);
		print_error_ln("lcounter out of memory to create data.");
		return NULL;
	}
	map -> logged_indices = malloc(sizeof(size_t) * fixedlength);
	if (map -> logged_indices == NULL) {
		free(map -> data);
		free(map);
		print_error_ln("lcounter out of memory to create data.");
		return NULL;
	}
	map -> allocated_length = fixedlength;
	map -> length = fixedlength;
	lcounter_reset_to_default(map);
	return map;
}

void lcounter_delete(lcounter* map) {
	free(map -> logged_indices);
	free(map -> data);
	free(map);
}
