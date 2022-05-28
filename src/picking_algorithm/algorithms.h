#ifndef ALGORITHMS_H_INCLUDED
#define ALGORITHMS_H_INCLUDED

#include <stdio.h>

#include "../wordle_solver/solver.h"

typedef struct wordle_game_solver solver;

typedef struct algo_registry algorithm;

struct algo_registry {
	int id;
	size_t order;
	char* name;
	char* info;
//	char* tag_desc; // Small desc
	size_t min_word_lists;
	char* (*suggest_word) (gbucket* guess_board, wlist** word_lists, size_t nword_lists, char show_word_list_to_user);
	void (*init) (solver* slvr, algorithm* algo);
	void (*cleanup) (solver* slvr, algorithm* algo); // Note that every wlist in the solver will be automatically freed after this.
	void (*guess_made) (solver* slvr, algorithm* algo); // Note that word lists with standard filter enabled will be filtered before this.
};

typedef struct algo_category_registry {
	int id;
	size_t order;
	char* name;
	char* info;
	char* tag_desc; // Small desc
	algorithm** registered_algorithms;
	size_t registered_algo_count;
	size_t registered_algo_alloc;
} alcat;

algorithm* register_algorithm(alcat* cat, int id, const char* name, size_t min_word_lists, char* (*suggest_word) (gbucket* guess_board, wlist** word_lists, size_t nword_lists, char show_word_list_to_user), void (*init) (solver* slvr, algorithm* algo), void (*cleanup) (solver* slvr, algorithm* algo), void (*guess_made) (solver* slvr, algorithm* algo));
alcat* algo_category_register(const int id, const char* name);
void alcat_add_desc(alcat* cat, const char* desc);
void register_algorithms();
void alcats_clear();

int select_cat_page(void (*print_title_stuff)(), void (*print_algo_title_stuff)(const char* cat_name), char* cat_select_prefix, const char* algo_select_prefix, algorithm** selected_algo, void (*print_algo_add_info)(int algo));

extern alcat* column_popular_cat;
extern alcat* information_theory_cat;
extern alcat* matt_dodge_cat;
extern alcat* random_pick_cat;

extern algorithm* column_popular;
extern algorithm* column_popular_larger_vocab;
extern algorithm* information_theory;
extern algorithm* information_theory_larger;
extern algorithm* information_theory_slightly_optimised;
extern algorithm* information_theory_slightly_optimised_larger;
extern algorithm* information_theory_hard;
extern algorithm* information_theory_hard_larger;
extern algorithm* anti_absurdle;
extern algorithm* anti_absurdle_hard;
extern algorithm* matt_dodge_hybrid;
extern algorithm* matt_dodge_hybrid_larger;
extern algorithm* matt_dodge_hybrid_hard;
extern algorithm* matt_dodge_hybrid_hard_larger;
extern algorithm* random_pick;
extern algorithm* random_pick_larger;
extern algorithm* random_pick_ignore_clues;
extern algorithm* random_pick_ignore_clues_larger;

#endif // ALGORITHMS_H_INCLUDED
