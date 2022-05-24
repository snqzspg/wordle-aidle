#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../sorting/heap_sort.h"
#include "../wordle/guess_bucket.h"
#include "../wordle/letter_counter.h"
#include "../wordle/word_list.h"
#include "../wordle_solver/solver.h"
#include "../word_list/hardcoded_dictionary.h"
#include "../word_list/word_bank.h"

#include "algorithms.h"
#include "most-frequent-in-column.h"

/**
 * Most frequent in the letter column.
 * This counts all the words for each column
 * of the word and tally the frequencies of
 * letter appearances for that specific
 * column.
 *
 * For example in these following words:
 *   bleed
 *   bleep
 *   blend
 *   bluer
 *   clerk
 *   clued
 *   elbow
 *   elder
 *   elegy
 *   fleck
 *   flyer
 *   olden
 *   older
 *   plier
 *   ulcer
 * For the first column, b has the highest score of 4 counts, e is the next with 2 counts and so on.
 * These will be tallied for each of the five columns.
 *
 * Then, the letters in the words will be matched up to their frequencies in their respective columns and added to a sum.
 * The word(s) with the highest sum(s) will be selected.
 *
 */

//static size_t getmax(const size_t* tally, const size_t len) {
//	size_t max = 0;
//	for (size_t i = 0; i < len; i++) {
//		if (tally[i] > max) {
//			max = tally[i];
//		}
//	}
//	return max;
//}

//static size_t tallyindex(const size_t* tally, const size_t len, const size_t val) {
//	for (size_t i = 0; i < len; i++) {
//		if (tally[i] == val) {
//			return i;
//		}
//	}
//	return 0;
//}

void column_popular_init(solver* slvr, algorithm* algo) {
	wbitem* item;
	wbank_foreachitem(item, hcded_dict) {
		wlist_append(slvr -> list_configs[0].list, item -> value);
	}
	slvr -> list_configs[0].standard_filter = 1;
}

void column_popular_larger_init(solver* slvr, algorithm* algo) {
	wbitem* item;
	wbank_foreachitem(item, hcded_dict) {
		wlist_append(slvr -> list_configs[0].list, item -> value);
	}
	wbank_foreachitem(item, valid_words) {
		wlist_append(slvr -> list_configs[0].list, item -> value);
	}
	slvr -> list_configs[0].standard_filter = 1;
}

struct word_score {
	char* word;
	size_t score;
};

static int cmpwscore(const void *a, const void *b) {
	return ((*(struct word_score*)b).score - (*(struct word_score*)a).score);
}

static char* pick_highest_score(wlist* word_list, lcounter** score_counters, const size_t wlen) {
	char* best_pick = NULL;
	size_t max_score = 0;
	wlword* j;
	wlist_foreach(j, word_list) {
		size_t score = 0;
		for (size_t i = 0; i < wlen; i++) {
			size_t letter_count = 0;
			lcounter_cpy(score_counters[i], &letter_count, j -> word[i]);
			score += letter_count;
		}
		if (score > max_score) {
			best_pick = j -> word;
			max_score = score;
		}
	}
	return best_pick;
}

static char* sort_by_score(wlist* word_list, lcounter** score_counters, const size_t wlen) {
	wlword* j;
	size_t count = 0;
	struct word_score scores[word_list -> length];
	wlist_foreach(j, word_list) {
		size_t score = 0;
		for (size_t i = 0; i < wlen; i++) {
			size_t letter_count = 0;
			lcounter_cpy(score_counters[i], &letter_count, j -> word[i]);
			score += letter_count;
		}
		scores[count].word = j -> word;
		scores[count].score = score;
		count++;
	}

	hsort(scores, word_list -> length, sizeof(struct word_score), cmpwscore);

	count = 0;
	wlist_foreach(j, word_list) {
		j -> word = scores[count].word;
		count++;
	}
	return wlist_get(word_list, 0);
}

/*
 * NOTE: Only the list at index 0 is displayed to the user
 */
char* guess_by_freq_cols(gbucket* guess_board, wlist** word_lists, size_t nword_lists, char show_word_list_to_user) {
	if (word_lists[0] -> length == 0) {
		return NULL;
	}
	size_t wlen = gbucket_getlastguess(guess_board) -> length;
	lcounter* score_counters[wlen];
	for (size_t i = 0; i < wlen; i++) {
		score_counters[i] = lcounter_create(NUM_LETTERS);
	}
	wlword* j;
	wlist_foreach(j, word_lists[0]) {
		for (size_t i = 0; i < wlen; i++) {
			size_t current_score = 0;
			lcounter_cpy(score_counters[i], &current_score, j -> word[i]);
			lcounter_set(score_counters[i], j -> word[i], current_score + 1);
		}
	}
//	size_t count = 0;
//	struct word_score scores[word_lists[0] -> length];
//	wlist_foreach(j, word_lists[0]) {
//		size_t score = 0;
//		for (size_t i = 0; i < wlen; i++) {
//			size_t letter_count = 0;
//			lcounter_cpy(score_counters[i], &letter_count, j -> word[i]);
//			score += letter_count;
//		}
//		scores[count].word = j -> word;
//		scores[count].score = score;
//		count++;
//	}
//
//	hsort(scores, word_lists[0] -> length, sizeof(struct word_score), cmpwscore);
//
//	count = 0;
//	wlist_foreach(j, word_lists[0]) {
//		j -> word = scores[count].word;
//		count++;
//	}
//
//	char* best_pick = wlist_get(word_lists[0], 0);
	char* best_pick = show_word_list_to_user ? sort_by_score(word_lists[0], score_counters, wlen) : pick_highest_score(word_lists[0], score_counters, wlen);

	if (nword_lists == 2 && word_lists[0] -> length > 2) {
		best_pick = pick_highest_score(word_lists[1], score_counters, wlen);
//		size_t max_score = 0;
//		wlist_foreach(j, word_lists[1]) {
//			size_t score = 0;
//			for (size_t i = 0; i < wlen; i++) {
//				size_t letter_count = 0;
//				lcounter_cpy(score_counters[i], &letter_count, j -> word[i]);
//				score += letter_count;
//			}
//			if (score > max_score) {
//				best_pick = j -> word;
//				max_score = score;
//			}
//		}
	}

	for (size_t i = 0; i < wlen; i++) {
		lcounter_delete(score_counters[i]);
	}
	return best_pick;
}
