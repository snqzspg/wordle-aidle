#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "pages/homepage.h"

#include "about.h"
#include "settings/settings.h"
#include "terminal_helper/cons_graphics.h"
#include "utilities/option_keys.h"
#include "word_list/hardcoded_dictionary.h"

#include "picking_algorithm/algorithms.h"

char colour_blind_mode = 1;
char no_clear_mode = 0;

const char* name = "Wordle Aidle";
const char* innername = "waidle";
const char* version = "snapshot-202205281316";

void cleanup() {
	alcats_clear();
	option_keys_cleanup();
	cleanup_dict();
}

int main(int argc, char **argv) {
	pgcg_init_console_graphics();
	if (argc > 1 && strcmp(argv[1], "--no_clear_console") == 0) {
		no_clear_mode = 1;
	} else {
		if (clear_warning_thread(argv[0])) {
			return 0;
		}
	}
	int exitcode = 0;
	srand(time(NULL));
	exitcode = init_dict();
	if (exitcode) {
		return exitcode;
	}
	exitcode = option_keys_init();
	if (exitcode) {
		cleanup_dict();
		return exitcode;
	}
	register_algorithms();
	exitcode = homepage_thread();
	if (exitcode) {
		cleanup();
		return exitcode;
	}
	cleanup();
	return exitcode;
}
