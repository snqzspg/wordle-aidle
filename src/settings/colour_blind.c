#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../about.h"
#include "../terminal_helper/cons_graphics.h"
#include "../terminal_helper/helper_fxs.h"
#include "../utilities/input-helper.h"
#include "settings.h"

char random_demo[12][6] = {
	"glenn",
	"cindy",
	"derek",
	"hazel",
	"clara",
	"grace",
	"emily",
	"nigel",
	"lucas",
	"keanu",
	"tessa",
	"louis"
};
const size_t demo_name_count = 12;
size_t random_picked = 0;
size_t namepick;
char colpick[6];

void pick_random() {
	if (!random_picked) {
		namepick = rand() % demo_name_count;
		char cols[4] = "bgy";

		size_t pick1 = rand() % 5;
		colpick[pick1] = cols[0];

		size_t pick2 = rand() % 4;
		if (pick2 >= pick1) pick2++;
		colpick[pick2] = cols[1];

		size_t pick3 = rand() % 3;
		if (pick3 >= pick1) pick3++;
		if (pick3 >= pick2) pick3++;
		colpick[pick3] = cols[2];
		for (size_t i = 0; i < 5; i++) {
			if (i != pick1 && i != pick2 && i != pick3) {
				colpick[i] = cols[rand() % 3];
			}
		}
		colpick[5] = '\0';
		random_picked = 1;
	}
}

void print_demo_tiles() {
	for (size_t i = 0; i < 5; i++) {
		printf(" ");
		if (colour_blind_mode < 2) pgcg_set_tile_colour(colpick[i]);
		printf(" %c ", random_demo[namepick][i]);
		if (colour_blind_mode < 2) pgcg_reset_colour();
	}
	printf("\n");
	if (colour_blind_mode) {
		for (size_t i = 0; i < 5; i++) {
			printf(" ");
			if (colour_blind_mode < 2) pgcg_set_tile_colour(colpick[i]);
			printf("(%c)", colpick[i]);
			if (colour_blind_mode < 2) pgcg_reset_colour();
		}
	}
}

void colour_blind_mode_printpage() {
	clear_console();
	printf("\n%s - Colour blind mode setting\n\n", name);
	printf("If you are having trouble seeing the tiles with the corresponding results below, you can change the setting below.\n\n");
	print_demo_tiles();
	printf("\n\n");
	char options[3][30] = {
		"Colour blind off",
		"Colour blind on, show colours",
		"Colour blind on, no colours"
	};
	printf("Currently selected: %s\n", options[(size_t)colour_blind_mode]);
	printf("Type and enter a number / letter corresponding to the options below\n");
	printf(" 1 - %s\n", options[0]);
	printf(" 2 - %s\n", options[1]);
	printf(" 3 - %s\n", options[2]);
	printf(" q - Exit\n");
}

void cb_mode_thread() {
	pick_random();
	while (1) {
		colour_blind_mode_printpage();
		printf(" >> ");
		char* input = ask_user();
		if (input == NULL) continue;
		if (strcmp(input, "q") == 0) {
			free(input);
			break;
		}
		if (strcmp(input, "1") == 0) colour_blind_mode = 0;
		if (strcmp(input, "2") == 0) colour_blind_mode = 1;
		if (strcmp(input, "3") == 0) colour_blind_mode = 2;
		free(input);
	}
}
