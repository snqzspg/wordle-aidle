#ifndef HASHMAP_H_INCLUDED
#define HASHMAP_H_INCLUDED

#include <stdio.h>

#include "hashmap_ll.h"

typedef struct {
	hm_item** mappings;
	hm_llist* items;
	size_t capacity;
	size_t length;
} hmap;

void hmap_set(hmap* h, const void* key, const size_t key_size, const void* value, const size_t val_size);
const void* hmap_get(hmap* h, const void* key, const size_t key_size, size_t* val_size_buf, const void* defval, const size_t defval_size);
//void hmap_inc(hmap* h, const void* key, const size_t key_size, size_t incvalue);
//void hmap_dec(hmap* h, const void* key, const size_t key_size, size_t decvalue);
char hmap_containskey(hmap* h, const void* key, const size_t key_size);
void hmap_deletekey(hmap* h, const void* key, const size_t key_size);

#define hmap_foreach(i, hmap) for (i = 0; i < hmap -> length; i++)
#define hmap_foreachitem(item, hmap) for (item = (hmap -> items -> first_item == NULL ? NULL : hmap -> items -> first_item); item != NULL; item = item -> next)

hmap* hmap_create();
void hmap_delete(hmap* b);

#endif // HASHMAP_H_INCLUDED
