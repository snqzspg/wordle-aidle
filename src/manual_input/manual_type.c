#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../about.h"
#include "../error/print_error.h"
#include "../picking_algorithm/algorithms.h"
#include "../picking_algorithm/highest_information_entropy.h"
#include "../picking_algorithm/hybrid.h"
#include "../picking_algorithm/most-frequent-in-column.h"
#include "../picking_algorithm/random_pick.h"
#include "../terminal_helper/cons_graphics.h"
#include "../terminal_helper/helper_fxs.h"
#include "../utilities/input-helper.h"
#include "../utilities/str_util.h"
#include "../wordle/guess_bucket.h"
#include "../wordle/wordle_logic.h"
#include "../wordle/word_list.h"
#include "../wordle_filter/list_words_by_clue.h"
#include "../word_list/hardcoded_dictionary.h"
#include "../word_list/word_bank.h"
#include "../wordle_stats.h"
#include "../wordle_solver/solver.h"

#define congratsmsglen 4
#define sorrymsglen 5

static char congratsmsg[congratsmsglen][10] = {
	"Congrats!",
	"Awesome!",
	"Nice!",
	"Fuiyoh!"
};

static char sorrymsg[sorrymsglen][49] = {
	"I am unfortunately unable to guess in 6 tries...",
	"Sorry, I can't do it...",
	"Sorry, I'm out of words...",
	"Haiyaa, this Wordle is a bit too tough...",
	"Do check if you've gave the correct colours..."
};

void print_suggestion_page(solver* s) {
	printf("The suggested word to try is:\n\n");
	printf("    %s\n\n", current_suggested_word(s));
	if (s -> list_configs[0].list -> length > 1) {
		printf("Type l to view all %lu possible answers.\n[Type l4 for 4 columns]\n", (long unsigned int) (s -> list_configs[0].list -> length));
	}
}

void print_version_banner() {
	printf("Version: %s\n", version);
}

static void print_mode_title() {
	printf("\nManual enter mode - Instructions\n");
	print_version_banner();
}

static void print_cat_sel_mode_title() {
	printf("\nManual enter mode - Algorithm categories\n");
	print_version_banner();
}

static void print_alg_sel_mode_title(const char* cat_name) {
	printf("\nManual enter mode - Select an algorithm\n");
	print_version_banner();
}

void print_intro_page() {
	clear_console();
	print_mode_title();
	printf("\n");
	printf("First, enter a starting word in Wordle.\n");
	printf("Then, look at the results.\n");
	printf("Enter your guess and the result in the format below:\n");
	printf("[word result]\n");
	printf("No square brackets!\n");
	printf("Where\n");
	print_wraped_linef("word - the word you've entered into wordle.", 1, PGINDENT);
	print_wraped_linef("result - the result of the entry as a word, following the format:", 1, PGINDENT);
	print_wraped_linef("b - black/grey - letter is not in the answer", 1, 9 + PGINDENT);
	print_wraped_linef("y - yellow - letter is in the answer but wrong spot", 1, 9 + PGINDENT);
	print_wraped_linef("g - green - letter is in the answer and in the correct spot\n", 1, 9 + PGINDENT);
	printf("Example: weary gbbbb\n");
	printf("Type your first result to continue, or 'q' to exit.\n");
}

char* prompt_input(char first_guess) {
	printf("word result >> ");
	return ask_user();
}

char is_valid_guess_result(const char* s) {
	if (!str_islen(s, wordle_word_length * 2 + 1)) {
		return 0;
	}
	size_t slen = strlen(s);
	if (!(slen % 2)) {
		return 0;
	}
	size_t wlen = (slen - 1) / 2;
	if (s[wlen] != ' ') {
		return 0;
	}
	char word[wlen + 1];
	for (size_t i = 0; i < wlen; i++) {
		word[i] = s[i];
	}
	word[wlen] = '\0';
	if (!wordle_in_dictionary(word)) {
		return 0;
	}
	for (s = s + wlen + 1; *s != '\0'; s++) {
		if (strchr("bgy", *s) == NULL) {
			return 0;
		}
	}
	return 1;
}

char is_char_digit(const char c) {
	return c >= '0' && c <= '9';
}

char is_valid_list_command(const char* s) {
	if (s == NULL) {
		return 0;
	}
	if (*s != 'l' && *s != 'L') {
		return 0;
	}
	for (s = s + 1; *s != '\0'; s++) {
		if (!is_char_digit(*s)) {
			return 0;
		}
	}
	return 1;
}

size_t get_lcommand_cols(const char* s) {
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

void print_game_page(solver* s, char suggestion) {
	clear_console();
	printf("\nManual enter mode\n");
	print_version_banner();
	printf("\n");
	gbucket_printall(s -> guesses);
	if (suggestion) {
		printf("\n");
		print_suggestion_page(s);
	}
}

void process_answer(solver* s, char* input) {
	size_t inlen = (strlen(input) - 1) / 2;
	char inword[inlen + 1];
	inword[inlen] = '\0';
	for (size_t i = 0; i < inlen; i++) {
		inword[i] = input[i];
	}
	input += inlen + 1;
	enter_guess_result(s, inword, input);
}

void list_all_words(solver* s, size_t cols) {
	clear_console();
	if (s -> list_configs[0].list -> length > 1) {
		printf("\nListing %lu possible words\n\n", (long unsigned) s -> list_configs[0].list -> length);
	} else {
		printf("\nListing the word left\n\n");
	}
	print_possible_words(s -> list_configs[0].list, cols, "    ");
	printf("\n");
	pause_console();
}

/**
 * Return 1 (true) if user choses to quit, 0 (false) otherwise
 */
static char first_entry(solver* slver) {
	char* input = NULL;
	char invalid = 0;
	while (input == NULL || !is_valid_guess_result(input)) {
		print_intro_page();
		pgcg_set_error_colour();
		if (invalid) printf("The input given was invalid; please follow the instructions above.\n");
		pgcg_reset_colour();
		free(input);
		input = prompt_input(1);
		if (input != NULL && !strcmp(input, "q")) {
			free(input);
			return 1;
		}
		invalid = 1;
	}
	process_answer(slver, input);
	free(input);
	return 0;
}

/**
 * Return 1 (true) if user choses to quit, 0 (false) otherwise
 */
static char subsequent_entries(solver* slver) {
	char* input = NULL;
	char invalid_input = 0;
	while (1) {
		if (!open_to_guess(slver)) {
			print_game_page(slver, 0);
			break;
		}
		while (input == NULL || !is_valid_guess_result(input)) {
			print_game_page(slver, 1);
			pgcg_set_error_colour();
			if (invalid_input) printf("The input given was invalid.\nFormat example: weary gbbbb\n");
			pgcg_reset_colour();
			free(input);
			input = prompt_input(0);
			if (input != NULL && !strcmp(input, "q")) {
				free(input);
				return 1;
			}
			if (is_valid_list_command(input)) {
				list_all_words(slver, get_lcommand_cols(input));
				invalid_input = 0;
			} else {
				invalid_input = 1;
			}
		}
		process_answer(slver, input);
		invalid_input = 0;
		free(input);
		input = NULL;
	}
	free(input);
	return 0;
}

void option1_thread() {
//	alcat* category = NULL;
	algorithm* algo = NULL;
	if (select_cat_page(print_cat_sel_mode_title, print_alg_sel_mode_title, "", "", &algo, NULL)) {
		return;
	}
//	if (select_algo_by_cat_page(category, print_mode_title, "", "", &algo, NULL)) {
//		return;
//	}
	solver* w_solver = solver_create(algo, 1);
//	int alg = 0;
//	if (select_algo_page(print_mode_title, &alg, print_algo_info)) {
//		return;
//	}
//	solver* w_solver = NULL;
//	switch (alg) {
//	case 11:
////		w_solver = solver_create(guess_randomly, 1);
//		w_solver = solver_create(random_pick_larger);
//		break;
//	case 10:
////		w_solver = solver_create(guess_randomly, 0);
//		w_solver = solver_create(random_pick);
//		break;
//	case 9:
////		w_solver = solver_create_w_alt_list(guess_by_information_freq_hybrid, 1, 2, 1, 0);
//		w_solver = solver_create(matt_dodge_hybrid_larger);
//		break;
//	case 8:
////		w_solver = solver_create_w_alt_list(guess_by_information_freq_hybrid, 0, 2, 1, 0);
//		w_solver = solver_create(matt_dodge_hybrid);
//		break;
//	case 7:
////		w_solver = solver_create_w_alt_list(guess_by_information_entropy_optimised_level_1, 1, 2, 1, 0);
//		w_solver = solver_create(information_theory_slightly_optimised_larger);
//		break;
//	case 6:
////		w_solver = solver_create_w_alt_list(guess_by_information_entropy_optimised_level_1, 0, 2, 1, 0);
//		w_solver = solver_create(information_theory_slightly_optimised);
//		break;
//	case 5:
////		w_solver = solver_create_w_alt_list(guess_by_information_entropy, 1, 2, 1, 0);
//		w_solver = solver_create(information_theory_larger);
//		break;
//	case 4:
////		w_solver = solver_create_w_alt_list(guess_by_information_entropy, 0, 2, 1, 0);
//		w_solver = solver_create(information_theory);
//		break;
//	case 3:
////		w_solver = solver_create_w_alt_list(guess_by_information_entropy, 1, 2, 1, 1);
//		w_solver = solver_create(information_theory_hard_larger);
//		break;
//	case 2:
////		w_solver = solver_create_w_alt_list(guess_by_information_entropy, 0, 2, 1, 1);
//		w_solver = solver_create(information_theory_hard);
//		break;
//	default:
////		w_solver = solver_create(guess_by_freq_cols, alg);
//		w_solver = solver_create(alg == 1 ? column_popular_larger_vocab : column_popular);
//	}
	if (w_solver == NULL) {
		print_error_ln("Can't start game: not enough memory.");
		pause_console();
		return;
	}
	if (first_entry(w_solver) || subsequent_entries(w_solver)) {
		solver_delete(w_solver);
		return;
	}
	switch (w_solver -> status) {
	case STATUS_WON:
		printf("%s\n", congratsmsg[rand() % congratsmsglen]);
		break;
	case STATUS_LOST:
		pgcg_set_error_colour();
		printf("%s\n", sorrymsg[rand() % sorrymsglen]);
		pgcg_reset_colour();
	}
	solver_delete(w_solver);
	pause_console();
}
