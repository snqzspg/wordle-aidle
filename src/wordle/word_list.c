#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "word_list.h"

/**
 * Note that this is a linked list.
 * Can be a template for future c projects for linked lists.
 */

wlword* wlword_create(const char* s) {
	wlword* w = malloc(sizeof(wlword));
	w -> length = strlen(s);
	w -> word = malloc(sizeof(char) * (w -> length + 1));
	strcpy(w -> word, s);
	w -> prev = NULL;
	w -> next = NULL;
	return w;
}

wlword* wlword_clone(wlword* w) {
	wlword* nw = malloc(sizeof(wlword));
	nw -> length = w -> length;
	nw -> word = malloc(sizeof(char) * (w -> length + 1));
	strcpy(nw -> word, w -> word);
	nw -> prev = NULL;
	nw -> next = NULL;
	return nw;
}

void wlword_delete(wlword* x) {
	free(x -> word);
	free(x);
}

void wlist_appendwl(wlist* l, wlword* x) {
	if (l -> first_item == NULL) {
		l -> first_item = x;
	} else {
		l -> last_item -> next = x;
		x -> prev = l -> last_item;
	}
	l -> last_item = x;
	l -> length++;
}

void wlist_append(wlist* l, const char* s) {
	wlword* nw = wlword_create(s);
	wlist_appendwl(l, nw);
}

void wlist_extend(wlist* l, wlist* x) {
	wlword* i;
	wlist_foreach(i, x) {
		wlist_appendwl(l, wlword_clone(i));
	}
}

wlword* wlist_getwl(wlist* l, size_t i) {
	wlword* el = l -> first_item;
	while (i > 0) {
		if (el == NULL) {
			printf("wlist: Out of range!\n");
			return NULL;
		}
		el = el -> next;
		i--;
	}
	return el;
}

void wlist_insertwl(wlist* l, size_t i, wlword* x) {
	wlword* behind = wlist_getwl(l, i);
	wlword* front = behind -> prev;
	front -> next = x;
	behind -> prev = x;
	x -> prev = front;
	x -> next = behind;
	l -> length++;
}

void wlist_insert(wlist* l, size_t i, const char* s) {
	wlist_insertwl(l, i, wlword_create(s));
}

void wlist_removewl(wlist* l, wlword* x) {
	wlword* behind = x -> next;
	wlword* front = x -> prev;
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
	wlword_delete(x);
	l -> length--;
}

void wlist_removebyi(wlist* l, size_t i) {
	wlist_removewl(l, wlist_getwl(l, i));
}

wlword* wlist_wlbystr(wlist* l, const char* s) {
	if (l -> first_item == NULL) {
		return NULL;
	}
	wlword* i;
	wlist_foreach(i, l) {
		if (!strcmp(i -> word, s)) { // Higher-order language speak: i -> word == s
			return i;
		}
	}
	return NULL;
}

/**
 * This will return inaccurate results if the word is not in the list.
 * Use wlist_contains to check first.
 */
size_t wlist_index(wlist* l, const char* s) {
	if (l -> first_item == NULL) {
		return 0;
	}
	size_t count = 0;
	wlword* i;
	wlist_foreach(i, l) {
		if (!strcmp(i -> word, s)) { // Higher-order language speak: i -> word == s
			return count;
		}
		count++;
	}
	return 0;
}

void wlist_remove(wlist* l, const char* s) {
	wlist_removewl(l, wlist_wlbystr(l, s));
}

void wlist_clear(wlist* l) {
	wlword* i = l -> first_item;
	wlword* tmp;
	while (i != NULL) {
		tmp = i -> next;
		// For those looking for example, you can insert your code for each iteration between "tmp = i -> next;" and "i = tmp;" statements.
		wlword_delete(i);
		i = tmp;
	}
	l -> first_item = NULL;
	l -> last_item = NULL;
	l -> length = 0;
}

size_t wlist_count(wlist* l, const char* s) {
	if (l -> first_item == NULL) {
		return 0;
	}
	size_t count = 0;
	wlword* i;
	wlist_foreach(i, l) {
		if (!strcmp(i -> word, s)) { // Higher-order language speak: i -> word == s
			count++;
		}
	}
	return count;
}

char wlist_contains(wlist* l, const char* s) {
	if (l -> first_item == NULL) {
		return 0;
	}
	wlword* i;
	wlist_foreach(i, l) {
		if (!strcmp(i -> word, s)) { // Higher-order language speak: i -> word == s
			return 1;
		}
	}
	return 0;
}

void wlist_set(wlist* l, size_t i, const char* s) {
	wlword* w = wlist_getwl(l, i);
	w -> word = realloc(w -> word, sizeof(char) * (strlen(s) + 1));
	strcpy(w -> word, s);
}

/**
 * Returns a pointer to the stored null-terminated string.
 */
char* wlist_get(wlist* l, size_t i) {
	return wlist_getwl(l, i) -> word;
}

wlist* wlist_create() {
	wlist* nl = malloc(sizeof(wlist));
	nl -> first_item = NULL;
	nl -> last_item = NULL;
	nl -> length = 0;
	return nl;
}

wlist* wlist_clone(const wlist* w) {
	wlist* newlist = malloc(sizeof(wlist));
	newlist -> first_item = NULL;
	newlist -> last_item = NULL;
	newlist -> length = 0;
	wlword* i;
	wlist_foreach(i, w) {
		wlist_appendwl(newlist, wlword_clone(i));
	}
	return newlist;
}

void wlist_delete(wlist* x) {
	if (x == NULL) {
		return;
	}
	wlist_clear(x);
	free(x);
}
