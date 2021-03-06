#ifndef HIGHEST_INFORMATION_ENTROPY_H_INCLUDED
#define HIGHEST_INFORMATION_ENTROPY_H_INCLUDED

#include "../wordle/guess_bucket.h"
#include "../wordle/word_list.h"
#include "../wordle_solver/solver.h"

#include "information_theory/string_to_size_t.h"
#include "information_theory/stts_item.h"

#include "algorithms.h"

extern char log_scores;

void information_theory_init(solver* slvr, algorithm* algo);
void information_theory_hard_init(solver* slvr, algorithm* algo);
void information_theory_more_vocab_init(solver* slvr, algorithm* algo);
void information_theory_more_vocab_hard_init(solver* slvr, algorithm* algo);

void gen_result_distri(stts_hmap* distmap, wlist* list, const char* guess_word);

char* guess_by_information_entropy(gbucket* guess_board, wlist** word_lists, size_t nword_lists, char show_word_list_to_user);

/**
 * Using Matt Dodge's optimisation method.
 * Credit:
 *     https://betterprogramming.pub/building-a-wordle-bot-in-under-100-lines-of-python-9b980539defb
 */
char* guess_by_information_entropy_optimised_level_1(gbucket* guess_board, wlist** word_lists, size_t nword_lists, char show_word_list_to_user);

#endif // HIGHEST_INFORMATION_ENTROPY_H_INCLUDED
