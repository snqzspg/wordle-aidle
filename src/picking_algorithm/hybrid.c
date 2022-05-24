#include "../wordle/guess_bucket.h"
#include "../wordle/word_list.h"
#include "../wordle_solver/solver.h"
#include "../word_list/hardcoded_dictionary.h"

#include "algorithms.h"
#include "highest_information_entropy.h"
#include "hybrid.h"
#include "most-frequent-in-column.h"

void matt_dodge_init(solver* slvr, algorithm* algo) {
	information_theory_init(slvr, algo);
}

void matt_dodge_hard_init(solver* slvr, algorithm* algo) {
	information_theory_hard_init(slvr, algo);
}

void matt_dodge_larger_init(solver* slvr, algorithm* algo) {
	information_theory_more_vocab_init(slvr, algo);
}

void matt_dodge_hard_larger_init(solver* slvr, algorithm* algo) {
	information_theory_more_vocab_hard_init(slvr, algo);
}

//char* guess_by_information_freq_hybrid(wlist* main_list, gbucket* g, wlist* alt_list) {
char* guess_by_information_freq_hybrid(gbucket* guess_board, wlist** word_lists, size_t nword_lists, char show_word_list_to_user) {
	if (word_lists[0] -> length == 0) {
		return NULL;
	}
	if (word_lists[0] -> length < 50) {
		return guess_by_information_entropy(guess_board, word_lists, nword_lists, show_word_list_to_user);
	}
	return guess_by_freq_cols(guess_board, word_lists, nword_lists, show_word_list_to_user);
}
