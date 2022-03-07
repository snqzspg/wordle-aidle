#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../terminal_helper/ccolours.h"
#include "../terminal_helper/helper_fxs.h"
#include "../utilities/input-helper.h"

#include "algorithms.h"

struct algo_registry* registered_algos = NULL;
size_t registered_algos_allocated = 0;
size_t registered_algos_len = 0;

struct algo_registry {
	int id;
	char* name;
	char* tag_desc; // Small desc
//	void* (*create_slvr) (test_sess* session);
//	char (*slvr_open) (void* s, test_sess* session);
//	const char* (*slvr_suggest) (void* s, test_sess* session);
//	void (*slvr_receive_result) (void* s, test_sess* session, char* word, char* result);
//	void (*delete_slvr) (void* s, test_sess* session);
};

//void register_algorithm(int id, const char* name, const char* short_desc, void* (*create_slvr) (test_sess* session), char (*slvr_open) (void* s, test_sess* session), const char* (*slvr_suggest) (void* s, test_sess* session), void (*slvr_receive_result) (void* s, test_sess* session, char* word, char* result), void (*delete_slvr) (void* s, test_sess* session)) {
//	if (registered_algos == NULL) {
//		registered_algos = malloc(sizeof(struct algo_registry) * 8);
//		registered_algos_allocated = 8;
//	}
//	if (registered_algos_len >= registered_algos_allocated) {
//		registered_algos = realloc(registered_algos, sizeof(struct algo_registry) * registered_algos_allocated * 2);
//		registered_algos_allocated *= 2;
//	}
//	registered_algos[registered_algos_len].id = id;
//	registered_algos[registered_algos_len].name = malloc(sizeof(char) * (strlen(name) + 1));
//	strcpy(registered_algos[registered_algos_len].name, name);
//	registered_algos[registered_algos_len].tag_desc = malloc(sizeof(char) * (strlen(short_desc) + 1));
//	strcpy(registered_algos[registered_algos_len].tag_desc, short_desc);
//	registered_algos[registered_algos_len].create_slvr = create_slvr;
//	registered_algos[registered_algos_len].slvr_open = slvr_open;
//	registered_algos[registered_algos_len].slvr_suggest = slvr_suggest;
//	registered_algos[registered_algos_len].slvr_receive_result = slvr_receive_result;
//	registered_algos[registered_algos_len].delete_slvr = delete_slvr;
//	registered_algos_len++;
//}
//
//void register_algorithms() {
//	//
//}

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
		printf("    w - Statistical (Hard mode) (Larger vocabulary)\n");
		if (print_algo_add_info != NULL) print_algo_add_info(3);
		printf("    e - Statistical (No hard mode)\n");
		if (print_algo_add_info != NULL) print_algo_add_info(4);
		printf("    s - Statistical (No hard mode) (Resilient)\n");
		if (print_algo_add_info != NULL) print_algo_add_info(5);
		printf("    d - Random guess\n");
		if (print_algo_add_info != NULL) print_algo_add_info(6);
		printf("    z - Random guess (Larger vocabulary)\n");
		if (print_algo_add_info != NULL) print_algo_add_info(7);
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
		if (strcmp(input, "w") == 0) {
			*algo = 3;
			free(input);
			return 0;
		}
		if (strcmp(input, "e") == 0) {
			*algo = 4;
			free(input);
			return 0;
		}
		if (strcmp(input, "s") == 0) {
			*algo = 5;
			free(input);
			return 0;
		}
		if (strcmp(input, "d") == 0) {
			*algo = 6;
			free(input);
			return 0;
		}
		if (strcmp(input, "z") == 0) {
			*algo = 7;
			free(input);
			return 0;
		}
	}
	return 0;
}
