#ifndef ANTI_ABSURDLE_H_INCLUDED
#define ANTI_ABSURDLE_H_INCLUDED

#include <stdio.h>

#include "../wordle/guess_bucket.h"
#include "../wordle/word_list.h"
#include "../wordle_solver/solver.h"
#include "algorithms.h"

void anti_absurdle_init(solver* slvr, algorithm* algo);
void anti_absurdle_hard_init(solver* slvr, algorithm* algo);

char* guess_using_anti_absurdle(gbucket* guess_board, wlist** word_lists, size_t nword_lists, char show_word_list_to_user);

#endif // ANTI_ABSURDLE_H_INCLUDED
