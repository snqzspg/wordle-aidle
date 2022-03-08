#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../error/print_error.h"
#include "../terminal_helper/ccolours.h"
#include "../terminal_helper/helper_fxs.h"
#include "../utilities/input-helper.h"

#include "algorithms.h"

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

void register_algorithm(alcat* cat, int id, const char* name, char* (*suggest_word) (gbucket* guess_board, wlist** word_lists, size_t nword_lists), void (*init) (solver* slvr, algorithm* algo), void (*cleanup) (solver* slvr, algorithm* algo), void (*guess_made) (solver* slvr, algorithm* algo)) {
	algorithm* a = malloc(sizeof(algorithm));
	a -> id = id;
	a -> name = malloc(sizeof(char) * (strlen(name) + 1));
	if (a -> name == NULL) {
		print_error_ln("ERROR no more memory to register algorithms.");
		return;
	}
	strcpy(a -> name, name);
	a -> suggest_word = suggest_word;
	a -> init = init;
	a -> cleanup = cleanup;
	a -> guess_made = guess_made;
	alcat_add_algo(cat, a);
}

//void register_algorithms() {
//	//
//}

void algo_category_register(const int id, const char* name) {
	if (!accepting_registration) {
		print_err_warning_ln("WARNING algorithm register process has ended.");
		return;
	}
	if (registered_algo_cats == NULL) {
		registered_algo_cats = (alcat*) malloc(sizeof(alcat) * init_alloc_size);
		if (registered_algo_cats == NULL) {
			print_error_ln("ERROR no more memory to register algorithm categories");
			return;
		}
		registered_algos_allocated = init_alloc_size;
	}
	if (registered_algos_len >= registered_algos_allocated) {
		registered_algos_allocated *= 2;
		registered_algo_cats = realloc(registered_algo_cats, sizeof(alcat) * registered_algos_allocated);
		if (registered_algo_cats == NULL) {
			print_error_ln("ERROR no more memory to register algorithm categories");
			return;
		}
	}
	size_t i = registered_algos_len;
	registered_algo_cats[i].id = id;
	registered_algo_cats[i].name = malloc(sizeof(char) * (strlen(name) + 1));
	if (registered_algo_cats[i].name == NULL) {
		print_error_ln("ERROR no more memory to register algorithm categories");
		return;
	}
	strcpy(registered_algo_cats[i].name, name);
	registered_algo_cats[i].info = NULL;
	registered_algo_cats[i].registered_algorithms = NULL;
	registered_algo_cats[i].registered_algo_count = 0;
	registered_algo_cats[i].registered_algo_alloc = 0;
	registered_algos_len++;
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

void algorithm_delete(algorithm* x) {
	free(x -> name);
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
		}
	}
	free(registered_algo_cats);
}

static void lowercase_ascii(char* s) {
	if (s == NULL) return;
	for (; *s != '\0'; s++) {
		if ('A' <= *s && *s <= 'Z') {
			*s += 32;
		}
	}
}

/**
 * Returns 1 if asked to cancel.
 * The selected algorithm will be copied into the dereferenced algo variable.
 */
int select_algo_page(void (*print_title_stuff)(), int* algo, void (*print_algo_add_info)(int algo)) {
	char* input = NULL;
	while (1) {
		clear_console();
		if (print_title_stuff != NULL) print_title_stuff();
		printf("\n");
		printf("Choose an Algorithm by typing the corresponding number:\n");
		printf("    1 - Most frequent letter per position\n");
		if (print_algo_add_info != NULL) print_algo_add_info(0);
		printf("    2 - Most frequent letter per position (Larger vocabulary)\n");
		if (print_algo_add_info != NULL) print_algo_add_info(1);
		printf("    3 - Statistical (Hard mode)\n");
		if (print_algo_add_info != NULL) print_algo_add_info(2);
		printf("    4 - Statistical (Hard mode) (Larger vocabulary)\n");
		if (print_algo_add_info != NULL) print_algo_add_info(3);
		printf("    w - Statistical (No hard mode)\n");
		if (print_algo_add_info != NULL) print_algo_add_info(4);
		printf("    e - Statistical (No hard mode) (Resilient)\n");
		if (print_algo_add_info != NULL) print_algo_add_info(5);
		printf("    r - Statistical (No hard mode) (Slightly Optimised) [WIP]\n");
		if (print_algo_add_info != NULL) print_algo_add_info(6);
		printf("    a - Statistical (No hard mode) (Resilient) (Slightly Optimised) [WIP]\n");
		if (print_algo_add_info != NULL) print_algo_add_info(7);
		printf("    s - Matt Dodge Hybrid (No hard mode) [WIP]\n");
		if (print_algo_add_info != NULL) print_algo_add_info(8);
		printf("    d - Matt Dodge Hybrid (No hard mode) (Resilient) [WIP]\n");
		if (print_algo_add_info != NULL) print_algo_add_info(9);
		printf("    f - Random guess\n");
		if (print_algo_add_info != NULL) print_algo_add_info(10);
		printf("    z - Random guess (Larger vocabulary)\n");
		if (print_algo_add_info != NULL) print_algo_add_info(11);
		printf("    q - Exit\n\n");
		printf(" >> ");
		free(input);
		input = ask_user();
		lowercase_ascii(input);
		if (input == NULL) {
			continue;
		}
		if (strcmp(input, "q") == 0) {
			free(input);
			return 1;
		}
		if (strcmp(input, "1") == 0) {
			*algo = 0;
			free(input);
			return 0;
		}
		if (strcmp(input, "2") == 0) {
			*algo = 1;
			free(input);
			return 0;
		}
		if (strcmp(input, "3") == 0) {
			*algo = 2;
			free(input);
			return 0;
		}
		if (strcmp(input, "4") == 0) {
			*algo = 3;
			free(input);
			return 0;
		}
		if (strcmp(input, "w") == 0) {
			*algo = 4;
			free(input);
			return 0;
		}
		if (strcmp(input, "e") == 0) {
			*algo = 5;
			free(input);
			return 0;
		}
		if (strcmp(input, "r") == 0) {
			*algo = 6;
			free(input);
			return 0;
		}
		if (strcmp(input, "a") == 0) {
			*algo = 7;
			free(input);
			return 0;
		}
		if (strcmp(input, "s") == 0) {
			*algo = 8;
			free(input);
			return 0;
		}
		if (strcmp(input, "d") == 0) {
			*algo = 9;
			free(input);
			return 0;
		}
		if (strcmp(input, "f") == 0) {
			*algo = 10;
			free(input);
			return 0;
		}
		if (strcmp(input, "z") == 0) {
			*algo = 11;
			free(input);
			return 0;
		}
	}
	return 0;
}
