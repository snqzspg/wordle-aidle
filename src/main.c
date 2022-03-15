#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "pages/homepage.h"

#include "about.h"
#include "settings/settings.h"
#include "terminal_helper/ccolours.h"
#include "word_list/hardcoded_dictionary.h"

char colour_blind_mode = 1;

const char* name = "Wordle Aidle";
const char* innername = "waidle";
const char* version = "0.3.1.1 Build 202203152244";

int main() {
	pgcg_init_console_graphics();
	if (clear_warning_thread()) {
		return 0;
	}
	srand(time(NULL));
	init_dict();
	homepage_thread();
	cleanup_dict();
	return 0;
}
