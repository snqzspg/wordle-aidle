#ifndef HYBRID_H_INCLUDED
#define HYBRID_H_INCLUDED

#include "../wordle/guess_bucket.h"
#include "../wordle/word_list.h"

char* guess_by_information_freq_hybrid(wlist* main_list, gbucket* g, wlist* alt_list);

#endif // HYBRID_H_INCLUDED
