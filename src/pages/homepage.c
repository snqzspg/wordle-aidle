#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../about.h"
#include "../manual_input/manual_type.h"
#include "../settings/colour_blind.h"
#include "../terminal_helper/cons_graphics.h"
#include "../terminal_helper/helper_fxs.h"
#include "../utilities/input-helper.h"
#include "../utilities/str_util.h"
#include "../wordle_simulator/simulate_game.h"

#include "tester.h"

static const char banner_lines[5][66] = {
	" _  _   __  ____  ____  __    ____     __   __  ____  __    ____ ",
	"/ )( \\ /  \\(  _ \\(    \\(  )  (  __)   / _\\ (  )(    \\(  )  (  __)",
	"\\ /\\ /(  O ))   / ) D (/ (_/\\ ) _)   /    \\ )(  ) D (/ (_/\\ ) _) ",
	"(_/\\_) \\__/(__\\_)(____/\\____/(____)  \\_/\\_/(__)(____/\\____/(____)",
	"(ASCII Art: Wordle Aidle)"
};

void print_title_banner() {
	int cols = pgcg_get_console_cols() - 1;
	printf("%.*s\n%.*s\n%.*s\n%.*s\n", cols, banner_lines[0], cols, banner_lines[1], cols, banner_lines[2], cols, banner_lines[3]);
	printf("%s\n", banner_lines[4]);
}

/**
 * Returns 1 if should exit, 0 otherwise.
 */
char print_clear_warning() {
	printf("\nWordle Aidle\n");
	printf("Version: %s\n\n", version);
	printf("WARNING: This application will clear your previous terminal entries!\n");
	printf("(Provided that your terminal supports it.)\n");
	printf("Enter 'c' to continue or leave blank to quit: ");
	char* input = ask_user();
	lowercase(input);
	if (input != NULL && strcmp(input, "c") == 0) {
		free(input);
		return 0;
	}
	free(input);
	return 1;
}

void print_homepage() {
	clear_console();
	print_title_banner();
	printf("SPOILER: This will ruin the fun of Wordle\n");
	printf("Version: %s\n\n", version);
	print_wraped_linef("Choose one of the options below by typing the corresponding number/letter.\n", 0, PGINDENT);
	printf(" 1 - Start solver\n");
	printf(" 2 - Simulate Wordle game\n");
	printf(" 3 - Test solvers\n");
	printf(" w - Colour blind mode\n");
	printf(" q - Quit\n");
}

char clear_warning_thread() {
	return print_clear_warning();
}

void homepage_thread() {
	while (1) {
		print_homepage();
		printf("\n >> ");
		char* input = ask_user();
		if (input == NULL) {
			continue;
		}
		if (strcmp(input, "q") == 0) {
			free(input);
			break;
		}
		if (strcmp(input, "1") == 0) {
			free(input);
			option1_thread();
			continue;
		}
		if (strcmp(input, "2") == 0) {
			free(input);
			sim_game_thread();
			continue;
		}
		if (strcmp(input, "3") == 0) {
			free(input);
			tester_thread();
			continue;
		}
		if (strcmp(input, "w") == 0) {
			free(input);
			cb_mode_thread();
			continue;
		}
		free(input);
	}
	clear_console();
}
