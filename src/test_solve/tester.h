#ifndef TESTER_H_INCLUDED
#define TESTER_H_INCLUDED

#include <stdlib.h>

#include "../wordle/guess_bucket.h"
#include "../wordle/wlgame.h"
#include "../wordle/word_list.h"

typedef struct guess_distribution {
	unsigned int* counts;
	size_t max_guesses;
} guess_dist;

void guess_dist_log_count(guess_dist* g, size_t guesscount);
unsigned int guess_dist_get(guess_dist* g, size_t guesscount);
void guess_dist_print_bars(guess_dist* g, int maxbarsize);
void guess_dist_log_bars(FILE* f, guess_dist* g, int maxbarsize);
guess_dist* guess_dist_create(size_t max_guesses);
void guess_dist_delete(guess_dist* d);

typedef struct test_session test_sess;

struct test_session {
	void* (*create_slvr) (test_sess* session);
	char (*slvr_open) (void* s, test_sess* session);
	const char* (*slvr_suggest) (void* s, test_sess* session);
	void (*slvr_receive_result) (void* s, test_sess* session, char* word, char* result);
	void (*delete_slvr) (void* s, test_sess* session);
	wlist* game_wlist;
	char* starting_word;
	unsigned int progress;
	unsigned int pass_count;
	unsigned int fail_count;
	unsigned int total_words;
	size_t best_score;
	size_t worst_score;
	double avg_score;
	double time_taken;
	gbucket** boards;
	guess_dist* distribution;
};

void test_sess_execute_singlethread(test_sess* t, void (*print_fx)(test_sess* tss));
int test_sess_log_result(test_sess* t, const char* logname);
void test_sess_print_result(test_sess* t);
size_t test_sess_first_unsuccessful(test_sess* t, size_t start, size_t fallback);
size_t test_sess_last_unsuccessful(test_sess* t, size_t start, size_t fallback);
test_sess* test_sess_create(wlist* list_to_test, const char* starting_word, void* (*create_slvr) (test_sess* session), char (*slvr_open) (void* s, test_sess* session), const char* (*slvr_suggest) (void* s, test_sess* session), void (*slvr_receive_result) (void* s, test_sess* session, char* word, char* result), void (*delete_slvr) (void* s, test_sess* session));
void test_sess_clear_boards(test_sess* t);
void test_sess_delete(test_sess* t);

#endif // TESTER_H_INCLUDED
