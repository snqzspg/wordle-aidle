#ifndef LIST_WORDS_BY_CLUE_H_INCLUDED
#define LIST_WORDS_BY_CLUE_H_INCLUDED

#include "../wordle/guess_bucket.h"
#include "../wordle/letter_counter.h"
#include "../wordle/word_list.h"


/**
 * Mutate the list by cumulatively filtering out the words from a pre-filted that does not match the patter given by wordle.
 */
void filter_wlist_by_last_clue(wlist* l, list_lrpair* guess, lcounter* min_letters, lcounter* exact_letters);

void print_possible_words(wlist* l, size_t cols, const char* prefix);

#endif // LIST_WORDS_BY_CLUE_H_INCLUDED
