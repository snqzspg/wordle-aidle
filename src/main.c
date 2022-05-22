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
const char* version = "snapshot-202205230146";

int main(void) {
	pgcg_init_console_graphics();
	if (clear_warning_thread()) {
		return 0;
	}
	srand(time(NULL));
	init_dict();
	if (option_keys_init()) {
		return 1;
	}
	register_algorithms();
	homepage_thread();
	alcats_clear();
	option_keys_cleanup();
	cleanup_dict();
	return 0;
}
