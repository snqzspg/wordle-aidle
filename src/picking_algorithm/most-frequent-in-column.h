#ifndef MOST_FREQUENT_IN_COLUMN_H_INCLUDED
#define MOST_FREQUENT_IN_COLUMN_H_INCLUDED

#include "../wordle/guess_bucket.h"
#include "../wordle/word_list.h"
#include "../wordle_solver/solver.h"
#include "algorithms.h"

/**
 * Most frequent in the letter column.
 * This counts all the words for each column
 * of the word and tally the frequencies of
 * letter appearances for that specific
 * column.
 *
 * For example in these following words:
 *   bleed
 *   bleep
 *   blend
 *   bluer
 *   clerk
 *   clued
 *   elbow
 *   elder
 *   elegy
 *   fleck
 *   flyer
 *   olden
 *   older
 *   plier
 *   ulcer
 * For the first column, b has the highest score of 4 counts, e is the next with 2 counts and so on.
 * These will be tallied for each of the five columns.
 *
 * Then, the letters in the words will be matched up to their frequencies in their respective columns and added to a sum.
 * The word(s) with the highest sum(s) will be selected.
 *
 */

void column_popular_init(solver* slvr, algorithm* algo);
void column_popular_larger_init(solver* slvr, algorithm* algo);

//char* guess_by_freq_cols(wlist* l, gbucket* g, wlist* alt_list);
char* guess_by_freq_cols(gbucket* guess_board, wlist** word_lists, size_t nword_lists);

#endif // MOST_FREQUENT_IN_COLUMN_H_INCLUDED
