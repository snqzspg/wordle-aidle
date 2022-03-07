#ifndef STRING_TO_SIZE_T_ITEM_H_INCLUDED
#define STRING_TO_SIZE_T_ITEM_H_INCLUDED

typedef struct stti_container stts_item;

struct stti_container {
	long hash;
	char* key;
	size_t value;
	stts_item* prev;
	stts_item* next;
	stts_item* mapprev;
	stts_item* mapnext;
};

long stts_item_make_hash(const char* key);
stts_item* stts_item_create(const char* key, const size_t val);
stts_item* stts_item_clone(stts_item* w);
void stts_item_delete(stts_item* x);

typedef struct stts_clist {
	stts_item* first_item;
	stts_item* last_item;
	size_t length;
} stts_list;

void stts_list_append(stts_list* l, stts_item* s);
void stts_list_extend(stts_list* l, stts_list* x);
void stts_list_insert(stts_list* l, size_t i, stts_item* s);
void stts_list_remove(stts_list* l, stts_item* s);
void stts_list_removebyi(stts_list* l, size_t i);
void stts_list_clear(stts_list* l);
size_t stts_list_index(stts_list* l, stts_item* s);
size_t stts_list_count(stts_list* l, stts_item* s);
char stts_list_contains(stts_list* l, stts_item* s);
void stts_list_set(stts_list* l, size_t i, stts_item* s);
stts_item* stts_list_get(stts_list* l, size_t i);

#define stts_list_foreach(i, list) for (i = (list -> first_item == NULL ? NULL : list -> first_item); i != NULL; i = i -> next)

stts_list* stts_list_create();
void stts_list_delete(stts_list* x);

#endif // STRING_TO_SIZE_T_ITEM_H_INCLUDED
