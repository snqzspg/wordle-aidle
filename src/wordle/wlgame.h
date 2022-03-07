#ifndef WLGAME_H_INCLUDED
#define WLGAME_H_INCLUDED

#include "guess_bucket.h"

#define GAME_IN_PROGRESS 0
#define GAME_FAIL 1
#define GAME_WIN 2

typedef struct wordle_game_session {
	gbucket* board;
	char status;
	size_t row_index; // Currently at row what
	char* solution;
} wlgame;

//void remaining(wlgame* g);
char wlgame_open_to_guess(wlgame* g);
void wlgame_enter_guess(wlgame* g, char* guess);
void wlgame_enter_guess_w_result(wlgame* g, char* guess, char* resbuffer);

wlgame* wlgame_create(char* answer, char* word_day_label);
gbucket* wlgame_delete_keepboard(wlgame* game);
void wlgame_delete(wlgame* game);

#endif // WLGAME_H_INCLUDED
