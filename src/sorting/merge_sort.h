#ifndef MERGE_SORT_H_INCLUDED
#define MERGE_SORT_H_INCLUDED

#include <stdio.h>

void msort_w_aux(void *list, void *aux_array, size_t nitems, size_t size, int (*compr)(const void*, const void*));
void msort(void* list, size_t nitems, size_t size, int (*compr)(const void*, const void*));

#endif // MERGE_SORT_H_INCLUDED
