#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "pages/homepage.h"

#include "about.h"
#include "settings/settings.h"
#include "terminal_helper/cons_graphics.h"
#include "utilities/option_keys.h"
#include "word_list/hardcoded_dictionary.h"

#include "picking_algorithm/algorithms.h"

char colour_blind_mode = 1;

const char* name = "Wordle Aidle";
const char* innername = "waidle";
const char* version = "snapshot-202205262312";

void cleanup() {
	alcats_clear();
	option_keys_cleanup();
	cleanup_dict();
}

int main(void) {
	int exitcode = 0;
	pgcg_init_console_graphics();
	if (clear_warning_thread()) {
		return 0;
	}
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
