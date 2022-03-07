#ifndef STRING_TO_SIZE_T_H_INCLUDED
#define STRING_TO_SIZE_T_H_INCLUDED

#include "stts_item.h"

typedef struct string_to_sizet_hash_mp {
	stts_item** mappings;
	stts_list* items;
	size_t capacity;
	size_t length;
} stts_hmap;

void stts_hmap_set(stts_hmap* h, const char* key, size_t value);
size_t stts_hmap_get(stts_hmap* h, const char* key, size_t defval);
void stts_hmap_inc(stts_hmap* h, const char* key, size_t incvalue);
void stts_hmap_dec(stts_hmap* h, const char* key, size_t decvalue);
char stts_hmap_containskey(stts_hmap* h, const char* key);
void stts_hmap_deletekey(stts_hmap* h, const char* key);

#define stts_hmap_foreach(i, hmap) for (i = 0; i < hmap -> length; i++)
#define stts_hmap_foreachitem(item, hmap) for (item = (hmap -> items -> first_item == NULL ? NULL : hmap -> items -> first_item); item != NULL; item = item -> next)

stts_hmap* stts_hmap_create();
void stts_hmap_delete(stts_hmap* b);

#endif // STRING_TO_SIZE_T_H_INCLUDED
