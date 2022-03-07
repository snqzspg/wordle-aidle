#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../sorting/heap_sort.h"
#include "../utilities/hashmap.h"
#include "../wordle/guess_bucket.h"
#include "../wordle/word_list.h"

/**
 * Self explanatory
 */
/*
 * NOTE: alt_list could be NULL
 */
char* guess_randomly(wlist* word_list, gbucket* g, wlist* alt_list) {
	if (word_list -> length == 0) {
		return NULL;
	}
	return wlist_get(word_list, (size_t) rand() % (word_list -> length));
}
