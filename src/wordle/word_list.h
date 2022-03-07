#ifndef WORD_LIST_H_INCLUDED
#define WORD_LIST_H_INCLUDED

typedef struct Wordle wlword;

struct Wordle {
	char* word;
	wlword* prev;
	wlword* next;
	size_t length;
};

wlword* wlword_create(const char* s);
wlword* wlword_clone(wlword* w);
void wlword_delete(wlword* x);

typedef struct Word_Linked_list {
	wlword* first_item;
	wlword* last_item;
	size_t length;
} wlist;

void wlist_append(wlist* l, const char* s);
void wlist_extend(wlist* l, wlist* x);
void wlist_insert(wlist* l, size_t i, const char* s);
void wlist_remove(wlist* l, const char* s);
void wlist_removebyi(wlist* l, size_t i);
void wlist_clear(wlist* l);
size_t wlist_index(wlist* l, const char* s);
size_t wlist_count(wlist* l, const char* s);
char wlist_contains(wlist* l, const char* s);
void wlist_set(wlist* l, size_t i, const char* s);
char* wlist_get(wlist* l, size_t i);
wlword* wlist_getwl(wlist* l, size_t i);

/**
 * A PAINFUL LESSON LEARNT
 *
 * Do not use this function to filter lists, or delete all values, if not it will segfault.
 * This is because once you've deleted a list, the -> next information will not be available to iterate.
 * To achieve the above, you might have to use a while loop, store the pointer for the next element, and then delete, so that you can still iterate using the stores pointer.
 * For an example, you may refer to the wlist_clear source code.
 */
#define wlist_foreach(i, list) for (i = (list -> first_item == NULL ? NULL : list -> first_item); i != NULL; i = i -> next)

wlist* wlist_create();
wlist* wlist_clone(const wlist* w);
void wlist_delete(wlist* x);

// For the sake of efficiency
void wlist_removewl(wlist* l, wlword* x);

#endif // WORD_LIST_H_INCLUDED
