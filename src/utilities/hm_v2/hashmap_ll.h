#ifndef HASHMAP_LL_H_INCLUDED
#define HASHMAP_LL_H_INCLUDED

#include <stdio.h>

typedef struct hm_container hm_item;

struct hm_container {
	long hash;
	void* key;
	size_t key_size;
	void* value;
	size_t value_size;
	hm_item* prev;
	hm_item* next;
	hm_item* mapprev;
	hm_item* mapnext;
};

long hm_item_make_hash(const void* key, const size_t key_size);
hm_item* hm_item_create(const void* key, const size_t key_size, const void* val, const size_t val_size);
hm_item* hm_item_clone(hm_item* w);
void hm_item_delete(hm_item* x);

typedef struct {
	hm_item* first_item;
	hm_item* last_item;
	size_t length;
} hm_llist;

void hm_llist_append(hm_llist* l, hm_item* s);
void hm_llist_extend(hm_llist* l, hm_llist* x);
void hm_llist_insert(hm_llist* l, size_t i, hm_item* s);
void hm_llist_remove(hm_llist* l, hm_item* s);
void hm_llist_removebyi(hm_llist* l, size_t i);
void hm_llist_clear(hm_llist* l);
size_t hm_llist_index(hm_llist* l, hm_item* s);
size_t hm_llist_count(hm_llist* l, hm_item* s);
char hm_llist_contains(hm_llist* l, hm_item* s);
void hm_llist_set(hm_llist* l, size_t i, hm_item* s);
hm_item* hm_llist_get(hm_llist* l, size_t i);

#define hm_llist_foreach(i, list) for (i = (list -> first_item == NULL ? NULL : list -> first_item); i != NULL; i = i -> next)

hm_llist* hm_llist_create();
void hm_llist_delete(hm_llist* x);

#endif // HASHMAP_LL_H_INCLUDED
