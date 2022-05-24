#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../error/print_error.h"
#include "../terminal_helper/cons_graphics.h"
#include "../terminal_helper/helper_fxs.h"
#include "../utilities/input-helper.h"
#include "../utilities/option_keys.h"
#include "../utilities/str_util.h"

#include "algorithms.h"
#include "highest_information_entropy.h"
#include "hybrid.h"
#include "most-frequent-in-column.h"
#include "random_pick.h"

#define init_alloc_size 4

char accepting_registration = 1;

alcat* registered_algo_cats = NULL;
size_t registered_algos_allocated = 0;
size_t registered_algos_len = 0;

static void alcat_add_algo(alcat* cat, algorithm* alg) {
	if (cat -> registered_algorithms == NULL) {
		cat -> registered_algorithms = malloc(sizeof(algorithm*) * init_alloc_size);
		if (cat -> registered_algorithms == NULL) {
			print_error_ln("ERROR no more memory to register algorithms");
			return;
		}
		cat -> registered_algo_alloc = init_alloc_size;
	}
	if (cat -> registered_algo_count >= cat -> registered_algo_alloc) {
		cat -> registered_algo_alloc *= 2;
		cat -> registered_algorithms = realloc(cat -> registered_algorithms, sizeof(algorithm*) * (cat -> registered_algo_alloc));
		if (cat -> registered_algorithms == NULL) {
			print_error_ln("ERROR no more memory to register algorithms");
			return;
		}
	}
	cat -> registered_algorithms[cat -> registered_algo_count] = alg;
	cat -> registered_algo_count++;
}

algorithm* register_algorithm(alcat* cat, int id, const char* name, size_t min_word_lists, char* (*suggest_word) (gbucket* guess_board, wlist** word_lists, size_t nword_lists, char show_word_list_to_user), void (*init) (solver* slvr, algorithm* algo), void (*cleanup) (solver* slvr, algorithm* algo), void (*guess_made) (solver* slvr, algorithm* algo)) {
	algorithm* a = malloc(sizeof(algorithm));
	a -> id = id;
	a -> order = cat -> registered_algo_count;
	a -> name = malloc(sizeof(char) * (strlen(name) + 1));
	if (a -> name == NULL) {
		print_error_ln("ERROR no more memory to register algorithms.");
		return NULL;
	}
	strcpy(a -> name, name);
	a -> info = NULL;
	a -> min_word_lists = min_word_lists;
	a -> suggest_word = suggest_word;
	a -> init = init;
	a -> cleanup = cleanup;
	a -> guess_made = guess_made;
	alcat_add_algo(cat, a);
	return a;
}

alcat* algo_category_register(const int id, const char* name) {
	if (!accepting_registration) {
		print_err_warning_ln("WARNING algorithm register process has ended.");
		return NULL;
	}
	if (registered_algo_cats == NULL) {
		registered_algo_cats = (alcat*) malloc(sizeof(alcat) * init_alloc_size);
		if (registered_algo_cats == NULL) {
			print_error_ln("ERROR no more memory to register algorithm categories");
			return NULL;
		}
		registered_algos_allocated = init_alloc_size;
	}
	if (registered_algos_len >= registered_algos_allocated) {
		registered_algos_allocated *= 2;
		registered_algo_cats = realloc(registered_algo_cats, sizeof(alcat) * registered_algos_allocated);
		if (registered_algo_cats == NULL) {
			print_error_ln("ERROR no more memory to register algorithm categories");
			return NULL;
		}
	}
	size_t i = registered_algos_len;
	registered_algo_cats[i].id = id;
	registered_algo_cats[i].order = i;
	registered_algo_cats[i].name = malloc(sizeof(char) * (strlen(name) + 1));
	if (registered_algo_cats[i].name == NULL) {
		print_error_ln("ERROR no more memory to register algorithm categories");
		return NULL;
	}
	strcpy(registered_algo_cats[i].name, name);
	registered_algo_cats[i].info = NULL;
	registered_algo_cats[i].registered_algorithms = NULL;
	registered_algo_cats[i].registered_algo_count = 0;
	registered_algo_cats[i].registered_algo_alloc = 0;
	registered_algos_len++;
	return registered_algo_cats + i;
}

void alcat_add_desc(alcat* cat, const char* desc) {
	if (cat -> info == NULL) {
		cat -> info = malloc(sizeof(char) * (strlen(desc) + 1));
	} else {
		cat -> info = realloc(cat -> info, sizeof(char) * (strlen(desc) + 1));
	}
	if (cat -> info == NULL) {
		print_error_ln("ERROR no more memory to add category description.");
		return;
	}
	strcpy(cat -> info, desc);
}

alcat* column_popular_cat = NULL;
alcat* information_theory_cat = NULL;
alcat* matt_dodge_cat = NULL;
alcat* random_pick_cat = NULL;

algorithm* column_popular;
algorithm* column_popular_larger_vocab;
algorithm* information_theory;
algorithm* information_theory_larger;
algorithm* information_theory_slightly_optimised;
algorithm* information_theory_slightly_optimised_larger;
algorithm* information_theory_hard;
algorithm* information_theory_hard_larger;
algorithm* matt_dodge_hybrid;
algorithm* matt_dodge_hybrid_larger;
algorithm* matt_dodge_hybrid_hard;
algorithm* matt_dodge_hybrid_hard_larger;
algorithm* random_pick;
algorithm* random_pick_larger;

void register_algorithms() {
	column_popular_cat = algo_category_register(0, "Column Popular");
	alcat_add_desc(column_popular_cat, "Picks the most frequent letter of matching words per position after each guess.\n\n\"No answer dependence\" algorithms avoids dependency on official Wordle answers.");

	column_popular = register_algorithm(column_popular_cat, 0, "Column Popular", 1, guess_by_freq_cols, column_popular_init, NULL, NULL);
	column_popular_larger_vocab = register_algorithm(column_popular_cat, 1, "Column Popular (No answer dependence)", 1, guess_by_freq_cols, column_popular_larger_init, NULL, NULL);

	information_theory_cat = algo_category_register(1, "Information Theory");
	alcat_add_desc(information_theory_cat, "The algorithm that uses Information Theory to produce guess that maximizes the information obtained.\nThis algorithm can be slow, especially if it is allowed to choose non-hard mode words.\n(Information Theory algorithms have time complexities of O(n^2).)\n\"No answer dependence\" algorithms avoids dependency on official Wordle answers.");

	information_theory = register_algorithm(information_theory_cat, 2, "Information Theory (No hard mode)", 2, guess_by_information_entropy, information_theory_init, NULL, NULL);
	information_theory_larger = register_algorithm(information_theory_cat, 3, "Information Theory (No hard mode) (No answer dependence)", 2, guess_by_information_entropy, information_theory_more_vocab_init, NULL, NULL);
	information_theory_slightly_optimised = register_algorithm(information_theory_cat, 4, "Information Theory (No hard mode) (Slightly Optimised)", 2, guess_by_information_entropy_optimised_level_1, information_theory_init, NULL, NULL);
	information_theory_slightly_optimised_larger = register_algorithm(information_theory_cat, 5, "Information Theory (No hard mode) (No answer dependence) (Slightly Optimised)", 2, guess_by_information_entropy_optimised_level_1, information_theory_more_vocab_init, NULL, NULL);
	information_theory_hard = register_algorithm(information_theory_cat, 6, "Information Theory (Hard mode)", 2, guess_by_information_entropy, information_theory_hard_init, NULL, NULL);
	information_theory_hard_larger = register_algorithm(information_theory_cat, 7, "Information Theory (Hard mode) (No answer dependence)", 2, guess_by_information_entropy, information_theory_more_vocab_hard_init, NULL, NULL);

	matt_dodge_cat = algo_category_register(2, "Matt Dodge Hybrid");
	alcat_add_desc(matt_dodge_cat, "Inspired by an article by Matt Dodge, this mixes the Column Popular and Information Theory algorithms in order to make the suggesting process more efficient.\n\n\"No answer dependence\" algorithms avoids dependency on official Wordle answers.");

	matt_dodge_hybrid = register_algorithm(matt_dodge_cat, 8, "Matt Dodge Hybrid (No hard mode)", 2, guess_by_information_freq_hybrid, matt_dodge_init, NULL, NULL);
	matt_dodge_hybrid_larger = register_algorithm(matt_dodge_cat, 9, "Matt Dodge Hybrid (No hard mode) (No answer dependence)", 2, guess_by_information_freq_hybrid, matt_dodge_larger_init, NULL, NULL);
	matt_dodge_hybrid_hard = register_algorithm(matt_dodge_cat, 10, "Matt Dodge Hybrid (Hard mode)", 2, guess_by_information_freq_hybrid, matt_dodge_hard_init, NULL, NULL);
	matt_dodge_hybrid_hard_larger = register_algorithm(matt_dodge_cat, 11, "Matt Dodge Hybrid (Hard mode) (No answer dependence)", 2, guess_by_information_freq_hybrid, matt_dodge_hard_larger_init, NULL, NULL);

	random_pick_cat = algo_category_register(3, "Random Guess");
	alcat_add_desc(random_pick_cat, "Self-explanatory\n\n\"No answer dependence\" algorithms avoids dependency on official Wordle answers.");

	random_pick = register_algorithm(random_pick_cat, 12, "Random Pick", 2, guess_randomly, random_pick_init, NULL, NULL);
	random_pick_larger = register_algorithm(random_pick_cat, 13, "Random Pick (No answer dependence)", 2, guess_randomly, random_pick_larger_init, NULL, NULL);
}

void algorithm_delete(algorithm* x) {
	free(x -> name);
	free(x -> info);
	free(x);
}

void alcats_clear() {
	for (size_t i = 0; i < registered_algos_len; i++) {
		free(registered_algo_cats[i].info);
		free(registered_algo_cats[i].name);
		if (registered_algo_cats[i].registered_algorithms != NULL) {
			for (size_t j = 0; j < registered_algo_cats[i].registered_algo_count; j++) {
				algorithm_delete(registered_algo_cats[i].registered_algorithms[j]);
			}
			free(registered_algo_cats[i].registered_algorithms);
		}
	}
	free(registered_algo_cats);
}

static const size_t get_algo_name_len(const void* bytes, const size_t algosize) {
	return strlen((*((algorithm**)bytes)) -> name);
}

static void cpy_algo_name(const void* bytes, const size_t algosize, char* buffer) {
	strcpy(buffer, (*((algorithm**)bytes)) -> name);
}

/**
 * Returns 1 if asked to cancel.
 * The selected algorithm will be copied into the dereferenced algo variable.
 */
int select_algo_by_cat_page(const alcat* category, void (*print_title_stuff)(const char* cat_name), const char* algo_select_prefix, algorithm** algo, void (*print_algo_add_info)(int algo)) {
	char* input = NULL;
	while (1) {
		clear_console();
		if (print_title_stuff != NULL) print_title_stuff(category -> name);
		printf("\nSelected category: %s\n", category -> name);
		printf("\n");
		print_wraped_linef("%s", 0, PGINDENT, algo_select_prefix == NULL || *algo_select_prefix == '\0' ? "Select a variant of this algorithm" : algo_select_prefix);
		print_options_list(category -> registered_algorithms, category -> registered_algo_count, sizeof(algorithm*), 1, get_algo_name_len, cpy_algo_name);
		print_wraped_linef("q - Back", 1, PGINDENT);
		pgcg_set_note_colour();
		if (category -> info != NULL) {
			printf("\n%s\n", category -> info);
		}
		if (log_scores && category == information_theory_cat) {
			pgcg_set_warning_colour();
			printf("DEBUG MODE: Whenever the selected IT algorithm makes a suggestion, it logs the information scores for each possible suggestion into 'debug.txt'. Type 'itdebug' to turn off.\n");
		}
		printf("\n");
		pgcg_reset_colour();
		printf(" >> ");
		free(input);
		input = ask_user();
		lowercase(input);
		if (input == NULL) {
			continue;
		}
		if (strcmp(input, "q") == 0) {
			free(input);
			return 1;
		}
		if (strcmp(input, "itdebug") == 0) {
			log_scores = !log_scores;
			continue;
		}
		size_t algo_idx = get_idx_from_option_key(input);
		if (algo_idx < category -> registered_algo_count) {
			*algo = category -> registered_algorithms[algo_idx];
			free(input);
			return 0;
		}
	}
    return 0;
}

static const size_t get_cat_name_len(const void* bytes, const size_t catsize) {
	return strlen((*((alcat*)bytes)).name);
}

static void cpy_cat_name(const void* bytes, const size_t catsize, char* buffer) {
	strcpy(buffer, (*((alcat*)bytes)).name);
}

/**
 * Returns 1 if asked to cancel.
 * The selected algorithm will be copied into the dereferenced selected_algo variable.
 */
int select_cat_page(void (*print_title_stuff)(), void (*print_algo_title_stuff)(const char* cat_name), char* cat_select_prefix, const char* algo_select_prefix, algorithm** selected_algo, void (*print_algo_add_info)(int algo)) {
	char* input = NULL;
	while (1) {
		clear_console();
		if (print_title_stuff != NULL) print_title_stuff();
		printf("\n");
		print_wraped_linef("%s", 0, PGINDENT, cat_select_prefix == NULL || *cat_select_prefix == '\0' ? "Select a category:" : cat_select_prefix);
		print_options_list(registered_algo_cats, registered_algos_len, sizeof(alcat), 1, get_cat_name_len, cpy_cat_name);
		print_wraped_linef("q - Exit\n", 1, PGINDENT);
		printf(" >> ");
		free(input);
		input = ask_user();
		lowercase(input);
		if (input == NULL) {
			continue;
		}
		if (strcmp(input, "q") == 0) {
			free(input);
			return 1;
		}
		if (strcmp(input, "itdebug") == 0) {
			log_scores = !log_scores;
			continue;
		}
		size_t algo_idx = get_idx_from_option_key(input);
		if (algo_idx < registered_algos_len) {
			alcat* selected_cat = registered_algo_cats + algo_idx;
			if (select_algo_by_cat_page(selected_cat, print_algo_title_stuff, algo_select_prefix, selected_algo, print_algo_add_info)) {
				continue;
			}
			free(input);
			return 0;
		}
    }

	return 0;
}

/**
 * A temporary array to store the names of the variables in order.
 * Once the new algorithm registry system is ready, this will be removed.
 */
//const char algorithm_names[12][67] = {
//	"Popular in position",
//	"Popular in position (Larger vocabulary)",
//	"Information Theory (Hard mode)",
//	"Information Theory (Hard mode) (Larger vocabulary)",
//	"Information Theory (No hard mode)",
//	"Information Theory (No hard mode) (Resilient)",
//	"Information Theory (No hard mode) (Slightly Optimised)",
//	"Information Theory (No hard mode) (Resilient) (Slightly Optimised)",
//	"Matt Dodge Hybrid (No hard mode)",
//	"Matt Dodge Hybrid (No hard mode) (Resilient)",
//	"Random guess",
//	"Random guess (Larger vocabulary)"
//};

/**
 * Returns 1 if asked to cancel.
 * The selected algorithm will be copied into the dereferenced algo variable.
 */
//int select_algo_page(void (*print_title_stuff)(), int* algo, void (*print_algo_add_info)(int algo)) {
//	char* input = NULL;
//	while (1) {
//		clear_console();
//		if (print_title_stuff != NULL) print_title_stuff();
//		printf("\n");
//		print_wraped_linef("Choose an Algorithm by typing the corresponding number:", 0, PGINDENT);
//		print_wraped_linef("1 - Popular in position", 1, PGINDENT);
//		if (print_algo_add_info != NULL) print_algo_add_info(0);
//		print_wraped_linef("2 - Popular in position (Larger vocabulary)", 1, PGINDENT);
//		if (print_algo_add_info != NULL) print_algo_add_info(1);
//		print_wraped_linef("3 - Information Theory (Hard mode)", 1, PGINDENT);
//		if (print_algo_add_info != NULL) print_algo_add_info(2);
//		print_wraped_linef("4 - Information Theory (Hard mode) (Larger vocabulary)", 1, PGINDENT);
//		if (print_algo_add_info != NULL) print_algo_add_info(3);
//		print_wraped_linef("w - Information Theory (No hard mode)", 1, PGINDENT);
//		if (print_algo_add_info != NULL) print_algo_add_info(4);
//		print_wraped_linef("e - Information Theory (No hard mode) (Resilient)", 1, PGINDENT);
//		if (print_algo_add_info != NULL) print_algo_add_info(5);
//		print_wraped_linef("r - Information Theory (No hard mode) (Slightly Optimised)", 1, PGINDENT);
//		if (print_algo_add_info != NULL) print_algo_add_info(6);
//		print_wraped_linef("a - Information Theory (No hard mode) (Resilient) (Slightly Optimised)", 1, PGINDENT);
//		if (print_algo_add_info != NULL) print_algo_add_info(7);
//		print_wraped_linef("s - Matt Dodge Hybrid (No hard mode)", 1, PGINDENT);
//		if (print_algo_add_info != NULL) print_algo_add_info(8);
//		print_wraped_linef("d - Matt Dodge Hybrid (No hard mode) (Resilient)", 1, PGINDENT);
//		if (print_algo_add_info != NULL) print_algo_add_info(9);
//		print_wraped_linef("f - Random guess", 1, PGINDENT);
//		if (print_algo_add_info != NULL) print_algo_add_info(10);
//		print_wraped_linef("z - Random guess (Larger vocabulary)", 1, PGINDENT);
//		if (print_algo_add_info != NULL) print_algo_add_info(11);
//		print_wraped_linef("q - Exit\n", 1, PGINDENT);
//		printf(" >> ");
//		free(input);
//		input = ask_user();
//		lowercase(input);
//		if (input == NULL) {
//			continue;
//		}
//		if (strcmp(input, "q") == 0) {
//			free(input);
//			return 1;
//		}
//		if (strcmp(input, "1") == 0) {
//			*algo = 0;
//			free(input);
//			return 0;
//		}
//		if (strcmp(input, "2") == 0) {
//			*algo = 1;
//			free(input);
//			return 0;
//		}
//		if (strcmp(input, "3") == 0) {
//			*algo = 2;
//			free(input);
//			return 0;
//		}
//		if (strcmp(input, "4") == 0) {
//			*algo = 3;
//			free(input);
//			return 0;
//		}
//		if (strcmp(input, "w") == 0) {
//			*algo = 4;
//			free(input);
//			return 0;
//		}
//		if (strcmp(input, "e") == 0) {
//			*algo = 5;
//			free(input);
//			return 0;
//		}
//		if (strcmp(input, "r") == 0) {
//			*algo = 6;
//			free(input);
//			return 0;
//		}
//		if (strcmp(input, "a") == 0) {
//			*algo = 7;
//			free(input);
//			return 0;
//		}
//		if (strcmp(input, "s") == 0) {
//			*algo = 8;
//			free(input);
//			return 0;
//		}
//		if (strcmp(input, "d") == 0) {
//			*algo = 9;
//			free(input);
//			return 0;
//		}
//		if (strcmp(input, "f") == 0) {
//			*algo = 10;
//			free(input);
//			return 0;
//		}
//		if (strcmp(input, "z") == 0) {
//			*algo = 11;
//			free(input);
//			return 0;
//		}
//	}
//	return 0;
//}
