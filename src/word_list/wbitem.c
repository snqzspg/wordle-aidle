#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../error/print_error.h"
#include "../terminal_helper/cons_graphics.h"
#include "wbitem.h"

/**
 * Note that this is a linked list.
 * Can be a template for future c projects for linked lists.
 */

/**
 * Stolen from python
 */
long wbitem__make_hash(char *s) {
	register size_t len;
	register long x;

	len = strlen(s);
	x = *s << 7;
	while (--len >= 10) {
		x = (1000003 * x) ^ (*s + 1);
	}
	x ^= strlen(s);
	if (x == -1) {
		x = -2;
	}
	return x;
}

long wbitem_make_hash(const char *s) {
	char tmp[strlen(s) + 1];
	strcpy(tmp, s);
	return wbitem__make_hash(tmp);
}

wbitem* wbitem_create(const char* val) {
	wbitem* w = malloc(sizeof(wbitem));
	if (w == NULL) {
		print_error_ln("ERROR wbitem: Haiyaa no more RAM for me :(");
		return NULL;
	}
	w -> length = strlen(val);
	w -> value = malloc(sizeof(char) * (w -> length + 1));
	if (w -> value == NULL) {
		free(w);
		print_error_ln("ERROR wbitem: Haiyaa no more RAM for me :(");
		return NULL;
	}
	strcpy(w -> value, val);
	w -> prev = NULL;
	w -> next = NULL;
	w -> mapprev = NULL;
	w -> mapnext = NULL;
	w -> hash = wbitem_make_hash(val);
	return w;
}

wbitem* wbitem_clone(wbitem* w) {
	wbitem* nw = malloc(sizeof(wbitem));
	nw -> length = w -> length;
	nw -> value = malloc(sizeof(char) * (w -> length + 1));
	strcpy(nw -> value, w -> value);
	nw -> prev = NULL;
	nw -> next = NULL;
	w -> mapprev = NULL;
	w -> mapnext = NULL;
	w -> hash = wbitem_make_hash(w -> value);
	return nw;
}

void wbitem_delete(wbitem* x) {
	free(x -> value);
	free(x);
}

void wbilist_append(wbilist* l, wbitem* x) {
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

void wbilist_extend(wbilist* l, wbilist* x) {
	wbitem* i;
	wbilist_foreach(i, x) {
		wbilist_append(l, wbitem_clone(i));
	}
}

wbitem* wbilist_get(wbilist* l, size_t i) {
	wbitem* el = l -> first_item;
	while (i > 0) {
		if (el == NULL) {
			printf("wbilist: Out of range!\n");
			return NULL;
		}
		el = el -> next;
		i--;
	}
	return el;
}

void wbilist_insert(wbilist* l, size_t i, wbitem* x) {
	wbitem* behind = wbilist_get(l, i);
	wbitem* front = behind -> prev;
	front -> next = x;
	behind -> prev = x;
	x -> prev = front;
	x -> next = behind;
}

void wbilist_remove(wbilist* l, wbitem* x) {
	wbitem* behind = x -> next;
	wbitem* front = x -> prev;
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
	wbitem_delete(x);
	l -> length--;
}

void wbilist_removebyi(wbilist* l, size_t i) {
	wbilist_remove(l, wbilist_get(l, i));
}

wbitem* wbilist_wlbystr(wbilist* l, wbitem* s) {
	if (l -> first_item == NULL) {
		return NULL;
	}
	wbitem* i;
	wbilist_foreach(i, l) {
		if (i == s) {
			return i;
		}
	}
	return NULL;
}

/**
 * This will return inaccurate results if the word is not in the list.
 * Use wbilist_contains to check first.
 */
size_t wbilist_index(wbilist* l, wbitem* s) {
	if (l -> first_item == NULL) {
		return 0;
	}
	size_t count = 0;
	wbitem* i;
	wbilist_foreach(i, l) {
		if (i == s) {
			return count;
		}
		count++;
	}
	return 0;
}

void wbilist_clear(wbilist* l) {
	wbitem* i = l -> first_item;
	wbitem* tmp;
	while (i != NULL) {
		tmp = i -> next;
		wbitem_delete(i);
		i = tmp;
	}
	l -> first_item = NULL;
	l -> last_item = NULL;
	l -> length = 0;
}

size_t wbilist_count(wbilist* l, wbitem* s) {
	if (l -> first_item == NULL) {
		return 0;
	}
	size_t count = 0;
	wbitem* i;
	wbilist_foreach(i, l) {
		if (i == s) {
			count++;
		}
	}
	return count;
}

char wbilist_contains(wbilist* l, wbitem* s) {
	if (l -> first_item == NULL) {
		return 0;
	}
	wbitem* i;
	wbilist_foreach(i, l) {
		if (i == s) {
			return 1;
		}
	}
	return 0;
}

//void wbilist_set(wbilist* l, size_t i, wbitem* s) {
//	wbitem* w = wbilist_getwl(l, i);
//	w -> word = realloc(w -> word, sizeof(char) * (strlen(s) + 1));
//	strcpy(w -> word, s);
//}

wbilist* wbilist_create() {
	wbilist* nl = malloc(sizeof(wbilist));
	nl -> first_item = NULL;
	nl -> last_item = NULL;
	nl -> length = 0;
	return nl;
}

void wbilist_delete(wbilist* x) {
	wbilist_clear(x);
	free(x);
}
