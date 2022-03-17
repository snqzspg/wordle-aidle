#ifndef WORDLE_LOGIC_H_INCLUDED
#define WORDLE_LOGIC_H_INCLUDED

#include "word_list.h"

void wordle_evaluate(const char* guess, const char* solution, char* __restrict__ res_buffer);
char wordle_in_dictionary(const char* wrd);

size_t wordle_get_tday_num();

/**
 * Copies the corresponding word into wbuffer.
 */
void wordle_cpy_day_word(size_t number, char* __restrict__ wbuffer);

/**
 * Copies today's word into wbuffer.
 * Returns the wordle number.
 */
size_t wordle_cpy_tday_word(char* __restrict__ wbuffer);

/**
 * Copies a random word into wbuffer.
 * Returns the chosen wordle number.
 */
size_t wordle_cpy_rand_word(char* __restrict__ wbuffer);

void appnd_wordle_ans(wlist* list);
void appnd_prev_wordle_ans(wlist* list);
void appnd_wordle_valids(wlist* list);

#endif // WORDLE_LOGIC_H_INCLUDED
