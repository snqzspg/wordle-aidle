#include <stdio.h>
#include <stdlib.h>

#include "../error/print_error.h"
#include "../terminal_helper/cons_graphics.h"
#include "heap_idx.h"

size_t heap_idx_idxfromrc(const size_t r, const size_t c) {
	return (1 << r) - 1 + c;
}

size_t heap_idx_parentof_idx(heap_idx* h, const size_t i) {
	if (!i) {
		return 0;
	}
	size_t r = h -> idx_to_row[i];
	size_t c = h -> idx_to_col[i];
	return heap_idx_idxfromrc(r - 1, c / 2);
}

size_t heap_idx_childrencount(heap_idx* h, const size_t i) {
	size_t r = h -> idx_to_row[i];
	size_t c = h -> idx_to_col[i];
	r += 1;
	c *= 2;
	if (heap_idx_idxfromrc(r, c + 1) < h -> length) {
		return 2;
	}
	if (heap_idx_idxfromrc(r, c) < h -> length) {
		return 1;
	}
	return 0;
}

void heap_idx_cpychildrenof_idx(heap_idx* h, size_t* buf, const size_t i) {
 	size_t r = h -> idx_to_row[i];
	size_t c = h -> idx_to_col[i];
	r += 1;
	c *= 2;
	buf[0] = 0;
	buf[1] = 0;
	size_t ret = heap_idx_idxfromrc(r, c + 1);
	if (ret < h -> length) {
		buf[1] = ret;
	}
	ret = heap_idx_idxfromrc(r, c);
	if (ret < h -> length) {
		buf[0] = ret;
	}
}

void heap_idx_shrinkby(heap_idx* h, const size_t x) {
	if (h -> length < x) {
		h -> length = 0;
	} else {
		h -> length -= x;
	}
}

heap_idx* heap_idx_create(const size_t len) {
	heap_idx* nh = malloc(sizeof(heap_idx));
	if (nh == NULL) {
		print_error_ln("ERROR heap_idx: no heap memory for a heap structure, isn't that ironic?");
		return NULL;
	}
	nh -> idx_to_row = malloc(sizeof(size_t) * len);
	nh -> idx_to_col = malloc(sizeof(size_t) * len);
	if (nh -> idx_to_row == NULL || nh -> idx_to_col == NULL) {
		print_error_ln("ERROR heap_idx: no heap memory for a heap structure, isn't that ironic?");
		free(nh);
		return NULL;
	}
	nh -> initial_len = len;
	nh -> length = len;
	size_t row = 0;
	size_t columns = 1;
	size_t a = 0;
	size_t i;
	while (a < len) {
		for (i = 0; i < columns; i++) {
			if (a >= len) {
				break;
			}
			nh -> idx_to_row[a] = row;
			nh -> idx_to_col[a] = i;
			a++;
		}
		columns *= 2;
		row++;
	}
	return nh;
}

void heap_idx_delete(heap_idx* h) {
	free(h -> idx_to_row);
	free(h -> idx_to_col);
	free(h);
}
