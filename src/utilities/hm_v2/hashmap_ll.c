#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../error/print_error.h"
#include "hashmap_ll.h"

/**
 * Note that this is a linked list.
 * Can be a template for future c projects for linked lists.
 */

long hm_item_make_hash(const void* key, const size_t key_size) {
	register size_t len;
	register long x;

	len = key_size;
	x = (*((unsigned char*)(key))) << 7;
	while (--len >= 10) {
		x = (1000003 * x) ^ ((*((unsigned char*)(key))) + 1);
	}
	x ^= key_size;
	if (x == -1) {
		x = -2;
	}
	return x;
}

hm_item* hm_item_create(const void* key, const size_t key_size, const void* val, const size_t val_size) {
	hm_item* item = (hm_item*) malloc(sizeof(hm_item));
	if (item == NULL) {
		print_err_warning_ln("ERROR hm_item I have no more space available..");
		return NULL;
	}
	item -> key = malloc(key_size);
	item -> value = malloc(val_size);
	if (item -> key == NULL || item -> value == NULL) {
		print_err_warning_ln("ERROR hm_item I have no more space available..");
		return NULL;
	}
	memcpy(item -> key, key, key_size);
	item -> key_size = key_size;
	memcpy(item -> value, val, val_size);
	item -> value_size = val_size;
	item -> prev = NULL;
	item -> next = NULL;
	item -> mapprev = NULL;
	item -> mapnext = NULL;
	item -> hash = hm_item_make_hash(key, key_size);
	return item;
}

hm_item* hm_item_clone(hm_item* w) {
	hm_item* nw = (hm_item*) malloc(sizeof(hm_item));
	nw -> key = malloc(w -> key_size);
	nw -> key_size = w -> key_size;
	if (nw -> key == NULL) {
		fprintf(stderr, "ERROR hm_item: Sorry I didn't get any RAM\n");
		return NULL;
	}
	memcpy(nw -> key, w -> key, w -> key_size);
	nw -> value = malloc(w -> value_size);
	nw -> value_size = w -> value_size;
	if (nw -> key == NULL) {
		fprintf(stderr, "ERROR hm_item: Sorry I didn't get any RAM\n");
		return NULL;
	}
	memcpy(nw -> value, w -> value, w -> value_size);
	nw -> hash = w -> hash;
	nw -> prev = NULL;
	nw -> next = NULL;
	nw -> mapprev = NULL;
	nw -> mapnext = NULL;
	return nw;
}

void hm_item_delete(hm_item* x) {
	free(x -> key);
	free(x -> value);
	free(x);
}

void hm_llist_append(hm_llist* l, hm_item* x) {
	if (x == NULL) {
		return;
	}
	if (l -> first_item == NULL) {
		l -> first_item = x;
	} else {
		l -> last_item -> next = x;
		x -> prev = l -> last_item;
	}
	l -> last_item = x;
	l -> length++;
}

void hm_llist_extend(hm_llist* l, hm_llist* x) {
	hm_item* i;
	hm_llist_foreach(i, x) {
		hm_llist_append(l, hm_item_clone(i));
	}
}

hm_item* hm_llist_get(hm_llist* l, size_t i) {
	hm_item* el = l -> first_item;
	while (i > 0) {
		if (el == NULL) {
			printf("hm_llist: Out of range!\n");
			return NULL;
		}
		el = el -> next;
		i--;
	}
	return el;
}

void hm_llist_insert(hm_llist* l, size_t i, hm_item* x) {
	hm_item* behind = hm_llist_get(l, i);
	hm_item* front = behind -> prev;
	front -> next = x;
	behind -> prev = x;
	x -> prev = front;
	x -> next = behind;
}

void hm_llist_remove(hm_llist* l, hm_item* x) {
	hm_item* behind = x -> next;
	hm_item* front = x -> prev;
	if (front != NULL) {
		front -> next = behind;
	}
	if (behind != NULL) {
		behind -> prev = front;
	}
	if (l -> first_item == x) {
		l -> first_item = behind;
	}
	if (l -> last_item == x) {
		l -> last_item = front;
	}
	hm_item_delete(x);
	l -> length--;
}

void hm_llist_removebyi(hm_llist* l, size_t i) {
	hm_llist_remove(l, hm_llist_get(l, i));
}

hm_item* hm_llist_wlbystr(hm_llist* l, hm_item* s) {
	if (l -> first_item == NULL) {
		return NULL;
	}
	hm_item* i;
	hm_llist_foreach(i, l) {
		if (i == s) {
			return i;
		}
	}
	return NULL;
}

/**
 * This will return inaccurate results if the word is not in the list.
 * Use hm_llist_contains to check first.
 */
size_t hm_llist_index(hm_llist* l, hm_item* s) {
	if (l -> first_item == NULL) {
		return 0;
	}
	size_t count = 0;
	hm_item* i;
	hm_llist_foreach(i, l) {
		if (i == s) {
			return count;
		}
		count++;
	}
	return 0;
}

void hm_llist_clear(hm_llist* l) {
	hm_item* i = l -> first_item;
	hm_item* tmp;
	while (i != NULL) {
		tmp = i -> next;
		hm_item_delete(i);
		i = tmp;
	}
	l -> first_item = NULL;
	l -> last_item = NULL;
	l -> length = 0;
}

size_t hm_llist_count(hm_llist* l, hm_item* s) {
	if (l -> first_item == NULL) {
		return 0;
	}
	size_t count = 0;
	hm_item* i;
	hm_llist_foreach(i, l) {
		if (i == s) {
			count++;
		}
	}
	return count;
}

char hm_llist_contains(hm_llist* l, hm_item* s) {
	if (l -> first_item == NULL) {
		return 0;
	}
	hm_item* i;
	hm_llist_foreach(i, l) {
		if (i == s) {
			return 1;
		}
	}
	return 0;
}

//void hm_llist_set(hm_llist* l, size_t i, hm_item* s) {
//	hm_item* w = hm_llist_getwl(l, i);
//	w -> word = realloc(w -> word, sizeof(char) * (strlen(s) + 1));
//	strcpy(w -> word, s);
//}

hm_llist* hm_llist_create() {
	hm_llist* nl = malloc(sizeof(hm_llist));
	if (nl == NULL) {
		fprintf(stderr, "ERROR hm_llist: Where my RAM??\n");
		return NULL;
	}
	nl -> first_item = NULL;
	nl -> last_item = NULL;
	nl -> length = 0;
	return nl;
}

void hm_llist_delete(hm_llist* x) {
	hm_llist_clear(x);
	free(x);
}
