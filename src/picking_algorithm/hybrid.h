#ifndef HYBRID_H_INCLUDED
#define HYBRID_H_INCLUDED

#include "../wordle/guess_bucket.h"
#include "../wordle/word_list.h"
#include "../wordle_solver/solver.h"
#include "algorithms.h"

void matt_dodge_init(solver* slvr, algorithm* algo);
void matt_dodge_hard_init(solver* slvr, algorithm* algo);
void matt_dodge_larger_init(solver* slvr, algorithm* algo);
void matt_dodge_hard_larger_init(solver* slvr, algorithm* algo);

//char* guess_by_information_freq_hybrid(wlist* main_list, gbucket* g, wlist* alt_list);
char* guess_by_information_freq_hybrid(gbucket* guess_board, wlist** word_lists, size_t nword_lists);

#endif // HYBRID_H_INCLUDED
