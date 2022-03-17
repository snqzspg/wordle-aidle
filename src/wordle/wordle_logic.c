#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../error/print_error.h"
#include "../sorting/heap_sort.h"
#include "../utilities/get_tz.h"
#include "../utilities/obfuscator.h"
#include "../utilities/str_util.h"
#include "../wordle_stats.h"
#include "../word_list/hardcoded_dictionary.h"
#include "../word_list/wbitem.h"
#include "../word_list/word_bank.h"
#include "wordle_logic.h"
#include "word_list.h"

#define DAY 86400

static void wlgame_removefirstoccur(char* remainbuffer, const char c) {
	size_t count = 0;
	size_t len = strlen(remainbuffer);
	char has_c = 0;
	for (size_t i = 0; i < len; i++) {
		if (remainbuffer[i] == c) {
			has_c = 1;
			break;
		}
		count++;
	}
	for (size_t i = count; i + 1 < len; i++) {
		remainbuffer[i] = remainbuffer[i + 1];
	}
	if (has_c) remainbuffer[len - 1] = '\0';
}

static char wordle_yorb(const char c, char* remaining) {
	if (strchr(remaining, c) == NULL) {
		return 'b';
	}
	wlgame_removefirstoccur(remaining, c);
	return 'y';
}

void wordle_evaluate(const char* guess, const char* solution, char* __restrict__ res_buffer) {
	size_t wlen = strlen(guess);
	char remaining[wlen + 1];
	size_t remaininglen = 0;
	size_t i;
	for (i = 0; i < wlen; i++) {
		if (guess[i] == solution[i]) {
			res_buffer[i] = 'g';
		} else {
			res_buffer[i] = 'u'; // Uncertain
			remaining[remaininglen] = solution[i];
			remaininglen++;
		}
	}
	remaining[remaininglen] = '\0';
	for (i = 0; i < wlen; i++) {
		if (res_buffer[i] == 'g') {
			continue;
		}
		res_buffer[i] = wordle_yorb(guess[i], remaining);
	}
	res_buffer[wlen] = '\0';
}

char wordle_in_dictionary(const char* wrd) {
	return wbank_contain(valid_words, wrd) || wbank_contain(hcded_dict, wrd);
}

size_t wordle_get_tday_num() {
	time_t ct = time(NULL);
	ct += get_timezoneoffset_s();
	if (ct < wordle_word_0) {
		return 0;
	}
//	if (ct >= wordle_word_0 + hcded_dict_len * DAY) {
//		return hcded_dict_len - 1;
//	}
	return (ct - wordle_word_0) / DAY;
}

/**
 * Copies the corresponding word into wbuffer.
 */
void wordle_cpy_day_word(size_t number, char* __restrict__ wbuffer) {
	number %= hcded_dict_len;
	strcpy(wbuffer, hcded_dict_ordered[number]);
	deobfs_str(wbuffer, number);
}

/**
 * Copies today's word into wbuffer.
 * Returns the wordle number.
 */
size_t wordle_cpy_tday_word(char* __restrict__ wbuffer) {
	size_t td = wordle_get_tday_num();
	size_t td_cycle = td  % hcded_dict_len;
	wordle_cpy_day_word(td_cycle, wbuffer);
//	strcpy(wbuffer, hcded_dict_ordered[td_cycle]);
//	deobfs_str(wbuffer, td_cycle);
	return td;
}

/**
 * Copies a random word into wbuffer.
 * Returns the chosen wordle number.
 */
size_t wordle_cpy_rand_word(char* __restrict__ wbuffer) {
	size_t rng = rand() % hcded_dict_len;
	strcpy(wbuffer, hcded_dict_ordered[rng]);
	deobfs_str(wbuffer, rng);
	return rng;
}

static int cmpstr(const void *a, const void *b) {
	return strcmp((*(char**) a), (*(char**) b));
}

static size_t max_w_length(wbank* l) {
	size_t max = 0;
	size_t clen = 0;
	wbitem* i;
	wbank_foreachitem(i, l) {
		clen = strlen(i -> value);
		if (clen > max) {
			max = clen;
		}
	}
	return max;
}

void appnd_wordle_ans(wlist* list) {
	wbitem* item;
	char wrd[max_w_length(hcded_dict) + 1];
	wbank_foreachitem(item, hcded_dict) {
		strcpy(wrd, item -> value);
		lowercase(wrd);
		wlist_append(list, wrd);
	}
	strcpy(wrd, hcded_dict_ordered[0]);
	deobfs_str(wrd, 0);
	wlist_append(list, wrd);
	wlist_removebyi(list, 0);
}

void appnd_prev_wordle_ans(wlist* list) {
	size_t td = wordle_get_tday_num();
	char wrd[hcded_dict_wlen + 1];
	for (size_t i = 1; i < td; i++) {
		strcpy(wrd, hcded_dict_ordered[i % hcded_dict_len]);
		deobfs_str(wrd, i % hcded_dict_len);
		wlist_append(list, wrd);
	}
}

void appnd_wordle_valids(wlist* list) {
	wbitem* item;
	char wrd[max_w_length(hcded_dict) + 1];
	wbank_foreachitem(item, hcded_dict) {
		strcpy(wrd, item -> value);
		lowercase(wrd);
		wlist_append(list, wrd);
	}
	wbank_foreachitem(item, valid_words) {
		strcpy(wrd, item -> value);
		lowercase(wrd);
		wlist_append(list, wrd);
	}
	size_t warrlen = list -> length;
	char** words_arr = malloc(sizeof(char*) * warrlen);
	if (words_arr == NULL) {
		print_err_warning_ln("WARNING Could not allocate enough memory for sorting.");
		return;
	}
	for (size_t i = 0; i < warrlen; i++) {
		words_arr[i] = NULL;
	}
	wlword* j;
	size_t counter = 0;
	wlist_foreach(j, list) {
		words_arr[counter] = j -> word;
		counter++;
	}

	hsort(words_arr, warrlen, sizeof(char*), cmpstr);

	counter = 0;
	wlist_foreach(j, list) {
		j -> word = words_arr[counter];
		counter++;
	}
	free(words_arr);
}
