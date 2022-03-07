#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "heap_idx.h"

/*
 * Thanks! https://stackoverflow.com/questions/2232706/swapping-objects-using-pointers
 */
void hsort_swap(void* a, void* b, size_t size) {
	char tmp[size];
	memcpy(tmp, b, size);
	memcpy(b, a, size);
	memcpy(a, tmp, size);
}

void* hsort_access_idx_ptr(void* list, size_t itemsize, size_t i) {
	return list + i * itemsize;
}

char hsort_partial_sift_down(void* list, heap_idx* heapmap, size_t itemsize, int (*compr)(const void*, const void*), size_t idx, size_t* swpchildidx) {
	size_t ccount = heap_idx_childrencount(heapmap, idx);
	if (ccount == 0) {
		return 0;
	}
	size_t children[2];
	heap_idx_cpychildrenof_idx(heapmap, children, idx);
	*swpchildidx = children[0];
	if (ccount == 2 && compr(hsort_access_idx_ptr(list, itemsize, *swpchildidx), hsort_access_idx_ptr(list, itemsize, children[1])) < 0) {
		*swpchildidx = children[1];
	}
	if (compr(hsort_access_idx_ptr(list, itemsize, idx), hsort_access_idx_ptr(list, itemsize, *swpchildidx)) < 0) {
		hsort_swap(hsort_access_idx_ptr(list, itemsize, idx), hsort_access_idx_ptr(list, itemsize, *swpchildidx), itemsize);
		return 1;
	}
	return 0;
}

void hsort_sift_down(void* list, heap_idx* heapmap, size_t itemsize, int (*compr)(const void*, const void*), size_t idx) {
	size_t i = idx;
	while (idx < heapmap -> length) {
		if (hsort_partial_sift_down(list, heapmap, itemsize, compr, idx, &i)) {
			idx = i;
		} else {
			break;
		}
	}
}

void hsort_heapify(void* list, heap_idx* heapmap, size_t itemsize, int (*compr)(const void*, const void*)) {
	size_t parent = heap_idx_parentof_idx(heapmap, heapmap -> length - 1);
	while (1) {
		hsort_sift_down(list, heapmap, itemsize, compr, parent);
		if (parent == 0) {
			break;
		}
		parent--;
	}
}

void hsort(void* list, size_t nitems, size_t size, int (*compr)(const void*, const void*)) {
	if (nitems <= 1) {
		return;
	}
	heap_idx* heapmap = heap_idx_create(nitems);
	hsort_heapify(list, heapmap, size, compr);
	while (heapmap -> length > 1) {
		hsort_swap(list, hsort_access_idx_ptr(list, size, heapmap -> length - 1), size);
		heap_idx_shrinkby(heapmap, 1);
		hsort_sift_down(list, heapmap, size, compr, 0);
	}
	heap_idx_delete(heapmap);
}
