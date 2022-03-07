#ifndef HIGHEST_INFORMATION_ENTROPY_H_INCLUDED
#define HIGHEST_INFORMATION_ENTROPY_H_INCLUDED

#include "../wordle/guess_bucket.h"
#include "../wordle/word_list.h"

char* guess_by_information_entropy(wlist* l, gbucket* g, wlist* alt_list);

#endif // HIGHEST_INFORMATION_ENTROPY_H_INCLUDED
