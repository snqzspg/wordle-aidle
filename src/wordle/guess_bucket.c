#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "guess_bucket.h"
#include "../error/print_error.h"
#include "../settings/settings.h"
#include "../terminal_helper/cons_graphics.h"
#include "letter_counter.h"

const int max_allowed_guesses = 6;

void debug_print(char* l) {
	printf("%s\n", l);
}

T* list_lrpair_alloc_mem(T* currentptr, size_t newsize) {
	if (currentptr == NULL) {
		currentptr = malloc(sizeof(T) * newsize);
	} else {
		currentptr = realloc(currentptr, sizeof(T) * newsize);
	}
	return currentptr;
}

void list_lrpair_append(list_lrpair* l, char letter, char result) {
	l -> length++;
	l -> arr = list_lrpair_alloc_mem(l -> arr, l -> length);
	if (l -> arr == NULL) {
		print_error_ln("ERROR: unable to allocate memory for list_lrpair during append!");
		return;
	}
	list_lrpair_set(l, l -> length - 1, letter, result);
}

void list_lrpair_extend(list_lrpair* l, list_lrpair* x) {
	for (size_t i = 0; i < x -> length; i++) {
		list_lrpair_append(l, list_lrpair_getletter(x, i), list_lrpair_getresult(x, i));
	}
}

void list_lrpair_insert(list_lrpair* l, size_t i, char letter, char result) {
	l -> length++;
	l -> arr = list_lrpair_alloc_mem(l -> arr, l -> length);
	if (l -> arr == NULL) {
		print_error_ln("ERROR: unable to allocate memory for list_lrpair during insert!");
		return;
	}
	for (size_t j = l -> length - 2; j >= i; j--) {
		list_lrpair_set(l, j + 1, l -> arr[j].letter, l -> arr[j].result);
	}
	list_lrpair_set(l, i, letter, result);
}

void list_lrpair_removei(list_lrpair* l, size_t i) {
	for (size_t j = i; j < l -> length - 1; j++) {
		list_lrpair_set(l, j, l -> arr[j + 1].letter, l -> arr[j + 1].result);
	}
	l -> length--;
	l -> arr = list_lrpair_alloc_mem(l -> arr, l -> length);
	if (l -> arr == NULL) {
		print_error_ln("ERROR: unable to allocate memory for list_lrpair during removei!");
		return;
	}
}

void list_lrpair_remove(list_lrpair* l, char letter, char result) {
	for (size_t j = 0; j < l -> length - 1; j++) {
		if (l -> arr[j].letter == letter && l -> arr[j].result == result) {
			list_lrpair_removei(l, j);
		}
	}
}

void list_lrpair_set(list_lrpair* l, size_t i, char letter, char result) {
	l -> arr[i].letter = letter;
	l -> arr[i].result = result;
}

char list_lrpair_getletter(list_lrpair* l, size_t i) {
	return l -> arr[i].letter;
}

char list_lrpair_getresult(list_lrpair* l, size_t i) {
	return l -> arr[i].result;
}

size_t list_lrpair_count(list_lrpair* l, char letter, char result) {
	size_t count = 0;
	size_t i;
	list_lrpair_foreach(i, l) {
		if (l -> arr[i].letter == letter && l -> arr[i].result == result) {
			count++;
		}
	}
	return count;
}

list_lrpair* list_lrpair_create() {
	list_lrpair* l = malloc(sizeof(list_lrpair));
	if (l == NULL) {
		print_error_ln("ERROR: unable to allocate memory for list_lrpair during create!");
		return NULL;
	}
	l -> length = 0;
	l -> arr = NULL;
	return l;
}

void list_lrpair_delete(list_lrpair* l) {
	free(l -> arr);
	free(l);
}

list_lrpair* gen_res_pairs(char* word, char* results) {
	if (strlen(word) != strlen(results)) {
		print_error_ln("ERROR: gen_res_pairs: the character count for word and results are not equal");
		return NULL;
	}
	list_lrpair* l = list_lrpair_create();
	size_t wlen = strlen(word);
	for (size_t i = 0; i < wlen; i++) {
		list_lrpair_append(l, word[i], results[i]);
	}
	return l;
}

static void gbucket_update_latestguess(list_lrpair* latest_guess, lcounter* min_letters, lcounter* exact_letters) {
	if (latest_guess == NULL) {
		print_error_ln("ERROR: gbucket_update_latestguess: latest_guess is null.");
		return;
	}
	size_t i;
	list_lrpair_foreach(i, latest_guess) {
		char let = latest_guess -> arr[i].letter;
		char res = latest_guess -> arr[i].result;
		if (res == 'b') {
			size_t max_count = list_lrpair_count(latest_guess, let, 'y');
			max_count += list_lrpair_count(latest_guess, let, 'g');
			size_t current_max_count;
			if (!lcounter_cpy(exact_letters, &current_max_count, let) || current_max_count < max_count) {
				lcounter_set(exact_letters, let, max_count);
			}
		} else {
			size_t min_count = list_lrpair_count(latest_guess, let, 'y');
			min_count += list_lrpair_count(latest_guess, let, 'g');
			size_t current_min_count;
			if (!lcounter_cpy(min_letters, &current_min_count, let) || current_min_count < min_count) {
				lcounter_set(min_letters, let, min_count);
			}
		}
	}
}

void gbucket_addguess(gbucket* l, char* word, char* results) {
	size_t gcount = l -> guess_count;
	if (gcount >= l -> max_guesses) {
		print_error_ln("ERROR: You've already maxed out the guesses for this gbucket");
		return;
	}
	l -> guesses[gcount] = gen_res_pairs(word, results);
	gbucket_update_latestguess(l -> guesses[gcount], l -> min_letters, l -> exact_letters);
	l -> guess_count++;
}

list_lrpair* gbucket_getlastguess(gbucket* l) {
	return l -> guesses[l -> guess_count - 1];
}

char gbucket_won(gbucket* l) {
	list_lrpair* lastg = gbucket_getlastguess(l);
	size_t glen = lastg -> length;
	for (size_t i = 0; i < glen; i++) {
		if (list_lrpair_getresult(lastg, i) != 'g') {
			return 0;
		}
	}
	return 1;
}

char gbucket_lost(gbucket* l) {
	return l -> guess_count == l -> max_guesses && !gbucket_won(l);
}

void gbucket_print(gbucket* l, size_t i) {
	list_lrpair* gs = l -> guesses[i];
	size_t glen = gs -> length;
	size_t i1;
	printf("\n");
	for (i1 = 0; i1 < glen; i1++) {
		printf(" ");
		if (colour_blind_mode < 2) pgcg_set_tile_colour(list_lrpair_getresult(gs, i1));
		printf(" %c ", list_lrpair_getletter(gs, i1));
		if (colour_blind_mode < 2) pgcg_reset_colour();
	}
	if (colour_blind_mode) {
		printf("\n");
		char res;
		for (i1 = 0; i1 < glen; i1++) {
			res = list_lrpair_getresult(gs, i1);
			printf(" ");
			if (colour_blind_mode < 2) pgcg_set_tile_colour(res);
			printf("(%c)", res);
			if (colour_blind_mode < 2) pgcg_reset_colour();
		}
	}
	printf("\n");
}

void gbucket_tessresprint(gbucket* l, size_t i, char print_spoiler) {
	list_lrpair* gs = l -> guesses[i];
	size_t i1;
	printf("\n");
	if (print_spoiler || colour_blind_mode < 2) {
		for (i1 = 0; i1 < gs -> length; i1++) {
			printf(" ");
			pgcg_set_tile_colour(list_lrpair_getresult(gs, i1));
			if (print_spoiler) {
				printf(" %c ", list_lrpair_getletter(gs, i1));
			} else {
				printf("   ");
			}
			pgcg_reset_colour();
		}
		printf("\n");
	}
	for (i1 = 0; i1 < gs -> length; i1++) {
		printf(" ");
		if (colour_blind_mode < 2) pgcg_set_tile_colour(list_lrpair_getresult(gs, i1));
		if (colour_blind_mode) {
			printf("(%c)", list_lrpair_getresult(gs, i1));
		} else {
			printf("   ");
		}
		if (colour_blind_mode < 2) pgcg_reset_colour();
	}
	printf("\n");
}

const char sq_bytes[6][5] = {
	{240, 159, 159, 167, 0}, // Correct high contrast
	{240, 159, 159, 169, 0}, // Correct normal
	{240, 159, 159, 166, 0}, // Present high contrast
	{240, 159, 159, 168, 0}, // Present normal
	{226, 172, 155, 0, 0}, // Absent dark theme
	{226, 172, 156, 0, 0} // Absent light theme
};

void gbucket_writeres_file(gbucket* l, FILE* f, size_t i, char dark_mode, char high_contrast) {
	list_lrpair* gs = l -> guesses[i];
	size_t i1;
	for (i1 = 0; i1 < gs -> length; i1++) {
		char res = list_lrpair_getresult(gs, i1);
		switch (res) {
		case 'g':
			fwrite(sq_bytes[high_contrast ? 0 : 1], sizeof(char), 4, f);
			break;
		case 'y':
			fwrite(sq_bytes[high_contrast ? 2 : 3], sizeof(char), 4, f);
			break;
		case 'b':
			fwrite(sq_bytes[dark_mode ? 4 : 5], sizeof(char), 3, f);
		}
	}
}

void gbucket_printall(gbucket* l) {
	size_t guesses = l -> guess_count;
	if (l -> label != NULL) printf("%s\n", l -> label);
	printf("Guesses performed: %lu/%lu\n", (long unsigned int)guesses, (long unsigned int)(l -> max_guesses));
	size_t i;
	for (i = 0; i < guesses; i++) {
		gbucket_print(l, i);
	}
	printf("\n");
}

void gbucket_tessresprintall(gbucket* l, char print_label, char print_spoiler) {
	size_t guesses = l -> guess_count;
	if (print_label) {
		if (gbucket_won(l)) {
			printf("%s %lu/%lu\n", l -> label == NULL ? "missingno" : l -> label, (long unsigned int)guesses, (long unsigned int)(l -> max_guesses));
		} else if (gbucket_lost(l)) {
			printf("%s X/%lu\n", l -> label == NULL ? "missingno" : l -> label, (long unsigned int)(l -> max_guesses));
		} else {
			printf("%s :(/%lu\n", l -> label == NULL ? "missingno" : l -> label, (long unsigned int)(l -> max_guesses));
		}
	}
	if (print_spoiler) {
		pgcg_set_warning_colour();
		printf("\nWARNING: SPOILER MODE ENABLED!\n");
		pgcg_reset_colour();
	}
	size_t i;
	for (i = 0; i < guesses; i++) {
		gbucket_tessresprint(l, i, print_spoiler);
	}
	printf("\nResult: ");
	if (gbucket_won(l)) {
		printf("PASSED\n");
	} else if (gbucket_lost(l)) {
		printf("FAILED\n");
	} else {
		printf("GAVE UP\n");
	}
	printf("\n");
}

void gbucket_writeres_all_file(gbucket* l, FILE* f, char dark_mode, char high_contrast) {
	size_t guesses = l -> guess_count;
	if (gbucket_won(l)) {
		fprintf(f, "%s %lu/%lu\n\n", l -> label == NULL ? "missingno" : l -> label, (long unsigned int)guesses, (long unsigned int)(l -> max_guesses));
	} else if (gbucket_lost(l)) {
		fprintf(f, "%s X/%lu\n\n", l -> label == NULL ? "missingno" : l -> label, (long unsigned int)(l -> max_guesses));
	} else {
		fprintf(f, "%s :(/%lu\n\n", l -> label == NULL ? "missingno" : l -> label, (long unsigned int)(l -> max_guesses));
	}
	size_t i;
	for (i = 0; i < guesses; i++) {
		gbucket_writeres_file(l, f, i, dark_mode, high_contrast);
		fwrite("\n", sizeof(char), 1, f);
	}
//	fwrite("\n", sizeof(char), 1, f);
//	fwrite("NOTE: Only the scores from the official Wordle on The New York Times can be considered as legitimate. This score above does not count!\n", sizeof(char), 135, f);
//	fwrite("Site: https://www.nytimes.com/games/wordle/index.html (Surface)\n", sizeof(char), 64, f);
//	fwrite("Site: https://www.nytimesn7cgmftshazwhfgzm37qxb44r64ytbb2dj3x62d2lljsciiyd.onion/games/wordle/index.html (Onion)\n", sizeof(char), 113, f);
//	fwrite("\n", sizeof(char), 1, f);
}

gbucket* gbucket_create(size_t max_guess, const char* label) {
	gbucket* g = malloc(sizeof(gbucket));
	list_lrpair** wordsptr = malloc(sizeof(list_lrpair*) * max_guess);
	if (g == NULL || wordsptr == NULL) {
		print_error_ln("ERROR gbucket not created: where's my RAM?!?!?!");
		free(g);
		return NULL;
	}
	g -> guesses = wordsptr;
	for (size_t i = 0; i < max_guess; i++) {
		wordsptr[i] = NULL;
	}
	g -> guess_count = 0;
	g -> max_guesses = max_guess;
	// This records the maximum count of black letters that should be in the correct word.
	// Most of the letters should be 0, but this is to combat duplicate letters.
	g -> min_letters = lcounter_create(NUM_LETTERS);
	g -> exact_letters = lcounter_create(NUM_LETTERS);
	if (label != NULL && strlen(label) > 0) {
		g -> label = malloc(sizeof(char) * (strlen(label) + 1));
		strcpy(g -> label, label);
	} else {
		g -> label = NULL;
	}
	return g;
}

void gbucket_delete(gbucket* b) {
	size_t i;
	gbucket_foreach(i, b) {
		list_lrpair_delete(b -> guesses[i]);
	}
	free(b -> guesses);
	lcounter_delete(b -> exact_letters);
	lcounter_delete(b -> min_letters);
	free(b -> label);
	free(b);
}
