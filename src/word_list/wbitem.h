#ifndef WBITEM_H_INCLUDED
#define WBITEM_H_INCLUDED

typedef struct word_container wbitem;

struct word_container {
	long hash;
	char* value;
	size_t length;
	wbitem* prev;
	wbitem* next;
	wbitem* mapprev;
	wbitem* mapnext;
};

/**
 * Stolen from python
 */
long wbitem_make_hash(const char *s);
wbitem* wbitem_create(const char* val);
wbitem* wbitem_clone(wbitem* w);
void wbitem_delete(wbitem* x);

typedef struct wbword_linked_list {
	wbitem* first_item;
	wbitem* last_item;
	size_t length;
} wbilist;

void wbilist_append(wbilist* l, wbitem* s);
void wbilist_extend(wbilist* l, wbilist* x);
void wbilist_insert(wbilist* l, size_t i, wbitem* s);
void wbilist_remove(wbilist* l, wbitem* s);
void wbilist_removebyi(wbilist* l, size_t i);
void wbilist_clear(wbilist* l);
size_t wbilist_index(wbilist* l, wbitem* s);
size_t wbilist_count(wbilist* l, wbitem* s);
char wbilist_contains(wbilist* l, wbitem* s);
void wbilist_set(wbilist* l, size_t i, wbitem* s);
wbitem* wbilist_get(wbilist* l, size_t i);

#define wbilist_foreach(i, list) for (i = (list -> first_item == NULL ? NULL : list -> first_item); i != NULL; i = i -> next)

wbilist* wbilist_create();
void wbilist_delete(wbilist* x);

#endif // WBITEM_H_INCLUDED
