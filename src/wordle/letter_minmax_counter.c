#include <stdio.h>
#include <stdlib.h>

#include "../error/print_error.h"

#include "letter_counter.h"

static size_t index_from_letter(const char c) {
	return (size_t)(c - 'a');
}

char lminmax_is_min_logged(const lminmax_item* item) {
	return item -> minmax_logged & 1;
}

char lminmax_is_max_logged(const lminmax_item* item) {
	return item -> minmax_logged & 2;
}

void lminmax_set_min_logged(lminmax_item* item, char x) {
	if (x) {
		item -> minmax_logged |= 1;
	} else if (item -> minmax_logged & 1) {
		item -> minmax_logged ^= 1;
	}
}

void lminmax_set_max_logged(lminmax_item* item, char x) {
	if (x) {
		item -> minmax_logged |= 2;
	} else if (item -> minmax_logged & 2) {
		item -> minmax_logged ^= 2;
	}
}

void lminmax_reset_item(lminmax_item* item) {
	item -> min = 0;
	item -> max = 0;
	item -> minmax_logged = 0;
}

/**
 * Produces inaccurate value if not logged.
 */
size_t lminmax_get_min_by_litem(const lminmax_item* item) {
	return item -> min;
}

/**
 * Only copies if it is logged. Returns 1 if logged and 0 if not.
 */
char lminmax_cpy_min_by_litem(size_t* __restrict__ dest, const lminmax_item* __restrict__ item) {
	if (!lminmax_is_min_logged(item)) return 0;
	*dest = lminmax_get_min_by_litem(item);
	return 1;
}

/**
 * Produces inaccurate value if not logged.
 */
size_t lminmax_get_max_by_litem(const lminmax_item* item) {
	return item -> max;
}

/**
 * Only copies if it is logged. Returns 1 if logged and 0 if not.
 */
char lminmax_cpy_max_by_litem(size_t* __restrict__ dest, const lminmax_item* __restrict__ item) {
	if (!lminmax_is_max_logged(item)) return 0;
	*dest = lminmax_get_max_by_litem(item);
	return 1;
}

void lminmax_set_min(lminmax* __restrict__ map, const char c, const size_t count) {
	size_t index = index_from_letter(c);
	if (!lminmax_is_min_logged()) {
		map -> logged_indices[map -> logged_indices_len] = index;
		map -> logged_indices_len++;
	}
	lminmax_set_min_logged(&(map -> data[index]), 1);
	map -> data[index].min = count;
}

void lminmax_set_max(lminmax* __restrict__ map, const char c, const size_t count) {
	size_t index = index_from_letter(c);
	if (!lminmax_is_max_logged()) {
		map -> logged_indices[map -> logged_indices_len] = index;
		map -> logged_indices_len++;
	}
	lminmax_set_max_logged(&(map -> data[index]), 1);
	map -> data[index].max = count;
}

/**
 * Only copies if it is logged. Returns 1 if logged and 0 if not.
 */
char lminmax_cpy_min(lminmax* map, size_t* dest, const char c) {
	size_t index = index_from_letter(c);
	return lminmax_cpy_min_by_litem(dest, &(map -> data[index]));
}

/**
 * Only copies if it is logged. Returns 1 if logged and 0 if not.
 */
char lminmax_cpy_max(lminmax* map, size_t* dest, const char c) {
	size_t index = index_from_letter(c);
	return lminmax_cpy_max_by_litem(dest, &(map -> data[index]));
}

void lminmax_reset_to_default(lminmax* __restrict__ map) {
	if (map -> data == NULL) {
		print_err_warning_ln("lminmax make sure map -> data is allocated before running lminmax_reset_to_default.");
		return;
	}
	if (map -> logged_indices == NULL) {
		print_err_warning_ln("lminmax make sure map -> logged_indices is allocated before running lminmax_reset_to_default.");
		return;
	}
	for (size_t i = 0; i < map -> allocated_length; i++) {
		lminmax_reset_item(&(map -> data[i]));
	}
	map -> logged_indices_len = 0;
}

lminmax* lminmax_create(const size_t fixedlength) {
	lminmax* map = malloc(sizeof(lminmax));
	if (map == NULL) {
		print_error_ln("lminmax out of memory to create lminmax.");
		return NULL;
	}
	map -> data = malloc(sizeof(lminmax_item) * fixedlength);
	if (map -> data == NULL) {
		free(map);
		print_error_ln("lminmax out of memory to create data.");
		return NULL;
	}
	map -> logged_indices = malloc(sizeof(size_t) * fixedlength);
	if (map -> logged_indices == NULL) {
		free(map -> data);
		free(map);
		print_error_ln("lminmax out of memory to create data.");
		return NULL;
	}
	map -> allocated_length = fixedlength;
	map -> length = fixedlength;
	lminmax_reset_to_default(map);
	return map;
}

void lminmax_delete(lminmax* map) {
	free(map -> logged_indices);
	free(map -> data);
	free(map);
}
