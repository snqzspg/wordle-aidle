#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../sorting/heap_sort.h"
#include "../utilities/hashmap.h"
#include "../wordle/guess_bucket.h"
#include "../wordle/word_list.h"
#include "../wordle_solver/solver.h"
#include "../word_list/hardcoded_dictionary.h"

#include "algorithms.h"
#include "random_pick.h"

void random_pick_init(solver* slvr, algorithm* algo) {
	wbitem* item;
	wbank_foreachitem(item, hcded_dict) {
		wlist_append(slvr -> list_configs[0].list, item -> value);
	}
	slvr -> list_configs[0].standard_filter = 1;
}

void random_pick_larger_init(solver* slvr, algorithm* algo) {
	wbitem* item;
	wbank_foreachitem(item, hcded_dict) {
		wlist_append(slvr -> list_configs[0].list, item -> value);
	}
	wbank_foreachitem(item, valid_words) {
		wlist_append(slvr -> list_configs[0].list, item -> value);
	}
	slvr -> list_configs[0].standard_filter = 1;
}

/**
 * Self explanatory
 */
/*
 * NOTE: Only the list at index 0 is displayed to the user
 */
//char* guess_randomly(wlist* word_list, gbucket* g, wlist* alt_list) {
char* guess_randomly(gbucket* guess_board, wlist** word_lists, size_t nword_lists) {
	if (word_lists[0] -> length == 0) {
		return NULL;
	}
	return wlist_get(word_lists[0], (size_t) rand() % (word_lists[0] -> length));
}
