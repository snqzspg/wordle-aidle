#ifndef LETTER_COUNTER_H_INCLUDED
#define LETTER_COUNTER_H_INCLUDED

#include <stdio.h>

#define NUM_LETTERS 26

typedef struct {
	size_t min;
	size_t max;
	char minmax_logged;
} lminmax_item;

char lminmax_is_min_logged(const lminmax_item* item);
char lminmax_is_max_logged(const lminmax_item* item);
void lminmax_set_min_logged(lminmax_item* item, char x);
void lminmax_set_max_logged(lminmax_item* item, char x);
void lminmax_reset_item(lminmax_item* item);

/**
 * Produces inaccurate value if not logged.
 */
size_t lminmax_get_min_by_litem(const lminmax_item* item);

/**
 * Only copies if it is logged. Returns 1 if logged and 0 if not.
 */
char lminmax_cpy_min_by_litem(size_t* __restrict__ dest, const lminmax_item* __restrict__ item);

/**
 * Produces inaccurate value if not logged.
 */
size_t lminmax_get_max_by_litem(const lminmax_item* item);

/**
 * Only copies if it is logged. Returns 1 if logged and 0 if not.
 */
char lminmax_cpy_max_by_litem(size_t* __restrict__ dest, const lminmax_item* __restrict__ item);

typedef struct {
	lminmax_item* data;
	size_t length;
	size_t* logged_indices;
	size_t logged_indices_len;
	size_t allocated_length;
} lminmax;

void lminmax_set_min(lminmax* __restrict__ map, const char c, const size_t count);
void lminmax_set_max(lminmax* __restrict__ map, const char c, const size_t count);

/**
 * Only copies if it is logged. Returns 1 if logged and 0 if not.
 */
char lminmax_cpy_min(lminmax* map, size_t* dest, const char c);

/**
 * Only copies if it is logged. Returns 1 if logged and 0 if not.
 */
char lminmax_cpy_max(lminmax* map, size_t* dest, const char c);
void lminmax_reset_to_default(lminmax* __restrict__ map);

lminmax* lminmax_create(const size_t fixedlength);
void lminmax_delete(lminmax* map);

#endif // LETTER_COUNTER_H_INCLUDED
