#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../sorting/heap_sort.h"
#include "../utilities/hashmap.h"
#include "../wordle/guess_bucket.h"
#include "../wordle/word_list.h"

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

struct word_score {
	char* word;
	size_t score;
};

static int cmpwscore(const void *a, const void *b) {
	return ((*(struct word_score*)b).score - (*(struct word_score*)a).score);
}

/*
 * NOTE: alt_list could be NULL
 */
char* guess_by_freq_cols(wlist* l, gbucket* g, wlist* alt_list) {
	if (l -> length == 0) {
		return NULL;
	}
	size_t wlen = gbucket_getlastguess(g) -> length;
	cts_hmap* d[wlen];
	for (size_t i = 0; i < wlen; i++) {
		d[i] = cts_hmap_createhashmap();
	}
	wlword* j;
	wlist_foreach(j, l) {
		for (size_t i = 0; i < wlen; i++) {
			cts_hmap_set(d[i], j -> word[i], cts_hmap_getwdefault(d[i], j -> word[i], 0) + 1);
		}
	}
	size_t count = 0;
	//size_t score_tally[l -> length];
	struct word_score scores[l -> length];
	wlist_foreach(j, l) {
		size_t score = 0;
		for (size_t i = 0; i < wlen; i++) {
			score += cts_hmap_getwdefault(d[i], j -> word[i], 0);
		}
		scores[count].word = j -> word;
		scores[count].score = score;
		//score_tally[count] = score;
		count++;
	}

	hsort(scores, l -> length, sizeof(struct word_score), cmpwscore);

	count = 0;
	wlist_foreach(j, l) {
		j -> word = scores[count].word;
		count++;
	}

	char* best_pick = wlist_get(l, 0);

	if (alt_list != NULL && l -> length > 2) {
		size_t max_score = 0;
		wlist_foreach(j, alt_list) {
			size_t score = 0;
			for (size_t i = 0; i < wlen; i++) {
				score += cts_hmap_getwdefault(d[i], j -> word[i], 0);
			}
			if (score > max_score) {
				best_pick = j -> word;
				max_score = score;
			}
		}
	}

	for (size_t i = 0; i < wlen; i++) {
		cts_hmap_deletehashmap(d[i]);
	}
	return best_pick;
	//return wlist_get(l, tallyindex(score_tally, l -> length, getmax(score_tally, l -> length)));
}
