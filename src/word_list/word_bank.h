#ifndef WORD_BANK_H_INCLUDED
#define WORD_BANK_H_INCLUDED

#include "wbitem.h"

typedef struct word_bank {
	wbitem** mappings;
	wbilist* items;
	size_t capacity;
	size_t length;
} wbank;

void wbank_add(wbank* b, const char* s);
char* wbank_getrandom(wbank* b);
char wbank_contain(wbank* b, const char* s);
void wbank_remove(wbank* b, const char* s);

#define wbank_foreach(i, set) for (i = 0; i < set -> length; i++)
#define wbank_foreachitem(item, set) for (item = (set -> items -> first_item == NULL ? NULL : set -> items -> first_item); item != NULL; item = item -> next)

wbank* wbank_create();
void wbank_delete(wbank* b);

#endif // WORD_BANK_H_INCLUDED
