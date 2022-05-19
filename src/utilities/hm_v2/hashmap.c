#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../error/print_error.h"
#include "../../terminal_helper/cons_graphics.h"
#include "hashmap.h"
#include "hashmap_ll.h"

void hmap_expandcapacity(hmap* h);
void hmap_expandcapacityifneeded(hmap* h) {
	if (h -> length >= h -> capacity) {
		hmap_expandcapacity(h);
	}
}
void hmap_chainitem(hmap* h, hm_item* item) {
	hmap_expandcapacityifneeded(h);

	hm_item** map = h -> mappings;

	long hash = item -> hash;
	size_t slot = hash % (h -> capacity);
	if (map[slot] == NULL) {
		map[slot] = item;
		return;
	}
	hm_item* i = map[slot];
	while (i -> mapnext != NULL) {
		i = i -> mapnext;
	}
	i -> mapnext = item;
	item -> mapprev = i;
}

void hmap_unchainitem(hmap* h, hm_item* item) {
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

void hmap_expandcapacity(hmap* h) {
	h -> capacity *= 2;
	h -> mappings = realloc(h -> mappings, sizeof(hm_item*) * (h -> capacity));
	if (h -> mappings == NULL) {
		print_error_ln("ERROR hmap uh oh.. can't reallocate hashset.. looks like it's null pointer error time!");
	}
	hm_item** map = h -> mappings;
	size_t capacity = h -> capacity;

	size_t i;
	for (i = 0; i < capacity; i++) {
		map[i] = NULL;
	}

	hm_item* item;
	hmap_foreachitem(item, h) {
		item -> mapnext = NULL;
		item -> mapprev = NULL;
	}

	hmap_foreachitem(item, h) {
		hmap_chainitem(h, item);
	}
}

char hmap_itemeq(hm_item* item, const void* key, const size_t key_size) {
	return item -> hash == hm_item_make_hash(key, key_size) && strcmp(item -> key, key) == 0;
}

hm_item* hmap_getcontainer(hmap* b, const void* key, const size_t key_size) {
	long hash = hm_item_make_hash(key, key_size);
	size_t slot = hash % (b -> capacity);
	hm_item* i = b -> mappings[slot];
	while (i != NULL) {
		if (hmap_itemeq(i, key, key_size)) {
			return i;
		}
		i = i -> mapnext;
	}
	return NULL;
}

void hmap_set(hmap* h, const void* key, const size_t key_size, const void* value, const size_t val_size) {
	hm_item* item = hmap_getcontainer(h, key, key_size);
	if (item == NULL) {
		item = hm_item_create(key, key_size, value, val_size);
		hmap_chainitem(h, item);
		hm_llist_append(h -> items, item);
		h -> length++;
		return;
	}
	if (val_size != item -> value_size) {
		item -> value = realloc(item -> value, val_size);
	}
	memcpy(item -> value, value, val_size);
}

const void* hmap_get(hmap* h, const void* key, const size_t key_size, size_t* val_size_buf, const void* defval, const size_t defval_size) {
	hm_item* item = hmap_getcontainer(h, key, key_size);
	if (item == NULL) {
		*val_size_buf = defval_size;
		return defval;
	}
	*val_size_buf = item -> value_size;
	return item -> value;
}

char hmap_containskey(hmap* h, const void* key, const size_t key_size) {
	return hmap_getcontainer(h, key, key_size) != NULL;
}

void hmap_deletekey(hmap* h, const void* key, const size_t key_size) {
	hm_item* item = hmap_getcontainer(h, key, key_size);
	if (item != NULL) {
		hmap_unchainitem(h, item);
		hm_llist_remove(h -> items, item);
	}
}

hmap* hmap_create() {
	const size_t initial_hash_size = 16;
	hmap* bank = (hmap*) malloc(sizeof(hmap));
	if (bank == NULL) {
		print_error_ln("ERROR hmap: failed to create; there's no space left for me in RAM D:");
		return NULL;
	}
	bank -> mappings = (hm_item**) malloc(sizeof(hm_item*) * initial_hash_size);
	bank -> items = hm_llist_create();
	if (bank -> mappings == NULL || bank -> items == NULL) {
		print_error_ln("ERROR hmap: failed to create; there's no space left for me in RAM D:");
		return NULL;
	}
	for (size_t i = 0; i < initial_hash_size; i++) {
		bank -> mappings[i] = NULL;
	}
	bank -> capacity = initial_hash_size;
	bank -> length = 0;
	return bank;
}

void hmap_delete(hmap* b) {
	hm_llist_delete(b -> items);
	free(b -> mappings);
	free(b);
}
