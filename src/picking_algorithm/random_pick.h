#ifndef RANDOM_PICK_H_INCLUDED
#define RANDOM_PICK_H_INCLUDED

#include "../wordle/guess_bucket.h"
#include "../wordle/word_list.h"

void random_pick_init(solver* slvr, algorithm* algo);
void random_pick_larger_init(solver* slvr, algorithm* algo);

/**
 * Self explanatory
 */
//char* guess_randomly(wlist* word_list, gbucket* g, wlist* alt_list);
char* guess_randomly(gbucket* guess_board, wlist** word_lists, size_t nword_lists);

#endif // RANDOM_PICK_H_INCLUDED
