#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../sorting/heap_sort.h"
#include "../terminal_helper/cons_graphics.h"
#include "../terminal_helper/helper_fxs.h"
#include "../utilities/input-helper.h"
#include "../utilities/str_util.h"
#include "../wordle/word_list.h"
#include "../wordle_stats.h"

#include "wlist_interface.h"

static void add_command_appn(wlist* list, const char* word) {
	wlist_append(list, word);
}

static void add_command_bef(wlist* list, size_t i, const char* word) {
	wlist_insert(list, i, word);
}

static void add_command_aft(wlist* list, size_t i, const char* word) {
	wlist_insert(list, i + 1, word);
}

static void rem_command(wlist* list, size_t i) {
	wlist_removebyi(list, i);
}

static void chg_command(wlist* list, size_t i, const char* word) {
	wlist_set(list, i, word);
}

static void shf_command(wlist* list) {
	const size_t len = list -> length;
	if (len <= 1) {
		return;
	}
	char** tmparr = malloc(sizeof(char*) * len);
	size_t c = 0;
	wlword* i;
	wlist_foreach(i, list) {
		tmparr[c] = i -> word;
		c++;
	}
	for (c = 0; c < len; c++) {
		size_t rng = (size_t)(rand() % (len - c)) + c;
		char* tmp = tmparr[c];
		tmparr[c] = tmparr[rng];
		tmparr[rng] = tmp;
	}
	c = 0;
	wlist_foreach(i, list) {
		i -> word = tmparr[c];
		c++;
	}
	free(tmparr);
}

static int cmp_alpha_order(const void *a, const void *b) {
	return strcmp((*(char**) a), (*(char**) b));
}

static int cmp_rev_alpha_order(const void *a, const void *b) {
	return strcmp((*(char**) b), (*(char**) a));
}

static void sort_command(wlist* list, char reverse) {
	const size_t len = list -> length;
	if (len <= 1) {
		return;
	}
	char** tmparr = malloc(sizeof(char*) * len);
	size_t c = 0;
	wlword* i;
	wlist_foreach(i, list) {
		tmparr[c] = i -> word;
		c++;
	}
	hsort(tmparr, len, sizeof(char*), reverse ? cmp_rev_alpha_order : cmp_alpha_order);
	c = 0;
	wlist_foreach(i, list) {
		i -> word = tmparr[c];
		c++;
	}
	free(tmparr);
}

static char is_available_alphebet(const char c) {
	return c >= 'a' && c <= 'z';
}

static char all_lc_alpheb(const char* s) {
	for (; *s != '\0'; s++) {
		if (!is_available_alphebet(*s)) {
			return 0;
		}
	}
	return 1;
}

/**
 * Return 1 if invalid
 * Return 2 if valid command, but not five-letter word
 */
static int parse_usr_command(wlist* list, char* command, size_t i_start_from, size_t min_num, size_t max_num) {
	if (strcmp(command, "shuffle") == 0) {
		shf_command(list);
		return 0;
	}
	if (strcmp(command, "sort") == 0) {
		sort_command(list, 0);
		return 0;
	}
	if (strcmp(command, "revsort") == 0) {
		sort_command(list, 1);
		return 0;
	}
	// Each item will be delimited by spaces.
	char* first_space = strchr(command, ' ');
	if (first_space == NULL) {
		return 1;
	}
	const size_t cmdlen = (size_t)(first_space - command);
	// First item is the command.
	char cmd[cmdlen + 1];
	strncpy(cmd, command, cmdlen);
	cmd[cmdlen] = '\0';

	first_space++;

	if (*first_space == ' ' || *first_space == '\0') {
		return 1;
	}

	if (strcmp(cmd, "add") == 0) {
		if (!str_islen(first_space, wordle_word_length) || !all_lc_alpheb(first_space)) {
			return 2;
		}
		add_command_appn(list, first_space);
		return 0;
	}

	// All commands below requires second item to be a number
	size_t i = 0;
	for (; *first_space != ' ' && *first_space != '\0'; first_space++) {
		if (*first_space < '0' || *first_space > '9') {
			return 1;
		}
		i += (size_t)(*first_space) - (size_t)'0';
		i *= 10;
	}
	i /= 10;
	if (i == 0 || i < min_num || i > max_num) {
		return 1;
	}
	i--;
	i += i_start_from;
	if (i >= list -> length) {
		return 1;
	}

	if (strcmp(cmd, "remove") == 0) {
		rem_command(list, i);
		return 0;
	}

	if (*first_space == '\0') {
		return 1;
	}
	first_space++;
	if (*first_space == ' ' || *first_space == '\0') {
		return 1;
	}

	if (strcmp(cmd, "change") == 0) {
		if (!str_islen(first_space, wordle_word_length) || !all_lc_alpheb(first_space)) {
			return 2;
		}
		chg_command(list, i, first_space);
		return 0;
	}

	char* next_space = strchr(first_space, ' ');
	if (next_space == NULL) {
		return 1;
	}

	size_t cmd2len = (size_t)(next_space - first_space);
	char cmd2[cmd2len + 1];
	strncpy(cmd2, first_space, cmd2len);
	cmd2[cmd2len] = '\0';

	if (strcmp(cmd2, "add")) {
		return 1;
	}

	next_space++;
	if (*next_space == ' ' || *next_space == '\0') {
		return 1;
	}

	if (!str_islen(next_space, wordle_word_length) || !all_lc_alpheb(next_space)) {
		return 2;
	}

	if (strcmp(cmd, "bef") == 0) {
		add_command_bef(list, i, next_space);
		return 0;
	}

	if (strcmp(cmd, "aft") == 0) {
		add_command_aft(list, i, next_space);
	}

	return 0;
}

static void display_err_msg(int err_type) {
	pgcg_set_error_colour();
	switch (err_type) {
	case 0:
		break;
	case 1:
		printf("Invalid input\n");
		break;
	case 2:
		printf("Must be a five-letter word\n");
	}
	pgcg_reset_colour();
}

static void display_commands() {
	printf("Type 'add <word>' to add a word\n");
	printf("Type 'bef|aft <existing number above> add <word>' to add a word in specific position\n");
	printf("Type 'remove <existing number above>' to remove a word\n");
	printf("Type 'change <existing number above> <word>' to change a word\n");
	printf("Type 'shuffle' to shuffle the list\n");
	printf("Type 'sort' or 'revsort' to sort the list\n");
	printf("Enter to submit the list or type 'q' to quit.\n");
}

/**
 * Return 1 if cancel
 */
int get_custom_wlist(wlist* list) {
	char* input = NULL;
	int err_type = 0;
	const size_t page_lim = 9;
	size_t start_idx = 0;
	char prev_next_page_needed = 0;
	while (1) {
		clear_console();
		printf("\nTest Solver - Words to test\n");
		printf("Create a list of words below.\n");
		printf("You can add custom words to screw up the algorithms.\n\n");
		wlword* wrd = wlist_getwl(list, start_idx);
		for (size_t i = 0; i < page_lim; i++) {
			if (wrd == NULL) {
				break;
			}
			printf("    %lu - %s\n", (long unsigned)(i + 1), wrd -> word);
			wrd = wrd -> next;
		}
		printf("\n");
		if (prev_next_page_needed) {
			printf("Page %lu of %lu\n", (long unsigned)(start_idx / page_lim + 1), (long unsigned)(list -> length / page_lim + (list -> length % page_lim ? 1 : 0)));
			if (prev_next_page_needed & 1) {
				printf("Type '>' or '.' for next page.\n");
			}
			if (prev_next_page_needed & 2) {
				printf("Type '<' or ',' for next page.\n");
			}
		}
		display_commands();
		display_err_msg(err_type);
		printf(" >> ");
		free(input);
		input = ask_user();
		lowercase(input);
		if (input == NULL) {
			break;
		}
		if (strcmp(input, "q") == 0) {
			free(input);
			return 1;
		}
		if ((prev_next_page_needed & 1) && (strcmp(input, ".") == 0 || strcmp(input, ">") == 0)) {
			start_idx += page_lim;
			prev_next_page_needed = ((start_idx != 0) << 1) + (list -> length > start_idx + page_lim);
			err_type = 0;
			continue;
		}
		if ((prev_next_page_needed & 2) && (strcmp(input, ",") == 0 || strcmp(input, "<") == 0)) {
			if (start_idx < page_lim) {
				start_idx = 0;
			} else {
				start_idx -= page_lim;
			}
			prev_next_page_needed = ((start_idx != 0) << 1) + (list -> length > start_idx + page_lim);
			err_type = 0;
			continue;
		}
		err_type = parse_usr_command(list, input, start_idx, 1, page_lim);
		prev_next_page_needed = ((start_idx != 0) << 1) + (list -> length > start_idx + page_lim);
		if (start_idx >= list -> length) {
			if (start_idx < page_lim) {
				start_idx = 0;
			} else {
				start_idx -= page_lim;
			}
		}
	}
	return list -> length == 0;
}
