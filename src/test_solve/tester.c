#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../error/print_error.h"
#include "../terminal_helper/cons_graphics.h"
#include "../utilities/int_util.h"
#include "../utilities/time_util.h"
#include "../utilities/str_util.h"
#include "../wordle/guess_bucket.h"
#include "../wordle/wlgame.h"
#include "../wordle/word_list.h"
#include "../wordle_solver/solver.h"

#include "tester.h"

void guess_dist_log_count(guess_dist* g, size_t guesscount) {
	if (guesscount > g -> max_guesses) {
		print_error_ln("guess_dist attempt to log a guess count higher than allowed.");
		return;
	}
	guesscount--;
	g -> counts[guesscount]++;
}

void guess_dist_log_fail(guess_dist* g) {
	g -> fails++;
}

unsigned int guess_dist_get(guess_dist* g, size_t guesscount) {
	if (guesscount > g -> max_guesses) {
		print_error_ln("guess_dist attempt to log a guess count higher than allowed.");
		return 0;
	}
	guesscount--;
	return g -> counts[guesscount];
}

unsigned int guess_dist_get_fails(guess_dist* g) {
	return g -> fails;
}

static void guess_dist_print_bar(int bar_len) {
	pgcg_set_distro_bar_colour();
	while (bar_len--) {
		printf("#");
	}
	pgcg_reset_colour();
	printf("\n");
}

static void guess_dist_log_bar(FILE* f, int bar_len) {
	while (bar_len--) {
		fprintf(f, "#");
	}
	fprintf(f, "\n");
}

static unsigned int guess_dist_mode_count(guess_dist* g) {
	unsigned int mode = 0;
	for (unsigned int i = 0; i < g -> max_guesses; i++) {
		if (g -> counts[i] > mode) {
			mode = g -> counts[i];
		}
	}
	return mode;
}

static unsigned int guess_dist_total_count(guess_dist* g) {
	unsigned int total = g -> fails;
	for (unsigned int i = 0; i < g -> max_guesses; i++) {
		total += g -> counts[i];
	}
	return total;
}

void guess_dist_print_bars(guess_dist* g, int maxbarsize) {
	int max_val = (int) guess_dist_mode_count(g);
	unsigned int total = guess_dist_total_count(g);
	printf("--- Guess distribution ---\n");
	for (size_t i = 0; i < g -> max_guesses; i++) {
		printf("%lu: %u", (long unsigned) i + 1, g -> counts[i]);
		if (total == 0) {
			printf("\n");
		} else {
			printf(" (%.2f%%)\n", (double)(g -> counts[i]) * 100 / (double) total);
		}
		if (max_val == 0) {
			printf("\n");
		} else {
			guess_dist_print_bar((int) (g -> counts[i]) * maxbarsize / max_val);
		}
	}

	// Print fails
	printf("X: %u", g -> fails);
	if (total == 0) {
		printf("\n");
	} else {
		printf(" (%.2f%%)\n", (double)(g -> fails) * 100 / (double) total);
	}
	if (max_val == 0) {
		printf("\n");
	} else {
		guess_dist_print_bar((int) (g -> fails) * maxbarsize / max_val);
	}
}

void guess_dist_log_bars(FILE* f, guess_dist* g, int maxbarsize) {
	int max_val = (int) guess_dist_mode_count(g);
	unsigned int total = guess_dist_total_count(g);
	fprintf(f, "--- Guess distribution ---\n");
	for (size_t i = 0; i < g -> max_guesses; i++) {
		fprintf(f, "%lu: %u", (long unsigned) i + 1, g -> counts[i]);
		if (total == 0) {
			fprintf(f, "\n");
		} else {
			fprintf(f, " (%.2f%%)\n", (double)(g -> counts[i]) * 100 / (double) total);
		}
		if (max_val == 0) {
			fprintf(f, "\n");
		} else {
			guess_dist_log_bar(f, (int) (g -> counts[i]) * maxbarsize / max_val);
		}
	}

	// Log fails
	fprintf(f, "X: %u", g -> fails);
	if (total == 0) {
		fprintf(f, "\n");
	} else {
		fprintf(f, " (%.2f%%)\n", (double)(g -> fails) * 100 / (double) total);
	}
	if (max_val == 0) {
		fprintf(f, "\n");
	} else {
		guess_dist_log_bar(f, (int) (g -> fails) * maxbarsize / max_val);
	}
}

guess_dist* guess_dist_create(size_t max_guesses) {
	guess_dist* nd = malloc(sizeof(guess_dist));
	if (nd == NULL) {
		print_error_ln("guess_dist not enough memory.");
		return NULL;
	}
	nd -> counts = malloc(sizeof(unsigned int) * max_guesses);
	if (nd -> counts == NULL) {
		print_error_ln("guess_dist not enough memory.");
		return NULL;
	}
	for (size_t i = 0; i < max_guesses; i++) {
		nd -> counts[i] = 0;
	}
	nd -> max_guesses = max_guesses;
	nd -> fails = 0;
	return nd;
}

void guess_dist_delete(guess_dist* d) {
	free(d -> counts);
	free(d);
}

/**
 * Returns 1 if successful, 0 if failed
 */
static char cpy_label(char* __restrict__ buffer, const size_t bufferlen, const char* prefix, const char numsep, const unsigned int prog, const unsigned int total) {
	size_t proglen = num_digit_unsigned_int(prog);
	size_t totallen = num_digit_unsigned_int(total);
	size_t prefxlen = strlen(prefix);
	if (bufferlen < prefxlen + 1 + totallen + proglen) {
		return 0;
	}
	strcpy(buffer, prefix);
	char numstr[totallen + 1];
	cpynumstr_usi(numstr, prog, proglen);
	strcat(buffer, numstr);
	numstr[0] = numsep;
	numstr[1] = '\0';
	strcat(buffer, numstr);
	cpynumstr_usi(numstr, total, totallen);
	strcat(buffer, numstr);
	return 1;
}

static void test_sess_test_word(test_sess* t, char* answer) {
	void* slvr = t -> create_slvr(t);
	if (slvr == NULL) {
		print_error_ln("test_sess_test_word slvr is NULL");
	}
	size_t len = 19 + 2 * num_digit_unsigned_int(t -> total_words);
	char label[len];
	cpy_label(label, len, "Wordle simulator ", '/', t -> progress + 1, t -> total_words);
	wlgame* game = wlgame_create(answer, label);
	size_t wlen = strlen(t -> starting_word);
	char result[wlen + 1];
	wlgame_enter_guess_w_result(game, t -> starting_word, result);
	t -> slvr_receive_result(slvr, t, t -> starting_word, result);
	char word[wlen + 1];
	while (t -> slvr_open(slvr, t) && strcmp(result, "ggggg") != 0) {
		strcpy(word, t -> slvr_suggest(slvr, t));
		wlgame_enter_guess_w_result(game, word, result);
		t -> slvr_receive_result(slvr, t, word, result);
	}
	if (gbucket_won(game -> board)) {
		t -> pass_count++;
		guess_dist_log_count(t -> distribution, game -> row_index);
		if (game -> row_index < t -> best_score) {
			t -> best_score = game -> row_index;
		}
		if (game -> row_index > t -> worst_score) {
			t -> worst_score = game -> row_index;
		}
		t -> avg_score = ((t -> avg_score) * (double)(t -> pass_count - 1) + (double)(game -> row_index)) / (double)(t -> pass_count);
	}
	if (gbucket_lost(game -> board)) {
		t -> fail_count++;
		guess_dist_log_fail(t -> distribution);
		t -> worst_score = max_allowed_guesses + 1;
	}
	t -> delete_slvr(slvr, t);
	t -> boards[t -> progress] = wlgame_delete_keepboard(game);
	t -> progress++;
}

void test_sess_execute_singlethread(test_sess* t, void (*print_fx)(test_sess* tss)) {
	clock_t temps = clock();
	wlword* i;
	wlist_foreach(i, t -> game_wlist) {
		test_sess_test_word(t, i -> word);
		t -> time_taken = ((double)(clock() - temps)) / CLOCKS_PER_SEC;
		print_fx(t);
	}
	t -> time_taken = ((double)(clock() - temps)) / CLOCKS_PER_SEC;
}

int test_sess_log_result(test_sess* t, const char* logname) {
	FILE* logfile = fopen(logname, "wb");
	if (logfile == NULL) {
		return 1;
	}
	fprintf(logfile, "--- Results ---\n");
	char sw[strlen(t -> starting_word) + 1];
	strcpy(sw, t -> starting_word);
	uppercase(sw);
	fprintf(logfile, "Algorithm: %s\n", t -> algorithm_name);
	fprintf(logfile, "Starting word: %s\n", sw);
	fprintf(logfile, "Words tested: %u\n", t -> total_words);
	fprintf(logfile, "Passed: %u (%.2f%%)\n", t -> pass_count, (double)(t -> pass_count) * 100 / (double)(t -> total_words));
	fprintf(logfile, "Failed: %u (%.2f%%)\n", t -> fail_count, (double)(t -> fail_count) * 100 / (double)(t -> total_words));
	fprintf(logfile, "Gave up: %u (%.2f%%)\n", t -> total_words - t -> pass_count - t -> fail_count, (double)(t -> total_words - t -> pass_count - t -> fail_count) * 100 / (double)(t -> total_words));
	fprintf(logfile, "Time taken: ");
	log_time_format(logfile, t -> time_taken);
	fprintf(logfile, "\n\n");
	fprintf(logfile, "--- Scores ---\n");
	fprintf(logfile, "Average: %f\n", t -> avg_score);
	if (t -> pass_count == 0 && t -> fail_count == 0) {
		fprintf(logfile, "Best: -/%d\n", max_allowed_guesses);
		fprintf(logfile, "Worst: -/%d\n\n", max_allowed_guesses);
	} else {
		if (t -> best_score > max_allowed_guesses) {
			fprintf(logfile, "Best: X/%d\n", max_allowed_guesses);
		} else {
			fprintf(logfile, "Best: %lu/%d\n", (long unsigned) t -> best_score, max_allowed_guesses);
		}
		if (t -> worst_score > max_allowed_guesses) {
			fprintf(logfile, "Worst: X/%d\n\n", max_allowed_guesses);
		} else {
			fprintf(logfile, "Worst: %lu/%d\n\n", (long unsigned) t -> worst_score, max_allowed_guesses);
		}
	}
	guess_dist_log_bars(logfile, t -> distribution, 30);
	fclose(logfile);
	return 0;
}

void test_sess_print_result(test_sess* t) {
	printf("--- Results ---\n");
	char sw[strlen(t -> starting_word) + 1];
	strcpy(sw, t -> starting_word);
	uppercase(sw);
	printf("Algorithm: %s\n", t -> algorithm_name);
	printf("Starting word: %s\n", sw);
	printf("Words tested: %u\n", t -> total_words);
	printf("Passed: %u (%.2f%%)\n", t -> pass_count, (double)(t -> pass_count) * 100 / (double)(t -> total_words));
	printf("Failed: %u (%.2f%%)\n", t -> fail_count, (double)(t -> fail_count) * 100 / (double)(t -> total_words));
	printf("Gave up: %u (%.2f%%)\n", t -> total_words - t -> pass_count - t -> fail_count, (double)(t -> total_words - t -> pass_count - t -> fail_count) * 100 / (double)(t -> total_words));
	printf("Time taken: ");
	print_time_format(t -> time_taken);
	printf("\n\n");
	printf("--- Scores ---\n");
	printf("Average: %f\n", t -> avg_score);
	if (t -> pass_count == 0 && t -> fail_count == 0) {
		printf("Best: -/%d\n", max_allowed_guesses);
		printf("Worst: -/%d\n\n", max_allowed_guesses);
	} else {
		if (t -> best_score > max_allowed_guesses) {
			printf("Best: X/%d\n", max_allowed_guesses);
		} else {
			printf("Best: %lu/%d\n", (long unsigned) t -> best_score, max_allowed_guesses);
		}
		if (t -> worst_score > max_allowed_guesses) {
			printf("Worst: X/%d\n\n", max_allowed_guesses);
		} else {
			printf("Worst: %lu/%d\n\n", (long unsigned) t -> worst_score, max_allowed_guesses);
		}
	}
	guess_dist_print_bars(t -> distribution, 30);
}

size_t test_sess_first_unsuccessful(test_sess* t, size_t start, size_t fallback) {
	while (start < t -> total_words) {
		if (t -> boards[start] != NULL && !gbucket_won(t -> boards[start])) {
			return start;
		}
		start++;
	}
	return fallback;
}

size_t test_sess_last_unsuccessful(test_sess* t, size_t start, size_t fallback) {
	while (start > 0) {
		if (t -> boards[start] != NULL && !gbucket_won(t -> boards[start])) {
			return start;
		}
		start--;
	}
	if (t -> boards[0] != NULL && !gbucket_won(t -> boards[0])) {
		return 0;
	}
	return fallback;
}

test_sess* test_sess_create(wlist* list_to_test, const char* algo_name, const char* starting_word, void* (*create_slvr) (test_sess* session), char (*slvr_open) (void* s, test_sess* session), const char* (*slvr_suggest) (void* s, test_sess* session), void (*slvr_receive_result) (void* s, test_sess* session, char* word, char* result), void (*delete_slvr) (void* s, test_sess* session)) {
	test_sess* newsession = malloc(sizeof(test_sess));
	if (newsession == NULL) {
		print_error_ln("test_sess nooo no more RAM left....");
		return NULL;
	}

	newsession -> create_slvr = create_slvr;
	newsession -> slvr_open = slvr_open;
	newsession -> slvr_suggest = slvr_suggest;
	newsession -> slvr_receive_result = slvr_receive_result;
	newsession -> delete_slvr = delete_slvr;

	newsession -> game_wlist = wlist_clone(list_to_test);
	if (newsession -> game_wlist == NULL) {
		print_error_ln("test_sess nooo no more RAM left....");
		free(newsession);
		return NULL;
	}

	newsession -> algorithm_name = malloc(sizeof(char) * (strlen(algo_name) + 1));
	if (newsession -> algorithm_name == NULL) {
		print_error_ln("");
		wlist_delete(newsession -> game_wlist);
		free(newsession);
		return NULL;
	}
	strcpy(newsession -> algorithm_name, algo_name);

	newsession -> starting_word = malloc(sizeof(char) * (strlen(starting_word) + 1));
	if (newsession -> starting_word == NULL) {
		print_error_ln("test_sess nooo no more RAM left....");
		free(newsession -> algorithm_name);
		wlist_delete(newsession -> game_wlist);
		free(newsession);
		return NULL;
	}
	strcpy(newsession -> starting_word, starting_word);

	newsession -> progress = 0;
	newsession -> pass_count = 0;
	newsession -> fail_count = 0;
	newsession -> total_words = list_to_test -> length;
	newsession -> best_score = max_allowed_guesses + 1;
	newsession -> worst_score = 0;
	newsession -> avg_score = 0.0;
	newsession -> time_taken = 0.0;
	newsession -> boards = malloc(sizeof(gbucket*) * newsession -> total_words);
	if (newsession -> boards == NULL) {
		print_error_ln("test_sess nooo no more RAM left....");
		wlist_delete(newsession -> game_wlist);
		free(newsession);
		return NULL;
	}
	for (size_t i = 0; i < newsession -> total_words; i++) {
		newsession -> boards[i] = NULL;
	}
	newsession -> distribution = guess_dist_create(max_allowed_guesses);
	if (newsession -> distribution == NULL) {
		print_error_ln("test_sess nooo no more RAM left....");
		test_sess_clear_boards(newsession);
		free(newsession -> boards);
		wlist_delete(newsession -> game_wlist);
		free(newsession);
		return NULL;
	}
	return newsession;
}

void test_sess_clear_boards(test_sess* t) {
	for (size_t i = 0; i < t -> total_words; i++) {
		if (t -> boards[i] != NULL) {
			gbucket_delete(t -> boards[i]);
			t -> boards[i] = NULL;
		}
	}
}

void test_sess_delete(test_sess* t) {
	guess_dist_delete(t -> distribution);
	test_sess_clear_boards(t);
	free(t -> boards);
	free(t -> algorithm_name);
	free(t -> starting_word);
	wlist_delete(t -> game_wlist);
	free(t);
}
