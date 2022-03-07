#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../error/print_error.h"
#include "../terminal_helper/ccolours.h"
#include "wbitem.h"
#include "word_bank.h"

void wbank_expandcapacity(wbank* h);
void wbank_expandcapacityifneeded(wbank* h) {
	if (h -> length >= h -> capacity) {
		wbank_expandcapacity(h);
	}
}
void wbank_chainitem(wbank* h, wbitem* item) {
	wbank_expandcapacityifneeded(h);

	wbitem** map = h -> mappings;

	long hash = item -> hash;
	size_t slot = hash % (h -> capacity);
	if (map[slot] == NULL) {
		map[slot] = item;
		return;
	}
	wbitem* i = map[slot];
	while (i -> mapnext != NULL) {
		i = i -> mapnext;
	}
	i -> mapnext = item;
	item -> mapprev = i;
}

void wbank_unchainitem(wbank* h, wbitem* item) {
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

void wbank_expandcapacity(wbank* h) {
	h -> capacity *= 2;
	h -> mappings = realloc(h -> mappings, sizeof(wbitem*) * (h -> capacity));
	if (h -> mappings == NULL) {
		print_error_ln("ERROR wbank uh oh.. can't reallocate hashset.. looks like it's null pointer error time!");
	}
	//h -> items = realloc(h -> items, sizeof(wbitem*) * (h -> capacity));
	wbitem** map = h -> mappings;
	size_t capacity = h -> capacity;

	size_t i;
	for (i = 0; i < capacity; i++) {
		map[i] = NULL;
	}

	wbitem* item;
	wbank_foreachitem(item, h) {
		item -> mapnext = NULL;
		item -> mapprev = NULL;
	}

	wbank_foreachitem(item, h) {
		wbank_chainitem(h, item);
	}
}

char wbank_itemeq(wbitem* item, const char* string) {
	return item -> hash == wbitem_make_hash(string) && !strcmp(item -> value, string);
}

void wbank_add(wbank* b, const char* s) {
	if (wbank_contain(b, s)) {
		return;
	}
	wbitem* item = wbitem_create(s);
	wbank_chainitem(b, item);
	wbilist_append(b -> items, item);
	b -> length++;
}

char* wbank_getrandom(wbank* b) {
	size_t randi = rand() % (b -> length);
	return wbilist_get(b -> items, randi) -> value;
}

wbitem* wbank_getcontainer(wbank* b, const char* s) {
	if (s == NULL || strlen(s) == 0) {
		return NULL;
	}
	long hash = wbitem_make_hash(s);
	size_t slot = hash % (b -> capacity);
	wbitem* i = b -> mappings[slot];
	while (i != NULL) {
		if (wbank_itemeq(i, s)) {
			return i;
		}
		i = i -> mapnext;
	}
	return NULL;
}

char wbank_contain(wbank* b, const char* s) {
	return wbank_getcontainer(b, s) != NULL;
}
void wbank_remove(wbank* b, const char* s) {
	wbitem* item = wbank_getcontainer(b, s);
	if (item == NULL) {
		return;
	}
	wbank_unchainitem(b, item);
	wbilist_remove(b -> items, item);
}

wbank* wbank_create() {
	const size_t initial_hash_size = 16;
	wbank* bank = malloc(sizeof(wbank));
	if (bank == NULL) {
		print_error_ln("ERROR wbank: failed to create; there's no space left for me in RAM D:");
		return NULL;
	}
	bank -> mappings = malloc(sizeof(wbitem*) * initial_hash_size);
	bank -> items = wbilist_create();
	if (bank -> mappings == NULL || bank -> items == NULL) {
		print_error_ln("ERROR wbank: failed to create; there's no space left for me in RAM D:");
		return NULL;
	}
	for (size_t i = 0; i < initial_hash_size; i++) {
		bank -> mappings[i] = NULL;
	}
	bank -> capacity = initial_hash_size;
	bank -> length = 0;
	return bank;
}

void wbank_delete(wbank* b) {
	wbilist_delete(b -> items);
	free(b -> mappings);
	free(b);
}
