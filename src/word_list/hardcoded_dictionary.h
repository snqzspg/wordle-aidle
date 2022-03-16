#ifndef HARDCODED_DICTIONARY_H_INCLUDED
#define HARDCODED_DICTIONARY_H_INCLUDED

#include <stdio.h>

#include "word_bank.h"

/**
 * Last updated 20220212
 * 25 words has been deleted from the Wordle word list by The New York Times.
 * The following words are removed as they are considered 'obscure', 'insensitive' or 'offensive'.
 *  agora
 *  bitch
 *  chink
 *  coons
 *  darky
 *  dyked
 *  dykes
 *  dykey
 *  faggy
 *  fagot
 *  fibre
 *  gooks
 *  homos
 *  kikes
 *  lesbo
 *  lynch
 *  pupal
 *  pussy
 *  slave
 *  sluts
 *  spick
 *  spics
 *  spiks
 *  wench
 *  whore
 */
extern const size_t hcded_dict_len;
extern const size_t hcded_dict_wlen;
extern char** hcded_dict_ordered;
extern wbank* hcded_dict;

extern const size_t win_loss_words_len;
extern char* win_loss_words[6];

//extern const size_t valid_words = 10657;
//extern const char valid_words[10657][6];

extern wbank* valid_words;
extern wbank* extra_words;

void init_dict();
void cleanup_dict();

#endif // HARDCODED_DICTIONARY_H_INCLUDED
