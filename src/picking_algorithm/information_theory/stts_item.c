#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../utilities/str_util.h"

#include "stts_item.h"

/**
 * Note that this is a linked list.
 * Can be a template for future c projects for linked lists.
 */

long stts_item_make_hash(const char* key) {
	return hash_str(key);
//	register size_t len;
//	register long x;
//
//	len = strlen(key);
//	x = *key << 7;
//	while (--len >= 10) {
//		x = (1000003 * x) ^ (*key + 1);
//	}
//	x ^= strlen(key);
//	if (x == -1) {
//		x = -2;
//	}
//	return x;
}

stts_item* stts_item_create(const char* key, const size_t val) {
	stts_item* w = malloc(sizeof(stts_item));
	if (w == NULL) {
		fprintf(stderr, "ERROR stts_item: Sorry I didn't get any RAM\n");
		return NULL;
	}
	w -> key = malloc(sizeof(char) * (strlen(key) + 1));
	if (w -> key == NULL) {
		fprintf(stderr, "ERROR stts_item: Sorry I didn't get any RAM\n");
		return NULL;
	}
	strcpy(w -> key, key);
	w -> value = val;
	w -> prev = NULL;
	w -> next = NULL;
	w -> mapprev = NULL;
	w -> mapnext = NULL;
	w -> hash = stts_item_make_hash(key);
	return w;
}

stts_item* stts_item_clone(stts_item* w) {
	stts_item* nw = malloc(sizeof(stts_item));
	nw -> key = malloc(sizeof(char) * (strlen(w -> key) + 1));
	if (nw -> key == NULL) {
		fprintf(stderr, "ERROR stts_item: Sorry I didn't get any RAM\n");
		return NULL;
	}
	strcpy(nw -> key, w -> key);
	nw -> value = w -> value;
	nw -> hash = w -> hash;
	nw -> prev = NULL;
	nw -> next = NULL;
//	w -> mapprev = NULL;
//	w -> mapnext = NULL;
	nw -> mapprev = NULL;
	nw -> mapnext = NULL;
	return nw;
}

void stts_item_delete(stts_item* x) {
	free(x -> key);
	free(x);
}

void stts_list_append(stts_list* l, stts_item* x) {
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

void stts_list_extend(stts_list* l, stts_list* x) {
	stts_item* i;
	stts_list_foreach(i, x) {
		stts_list_append(l, stts_item_clone(i));
	}
}

stts_item* stts_list_get(stts_list* l, size_t i) {
	stts_item* el = l -> first_item;
	while (i > 0) {
		if (el == NULL) {
			printf("stts_list: Out of range!\n");
			return NULL;
		}
		el = el -> next;
		i--;
	}
	return el;
}

void stts_list_insert(stts_list* l, size_t i, stts_item* x) {
	stts_item* behind = stts_list_get(l, i);
	stts_item* front = behind -> prev;
	front -> next = x;
	behind -> prev = x;
	x -> prev = front;
	x -> next = behind;
}

void stts_list_remove(stts_list* l, stts_item* x) {
	stts_item* behind = x -> next;
	stts_item* front = x -> prev;
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
	stts_item_delete(x);
	l -> length--;
}

void stts_list_removebyi(stts_list* l, size_t i) {
	stts_list_remove(l, stts_list_get(l, i));
}

stts_item* stts_list_wlbystr(stts_list* l, stts_item* s) {
	if (l -> first_item == NULL) {
		return NULL;
	}
	stts_item* i;
	stts_list_foreach(i, l) {
		if (i == s) {
			return i;
		}
	}
	return NULL;
}

/**
 * This will return inaccurate results if the word is not in the list.
 * Use stts_list_contains to check first.
 */
size_t stts_list_index(stts_list* l, stts_item* s) {
	if (l -> first_item == NULL) {
		return 0;
	}
	size_t count = 0;
	stts_item* i;
	stts_list_foreach(i, l) {
		if (i == s) {
			return count;
		}
		count++;
	}
	return 0;
}

void stts_list_clear(stts_list* l) {
	stts_item* i = l -> first_item;
	stts_item* tmp;
	while (i != NULL) {
		tmp = i -> next;
		stts_item_delete(i);
		i = tmp;
	}
	l -> first_item = NULL;
	l -> last_item = NULL;
	l -> length = 0;
}

size_t stts_list_count(stts_list* l, stts_item* s) {
	if (l -> first_item == NULL) {
		return 0;
	}
	size_t count = 0;
	stts_item* i;
	stts_list_foreach(i, l) {
		if (i == s) {
			count++;
		}
	}
	return count;
}

char stts_list_contains(stts_list* l, stts_item* s) {
	if (l -> first_item == NULL) {
		return 0;
	}
	stts_item* i;
	stts_list_foreach(i, l) {
		if (i == s) {
			return 1;
		}
	}
	return 0;
}

//void stts_list_set(stts_list* l, size_t i, stts_item* s) {
//	stts_item* w = stts_list_getwl(l, i);
//	w -> word = realloc(w -> word, sizeof(char) * (strlen(s) + 1));
//	strcpy(w -> word, s);
//}

stts_list* stts_list_create() {
	stts_list* nl = malloc(sizeof(stts_list));
	if (nl == NULL) {
		fprintf(stderr, "ERROR stts_list: Where my RAM??\n");
		return NULL;
	}
	nl -> first_item = NULL;
	nl -> last_item = NULL;
	nl -> length = 0;
	return nl;
}

void stts_list_delete(stts_list* x) {
	stts_list_clear(x);
	free(x);
}
