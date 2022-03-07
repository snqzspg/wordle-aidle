#include <stdio.h>
#include <stdlib.h>

#include "hashmap.h"

char cts_hmap_isinitemlist(cts_hmap* h, cts_hmap_item* item) {
	cts_hmap_item** items = h -> items;
	size_t i;
	cts_hmap_foreach(i, h) {
		if (items[i] == item) {
			return 1;
		}
	}
	return 0;
}

void cts_hmap_addtointitemlist(cts_hmap* h, cts_hmap_item* item) {
	if (cts_hmap_isinitemlist(h, item)) {
		return;
	}
	h -> items[h -> length] = item;
	h -> length++;
}

void cts_hmap_chainitem(cts_hmap* h, char key, cts_hmap_item* item) {
	cts_hmap_expandcapacityifneeded(h);
	cts_hmap_addtointitemlist(h, item);

	cts_hmap_item** map = h -> mappings;

	size_t hash = item -> hash;
	size_t map_slot = hash % (h -> capacity);
	if (map[map_slot] == NULL) {
		map[map_slot] = item;
		return;
	}
	cts_hmap_item* el = map[map_slot];
	while (el -> next != NULL) {
		el = el -> next;
	}
	el -> next = item;
	item -> prev = el;
}

void cts_hmap_expandcapacity(cts_hmap* h) {
	h -> capacity *= 2;
	h -> mappings = realloc(h -> mappings, sizeof(cts_hmap_item*) * (h -> capacity));
	h -> items = realloc(h -> items, sizeof(cts_hmap_item*) * (h -> capacity));
	cts_hmap_item** map = h -> mappings;
	cts_hmap_item** items = h -> items;
	size_t capacity = h -> capacity;

	size_t i;
	for (i = 0; i < capacity; i++) {
		map[i] = NULL;
	}

	cts_hmap_foreach(i, h) {
		items[i] -> next = NULL;
		items[i] -> prev = NULL;
	}

	cts_hmap_foreach(i, h) {
		cts_hmap_chainitem(h, items[i] -> key, items[i]);
	}
}

void cts_hmap_expandcapacityifneeded(cts_hmap* h) {
	if (h -> length >= h -> capacity) {
		cts_hmap_expandcapacity(h);
	}
}

size_t cts_hmap_hash_key(char key) {
	return key;
}

cts_hmap_item* cts_hmap_getitem(cts_hmap* h, char key) {
	size_t hash = cts_hmap_hash_key(key);
	size_t map_slot = hash % (h -> capacity);
	cts_hmap_item* el = h -> mappings[map_slot];
	while (el != NULL) {
		if (el -> hash == hash && el -> key == key) {
			return el;
		}
		el = el -> next;
	}
	return NULL;
}

cts_hmap_item* cts_hmap_item_create(char key, size_t value) {
	cts_hmap_item* item = malloc(sizeof(cts_hmap_item));
	item -> hash = cts_hmap_hash_key(key);
	item -> key = key;
	item -> value = value;
	item -> next = NULL;
	item -> prev = NULL;
	return item;
}

void cts_hmap_item_delete(cts_hmap_item* item) {
	free(item);
}

void cts_hmap_set(cts_hmap* h, char key, size_t value) {
	cts_hmap_item* item = cts_hmap_getitem(h, key);
	if (item == NULL) {
		item = cts_hmap_item_create(key, value);
		cts_hmap_chainitem(h, key, item);
	}
	item -> value = value;
}

size_t cts_hmap_getwdefault(cts_hmap* h, char key, size_t defval) {
	cts_hmap_item* item = cts_hmap_getitem(h, key);
	if (item == NULL) {
		return defval;
	}
	return item -> value;
}

size_t cts_hmap_get(cts_hmap* h, char key) {
	return cts_hmap_getwdefault(h, key, 0);
}

char cts_hmap_containskey(cts_hmap* h, char key) {
	return cts_hmap_getitem(h, key) != NULL;
}

void cts_hmap_inc(cts_hmap* h, char key, size_t inc_value) {
	cts_hmap_set(h, key, cts_hmap_get(h, key) + inc_value);
}

void cts_hmap_dec(cts_hmap* h, char key, size_t dec_value) {
	cts_hmap_set(h, key, cts_hmap_get(h, key) - dec_value);
}

void cts_hmap_unchainitem(cts_hmap* h, cts_hmap_item* item) {
	if (item -> prev == NULL) {
		h -> mappings[(item -> hash) % (h -> capacity)] = item -> next;
		if (item -> next != NULL) item -> next -> prev = NULL;
	} else {
		item -> prev -> next = item -> next;
		if (item -> next != NULL) item -> next -> prev = item -> prev;
	}
}

char in_items(cts_hmap_item** items, size_t items_count, cts_hmap_item* x) {
	for (size_t i = 0; i < items_count; i++) {
		if (items[i] == x) {
			return 1;
		}
	}
	return 0;
}

void cts_hmap_deleteitems(cts_hmap* h, cts_hmap_item** items, size_t items_count) {
	size_t i;
	for (i = 0; i < items_count; i++) {
		if (cts_hmap_containskey(h, items[i] -> key)) {
			cts_hmap_unchainitem(h, items[i]);
		}
	}
	//printf("DEBUG 'a'? %d\n", cts_hmap_containskey(h, 'a'));
	size_t offset = 0;
	cts_hmap_foreach(i, h) {
		if (i + offset >= h -> length) {
			break;
		}
		cts_hmap_item* item1 = h -> items[i + offset];
		if (in_items(items, items_count, item1)) {
			offset++;
		}
		if (offset != 0) {
			h -> items[i] = item1;
		}
	}
	h -> length -= offset;
	for (i = 0; i < items_count; i++) {
		if (cts_hmap_containskey(h, items[i] -> key)) {
			cts_hmap_item_delete(items[i]);
		}
	}
}

void cts_hmap_deletekeys(cts_hmap* h, char* keys, size_t keys_count) {
	cts_hmap_item** items = malloc(sizeof(cts_hmap_item*) * keys_count);
	for (size_t i = 0; i < keys_count; i++) {
		items[i] = cts_hmap_getitem(h, keys[i]);
	}
	cts_hmap_deleteitems(h, items, keys_count);
	free(items);
}

void cts_hmap_deleteitem(cts_hmap* h, cts_hmap_item* item) {
	cts_hmap_deleteitems(h, &item, 1);
}

void cts_hmap_deletekey(cts_hmap* h, char key) {
	cts_hmap_deletekeys(h, &key, 1);
}

cts_hmap* cts_hmap_createhashmap() {
	const size_t initial_hash_size = 16;
	cts_hmap* map = malloc(sizeof(cts_hmap));
	map -> mappings = malloc(sizeof(cts_hmap_item*) * initial_hash_size);
	map -> items = malloc(sizeof(cts_hmap_item*) * initial_hash_size);
	for (size_t i = 0; i < initial_hash_size; i++) {
		map -> mappings[i] = NULL;
		map -> items[i] = NULL;
	}
	map -> capacity = initial_hash_size;
	map -> length = 0;
	return map;
}

void cts_hmap_deletehashmap(cts_hmap* map) {
	size_t i;
	cts_hmap_foreach(i, map) {
		cts_hmap_item_delete(map -> items[i]);
	}
	free(map -> mappings);
	free(map -> items);
	free(map);
}
