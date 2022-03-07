#ifndef HEAP_SORT_H_INCLUDED
#define HEAP_SORT_H_INCLUDED

void hsort(void* list, size_t nitems, size_t size, int (*compr)(const void*, const void*));

#endif // HEAP_SORT_H_INCLUDED
