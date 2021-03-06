#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../about.h"
#include "../picking_algorithm/highest_information_entropy.h"
#include "../manual_input/manual_type.h"
#include "../settings/colour_blind.h"
#include "../settings/settings.h"
#include "../terminal_helper/cons_graphics.h"
#include "../terminal_helper/helper_fxs.h"
#include "../test_solve/tester.h"
#include "../utilities/hash_util.h"
#include "../utilities/input-helper.h"
#include "../utilities/obfuscator.h"
#include "../utilities/str_util.h"
#include "../wordle_simulator/simulate_game.h"

#include "tester.h"
#include "usage_info.h"

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
static char print_clear_warning(const char* arg0) {
	printf("\nWordle Aidle\n");
	printf("Version: %s\n\n", version);
	printf("WARNING: This application will clear your previous terminal entries!\n");
	printf("(If you proceed in default mode, provided that your terminal supports it.)\n\n");
	print_usage_info(arg0);
	printf("\nEnter 'c' to continue in default mode,\n'nc' for no console clearing\nor leave blank to quit: ");
	char* input = ask_user();
	lowercase(input);
	if (input == NULL) {
		free(input);
		return 1;
	}
	if (strcmp(input, "c") == 0) {
		free(input);
		return 0;
	}
	if (strcmp(input, "nc") == 0) {
		no_clear_mode = 1;
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

char clear_warning_thread(const char* arg0) {
	return print_clear_warning(arg0);
}

int homepage_thread() {
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
		if (strcmp(input, "itdebug") == 0) {
			log_scores = !log_scores;
			continue;
		}
		if (str_islen(input, 6)) {
			char beguin_testee[7];
			strcpy(beguin_testee, input);
			beguin_testee[6] = '\0';
			obfs_str(beguin_testee, 1);
			if (strcmp(beguin_testee, "8).1)%") == 0) {
				spoiler_mode = !spoiler_mode;
				continue;
			}
		}
		if (verify_dword_hash(sdbm_hash_str(input), (long) 0x50821cf3)) {
			spoiler_mode = !spoiler_mode;
			continue;
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
	return 0;
}
