#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../error/print_error.h"
#include "../terminal_helper/cons_graphics.h"
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

solver* solver_create(char* (*sugg_algo) (gbucket* guess_board, wlist** word_lists, size_t nword_lists), char include_all_valid_words) {
	return solver_create_w_alt_list(sugg_algo, include_all_valid_words, 0, 1, 0);
}

/**
 * For alt_list:
 *   0 - No alternate list
 *   1 - Wordle answers
 *   2 - Valid words
 * Note that only main list will be printed to the user.
 */
solver* solver_create_w_alt_list(char* (*sugg_algo) (gbucket* guess_board, wlist** word_lists, size_t nword_lists), char include_all_valid_words, int alt_list, char filter_main_list, char filter_alt_list) {
	solver* slvr = malloc(sizeof(solver));
	slvr -> guesses = gbucket_create(wordle_max_guesses, NULL);
	slvr -> status = STATUS_IN_PROGRESS;
	slvr -> suggestion_algorithm = sugg_algo;
	slvr -> suggested_word = NULL;

	// New list format
	// So far no evidence of memory leak
	// Alt list segfault-ed
	size_t min_word_lists = alt_list ? 2 : 1;
	slvr -> word_list_config_len = min_word_lists;
	slvr -> list_configs = malloc(sizeof(struct word_list_config) * min_word_lists);
	if (slvr -> list_configs == NULL) {
		gbucket_delete(slvr -> guesses);
		free(slvr);
		return NULL;
	}
//	size_t error_clear_count = 0;
//	for (size_t i = 0; i < min_word_lists; i++) {
//		slvr -> list_configs[i].list = wlist_create();
//		if (slvr -> list_configs[i].list == NULL) {
//			for (size_t j = 0; j < error_clear_count; j++) {
//				wlist_delete(slvr -> list_configs[i].list);
//			}
//			free(slvr -> list_configs);
//			gbucket_delete(slvr -> guesses);
//			free(slvr);
//			print_error_ln("ERROR solver not created: No more memory space available.");
//			return NULL;
//		}
//		slvr -> list_configs[i].standard_filter = 1;
//		error_clear_count++;
//	}

//	if (slvr -> suggest_algo -> init != NULL) {
//		slvr -> suggest_algo -> init(s, slvr -> suggest_algo);
//	}

	wlist* main_list = wlist_create();
	if (main_list == NULL) {
		free(slvr -> list_configs);
		gbucket_delete(slvr -> guesses);
		free(slvr);
		print_error_ln("ERROR solver not created: No more memory space available.");
		return NULL;
	}
	wbitem* item;
	wbank_foreachitem(item, hcded_dict) {
		wlist_append(main_list, item -> value);
	}
	if (include_all_valid_words) {
		wbank_foreachitem(item, valid_words) {
			wlist_append(main_list, item -> value);
		}
	}
	slvr -> list_configs[0].standard_filter = filter_main_list;
	slvr -> list_configs[0].list = main_list;

	wlist* alt_list_ = NULL;
	if (alt_list) {
		alt_list_ = wlist_create();
		if (alt_list_ == NULL) {
			free(main_list);
			free(slvr -> list_configs);
			gbucket_delete(slvr -> guesses);
			free(slvr);
			print_error_ln("ERROR solver not created: No more memory space available.");
			return NULL;
		}
		wbank_foreachitem(item, hcded_dict) {
			wlist_append(alt_list_, item -> value);
		}
		if (alt_list == 2) {
			wbank_foreachitem(item, valid_words) {
				wlist_append(alt_list_, item -> value);
			}
		}
		slvr -> list_configs[1].standard_filter = filter_alt_list;
		slvr -> list_configs[1].list = alt_list_;
	}
	return slvr;
}

char* current_suggested_word(solver* s) {
	return s -> suggested_word;
}

char open_to_guess(solver* s) {
	return s -> status == STATUS_IN_PROGRESS;
}

static void filter_lists(solver* s, list_lrpair* guess, lcounter* min_letters, lcounter* exact_letters/*, cts_hmap* min_counts, cts_hmap* exact_counts*/) {
	struct word_list_config* list_config = s -> list_configs;
	for (size_t i = 0; i < s -> word_list_config_len; i++) {
		if (list_config[i].standard_filter) {
			filter_wlist_by_last_clue(list_config[i].list, gbucket_getlastguess(s -> guesses), min_letters, exact_letters);
		}
	}
}

void enter_guess_result(solver* s, char* word, char* result) {
	if (!open_to_guess(s)) {
		printf("Solver is not open for guessing.\n");
		return;
	}
	gbucket_addguess(s -> guesses, word, result);

	//New format
	filter_lists(s, gbucket_getlastguess(s -> guesses), s -> guesses -> min_letters, s -> guesses -> exact_letters);

	wlist* wordlists[(const size_t)(s -> word_list_config_len)];
	for (size_t i = 0; i < s -> word_list_config_len; i++) {
		wordlists[i] = s -> list_configs[i].list;
	}
	s -> suggested_word = s -> suggestion_algorithm(s -> guesses, wordlists, s -> word_list_config_len);
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

static void solver_clear_lists(solver* s) {
	for (size_t i = 0; i < s -> word_list_config_len; i++) {
		wlist_delete(s -> list_configs[i].list);
	}
	free(s -> list_configs);
}

void solver_delete(solver* s) {
	gbucket_delete(s -> guesses);
	solver_clear_lists(s);
	free(s);
}
