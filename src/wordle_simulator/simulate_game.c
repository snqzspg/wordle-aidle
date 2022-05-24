#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../about.h"
#include "../error/print_error.h"
#include "../settings/settings.h"
#include "../terminal_helper/cons_graphics.h"
#include "../terminal_helper/helper_fxs.h"
#include "../utilities/get_tz.h"
#include "../utilities/input-helper.h"
#include "../utilities/int_util.h"
#include "../utilities/obfuscator.h"
#include "../utilities/str_util.h"
#include "../wordle/guess_bucket.h"
#include "../wordle/wlgame.h"
#include "../wordle/wordle_logic.h"
#include "../wordle_stats.h"
#include "../word_list/hardcoded_dictionary.h"
#include "../word_list/word_bank.h"

#define DAY 86400

static const size_t banner_lines_count = 5;
static const char banner_lines[5][83] = {
	" ____  __  _  _  _  _  __     __  ____  ____    _  _   __  ____  ____  __    ____ ",
	"/ ___)(  )( \\/ )/ )( \\(  )   / _\\(_  _)(  __)  / )( \\ /  \\(  _ \\(    \\(  )  (  __)",
	"\\___ \\ )( / \\/ \\) \\/ (/ (_/\\/    \\ )(   ) _)   \\ /\\ /(  O ))   / ) D (/ (_/\\ ) _) ",
	"(____/(__)\\_)(_/\\____/\\____/\\_/\\_/(__) (____)  (_/\\_) \\__/(__\\_)(____/\\____/(____)",
	"(ASCII Art: Simulate Wordle)"
};

static const size_t displaimer_lines_count = 5;
static const char displaimer_lines[5][68] = {
	" ____   __   ____   ___  __     __    __   _  _  ____  ____   _   ",
	"(    \\ (  ) / ___) / __)(  )   / _\\  (  ) ( \\/ )(  __)(  _ \\ / \\  ",
	" ) D (  )(  \\___ \\( (__ / (_/\\/    \\  )(  / \\/ \\ ) _)  )   / \\_/  ",
	"(____/ (__) (____/ \\___)\\____/\\_/\\_/ (__) \\_)(_/(____)(__\\_) (_)  ",
	"(ASCII Art: DISCLAIMER!)"
};

//static void print_fitted_banner(char[][] lines, size_t count) {
//	int cols = pgcg_get_console_cols() - 1;
//	for (; count > 0; count--) {
//		printf("%.*s\n", cols, lines[count - 1]);
//	}
//}

static void sim_game_print_banner() {
	int cols = pgcg_get_console_cols() - 1;
//	printf("%.*s\n%.*s\n%.*s\n%.*s\n", cols, banner_lines[0], cols, banner_lines[1], cols, banner_lines[2], cols, banner_lines[3]);
//	printf("%s\n", banner_lines[4]);
	for (size_t i = 0; i < banner_lines_count; i++) {
		printf("%.*s\n", cols, banner_lines[i]);
	}
}

static void sim_game_print_disclaimer_banner() {
	int cols = pgcg_get_console_cols() - 1;
//	printf("%.*s\n%.*s\n%.*s\n%.*s\n", cols, displaimer_lines[0], cols, displaimer_lines[1], cols, displaimer_lines[2], cols, displaimer_lines[3]);
//	printf("%s\n", displaimer_lines[4]);
	pgcg_set_warning_colour();
	for (size_t i = 0; i < displaimer_lines_count; i++) {
		printf("%.*s\n", cols, displaimer_lines[i]);
	}
	pgcg_reset_colour();
}

static void sim_game_print_version_banner() {
	printf("Version: %s\n", version);
}

static char tday_word_expired() {
	return time(NULL) + get_timezoneoffset_s() < wordle_word_0;
}

static void sim_game_print_disclaimer() {
	clear_console();
	sim_game_print_disclaimer_banner();
	printf("\n");
	print_wraped_linef("Recently, The New York Times (NYT) has requested to shut down Wordle Archive, because (as quoted by a New York Times representative):", 0, PGINDENT);
	print_wraped_linef("\"The usage was unauthorized, and we were in touch with them\"", 1, PGINDENT);
	print_wraped_linef("The representative also said, \"We don't plan to comment beyond that,\" leaving us with no clues on what are the fates of Wordle imitations.", 0, PGINDENT);
	print_wraped_linef("It's the first time that the NYT has actively taken down a Wordle-like game. This might indicate a threat for all existing Wordle clone games, especially those that use the official Wordle list like this solver.", 0, PGINDENT);
	print_wraped_linef("The NYT had also filed for a trademark for Wordle when they bought the game, but there's no evidence that the trademark had been enforced.", 0, PGINDENT);
	print_wraped_linef("Speculation: It is possible that the NYT is intending to put ads on Wordle to earn ad revenue, and any clone games are a threat to their business.", 0, PGINDENT);
	print_wraped_linef("However, since this project does not have large attention, I will include a feature to allow you to play past Wordle words. This keeps the archives accessible for the time being.", 0, PGINDENT);
	print_wraped_linef("Also, I will be adding a warning for each of the games that are logged to make it clear that only official scores on the NYT are counted.", 0, PGINDENT);
	print_wraped_linef("In case of a threat by the NYT, the name of this tool is subject to change, and some of the solver's features, or the entire solver itself, might have to be removed.", 0, PGINDENT);
	print_wraped_linef("If the worst happens, you may still email snqzspg@gmail.com if you want a copy of this solver, though I might not be able to fulfill mass requests.", 0, PGINDENT);
	printf("\nBy continuing, I would assume that you have understood the situation.\n");
	pause_console();
}

static void sim_game_print_word_option() {
	clear_console();
	sim_game_print_banner();
	sim_game_print_version_banner();

	printf("\n");
	print_wraped_linef("This is a console imitation of Wordle by the New York Times.", 0, PGINDENT);
	print_wraped_linef("This is meant to test the algorithms coded.\n", 0, PGINDENT);
	if (tday_word_expired()) {
		print_wraped_linef(" 1 - Play with today's word (Not available)", 1, PGINDENT);
	} else {
		print_wraped_linef(" 1 - Play with today's word", 1, PGINDENT);
	}
	print_wraped_linef(" 2 - Play with past words", 1, PGINDENT);
	print_wraped_linef(" 3 - Play with a random word", 1, PGINDENT);
	print_wraped_linef(" 4 - Play with a specific word", 1, PGINDENT);
	print_wraped_linef(" q - Quit", 1, PGINDENT);
	pgcg_set_note_colour();
	print_wraped_linef("\nThe algorithms don't know what's today's word..\nI think..", 0, PGINDENT);
	pgcg_reset_colour();
}

static void sim_game_print_archive_option(size_t min, size_t max) {
	clear_console();
	printf("\nSimulate Wordle - Pick an archive\n");
	printf("Type the number to pick the corresponding archive. (%lu - %lu)\n", (long unsigned) min, (long unsigned) max);
	printf("Leave blank for a random pick.\n");
	printf("Type 'q' to quit.\n");
}

static void sim_game_set_col(char c) {
	if (colour_blind_mode < 2) {
		pgcg_set_tile_colour(c);
	}
}

static void sim_game_reset_col() {
	if (colour_blind_mode < 2) {
		pgcg_reset_colour();
	}
}

static void sim_game_print_intro_page() {
	clear_console();
	printf("\nSimulate Wordle - Instructions\n");
	sim_game_print_version_banner();
	printf("\n");
	print_wraped_linef("You are to guess a mystery five-letter word in six tries.", 0, PGINDENT);
	printf("Each word have to be an actual five letter word.\n");
	printf("After each guess, the guess will be shown with the colours / letters below:\n");
	printf("    g - green - letter is in the answer and in the correct spot\n");
	printf("    y - yellow - letter is in the answer but wrong spot\n");
	printf("    b - black/grey - letter is not in the answer\n");
	printf("Example: ");
	sim_game_set_col('g'); printf(" w "); sim_game_reset_col(); printf(" ");
	sim_game_set_col('b'); printf(" e "); sim_game_reset_col(); printf(" ");
	sim_game_set_col('b'); printf(" a "); sim_game_reset_col(); printf(" ");
	sim_game_set_col('b'); printf(" r "); sim_game_reset_col(); printf(" ");
	sim_game_set_col('b'); printf(" y "); sim_game_reset_col(); printf("\n");
	if (colour_blind_mode) {
		printf("         ");
		sim_game_set_col('g'); printf("(g)"); sim_game_reset_col(); printf(" ");
		sim_game_set_col('b'); printf("(b)"); sim_game_reset_col(); printf(" ");
		sim_game_set_col('b'); printf("(b)"); sim_game_reset_col(); printf(" ");
		sim_game_set_col('b'); printf("(b)"); sim_game_reset_col(); printf(" ");
		sim_game_set_col('b'); printf("(b)"); sim_game_reset_col(); printf("\n\n");
	}

	printf("Type your first five-letter word to continue, or 'q' to exit.\n");
}

static char* sim_game_prompt_input() {
	printf("5-letter word >> ");
	return ask_user();
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

static char is_valid_guess(const char* s, const char mode3) {
	if (!str_islen(s, wordle_word_length)) {
		return 0;
	}
	if (!mode3 && !wordle_in_dictionary(s)) {
		return 0;
	}
	return all_lc_alpheb(s);
}

static void sim_game_print_game_page(wlgame* g) {
	clear_console();
	printf("\nSimulate Wordle\n");
	sim_game_print_version_banner();
	printf("\n");
	gbucket_printall(g -> board);
}

static void sim_game_process_answer(wlgame* g, char* input) {
	wlgame_enter_guess(g, input);
}

/**
 * Returns 1 if user decides to quit, 0 otherwise.
 * The word will be copied into solnbuffer.
 */
static char select_word_page(char* solnbuffer) {
	char* input = NULL;
	while (input == NULL || !is_valid_guess(input, 1)) {
		clear_console();
		sim_game_print_banner();
		sim_game_print_version_banner();
		printf("\n");
		printf("Type the word that you want as the solution, and test playing it.\n");
		printf("The word you choose must be a five-letter word.\n\n");
		printf("Type 'q' to exit.\n");
		printf(" >> ");
		input = ask_user();
		if (input != NULL && !strcmp(input, "q")) {
			free(input);
			return 1;
		}
	}
	strcpy(solnbuffer, input);
	return 0;
}

/**
 * Delete using free()
 */
static char* create_wdle_label_num(size_t n) {
	if (n >= hcded_dict_len) {
		return NULL;
	}
	size_t len = 7 + num_digit(n);
	char* r = malloc(sizeof(char) * (len + 1));
	strcpy(r, "Wordle ");
	char numstr[num_digit(n) + 1];
	cpynumstr(numstr, n, num_digit(n));
	strcat(r, numstr);
	return r;
}

/**
 * Delete using free()
 */
static char* create_wdle_label_wrd(const char* w) {
	char* r;
	if (!wbank_contain(hcded_dict, w)) {
		r = malloc(sizeof(char) * 14);
		strcpy(r, "Wordle custom");
		return r;
	}
	char tmp[strlen(w) + 1];
	strcpy(tmp, w);
	size_t num = hcded_dict_len;
	for (size_t i = hcded_dict_len - 1; i >= 0; i--) {
		obfs_str(tmp, i);
		if (strcmp(tmp, hcded_dict_ordered[i]) == 0) {
			num = i;
			break;
		}
		strcpy(tmp, w);
	}
	return create_wdle_label_num(num);
}

static void feedback_user(const char* input) {
	if (input != NULL && all_lc_alpheb(input)){
		if (str_islen(input, wordle_word_length)) {
			printf("The input given was not in the word bank here.\n");
		} else {
			printf("The input given must be a five-letter word.\n");
		}
	} else {
		printf("The input given was invalid.\n");
	}
}

/**
 *  Return 1 if should quit, 0 otherwise.
 */
static char archive_number_selection(char* soln, char** label) {
	size_t max = wordle_get_tday_num() - 1;
	while (1) {
		sim_game_print_archive_option(1, max);
		printf("\n >> ");
		char* a = ask_user();
		if (a == NULL || *a == '\0') {
			free(a);
			size_t rng = (rand() % max) + 1;
			wordle_cpy_day_word(rng, soln);
			*label = create_wdle_label_num(rng);
			return 0;
		}
		if (strcmp(a, "q") == 0) {
			free(a);
			return 1;
		}
		if (is_pure_unsigned_number(a)) {
			size_t idx = idx_from_str(a);
			free(a);
			if (idx < 1 || idx > max) {
				continue;
			}
			wordle_cpy_day_word(idx, soln);
			*label = create_wdle_label_num(idx);
			return 0;
		}
		free(a);
	}
}

/**
 *  Return 1 if should quit, 0 otherwise.
 */
static char word_option_page(char* soln, char** label, char* mode_3) {
	while (1) {
		sim_game_print_word_option();
		printf("\n >> ");
		char* a = ask_user();
		if (a == NULL) {
			continue;
		}
		if (strcmp(a, "q") == 0) {
			free(a);
			return 1;
		}
		if (strcmp(a, "1") == 0 && !tday_word_expired()) {
			free(a);
			size_t td = wordle_cpy_tday_word(soln);
			*label = create_wdle_label_num(td);
			break;
		}
		if (strcmp(a, "2") == 0) {
			free(a);
			if (archive_number_selection(soln, label)) {
				return 1;
			}
			break;
		}
		if (strcmp(a, "3") == 0) {
			free(a);
			size_t rng = wordle_cpy_rand_word(soln);
			*label = create_wdle_label_num(rng);
			break;
		}
		if (strcmp(a, "4") == 0) {
			free(a);
			if (select_word_page(soln)) {
				return 1;
			}
			*label = create_wdle_label_wrd(soln);
			*mode_3 = 1;
			break;
		}
		free(a);
	}
	return 0;
}

/**
 *  Return 1 if should quit, 0 otherwise.
 */
static char game_input_page(wlgame* game, const char mode_3) {
	char* input = NULL;
	char first_time = 1;
	char entered_invalid = 0;
	while (1) {
		if (!wlgame_open_to_guess(game)) {
			sim_game_print_game_page(game);
			break;
		}
		do {
			if (first_time) {
				sim_game_print_intro_page();
			} else {
				sim_game_print_game_page(game);
			}
			if (entered_invalid) {
				pgcg_set_error_colour();
				feedback_user(input);
				pgcg_reset_colour();
			}
			free(input);
			input = sim_game_prompt_input();
			lowercase(input);
			if (input != NULL && !strcmp(input, "q")) {
				free(input);
				return 1;
			}
			entered_invalid = 1;
		} while (input == NULL || !is_valid_guess(input, mode_3));
		sim_game_process_answer(game, input);
		first_time = 0;
		entered_invalid = 0;
	}
	free(input);
	return 0;
}

void sim_game_thread() {
	sim_game_print_disclaimer();
	char soln[wordle_word_length + 1];
	char *label = NULL;
	char mode_3 = 0;
	if (word_option_page(soln, &label, &mode_3)) return;

	sim_game_print_intro_page();

	wlgame* game = wlgame_create(soln, label);
	free(label);
	if (game == NULL) {
		print_error_ln("Can't start game: not enough memory.");
		pause_console();
		return;
	}

	if (game_input_page(game, mode_3)) {
		wlgame_delete(game);
		return;
	}
	switch (game -> status) {
	case GAME_WIN:
		printf("%s!\n", win_loss_words[game -> row_index - 1]);
		break;
	case GAME_FAIL:
		printf("The mystery word is '%s'.\n", game -> solution);
		pgcg_set_note_colour();
		printf("If you spot any wrong hints given please screenshot/copy your attempts and send to your sender to report.\n");
		pgcg_reset_colour();
	}
	wlgame_delete(game);
	pause_console();
}
