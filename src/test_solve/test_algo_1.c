#include <stdio.h>
#include <stdlib.h>

#include "../error/print_error.h"
#include "../pages/tester.h"
#include "../picking_algorithm/algorithms.h"
#include "../picking_algorithm/highest_information_entropy.h"
#include "../picking_algorithm/hybrid.h"
#include "../picking_algorithm/most-frequent-in-column.h"
#include "../picking_algorithm/random_pick.h"
#include "../terminal_helper/cons_graphics.h"
#include "../terminal_helper/helper_fxs.h"
#include "../wordle_solver/solver.h"
#include "../wordle/word_list.h"
#include "../word_list/hardcoded_dictionary.h"
#include "../word_list/wbitem.h"
#include "../word_list/word_bank.h"

#include "tester.h"

static void* create_solver_1(algorithm* algo, test_sess* session) {
	return solver_create(algo, 0);
}

static char is_solver_1_open(void* s, test_sess* session) {
	return open_to_guess((solver*)s);
}

static const char* solver_1_suggest(void* s, test_sess* session) {
	return current_suggested_word((solver*)s);
}

static void solver_1_receive_result(void* s, test_sess* session, char* word, char* result) {
	enter_guess_result((solver*)s, word, result);
}

static void delete_solver_1(void* s, test_sess* session) {
	solver_delete((solver*)s);
}

/**
 * Return 0 if success.
 */
int test_algo_1_thread(wlist* word_list, algorithm* algo, const char* starting_word) {
	test_sess* session = test_sess_create(word_list, algo, starting_word, create_solver_1, is_solver_1_open, solver_1_suggest, solver_1_receive_result, delete_solver_1);
	if (session == NULL) {
		print_error_ln("ERROR: No more memory space available.");
		return 1;
	}
	test_sess_execute_singlethread(session, print_test_line);
	const size_t total_pages = session -> total_words + 1;
	size_t current_page = 0;
	char invalid = 0;
	while (!results_page_loop(session, &current_page, total_pages, &invalid));
	test_sess_delete(session);
	return 0;
}
