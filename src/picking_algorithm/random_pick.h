#ifndef RANDOM_PICK_H_INCLUDED
#define RANDOM_PICK_H_INCLUDED

#include "../wordle/guess_bucket.h"
#include "../wordle/word_list.h"

/**
 * Self explanatory
 */
char* guess_randomly(wlist* word_list, gbucket* g, wlist* alt_list);

#endif // RANDOM_PICK_H_INCLUDED
