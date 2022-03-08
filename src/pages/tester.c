#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../error/print_error.h"
#include "../picking_algorithm/algorithms.h"
#include "../terminal_helper/ccolours.h"
#include "../terminal_helper/helper_fxs.h"
#include "../terminal_helper/progress_bar.h"
#include "../test_solve/tester.h"
#include "../test_solve/test_algo_1.h"
#include "../utilities/file_util.h"
#include "../utilities/input-helper.h"
#include "../utilities/int_util.h"
#include "../utilities/str_util.h"
#include "../utilities/time_util.h"
#include "../wordle/guess_bucket.h"
#include "../wordle/wordle_logic.h"
#include "../wordle/word_list.h"
#include "../wordle_stats.h"
#include "../word_list/hardcoded_dictionary.h"
#include "../word_list/wbitem.h"
#include "../word_list/word_bank.h"

#include "wlist_interface.h"
#include "homepage.h"

static void lowercase_ascii(char* s) {
	if (s == NULL) return;
	for (; *s != '\0'; s++) {
		if ('A' <= *s && *s <= 'Z') {
			*s += 32;
		}
	}
}

/**
 * Return 1 if exit
 */
int select_word_bank(wlist* list) {
	char* input = NULL;
	while (1) {
		clear_console();
		printf("Test Solver - Select word bank\n");
		printf("Select a word bank by typing the corresponding number.\n\n");
		printf("    1 - %lu Wordle answers\n", (long unsigned) hcded_dict_len);
		printf("    2 - All valid Wordle inputs\n");
		printf("    3 - Previous wordle answers\n");
		printf("    w - Extended word list\n");
		printf("    e - Custom list\n");
		printf("    q - Exit\n\n");
		printf(" >> ");
		free(input);
		input = ask_user();
		lowercase_ascii(input);
		if (input == NULL) {
			continue;
		}
		if (strcmp(input, "q") == 0) {
			free(input);
			return 1;
		}
		if (strcmp(input, "1") == 0) {
			appnd_wordle_ans(list);
			free(input);
			return 0;
		}
		if (strcmp(input, "2") == 0) {
			appnd_wordle_valids(list);
			free(input);
			return 0;
		}
		if (strcmp(input, "3") == 0) {
			appnd_prev_wordle_ans(list);
			free(input);
			return 0;
		}
		if (strcmp(input, "w") == 0) {
			appnd_wordle_valids(list);
			wbitem* item;
			wbank_foreachitem(item, extra_words) {
				wlist_append(list, item -> value);
			}
			free(input);
			return 0;
		}
		if (strcmp(input, "e") == 0) {
			if (get_custom_wlist(list)) {
				return 1;
			}
			free(input);
			return 0;
		}
	}
	return 0;
}

static void print_mode_title() {
	printf("\nTest Solver - Select algorithm\n");
}

static void print_alg_info(int algo) {
	if (algo == 9) {
		pgcg_set_warning_colour();
		printf("    WARNING: Statistical algorithms can be very slow!\n    Especially if it can pick non-hard-mode words!\n    Proceed only if you have the patience :D\n");
		pgcg_reset_colour();
	}
}

static char is_available_alphebet(const char c) {
	return c >= 'a' && c <= 'z';
}

static char all_lc_alpheb(const char* s) {
	for (; *s != '\0'; s++) {
		if (!is_available_alphebet(*s)) {
			return 0;
		}
	}
	return 1;
}

static char is_valid_sw(const char* s) {
	if (!str_islen(s, wordle_word_length)) {
		return 0;
	}
	if (!wordle_in_dictionary(s)) {
		return 0;
	}
	return all_lc_alpheb(s);
}

int select_starting_word(char* buffer) {
	char* input = NULL;
	char invalid_input = 0;
	while (1) {
		clear_console();
		printf("Test Solver - Select starting word\n");
		printf("Type your starting five-letter word below\n");
		printf("or type 'q' to exit.\n");
		switch(invalid_input) {
		case 1:
			pgcg_set_error_colour();
			printf("Invalid input\n");
			pgcg_reset_colour();
			break;
		case 2:
			pgcg_set_error_colour();
			printf("Must be a five-letter word\n");
			pgcg_reset_colour();
		}
		printf(" >> ");
		free(input);
		input = ask_user();
		lowercase_ascii(input);
		if (input == NULL) {
			invalid_input = 1;
			continue;
		}
		if (strcmp(input, "q") == 0) {
			free(input);
			return 1;
		}
		if (!is_valid_sw(input)) {
			invalid_input = all_lc_alpheb(input) ? 2 : 1;
			continue;
		}
		strncpy(buffer, input, wordle_word_length + 1);
		free(input);
		return 0;
	}
	return 0;
}

void testing_screen(const int algo) {
	clear_console();
	print_title_banner();
	switch (algo) {
	case 1:
		printf("\nPerforming test on Most frequent letter per position\n\n");
		break;
	case 2:
		printf("\nPerforming test on Most frequent letter per position (Larger vocabulary)\n\n");
		break;
	case 3:
		printf("\nPerforming test on Statistical Algorithm (Hard mode)\n\n");
		break;
	case 4:
		printf("\nPerforming test on Statistical Algorithm (Hard mode) (Larger vocabulary)\n\n");
		break;
	case 5:
		printf("\nPerforming test on Statistical Algorithm (No hard mode)\n\n");
		break;
	case 6:
		printf("\nPerforming test on Statistical Algorithm (No hard mode) (Resilient)\n\n");
		break;
	case 7:
		printf("\nPerforming test on Statistical Algorithm (No hard mode) (Slightly Optimised)\n\n");
		break;
	case 8:
		printf("\nPerforming test on Statistical Algorithm (No hard mode) (Larger vocabulary) (Slightly Optimised)\n\n");
		break;
	case 9:
		printf("\nPerforming test on Matt Dodge Hybrid Algorithm (No hard mode)\n\n");
		break;
	case 10:
		printf("\nPerforming test on Matt Dodge Hybrid Algorithm (No hard mode) (Larger vocabulary)\n\n");
		break;
	case 11:
		printf("\nPerforming test on Random guess Algorithm\n\n");
		break;
	case 12:
		printf("\nPerforming test on Random guess Algorithm (Larger vocabulary)\n\n");
		break;
	default:
		printf("\nPerforming test on Algorithm ?\n\n");
	}
}

void print_test_line(test_sess* session) {
	printf("\r");
	print_progress_bar_u(session -> progress, session -> total_words, 30);
	printf(" Wordle %u/%u, time ", session -> progress, session -> total_words);
	print_time_format_2dp(session -> time_taken);
	fflush(stdout);
}

static char is_char_digit(const char c) {
	return c >= '0' && c <= '9';
}

static char is_valid_page_command(const char* s) {
	if (s == NULL) {
		return 0;
	}
	if (*s != 'p' && *s != 'P') {
		return 0;
	}
	for (s = s + 1; *s != '\0'; s++) {
		if (!is_char_digit(*s)) {
			return 0;
		}
	}
	return 1;
}

static size_t get_pcommand_num(const char* s) {
	if (strlen(s) <= 1) {
		return 3;
	}
	size_t r = 0;
	for (s = s + 1; *s != '\0'; s++) {
		r += *s - '0';
		r *= 10;
	}
	return r / 10;
}

static int log_summary(test_sess* session, char dark_mode, char high_contrast) {
	clear_console();
	printf("Test Solver - Logging results summary\n\n");
	char* logname = create_log_filename("waidle-result-summary");
	printf("Logging summary to '%s'... ", logname);
	if (test_sess_log_result(session, logname)) {
		printf("failed D:\n");
		pgcg_set_error_colour_stderr();
		fprintf(stderr, "ERROR could not write summary to file '%s'.\n", logname);
		free(logname);
		pgcg_reset_colour_stderr();
		pause_console();
		return 1;
	}
	free(logname);
	printf("Done!\n");
	logname = create_log_filename("waidle-solver-test-results");
	printf("Logging all results to '%s'... ", logname);
	FILE* reslog = fopen(logname, "wb");
	if (reslog == NULL) {
		printf("failed D:\n");
		pgcg_set_error_colour_stderr();
		fprintf(stderr, "ERROR could not write results to file '%s'.\n", logname);
		free(logname);
		pgcg_reset_colour_stderr();
		pause_console();
		return 1;
	}
	free(logname);
	for (size_t i = 0; i < session -> total_words; i++) {
		gbucket_writeres_all_file(session -> boards[i], reslog, dark_mode, high_contrast);
	}
	fclose(reslog);
	printf("Done!\n\n");
	pause_console();
	return 0;
}

static int log_result(test_sess* session, const size_t current_page, const size_t total_pages, char dark_mode, char high_contrast) {
	clear_console();
	printf("Test Solver - Logging result %lu/%lu\n\n", (long unsigned)current_page, (long unsigned)total_pages);
	const size_t pnolen = num_digit(current_page);
	char prefix[pnolen + 27];
	strcpy(prefix, "waidle-solver-test-result-");
	cpynumstr((char*)prefix + 26, current_page, pnolen);
	char* logname = create_log_filename(prefix);
	printf("Logging result %lu to '%s'... ", (long unsigned)current_page, logname);
	FILE* reslog = fopen(logname, "wb");
	if (reslog == NULL) {
		printf("failed D:\n");
		pgcg_set_error_colour_stderr();
		fprintf(stderr, "ERROR could not write results to file '%s'.\n", logname);
		free(logname);
		pgcg_reset_colour_stderr();
		pause_console();
		return 1;
	}
	free(logname);
	gbucket_writeres_all_file(session -> boards[current_page - 1], reslog, dark_mode, high_contrast);
	fclose(reslog);
	printf("Done!\n\n");
	pause_console();
	return 0;
}

/**
 * Return 0 for exit
 */
static int cpy_page_request(test_sess* session, const size_t total_pages, size_t* current_page, char* invalid) {
	printf("Page %lu out of %lu\n", (long unsigned) *current_page, (long unsigned) total_pages - 1);
	if (*current_page != 0) printf("Type '<' or ',' for previous page.\n");
	if (*current_page < total_pages - 1) printf("Type '>' or '.' for next page.\n");
	if (session -> pass_count != session -> total_words) {
		if (*current_page != 0) printf("Type 'fb' to jump to the previous failed test.\n");
		if (*current_page < total_pages - 1) printf("Type 'f' to jump to the next failed test.\n");
	}
	if (*current_page == 0) {
		printf("Type 'p2' for page 2.\n");
		printf("Type 'l', 'lh', 'ld', or 'lhd' to log all results.\n");
	} else {
		printf("Type 'p0' to return to summary.\n");
		printf("Type 'l', 'lh', 'ld', or 'lhd' to log this result.\n");
	}
	printf("Type 'q' to exit\n");
	if (*invalid) {
		pgcg_set_error_colour();
		printf("Invalid input\n");
		pgcg_reset_colour();
		*invalid = 0;
	}
	printf(" >> ");
	char* input = ask_user();
	lowercase_ascii(input);
	if ((input == NULL || strcmp(input, ".") == 0 || strcmp(input, ">") == 0) && *current_page < total_pages - 1) {
		(*current_page)++;
		free(input);
		return 1;
	}
	if (input == NULL) {
		*invalid = 1;
		free(input);
		return 1;
	}
	if (strcmp(input, "q") == 0) {
		free(input);
		return 0;
	}

	if ((strcmp(input, ",") == 0 || strcmp(input, "<") == 0) && *current_page != 0) {
		(*current_page)--;
		free(input);
		return 1;
	}

	if (strcmp(input, "f") == 0 && *current_page < total_pages - 1) {
		pgcg_set_note_colour();
		printf("Searching for fails...\n");
		pgcg_reset_colour();
		*current_page = test_sess_first_unsuccessful(session, *current_page, *current_page - 1) + 1;
		free(input);
		return 1;
	}
	if (strcmp(input, "fb") == 0 && *current_page != 0) {
		pgcg_set_note_colour();
		printf("Searching for fails...\n");
		pgcg_reset_colour();
		*current_page = test_sess_last_unsuccessful(session, *current_page - 2, *current_page - 1) + 1;
		free(input);
		return 1;
	}

	if (strcmp(input, "l") == 0) {
		if (*current_page == 0) {
			log_summary(session, 0, 0);
		} else {
			log_result(session, *current_page, total_pages, 0, 0);
		}
		free(input);
		return 1;
	}

	if (strcmp(input, "lh") == 0) {
		if (*current_page == 0) {
			log_summary(session, 0, 1);
		} else {
			log_result(session, *current_page, total_pages, 0, 1);
		}
		free(input);
		return 1;
	}

	if (strcmp(input, "ld") == 0) {
		if (*current_page == 0) {
			log_summary(session, 1, 0);
		} else {
			log_result(session, *current_page, total_pages, 1, 0);
		}
		free(input);
		return 1;
	}

	if (strcmp(input, "lhd") == 0) {
		if (*current_page == 0) {
			log_summary(session, 1, 1);
		} else {
			log_result(session, *current_page, total_pages, 1, 1);
		}
		free(input);
		return 1;
	}

	if (is_valid_page_command(input)) {
		size_t n = get_pcommand_num(input);
		free(input);
		if (n >= total_pages) {
			*current_page = total_pages - 1;
			return 1;
		}
		*current_page = n;
		return 1;
	}
	pgcg_set_error_colour();
	printf("Invalid input\n");
	pgcg_reset_colour();
	*invalid = 1;
	free(input);
	return 1;
}

/**
 * Return 1 for exit
 */
int results_summary_page(test_sess* session, size_t* current_page, const size_t total_pages, char* invalid) {
	clear_console();
	printf("Test Solver - Results summary\n\n");
	test_sess_print_result(session);
	printf("\n");
	if (!cpy_page_request(session, total_pages, current_page, invalid)) {
		return 1;
	}
	return 0;
}

/**
 * Return 1 for exit
 */
int results_guesses_page(test_sess* session, size_t* current_page, const size_t total_pages, char* invalid) {
	clear_console();
	printf("Test Solver - Guess results\n\n");
	if (session -> boards[*current_page - 1] == NULL) {
		printf("Unable to display result.\n");
		pgcg_set_error_colour_stderr();
		fprintf(stderr, "ERROR: boards[%lu] is null.\n", (long unsigned) *current_page - 1);
		pgcg_reset_colour_stderr();
	} else {
		gbucket_tessresprintall(session -> boards[*current_page - 1], 1);
	}
	printf("\n");
	if (!cpy_page_request(session, total_pages, current_page, invalid)) {
		return 1;
	}
	return 0;
}

/**
 * Return 1 for exit
 */
int results_page_loop(test_sess* session, size_t* current_page, const size_t total_pages, char* invalid) {
	if (*current_page == 0) {
		return results_summary_page(session, current_page, total_pages, invalid);
	}
	return results_guesses_page(session, current_page, total_pages, invalid);
}

void tester_thread() {
	wlist* test_list = wlist_create();
	if (test_list == NULL) {
		print_error_ln("ERROR: No more memory space available.");
		return;
	}
	int algorithm = 0;
	char starting_word[wordle_word_length + 1];
	if (select_word_bank(test_list)) {
		wlist_delete(test_list);
		return;
	}
	if (select_algo_page(print_mode_title, &algorithm, print_alg_info)) {
		wlist_delete(test_list);
		return;
	}
	algorithm++;
	if (select_starting_word(starting_word)) {
		wlist_delete(test_list);
		return;
	}
	testing_screen(algorithm);
	if (test_algo_1_thread(test_list, algorithm, starting_word)) {
		wlist_delete(test_list);
		return;
	}
	wlist_delete(test_list);
}
