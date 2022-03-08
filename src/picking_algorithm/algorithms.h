#ifndef ALGORITHMS_H_INCLUDED
#define ALGORITHMS_H_INCLUDED

#include <stdio.h>

#include "../wordle_solver/solver.h"

typedef struct wordle_game_solver solver;

typedef struct algo_registry algorithm;

struct algo_registry {
	int id;
	char* name;
//	char* tag_desc; // Small desc
	size_t min_word_lists;
	char* (*suggest_word) (gbucket* guess_board, wlist** word_lists, size_t nword_lists);
	void (*init) (solver* slvr, algorithm* algo);
	void (*cleanup) (solver* slvr, algorithm* algo); // Note that every wlist in the solver will be automatically freed after this.
	void (*guess_made) (solver* slvr, algorithm* algo); // Note that word lists with standard filter enabled will be filtered before this.
};

typedef struct algo_category_registry {
	int id;
	char* name;
	char* info;
	algorithm** registered_algorithms;
	size_t registered_algo_count;
	size_t registered_algo_alloc;
} alcat;

int select_algo_page(void (*print_title_stuff)(), int* algo, void (*print_algo_add_info)(int algo));

#endif // ALGORITHMS_H_INCLUDED
