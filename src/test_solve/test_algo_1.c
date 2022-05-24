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
//	return solver_create(guess_by_freq_cols, 0);
	return solver_create(algo, 0);
//	return solver_create_w_alt_list(guess_by_freq_cols, 0, 2, 1, 0);
}

//static void* create_solver_2(test_sess* session) {
////	return solver_create(guess_by_freq_cols, 1);
//	return solver_create(column_popular_larger_vocab);
//}
//
//static void* create_solver_3(test_sess* session) {
////	return solver_create_w_alt_list(guess_by_information_entropy, 0, 2, 1, 1);
//	return solver_create(information_theory_hard);
//}
//
//static void* create_solver_4(test_sess* session) {
////	return solver_create_w_alt_list(guess_by_information_entropy, 1, 2, 1, 1);
//	return solver_create(information_theory_hard_larger);
//}
//
//static void* create_solver_5(test_sess* session) {
////	return solver_create_w_alt_list(guess_by_information_entropy, 0, 2, 1, 0);
//	return solver_create(information_theory);
//}
//
//static void* create_solver_6(test_sess* session) {
////	return solver_create_w_alt_list(guess_by_information_entropy, 1, 2, 1, 0);
//	return solver_create(information_theory_larger);
//}
//
//static void* create_solver_7(test_sess* session) {
////	return solver_create_w_alt_list(guess_by_information_entropy_optimised_level_1, 0, 2, 1, 0);
//	return solver_create(matt_dodge_hybrid);
//}
//
//static void* create_solver_8(test_sess* session) {
////	return solver_create_w_alt_list(guess_by_information_entropy_optimised_level_1, 1, 2, 1, 0);
//	return solver_create(matt_dodge_hybrid_larger);
//}
//
//static void* create_solver_9(test_sess* session) {
////	return solver_create_w_alt_list(guess_by_information_freq_hybrid, 0, 2, 1, 0);
//	return solver_create(matt_dodge_hybrid);
//}
//
//static void* create_solver_10(test_sess* session) {
////	return solver_create_w_alt_list(guess_by_information_freq_hybrid, 1, 2, 1, 0);
//	return solver_create(matt_dodge_hybrid_larger);
//}
//
//static void* create_solver_11(test_sess* session) {
////	return solver_create(guess_randomly, 0);
//	return solver_create(random_pick);
//}
//
//static void* create_solver_12(test_sess* session) {
////	return solver_create(guess_randomly, 1);
//	return solver_create(random_pick_larger);
//}

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
int test_algo_1_thread(wlist* word_list, /*const int*/ algorithm* algo, const char* starting_word) {
//	void* (*c_slvr_fx) (test_sess* session);
//	switch (algo) {
//	case 12:
//		c_slvr_fx = create_solver_12;
//		break;
//	case 11:
//		c_slvr_fx = create_solver_11;
//		break;
//	case 10:
//		c_slvr_fx = create_solver_10;
//		break;
//	case 9:
//		c_slvr_fx = create_solver_9;
//		break;
//	case 8:
//		c_slvr_fx = create_solver_8;
//		break;
//	case 7:
//		c_slvr_fx = create_solver_7;
//		break;
//	case 6:
//		c_slvr_fx = create_solver_6;
//		break;
//	case 5:
//		c_slvr_fx = create_solver_5;
//		break;
//	case 4:
//		c_slvr_fx = create_solver_4;
//		break;
//	case 3:
//		c_slvr_fx = create_solver_3;
//		break;
//	case 2:
//		c_slvr_fx = create_solver_2;
//		break;
//	default:
//		c_slvr_fx = create_solver_1;
//		break;
//	}
//	const char* algo_name = algorithm_names[algo - 1];
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
