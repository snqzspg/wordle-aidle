#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "../utilities/hashmap.h"
#include "../wordle/guess_bucket.h"
#include "../wordle/letter_counter.h"
#include "../wordle/word_list.h"

static size_t strcount(char* s, char c) {
	size_t count = 0;
	while (*s != '\0') {
		if (*s == c) {
			count++;
		}
		s++;
	}
	return count;
}

struct gyletter_to_pos {
	char letter;
	size_t pos;
};

/**
 * TODO To fix a bug where a letter appear twice in the guess, both are either green or yellow.
 * The filter fails to recognise that there must be at least two of those letter.
 * Note to self: to include another hash map cts_hmap to count the min letters required.
 * These will be determined by the yellow and green letters, just like how black letters determines the maximum number of a letter.
 *
 * Because I've spent too much time on this, I shall not fix this as it is quite involved.
 * This may be pushed to a later time, probably where the hype of wordle dies down.
 */
static char word_pass_green_letters(char* s, struct gyletter_to_pos* gyl_pos, size_t gyl_pos_len, lcounter* min_letters/*, cts_hmap* min_counts*/) {
	size_t i;
	for (i = 0; i < gyl_pos_len; i++) {
		// letter is represented by (gyl_pos[i].letter)
		// position (from 0 - 4) is represented by (gyl_pos[i].pos)
		// if letter != position -> fail green test
		if (s[gyl_pos[i].pos] != gyl_pos[i].letter) {
			return 0;
		}
	}
	lcounter_foreach(i, min_letters) {
//	cts_hmap_foreach(i, min_counts) {
//		if (strcount(s, min_counts -> items[i] -> key) < min_counts -> items[i] -> value) {
		if (strcount(s, min_letters -> data[min_letters -> logged_indices[i]].letter) < min_letters -> data[min_letters -> logged_indices[i]].count) {
			return 0;
		}
	}
	return 1;
}

static char word_pass_yellow_letters(char* s, struct gyletter_to_pos* gyl_pos, size_t gyl_pos_len, lcounter* min_letters/*, cts_hmap* min_counts*/) {
	size_t i;
	for (i = 0; i < gyl_pos_len; i++) {
		// letter is represented by (gyl_pos[i].letter)
		// position (from 0 - 4) is represented by (gyl_pos[i].pos)
		// if letter == position -> fail yellow test
		if (s[gyl_pos[i].pos] == gyl_pos[i].letter) {
			return 0;
		}
	}
	lcounter_foreach(i, min_letters) {
//	cts_hmap_foreach(i, min_counts) {
		// letter is represented by (gyl_pos[i].letter)
		// <del>if string don't have letter -> fail yellow test</del>
		// UPDATE: if letter count < min letter count in map -> fail yellow test.
		if (strcount(s, min_letters -> data[min_letters -> logged_indices[i]].letter) < min_letters -> data[min_letters -> logged_indices[i]].count) {
			return 0;
		}
	}
	return 1;
}

static size_t max_w_length(wlist* l) {
	size_t max = 0;
	size_t clen = 0;
	wlword* i;
	wlist_foreach(i, l) {
		clen = strlen(i -> word);
		if (clen > max) {
			max = clen;
		}
	}
	return max;
}

static void filter_gy_letters(wlist* l, list_lrpair* guess, char result, lcounter* min_letters/*, cts_hmap* min_counts*/) {
	if (l -> length == 0) {
		return;
	}
	size_t wlen = max_w_length(l);
	struct gyletter_to_pos gyl_to_pos[wlen];
	size_t filled_len = 0;
	size_t i;
	list_lrpair_foreach(i, guess) {
		if (list_lrpair_getresult(guess, i) == result) {
			gyl_to_pos[filled_len].letter = list_lrpair_getletter(guess, i);
			gyl_to_pos[filled_len].pos = i;
			filled_len++;
		}
	}
	wlword* j;
	wlword* tmp;
	switch (result) {
	case 'g':
		j = l -> first_item;
		while (j != NULL) {
			tmp = j -> next;
			if (!word_pass_green_letters(j -> word, gyl_to_pos, filled_len, min_letters)) {
				wlist_removewl(l, j);
			}
			j = tmp;
		}
		break;
	case 'y':
		j = l -> first_item;
		while (j != NULL) {
			tmp = j -> next;
			if (!word_pass_yellow_letters(j -> word, gyl_to_pos, filled_len, min_letters)) {
				wlist_removewl(l, j);
			}
			j = tmp;
		}
	}
}

static char word_pass_black_letters(char* s, lcounter* exact_letters/*, cts_hmap* l_counts*/) {
	size_t i;
	lcounter_foreach(i, exact_letters) {
//		printf("DEBUG Letter %c\n", exact_letters -> data[exact_letters -> logged_indices[i]].letter);
//		printf("DEBUG count %c\n", exact_letters -> data[exact_letters -> logged_indices[i]].count);
//		system("pause");
//	cts_hmap_foreach(i, exact_letters) {
		// letter is represented by (exact_letters -> data[exact_letters -> logged_indices[i]] -> letter)
		// max occurrence is represented by (exact_letters -> data[exact_letters -> logged_indices[i]] -> count)
		// if occurrence != max occurrence -> fail black test
		if (strcount(s, exact_letters -> data[exact_letters -> logged_indices[i]].letter) != exact_letters -> data[exact_letters -> logged_indices[i]].count) {
			return 0;
		}
	}
	return 1;
}

static void filter_black_letters(wlist* l, lcounter* exact_letters/*, cts_hmap* l_counts*/) {
	wlword* j = l -> first_item;
	wlword* tmp;
	while (j != NULL) {
		tmp = j -> next;
		if (!word_pass_black_letters(j -> word, exact_letters)) {
			wlist_removewl(l, j);
		}
		j = tmp;
	}
}

/**
 * Mutate the list by cumulatively filtering out the words from a pre-filted that does not match the patter given by wordle.
 */
void filter_wlist_by_last_clue(wlist* l, list_lrpair* guess, lcounter* min_letters, lcounter* exact_letters/*, cts_hmap* min_counts, cts_hmap* exact_counts*/) {
	filter_gy_letters(l, guess, 'g', min_letters);
	filter_black_letters(l, exact_letters);
	filter_gy_letters(l, guess, 'y', min_letters);
}

void print_possible_words(wlist* l, size_t cols, const char* prefix) {
	size_t i = 0;
	wlword* o;
	if (l -> length > 0) printf("%s", prefix);
	wlist_foreach(o, l) {
		printf("%s", o -> word);
		i++;
		printf("%c%s", i % cols ? ' ' : '\n', i % cols ? "" : prefix);
	}
	if (i % cols) {
		printf("\n");
	}
	printf("\n");
}
