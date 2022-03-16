#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../error/print_error.h"
#include "../../terminal_helper/cons_graphics.h"
#include "string_to_size_t.h"
#include "stts_item.h"

void stts_hmap_expandcapacity(stts_hmap* h);
void stts_hmap_expandcapacityifneeded(stts_hmap* h) {
	if (h -> length >= h -> capacity) {
		stts_hmap_expandcapacity(h);
	}
}
void stts_hmap_chainitem(stts_hmap* h, stts_item* item) {
	stts_hmap_expandcapacityifneeded(h);

	stts_item** map = h -> mappings;

	long hash = item -> hash;
	size_t slot = hash % (h -> capacity);
	if (map[slot] == NULL) {
		map[slot] = item;
		return;
	}
	stts_item* i = map[slot];
	while (i -> mapnext != NULL) {
		i = i -> mapnext;
	}
	i -> mapnext = item;
	item -> mapprev = i;
}

void stts_hmap_unchainitem(stts_hmap* h, stts_item* item) {
	if (item -> mapprev == NULL) {
		h -> mappings[(item -> hash) % (h -> capacity)] = item -> mapnext;
		if (item -> mapnext != NULL) item -> mapnext -> mapprev = NULL;
	} else {
		item -> mapprev -> mapnext = item -> mapnext;
		if (item -> mapnext != NULL) item -> mapnext -> mapprev = item -> mapprev;
	}
	item -> mapprev = NULL;
	item -> mapnext = NULL;
}

void stts_hmap_expandcapacity(stts_hmap* h) {
	h -> capacity *= 2;
	h -> mappings = realloc(h -> mappings, sizeof(stts_item*) * (h -> capacity));
	if (h -> mappings == NULL) {
		print_error_ln("ERROR stts_hmap uh oh.. can't reallocate hashset.. looks like it's null pointer error time!");
	}
	stts_item** map = h -> mappings;
	size_t capacity = h -> capacity;

	size_t i;
	for (i = 0; i < capacity; i++) {
		map[i] = NULL;
	}

	stts_item* item;
	stts_hmap_foreachitem(item, h) {
		item -> mapnext = NULL;
		item -> mapprev = NULL;
	}

	stts_hmap_foreachitem(item, h) {
		stts_hmap_chainitem(h, item);
	}
}

char stts_hmap_itemeq(stts_item* item, const char* key) {
	return item -> hash == stts_item_make_hash(key) && strcmp(item -> key, key) == 0;
}

stts_item* stts_hmap_getcontainer(stts_hmap* b, const char* key) {
	long hash = stts_item_make_hash(key);
	size_t slot = hash % (b -> capacity);
	stts_item* i = b -> mappings[slot];
	while (i != NULL) {
		if (stts_hmap_itemeq(i, key)) {
			return i;
		}
		i = i -> mapnext;
	}
	return NULL;
}

void stts_hmap_set(stts_hmap* h, const char* key, size_t value) {
	stts_item* item = stts_hmap_getcontainer(h, key);
	if (item == NULL) {
		item = stts_item_create(key, value);
		stts_hmap_chainitem(h, item);
		stts_list_append(h -> items, item);
		h -> length++;
		return;
	}
	item -> value = value;
}

size_t stts_hmap_get(stts_hmap* h, const char* key, size_t defval) {
	stts_item* item = stts_hmap_getcontainer(h, key);
	if (item == NULL) {
		return defval;
	}
	return item -> value;
}

void stts_hmap_inc(stts_hmap* h, const char* key, size_t incvalue) {
	stts_item* item = stts_hmap_getcontainer(h, key);
	if (item == NULL) {
		item = stts_item_create(key, incvalue);
		stts_hmap_chainitem(h, item);
		stts_list_append(h -> items, item);
		h -> length++;
		return;
	}
	item -> value += incvalue;
}

void stts_hmap_dec(stts_hmap* h, const char* key, size_t decvalue) {
	stts_item* item = stts_hmap_getcontainer(h, key);
	if (item == NULL) {
		return;
	}
	item -> value -= decvalue;
}

char stts_hmap_containskey(stts_hmap* h, const char* key) {
	return stts_hmap_getcontainer(h, key) != NULL;
}

void stts_hmap_deletekey(stts_hmap* h, const char* key) {
	stts_item* item = stts_hmap_getcontainer(h, key);
	if (item != NULL) {
		stts_hmap_unchainitem(h, item);
		stts_list_remove(h -> items, item);
	}
}

stts_hmap* stts_hmap_create() {
	const size_t initial_hash_size = 16;
	stts_hmap* bank = malloc(sizeof(stts_hmap));
	if (bank == NULL) {
		print_error_ln("ERROR stts_hmap: failed to create; there's no space left for me in RAM D:");
		return NULL;
	}
	bank -> mappings = malloc(sizeof(stts_item*) * initial_hash_size);
	bank -> items = stts_list_create();
	if (bank -> mappings == NULL || bank -> items == NULL) {
		print_error_ln("ERROR stts_hmap: failed to create; there's no space left for me in RAM D:");
		return NULL;
	}
	for (size_t i = 0; i < initial_hash_size; i++) {
		bank -> mappings[i] = NULL;
	}
	bank -> capacity = initial_hash_size;
	bank -> length = 0;
	return bank;
}

void stts_hmap_delete(stts_hmap* b) {
	stts_list_delete(b -> items);
	free(b -> mappings);
	free(b);
}
