#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../error/print_error.h"
#include "../terminal_helper/cons_graphics.h"
#include "../wordle_stats.h"
#include "guess_bucket.h"
#include "wlgame.h"
#include "wordle_logic.h"

const char not_enough_memory[] = "ERROR wlgame: Not enough memory I guess?";

static void wlgame_updatestatus(wlgame* g) {
	g -> row_index = g -> board -> guess_count;
	if (gbucket_won(g -> board)) {
		g -> status = GAME_WIN;
	}
	if (gbucket_lost(g -> board)) {
		g -> status = GAME_FAIL;
	}
}

char wlgame_open_to_guess(wlgame* g) {
	return g -> status == GAME_IN_PROGRESS;
}

void wlgame_enter_guess(wlgame* g, char* guess) {
	if (!wlgame_open_to_guess(g)) {
		printf("Game is not open for guessing.\n");
		return;
	}
	size_t wlen = strlen(guess);
	char result[wlen + 1];
	wlgame_enter_guess_w_result(g, guess, result);
}

void wlgame_enter_guess_w_result(wlgame* g, char* guess, char* resbuffer) {
	if (!wlgame_open_to_guess(g)) {
		printf("Game is not open for guessing.\n");
		return;
	}
	wordle_evaluate(guess, g -> solution, resbuffer);
	gbucket_addguess(g -> board, guess, resbuffer);
	wlgame_updatestatus(g);
}

wlgame* wlgame_create(char* answer, char* word_day_label) {
	wlgame* newgame = malloc(sizeof(wlgame));
	if (newgame == NULL) {
		print_error_ln(not_enough_memory);
		return NULL;
	}
	newgame -> board = gbucket_create(max_allowed_guesses, word_day_label);
	newgame -> solution = malloc(sizeof(char) * (strlen(answer) + 1));
	if (newgame -> board == NULL || newgame -> solution == NULL) {
		print_error_ln(not_enough_memory);
		free(newgame -> board);
		free(newgame);
		return NULL;
	}
	strcpy(newgame -> solution, answer);
	newgame -> row_index = 0;
	newgame -> status = GAME_IN_PROGRESS;
	return newgame;
}

gbucket* wlgame_delete_keepboard(wlgame* game) {
	gbucket* board = game -> board;
	free(game -> solution);
	free(game);
	return board;
}

void wlgame_delete(wlgame* game) {
	gbucket_delete(game -> board);
	free(game -> solution);
	free(game);
}
