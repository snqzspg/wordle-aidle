#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../error/print_error.h"
#include "../terminal_helper/ccolours.h"
#include "../wordle/guess_bucket.h"
#include "../wordle/word_list.h"
#include "../wordle_filter/list_words_by_clue.h"
#include "../wordle_stats.h"
#include "../word_list/hardcoded_dictionary.h"
#include "../word_list/wbitem.h"
#include "../word_list/word_bank.h"
#include "solver.h"

void solver_request_additional_wlist(solver* slvr) {
	if (slvr -> word_list_config_len + 1 > slvr -> wlconfig_alloc_size) {
		slvr -> wlconfig_alloc_size *= 2;
		slvr -> list_configs = realloc(slvr -> list_configs, sizeof(struct word_list_config) * slvr -> wlconfig_alloc_size);
	}
	size_t i = slvr -> word_list_config_len;
	slvr -> list_configs[i].list = wlist_create();
	slvr -> word_list_config_len++;
}

solver* solver_create(char* (*sugg_algo) (wlist* l, gbucket* g, wlist* altl), char include_all_valid_words) {
	return solver_create_w_alt_list(sugg_algo, include_all_valid_words, 0, 1, 0);
}

/**
 * For alt_list:
 *   0 - No alternate list
 *   1 - Wordle answers
 *   2 - Valid words
 * Note that only main list will be printed to the user.
 */
solver* solver_create_w_alt_list(char* (*sugg_algo) (wlist* l, gbucket* g, wlist* altl), char include_all_valid_words, int alt_list, char filter_main_list, char filter_alt_list) {
	solver* slvr = malloc(sizeof(solver));
	slvr -> guesses = gbucket_create(wordle_max_guesses, NULL);
	slvr -> words_remaining = wlist_create();
	if (slvr -> guesses == NULL || slvr -> words_remaining == NULL) {
		print_error_ln("ERROR solver not created: No more memory space available.");
		return NULL;
	}
	slvr -> status = STATUS_IN_PROGRESS;
	slvr -> suggestion_algorithm = sugg_algo;
	slvr -> suggested_word = NULL;
	wbitem* item;
	wbank_foreachitem(item, hcded_dict) {
		wlist_append(slvr -> words_remaining, item -> value);
	}
	if (include_all_valid_words) {
		wbank_foreachitem(item, valid_words) {
			wlist_append(slvr -> words_remaining, item -> value);
		}
	}
	slvr -> filter_main_list = filter_main_list;
	if (alt_list) {
		slvr -> words_remaining_2 = wlist_create();
		if (slvr -> words_remaining_2 == NULL) {
			print_error_ln("ERROR solver not created: No more memory space available.");
			free(slvr -> guesses);
			wlist_delete(slvr -> words_remaining);
			return NULL;
		}
		wbank_foreachitem(item, hcded_dict) {
			wlist_append(slvr -> words_remaining_2, item -> value);
		}
		if (alt_list == 2) {
			wbank_foreachitem(item, valid_words) {
				wlist_append(slvr -> words_remaining_2, item -> value);
			}
		}
		slvr -> filter_alt_list = filter_alt_list;
	} else {
		slvr -> words_remaining_2 = NULL;
		slvr -> filter_alt_list = 0;
	}
	return slvr;
}

char* current_suggested_word(solver* s) {
	return s -> suggested_word;
}

char open_to_guess(solver* s) {
	return s -> status == STATUS_IN_PROGRESS;
}

void enter_guess_result(solver* s, char* word, char* result) {
	if (!open_to_guess(s)) {
		printf("Solver is not open for guessing.\n");
		return;
	}
	gbucket_addguess(s -> guesses, word, result);
	if (s -> filter_main_list) {
		filter_wlist_by_last_clue(s -> words_remaining, gbucket_getlastguess(s -> guesses), s -> guesses -> min_letters, s -> guesses -> max_letters);
	}
	if (s -> filter_alt_list && s -> words_remaining_2 != NULL) {
		filter_wlist_by_last_clue(s -> words_remaining_2, gbucket_getlastguess(s -> guesses), s -> guesses -> min_letters, s -> guesses -> max_letters);
	}
	s -> suggested_word = s -> suggestion_algorithm(s -> words_remaining, s -> guesses, s -> words_remaining_2);
	if (s -> suggested_word == NULL) {
		s -> status = STATUS_LOST;
	}
	if (gbucket_won(s -> guesses)) {
		s -> status = STATUS_WON;
	}
	if (gbucket_lost(s -> guesses)) {
		s -> status = STATUS_LOST;
	}
}

char get_current_status(solver* s) {
	return s -> status;
}

void solver_delete(solver* s) {
	gbucket_delete(s -> guesses);
	wlist_delete(s -> words_remaining);
	wlist_delete(s -> words_remaining_2);
	free(s);
}
