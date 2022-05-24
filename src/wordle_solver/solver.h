#ifndef SUGGEST_H_INCLUDED
#define SUGGEST_H_INCLUDED

#define STATUS_IN_PROGRESS 0
#define STATUS_LOST 1
#define STATUS_WON 2

#include <stdio.h>

#include "../picking_algorithm/algorithms.h"
#include "../wordle/guess_bucket.h"
#include "../wordle/word_list.h"

struct word_list_config {
	wlist* list;
	char standard_filter;
};

typedef struct algo_registry algorithm;

typedef struct wordle_game_solver {
	gbucket* guesses;
	char filter_main_list;
	char filter_alt_list;
	struct word_list_config* list_configs;
	size_t word_list_config_len;
	size_t wlconfig_alloc_size;
	char status;
	char* (*suggestion_algorithm) (gbucket* guess_board, wlist** word_lists, size_t nword_lists);
	algorithm* suggest_algo;
	char* suggested_word;
	char display_word_list;
} solver;

//solver* solver_create(char* (*sugg_algo) (gbucket* guess_board, wlist** word_lists, size_t nword_lists), char include_all_valid_words);

solver* solver_create(algorithm* sugg_algorithm, char show_word_list_to_user);

/**
 * For alt_list:
 *   0 - No alternate list
 *   1 - Wordle answers
 *   2 - Valid words
 */
//solver* solver_create_w_alt_list(char* (*sugg_algo) (gbucket* guess_board, wlist** word_lists, size_t nword_lists), char include_all_valid_words, int alt_list, char filter_main_list, char filter_alt_list);

/**
 * Returns 1 if this is open to guesses, 0 if it is not.
 */
char open_to_guess(solver* s);

char* current_suggested_word(solver* s);

void enter_guess_result(solver* s, char* word, char* result);

/**
 * 0 - In progress
 * 1 - Lost
 * 2 - Won
 */
char get_current_status(solver* s);

void solver_delete(solver* s);

#endif // SUGGEST_H_INCLUDED
