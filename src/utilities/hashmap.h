#ifndef HASHMAP_H_INCLUDED
#define HASHMAP_H_INCLUDED

typedef struct char_to_sizet_hash_mp_item cts_hmap_item;

struct char_to_sizet_hash_mp_item {
	size_t hash;
	char key;
	size_t value;
	cts_hmap_item* next;
	cts_hmap_item* prev;
};

cts_hmap_item* cts_hmap_item_create(char key, size_t value);
void cts_hmap_item_delete(cts_hmap_item* item);

typedef struct char_to_sizet_hash_mp {
	cts_hmap_item** mappings;
	cts_hmap_item** items;
	size_t capacity;
	size_t length;
} cts_hmap;

void cts_hmap_expandcapacity(cts_hmap* h);
void cts_hmap_expandcapacityifneeded(cts_hmap* h);

size_t cts_hmap_hash_key(char key);

void cts_hmap_set(cts_hmap* h, char key, size_t value);
size_t cts_hmap_get(cts_hmap* h, char key);
size_t cts_hmap_getwdefault(cts_hmap* h, char key, size_t defval) ;
char cts_hmap_containskey(cts_hmap* h, char key);
void cts_hmap_inc(cts_hmap* h, char key, size_t inc_value);
void cts_hmap_dec(cts_hmap* h, char key, size_t dec_value);
void cts_hmap_deletekey(cts_hmap* h, char key);
void cts_hmap_deletekeys(cts_hmap* h, char* keys, size_t keys_count);

#define cts_hmap_foreach(i, hmap) for (i = 0; i < hmap -> length; i++)

cts_hmap* cts_hmap_createhashmap();
void cts_hmap_deletehashmap(cts_hmap* map);

#endif // HASHMAP_H_INCLUDED
