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

solver* solver_create(algorithm* sugg_algorithm, char show_word_list_to_user) {
	solver* slvr = malloc(sizeof(solver));
	slvr -> guesses = gbucket_create(wordle_max_guesses, NULL);
	slvr -> status = STATUS_IN_PROGRESS;
	slvr -> suggest_algo = sugg_algorithm;
	slvr -> suggested_word = NULL;
	slvr -> display_word_list = show_word_list_to_user;

	size_t min_word_lists = sugg_algorithm -> min_word_lists;
	slvr -> word_list_config_len = min_word_lists;
	slvr -> list_configs = malloc(sizeof(struct word_list_config) * min_word_lists);
	if (slvr -> list_configs == NULL) {
		gbucket_delete(slvr -> guesses);
		free(slvr);
		return NULL;
	}

	for (size_t i = 0; i < min_word_lists; i++) {
		slvr -> list_configs[i].standard_filter = 0;
		slvr -> list_configs[i].list = wlist_create();
	}

	sugg_algorithm -> init(slvr, sugg_algorithm);
	return slvr;
}

char* current_suggested_word(solver* s) {
	return s -> suggested_word;
}

char open_to_guess(solver* s) {
	return s -> status == STATUS_IN_PROGRESS;
}

static void filter_lists(solver* s, list_lrpair* guess, lcounter* min_letters, lcounter* exact_letters) {
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

	// Tentative
	if (s -> suggest_algo -> guess_made != NULL) {
		s -> suggest_algo -> guess_made(s, s -> suggest_algo);
	}

	wlist* wordlists[(const size_t)(s -> word_list_config_len)];
	for (size_t i = 0; i < s -> word_list_config_len; i++) {
		wordlists[i] = s -> list_configs[i].list;
	}
	s -> suggested_word = s -> suggest_algo -> suggest_word(s -> guesses, wordlists, s -> word_list_config_len, s -> display_word_list);
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
	if (s -> suggest_algo -> cleanup) {
		s -> suggest_algo -> cleanup(s, s -> suggest_algo);
	}
	solver_clear_lists(s);
	free(s);
}
