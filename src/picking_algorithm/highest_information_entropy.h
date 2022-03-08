#ifndef HIGHEST_INFORMATION_ENTROPY_H_INCLUDED
#define HIGHEST_INFORMATION_ENTROPY_H_INCLUDED

#include "../wordle/guess_bucket.h"
#include "../wordle/word_list.h"

char* guess_by_information_entropy(wlist* l, gbucket* g, wlist* alt_list);

/**
 * Using Matt Dodge's optimisation method.
 * Credit:
 *     https://betterprogramming.pub/building-a-wordle-bot-in-under-100-lines-of-python-9b980539defb
 */
char* guess_by_information_entropy_optimised_level_1(wlist* l, gbucket* g, wlist* alt_list);

#endif // HIGHEST_INFORMATION_ENTROPY_H_INCLUDED
