#include <stdio.h>

#include "../terminal_helper/helper_fxs.h"
#include "hm_v1/cts_hashmap.h"
#include "option_keys.h"

//static const char querty_key_seq[36] = "123weasdzxc456rtyfghvbn789uiojklm0p";
static const char querty_key_seq[36] = "1234werasdfzxcv5678tyuighjkbnm90opl";

static cts_hmap* key_seq_idx = NULL;

int option_keys_init() {
	key_seq_idx = cts_hmap_createhashmap();
	if (key_seq_idx == NULL) {
		return 1;
	}
	for (size_t i = 0; i < 35; i++) {
		cts_hmap_set(key_seq_idx, querty_key_seq[i], i);
	}
	return 0;
}

void option_keys_cleanup() {
	cts_hmap_deletehashmap(key_seq_idx);
	key_seq_idx = NULL;
}

/**
 * Note that first option starts from 0
 * Returns the length of the corresponding option key string of i
 */
size_t get_option_key_len(size_t i) {
	return i / 35 + 1;
}

/**
 * Note that first option starts from 0
 * Copies the corresponding option key of i into opt
 */
void cpy_option_key(size_t i, char* optbuf) {
	while (i >= 35) {
		*optbuf = querty_key_seq[i % 35];
		optbuf++;
		i /= 35;
	}
	*optbuf = querty_key_seq[i % 35];
	optbuf++;
	*optbuf = '\0';
}

size_t get_idx_from_option_key(const char* optkey) {
    size_t ret = 0;
    for (const char* p = optkey; *p != '\0'; p++) {
        ret *= 35;
        ret += cts_hmap_get(key_seq_idx, *p);
    }
    return ret;
}

void print_options_list(const void* list, const size_t nitems, const size_t size, const size_t indent, const size_t (*get_name_len)(const void*, const size_t), void (*cpy_name)(const void*, const size_t, char* buffer)) {
	for (size_t i = 0; i < nitems; i++) {
		const size_t optlen = get_option_key_len(i);
		char optkey[optlen + 1];
		cpy_option_key(i, optkey);
		char name_buffer[get_name_len(list + nitems * size, size) + 1];
		cpy_name(list + nitems * size, size, name_buffer);
		print_wraped_linef("%s - %s", indent, PGINDENT, optkey, name_buffer);
	}
}
