#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "pages/homepage.h"

#include "about.h"
#include "settings/settings.h"
#include "terminal_helper/cons_graphics.h"
#include "word_list/hardcoded_dictionary.h"

//#include "picking_algorithm/algorithms.h"

char colour_blind_mode = 1;

const char* name = "Wordle Aidle";
const char* innername = "waidle";
const char* version = "0.3.4 Build 202205120257";

int main(void) {
	pgcg_init_console_graphics();
	if (clear_warning_thread()) {
		return 0;
	}
	srand(time(NULL));
	init_dict();
//	register_algorithms();
	homepage_thread();
//	alcats_clear();
	cleanup_dict();
	return 0;
}
