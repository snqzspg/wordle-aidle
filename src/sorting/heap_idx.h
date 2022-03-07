#ifndef HEAP_IDX_H_INCLUDED
#define HEAP_IDX_H_INCLUDED

typedef struct heap_index_struct {
	//pti_hmap* coord_to_idx;
	size_t* idx_to_row;
	size_t* idx_to_col;
	size_t initial_len;
	size_t length;
} heap_idx;

size_t heap_idx_parentof_idx(heap_idx* h, const size_t i);
size_t heap_idx_childrencount(heap_idx* h, const size_t i);
void heap_idx_cpychildrenof_idx(heap_idx* h, size_t* buf, const size_t i);
void heap_idx_shrinkby(heap_idx* h, const size_t x);

heap_idx* heap_idx_create(const size_t len);
void heap_idx_delete(heap_idx* h);

#endif // HEAP_IDX_H_INCLUDED
