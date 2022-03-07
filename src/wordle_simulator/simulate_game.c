#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../about.h"
#include "../error/print_error.h"
#include "../settings/settings.h"
#include "../terminal_helper/ccolours.h"
#include "../terminal_helper/helper_fxs.h"
#include "../utilities/get_tz.h"
#include "../utilities/input-helper.h"
#include "../utilities/obfuscator.h"
#include "../utilities/str_util.h"
#include "../wordle/guess_bucket.h"
#include "../wordle/wlgame.h"
#include "../wordle/wordle_logic.h"
#include "../wordle_stats.h"
#include "../word_list/hardcoded_dictionary.h"
#include "../word_list/word_bank.h"

#define DAY 86400

static void sim_game_print_banner() {
	printf(" ____  __  _  _  _  _  __     __  ____  ____    _  _   __  ____  ____  __    ____ \n");
	printf("/ ___)(  )( \\/ )/ )( \\(  )   / _\\(_  _)(  __)  / )( \\ /  \\(  _ \\(    \\(  )  (  __)\n");
	printf("\\___ \\ )( / \\/ \\) \\/ (/ (_/\\/    \\ )(   ) _)   \\ /\\ /(  O ))   / ) D (/ (_/\\ ) _) \n");
	printf("(____/(__)\\_)(_/\\____/\\____/\\_/\\_/(__) (____)  (_/\\_) \\__/(__\\_)(____/\\____/(____)\n");
	printf("(ASCII Art: Simulate Wordle)\n");
}

static void sim_game_print_version_banner() {
	printf("Version: %s\n", version);
}

static char tday_word_expired() {
	return time(NULL) + get_timezoneoffset_s() < wordle_word_0;
}

static void sim_game_print_word_option() {
	clear_console();
	sim_game_print_banner();
	sim_game_print_version_banner();
	printf("\n");
	printf("This is basically a console imitation to the Wordle game.\n");
	printf("It is meant to test the algorithms coded.\n\n");
	if (tday_word_expired()) {
		printf(" 1 - Play with today's word (Not available)\n");
	} else {
		printf(" 1 - Play with today's word\n");
	}
	printf(" 2 - Play with a random word\n");
	printf(" 3 - Play with a specific word\n");
	printf(" q - Quit\n");
	pgcg_set_note_colour();
	printf("\nThe algorithms don't know what's today's word..\nI think..\n");
	pgcg_reset_colour();
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
	printf("You are to guess a mystery five-letter word in six tries.\n");
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

static void lowercase_ascii(char* s) {
	if (s == NULL) return;
	for (; *s != '\0'; s++) {
		if ('A' <= *s && *s <= 'Z') {
			*s += 32;
		}
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

static size_t num_digit(size_t n) {
	size_t d = 1;
	while (n >= 10){
		n /= 10;
		d++;
	}
	return d;
}

static void cpynumstr(char* buf, size_t n, size_t ndigits) {
	buf[ndigits] = '\0';
	while (ndigits) {
		buf[ndigits - 1] = (char)(48 + (n % 10));
		n /= 10;
		ndigits--;
	}
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
			size_t rng = wordle_cpy_rand_word(soln);
			*label = create_wdle_label_num(rng);
			break;
		}
		if (strcmp(a, "3") == 0) {
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
			lowercase_ascii(input);
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
