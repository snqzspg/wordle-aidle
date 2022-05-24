#ifndef HIGHEST_INFORMATION_ENTROPY_H_INCLUDED
#define HIGHEST_INFORMATION_ENTROPY_H_INCLUDED

#include "../wordle/guess_bucket.h"
#include "../wordle/word_list.h"
#include "../wordle_solver/solver.h"
#include "algorithms.h"

void information_theory_init(solver* slvr, algorithm* algo);
void information_theory_hard_init(solver* slvr, algorithm* algo);
void information_theory_more_vocab_init(solver* slvr, algorithm* algo);
void information_theory_more_vocab_hard_init(solver* slvr, algorithm* algo);

//char* guess_by_information_entropy(wlist* l, gbucket* g, wlist* alt_list);
char* guess_by_information_entropy(gbucket* guess_board, wlist** word_lists, size_t nword_lists, char show_word_list_to_user);

/**
 * Using Matt Dodge's optimisation method.
 * Credit:
 *     https://betterprogramming.pub/building-a-wordle-bot-in-under-100-lines-of-python-9b980539defb
 */
//char* guess_by_information_entropy_optimised_level_1(wlist* l, gbucket* g, wlist* alt_list);
char* guess_by_information_entropy_optimised_level_1(gbucket* guess_board, wlist** word_lists, size_t nword_lists, char show_word_list_to_user);

#endif // HIGHEST_INFORMATION_ENTROPY_H_INCLUDED
