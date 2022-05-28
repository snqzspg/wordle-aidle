#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../sorting/heap_sort.h"
#include "../wordle/guess_bucket.h"
#include "../wordle/wordle_logic.h"
#include "../wordle/word_list.h"
#include "../wordle_solver/solver.h"
#include "../word_list/hardcoded_dictionary.h"

#include "information_theory/string_to_size_t.h"
#include "information_theory/stts_item.h"

#include "algorithms.h"
#include "anti_absurdle.h"
#include "highest_information_entropy.h"

void anti_absurdle_init(solver* slvr, algorithm* algo) {
	wbitem* item;
	wbank_foreachitem(item, hcded_dict) {
		wlist_append(slvr -> list_configs[0].list, item -> value);
	}
	wbank_foreachitem(item, hcded_dict) {
		wlist_append(slvr -> list_configs[1].list, item -> value);
	}
	wbank_foreachitem(item, valid_words) {
		wlist_append(slvr -> list_configs[1].list, item -> value);
	}
	slvr -> list_configs[0].standard_filter = 1;
	slvr -> list_configs[1].standard_filter = 0;
}

static size_t get_worst_count(wlist* list, const char* guess_word) {
	if (list -> length <= 1) {
		return 0;
	}
	stts_hmap* result_count_map = stts_hmap_create();
	gen_result_distri(result_count_map, list, guess_word);
	size_t highest_count = 0;
	stts_item* j;
	stts_hmap_foreachitem(j, result_count_map) {
		if (j -> value > highest_count) {
			highest_count = j -> value;
		}
	}
	stts_hmap_delete(result_count_map);
	return highest_count;
}

static char* word_w_best_worst_case(wlist* list, wlist* ref_list) {
	if (list -> length == 0 || ref_list -> length == 0) {
		return NULL;
	}
	size_t worst_count = 0;
	char* ret_word = wlist_get(list, 0);
	size_t best_worst_count = get_worst_count(ref_list, ret_word);
	wlword* i;
	wlist_foreach(i, list) {
		worst_count = get_worst_count(ref_list, i -> word);
		if (worst_count < best_worst_count) {
			best_worst_count = worst_count;
			ret_word = i -> word;
		}
	}
	return ret_word;
}

struct word_absurd_count {
	char* word;
	size_t count;
};

static int cmp_alpha_order(const void *a, const void *b) {
	return strcmp((*(char**) a), (*(char**) b));
}

static int cmpwacount(const void *a, const void *b) {
	return (*(struct word_absurd_count*)a).count - (*(struct word_absurd_count*)b).count;
	return 0;
}

static void log_scores_itdebug(wlist* list, wlist* ref_list) {
	struct word_absurd_count* scores = malloc(sizeof(struct word_absurd_count) * list -> length);
	size_t counter = 0;
	wlword* j;
	wlist_foreach(j, list) {
		scores[counter].word = j -> word;
		scores[counter].count = get_worst_count(ref_list, j -> word);
		counter++;
	}
	hsort(scores, list -> length, sizeof(struct word_absurd_count), cmpwacount);
	counter = 0;
	FILE* debug = fopen("debug.log", "wb");
	wlist_foreach(j, list) {
		fwrite(scores[counter].word, sizeof(char), 5, debug);
		fprintf(debug, " - %lu\n", (long unsigned) scores[counter].count);
		counter++;
	}
	fclose(debug);
	free(scores);
}

static char should_pick_alt_list(wlist* word_list) {
	return word_list -> length > 2 /*g -> max_guesses - g -> guess_count*/;
}

char* guess_using_anti_absurdle(gbucket* guess_board, wlist** word_lists, size_t nword_lists, char show_word_list_to_user) {
	wlist* alt_list = nword_lists == 2 ? word_lists[1] : word_lists[0];
	if (word_lists[0] -> length == 0) {
		return NULL;
	}
	wlist* topick = should_pick_alt_list(word_lists[0]) ? alt_list : word_lists[0];
	if (show_word_list_to_user) {
		if (log_scores) {
			log_scores_itdebug(topick, word_lists[0]);
		}
		char** tmp_word_list = malloc(sizeof(char*) * word_lists[0] -> length);
		size_t counter = 0;
		wlword* j;
		wlist_foreach(j, word_lists[0]) {
			tmp_word_list[counter] = j -> word;
			counter++;
		}
		hsort(tmp_word_list, word_lists[0] -> length, sizeof(char*), cmp_alpha_order);
		counter = 0;
		wlist_foreach(j, word_lists[0]) {
			j -> word = tmp_word_list[counter];
			counter++;
		}
		free(tmp_word_list);
	}
	return word_w_best_worst_case(topick, word_lists[0]);
}
