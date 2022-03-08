#include "../wordle/guess_bucket.h"
#include "../wordle/word_list.h"

#include "highest_information_entropy.h"
#include "most-frequent-in-column.h"

char* guess_by_information_freq_hybrid(wlist* main_list, gbucket* g, wlist* alt_list) {
	if (main_list -> length == 0) {
		return NULL;
	}
	if (main_list -> length < 50) {
		return guess_by_information_entropy(main_list, g, alt_list);
	}
	return guess_by_freq_cols(main_list, g, alt_list);
}
