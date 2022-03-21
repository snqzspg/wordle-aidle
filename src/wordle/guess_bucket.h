#ifndef GUESS_BUCKET_H_INCLUDED
#define GUESS_BUCKET_H_INCLUDED

//#include "../utilities/hashmap.h"

#include "letter_counter.h"

extern const int max_allowed_guesses;

typedef struct letter_result_pair {
	char letter;
	char result;
} lrpair;

#define T lrpair

typedef struct lrpair_list {
	lrpair* arr;
	size_t length;
} list_lrpair;

// Standard method formats for list-like structures.
//void list_lrpair_append(list_lrpair* l, T* x);
//void list_lrpair_extend(list_lrpair* l, list_lrpair* x);
//void list_lrpair_insert(list_lrpair* l, size_t i, T* x);
//void list_lrpair_remove(list_lrpair* l, T* x);
//T* list_lrpair_pop(list_lrpair* l, size_t i);
//void list_lrpair_clear(list_lrpair* l);
//int list_lrpair_index(list_lrpair* l, T* x);
//int list_lrpair_count(list_lrpair* l, T* x);
//void list_lrpair_sort(list_lrpair* l);
//void list_lrpair_reverse(list_lrpair* l);
//list_lrpair* list_lrpair_clone(list_lrpair* l);
//char list_lrpair_contains(list_lrpair* l, T* x);
//char list_lrpair_equals(list_lrpair* l, list_lrpair* l2);
//T* list_lrpair_get(list_lrpair* l, size_t i);
//void list_lrpair_set(list_lrpair* l, size_t i, T* x);

void list_lrpair_append(list_lrpair* l, char letter, char result);
void list_lrpair_extend(list_lrpair* l, list_lrpair* x);
void list_lrpair_insert(list_lrpair* l, size_t i, char letter, char result);
void list_lrpair_remove(list_lrpair* l, char letter, char result);
void list_lrpair_removei(list_lrpair* l, size_t i);
void list_lrpair_clear(list_lrpair* l);
int list_lrpair_index(list_lrpair* l, char letter, char result);
size_t list_lrpair_count(list_lrpair* l, char letter, char result);
//void list_lrpair_sort(list_lrpair* l);
//void list_lrpair_reverse(list_lrpair* l);
//list_lrpair* list_lrpair_clone(list_lrpair* l);
//char list_lrpair_contains(list_lrpair* l, char letter, char result);
//char list_lrpair_equals(list_lrpair* l, list_lrpair* l2);
//char list_lrpair_get_letter(list_lrpair* l, size_t i);
//char list_lrpair_get_result(list_lrpair* l, size_t i);
void list_lrpair_set(list_lrpair* l, size_t i, char letter, char result);
char list_lrpair_getletter(list_lrpair* l, size_t i);
char list_lrpair_getresult(list_lrpair* l, size_t i);

#define list_lrpair_foreach(i, list) for (i = 0; i < list -> length; i++)

list_lrpair* list_lrpair_create();
list_lrpair* gen_res_pairs(char* word, char* results);
void list_lrpair_delete(list_lrpair* l);

typedef struct Guess_Bucket {
	list_lrpair** guesses;
	size_t guess_count;
	size_t max_guesses;
//	cts_hmap* min_letters;
//	cts_hmap* max_letters;
	lcounter* min_letters;
	lcounter* exact_letters;
	// A cts_hashmap to keep track of the highest black letter count in the correct word. Most should give 0.
	char* label;
} gbucket;

void gbucket_addguess(gbucket* l, char* word, char* results);
list_lrpair* gbucket_getlastguess(gbucket* l);
char gbucket_won(gbucket* l);
char gbucket_lost(gbucket* l);
void gbucket_print(gbucket* l, size_t i);
void gbucket_writeres_file(gbucket* l, FILE* f, size_t i, char dark_mode, char high_contrast);
void gbucket_printall(gbucket* l);
void gbucket_tessresprintall(gbucket* l, char print_label, char print_spoiler);
void gbucket_writeres_all_file(gbucket* l, FILE* f, char dark_mode, char high_contrast);
//void gbucket_remove(gbucket* l, char letter, char result);
//void gbucket_removei(gbucket* l, size_t i);
//void gbucket_set(gbucket* l, size_t i, char letter, char result);

#define gbucket_foreach(i, list) for (i = 0; i < list -> guess_count; i++)

gbucket* gbucket_create(size_t max_guess, const char* label);
void gbucket_delete(gbucket* b);

#endif // GUESS_BUCKET_H_INCLUDED
