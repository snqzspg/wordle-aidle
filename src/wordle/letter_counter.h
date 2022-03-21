#ifndef LETTER_COUNTER_H_INCLUDED
#define LETTER_COUNTER_H_INCLUDED

#include <stdio.h>

#define NUM_LETTERS 26

typedef struct {
	char letter;
	size_t count;
	char logged;
} lcounter_item;

char lcounter_is_logged(const lcounter_item* item);
void lcounter_set_logged(lcounter_item* item, char x);
void lcounter_reset_item(lcounter_item* item);

/**
 * Produces inaccurate value if not logged.
 */
size_t lcounter_get_by_litem(const lcounter_item* item);

/**
 * Only copies if it is logged. Returns 1 if logged and 0 if not.
 */
char lcounter_cpy_by_litem(size_t* __restrict__ dest, const lcounter_item* __restrict__ item);

typedef struct {
	lcounter_item* data;
	size_t length;
	size_t* logged_indices;
	size_t logged_indices_len;
	size_t allocated_length;
} lcounter;

void lcounter_set(lcounter* __restrict__ map, const char c, const size_t count);

/**
 * Only copies if it is logged. Returns 1 if logged and 0 if not.
 */
char lcounter_cpy(lcounter* map, size_t* dest, const char c);
void lcounter_reset_to_default(lcounter* __restrict__ map);

#define lcounter_foreach(i, map) for (i = 0; i < map -> logged_indices_len; i++)

lcounter* lcounter_create(const size_t fixedlength);
void lcounter_delete(lcounter* map);

#endif // LETTER_COUNTER_H_INCLUDED
