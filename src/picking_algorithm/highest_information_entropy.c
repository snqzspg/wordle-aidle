#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../sorting/heap_sort.h"
//#include "../utilities/hashmap.h"
#include "../wordle/guess_bucket.h"
#include "../wordle/wordle_logic.h"
#include "../wordle/word_list.h"
#include "../wordle_filter/list_words_by_clue.h"
#include "../wordle_solver/solver.h"
#include "../word_list/hardcoded_dictionary.h"

#include "information_theory/string_to_size_t.h"
#include "information_theory/stts_item.h"

#include "algorithms.h"
#include "highest_information_entropy.h"

static void cpy_answers(solver* slvr, algorithm* algo, int list_index) {
	wbitem* item;
	wbank_foreachitem(item, hcded_dict) {
		wlist_append(slvr -> list_configs[list_index].list, item -> value);
	}
}

static void cpy_valids(solver* slvr, algorithm* algo, int list_index) {
	cpy_answers(slvr, algo, list_index);
	wbitem* item;
	wbank_foreachitem(item, valid_words) {
		wlist_append(slvr -> list_configs[list_index].list, item -> value);
	}
}

void information_theory_init(solver* slvr, algorithm* algo) {
	cpy_answers(slvr, algo, 0);
	cpy_valids(slvr, algo, 1);
	slvr -> list_configs[0].standard_filter = 1;
	slvr -> list_configs[1].standard_filter = 0;
}

void information_theory_hard_init(solver* slvr, algorithm* algo) {
	cpy_answers(slvr, algo, 0);
	cpy_valids(slvr, algo, 1);
	slvr -> list_configs[0].standard_filter = 1;
	slvr -> list_configs[1].standard_filter = 1;
}

void information_theory_more_vocab_init(solver* slvr, algorithm* algo) {
	cpy_valids(slvr, algo, 0);
	cpy_valids(slvr, algo, 1);
	slvr -> list_configs[0].standard_filter = 1;
	slvr -> list_configs[1].standard_filter = 0;
}

void information_theory_more_vocab_hard_init(solver* slvr, algorithm* algo) {
	cpy_valids(slvr, algo, 0);
	cpy_valids(slvr, algo, 1);
	slvr -> list_configs[0].standard_filter = 1;
	slvr -> list_configs[1].standard_filter = 1;
}

/**
 * Copy the result distribution into distmap.
 */
static void gen_result_distri(stts_hmap* distmap, wlist* list, const char* guess_word) {
	char result_str[strlen(guess_word) + 1];
	wlword* i;
	wlist_foreach(i, list) {
		wordle_evaluate(guess_word, i -> word, result_str);
		stts_hmap_inc(distmap, result_str, 1);
	}
}

//static void filter_word_list(wlist* list, const char* guess_word, const char* result) {      // DO NOT REMOVE! Future implementation.
//	list_lrpair* wrpairs = gen_res_pairs(guess_word, result);                                // DO NOT REMOVE! Future implementation.
//	cts_hmap* map_min = cts_hmap_createhashmap();                                            // DO NOT REMOVE! Future implementation.
//	cts_hmap* map_max = cts_hmap_createhashmap();                                            // DO NOT REMOVE! Future implementation.
//	size_t i;                                                                                // DO NOT REMOVE! Future implementation.
//	list_lrpair_foreach(i, wrpairs) {                                                        // DO NOT REMOVE! Future implementation.
//		char let = wrpairs -> arr[i].letter;                                                 // DO NOT REMOVE! Future implementation.
//		char res = wrpairs -> arr[i].result;                                                 // DO NOT REMOVE! Future implementation.
//		if (res == 'b') {                                                                    // DO NOT REMOVE! Future implementation.
//			cts_hmap_set(map_max, let, list_lrpair_count(wrpairs, let, 'y') + list_lrpair_count(wrpairs, let, 'g'));    // DO NOT REMOVE! Future implementation.
//		} else {                                                                             // DO NOT REMOVE! Future implementation.
//			cts_hmap_set(map_min, let, list_lrpair_count(wrpairs, let, 'y') + list_lrpair_count(wrpairs, let, 'g'));    // DO NOT REMOVE! Future implementation.
//		}                                                                                    // DO NOT REMOVE! Future implementation.
//	}                                                                                        // DO NOT REMOVE! Future implementation.
//	filter_wlist_by_last_clue(list, wrpairs, map_min, map_max);                              // DO NOT REMOVE! Future implementation.
//	cts_hmap_deletehashmap(map_min);                                                         // DO NOT REMOVE! Future implementation.
//	cts_hmap_deletehashmap(map_max);                                                         // DO NOT REMOVE! Future implementation.
//	list_lrpair_delete(wrpairs);                                                             // DO NOT REMOVE! Future implementation.
//}                                                                                            // DO NOT REMOVE! Future implementation.

//static double find_max_sub_entropy(wlist* list, const int recursion_depth);                  // DO NOT REMOVE! Future implementation.

/*
 * This process is O(n^recursion_depth)
 */
static double get_entropy(wlist* list, const char* guess_word/*, const int recursion_depth*/) { // DO NOT REMOVE! Future implementation.
	if (list -> length <= 1) {
		return 0.0;
	}
	stts_hmap* result_count_map = stts_hmap_create();
	gen_result_distri(result_count_map, list, guess_word);
	size_t total = list -> length;
	double prob = 0.0;
	double information = 0.0;
	double entropy = 0.0;
	stts_item* j;
	stts_hmap_foreachitem(j, result_count_map) {
		prob = (double) (j -> value) / (double) (total);
		information = log2(prob) * -1;
		entropy += prob * information;
//		wlist* filteredl = wlist_clone(list);                                   // DO NOT REMOVE! Future implementation.
//		filter_word_list(filteredl, guess_word, j -> key);                      // DO NOT REMOVE! Future implementation.
//		entropy += find_max_sub_entropy(filteredl, recursion_depth) * prob;     // DO NOT REMOVE! Future implementation.
//		wlist_delete(filteredl);                                                // DO NOT REMOVE! Future implementation.
	}
	stts_hmap_delete(result_count_map);
	return entropy;
}

/*
 * This process is O(n^2)
 */
//static double find_max_sub_entropy(wlist* list, const int recursion_depth) {    // DO NOT REMOVE! Future implementation.
//	if (recursion_depth - 1 == 0 || list -> length <= 1) {                      // DO NOT REMOVE! Future implementation.
//		return 0.0;                                                             // DO NOT REMOVE! Future implementation.
//	}                                                                           // DO NOT REMOVE! Future implementation.
//	double max = 0.0;                                                           // DO NOT REMOVE! Future implementation.
//	double entropy = 0.0;                                                       // DO NOT REMOVE! Future implementation.
//	wlword* i;                                                                  // DO NOT REMOVE! Future implementation.
//	wlist_foreach(i, list) {                                                    // DO NOT REMOVE! Future implementation.
//		entropy = get_entropy(list, i -> word/*, recursion_depth - 1*/);        // DO NOT REMOVE! Future implementation.
//		if (entropy > max) {                                                    // DO NOT REMOVE! Future implementation.
//			max = entropy;                                                      // DO NOT REMOVE! Future implementation.
//		}                                                                       // DO NOT REMOVE! Future implementation.
//	}                                                                           // DO NOT REMOVE! Future implementation.
//	return max;                                                                 // DO NOT REMOVE! Future implementation.
//}                                                                               // DO NOT REMOVE! Future implementation.

//struct word_entropy {
//	char* word;
//	double entropy;
//};

static int cmp_alpha_order(const void *a, const void *b) {
	return strcmp((*(char**) a), (*(char**) b));
}

//static int cmpwentropy(const void *a, const void *b) {
//	if ((*(struct word_entropy*)b).entropy > (*(struct word_entropy*)a).entropy) {
//		return 1;
//	}
//	if ((*(struct word_entropy*)b).entropy < (*(struct word_entropy*)a).entropy) {
//		return -1;
//	}
//	return 0;
//}

//static size_t getmax(const size_t* tally, const size_t len) {
//	size_t max = 0;
//	for (size_t i = 0; i < len; i++) {
//		if (tally[i] > max) {
//			max = tally[i];
//		}
//	}
//	return max;
//}

/*
 * This is the bulk of the thinking work.
 * In future, "hard drive" memoisation can be developed to speed this process up.
 * Or maybe not.
 */
static char* word_w_max_entropy(wlist* list, wlist* ref_list) {
	if (list -> length == 0 || ref_list -> length == 0) {
		return NULL;
	}
	double max_entro = 0.0;
	double entro = 0.0;
	char* ret_word = wlist_get(list, 0);
	wlword* i;
	wlist_foreach(i, list) {
		entro = get_entropy(ref_list, i -> word);
		if (entro > max_entro) {
			max_entro = entro;
			ret_word = i -> word;
		}
	}
	return ret_word;
}

static char should_pick_alt_list(wlist* word_list, char optimise) {
	return word_list -> length > 2 && (!optimise || word_list -> length < 50);
}

/*
 * NOTE: Only the list at index 0 is displayed to the user
 */
//static char* guess_by_information_entropy_base(wlist* word_list, gbucket* g, wlist* alt_list, char optimise) {
static char* guess_by_information_entropy_base(gbucket* guess_board, wlist** word_lists, size_t nword_lists, char optimise) {
	wlist* alt_list = nword_lists == 2 ? word_lists[1] : word_lists[0];
	if (word_lists[0] -> length == 0) {
		return NULL;
	}
	wlist* topick = should_pick_alt_list(word_lists[0], optimise)/*g -> max_guesses - g -> guess_count*/ ? alt_list : word_lists[0];
	//size_t wlen = gbucket_getlastguess(g) -> length;
//	struct word_entropy* entropies = malloc(sizeof(struct word_entropy) * topick -> length);
	char** tmp_word_list = malloc(sizeof(char*) * word_lists[0] -> length);
//	size_t counter = 0;
//	wlword* j;
//	wlist_foreach(j, topick) {
//		entropies[counter].word = j -> word;
//		entropies[counter].entropy = get_entropy(word_lists[0], j -> word);
//		counter++;
//	}
	size_t counter = 0;
	wlword* j;
	wlist_foreach(j, word_lists[0]) {
		tmp_word_list[counter] = j -> word;
		counter++;
	}
//	hsort(entropies, topick -> length, sizeof(struct word_entropy), cmpwentropy);
	hsort(tmp_word_list, word_lists[0] -> length, sizeof(char*), cmp_alpha_order);
	counter = 0;
	//FILE* debug = fopen("debug.log", "wb");
//	wlist_foreach(j, topick) {
		//fwrite(entropies[counter].word, sizeof(char), 5, debug);
		//fprintf(debug, " - %f\n", entropies[counter].entropy);
//		j -> word = entropies[counter].word;
//		counter++;
//	}
	counter = 0;
	wlist_foreach(j, word_lists[0]) {
		j -> word = tmp_word_list[counter];
		counter++;
	}
	//fclose(debug);
	free(tmp_word_list);
//	free(entropies);
	return word_w_max_entropy(topick, word_lists[0]);
}

//char* guess_by_information_entropy(wlist* l, gbucket* g, wlist* alt_list) {
char* guess_by_information_entropy(gbucket* guess_board, wlist** word_lists, size_t nword_lists) {
	return guess_by_information_entropy_base(guess_board, word_lists, nword_lists, 0);
}

/**
 * Using Matt Dodge's optimisation method.
 * Credit:
 *     https://betterprogramming.pub/building-a-wordle-bot-in-under-100-lines-of-python-9b980539defb
 */
//char* guess_by_information_entropy_optimised_level_1(wlist* l, gbucket* g, wlist* alt_list) {
char* guess_by_information_entropy_optimised_level_1(gbucket* guess_board, wlist** word_lists, size_t nword_lists) {
	return guess_by_information_entropy_base(guess_board, word_lists, nword_lists, 1);
}
