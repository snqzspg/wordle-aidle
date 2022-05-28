#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../error/print_error.h"

#include "merge_sort.h"

static void merge(void *left_half, void *right_half, void *end, void *aux_array, const size_t size, int (*compr)(const void*, const void*)) {
	void* start = left_half, *half = right_half, *aux_ptr = aux_array;

	while (left_half < half && right_half < end) {
		if (compr(left_half, right_half) <= 0) {
			memcpy(aux_ptr, left_half, size);
			left_half += size;
		} else {
			memcpy(aux_ptr, right_half, size);
			right_half += size;
		}
		aux_ptr += size;
	}

	for (; left_half < half; left_half += size) {
		memcpy(aux_ptr, left_half, size);
		aux_ptr += size;
	}

	for (; right_half < end; right_half += size) {
		memcpy(aux_ptr, right_half, size);
		aux_ptr += size;
	}

	aux_ptr = aux_array;
	void *ptr = start;
	for (; ptr < end; aux_ptr += size, ptr += size) {
		memcpy(ptr, aux_ptr, size);
	}
}

void msort_w_aux(void *list, void *aux_array, size_t nitems, size_t size, int (*compr)(const void*, const void*)) {
	if (nitems <= 1) {
		return;
	}
	void* left_half = list;
	void* right_half = list + nitems / 2 * size;
	size_t left_half_len = nitems / 2;
	size_t right_half_len = nitems - left_half_len;

	msort_w_aux(left_half, aux_array, left_half_len, size, compr);
	msort_w_aux(right_half, aux_array, right_half_len, size, compr);

	merge(left_half, right_half, list + nitems * size, aux_array, size, compr);
}

/**
 * Stable sorting algorithm.
 * This is not thread safe!
 */
void msort(void* list, size_t nitems, size_t size, int (*compr)(const void*, const void*)) {
	void* aux_array = malloc(nitems * size);
	if (aux_array == NULL) {
		print_error_ln("Error mergesort could not find enough space for an auxiliary array.");
		return;
	}
	msort_w_aux(list, aux_array, nitems, size, compr);
	free(aux_array);
}
