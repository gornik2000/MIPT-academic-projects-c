#include "list.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int listCtor(list *l, unsigned int capacity) {
	assert(l);

	l->data = (list_data_t *) calloc(capacity + 1, sizeof(*(l->data)));
	l->next = (int *) calloc(capacity + 1, sizeof(*(l->next)));
	l->prev = (int *) calloc(capacity + 1, sizeof(*(l->prev)));

	if (l->data == NULL || l->next == NULL || l->prev == NULL) {
		l->errNo = LIST_ALLOCATION_ERROR;
		return l->errNo;
	}

	l->data[0] = 0;
	l->next[0] = 0;
	l->prev[0] = 0;
	for (int i = 1; i < capacity; i++) { // free list
		l->next[i] = i + 1;
		l->prev[i] = -1;
	}
	l->next[capacity] = 0;
	l->prev[capacity] = -1;

	l->capacity = capacity;
	l->size = 0;
	l->head = 0;
	l->tail = 0;
	l->free = 1;

	l->errNo = LIST_NO_ERROR;

	l->sorted = 1;

	// call OK !!!!

	return l->errNo;
}

void listDtor(list *l) {
	assert(l);
	assert(l->data);
	assert(l->next);
	assert(l->prev);

	for (int i = 0; i < l->capacity; i++) {
		if (l->data[i] != NULL) {
			free(l->data[i]->key);
			free(l->data[i]);
		}
	}
	free(l->data);
	l->data = NULL;
	free(l->next);
	l->next = NULL;
	free(l->prev);
	l->prev = NULL;

	l->capacity = 0;
	l->size = 0;
	l->head = 0;
	l->tail = 0;
	l->free = 0;

	l->errNo = 0;

	l->sorted = 0;
}

int listOk(list *l) {
	if (l == NULL)
		return 0;

	if (l->errNo != LIST_NO_ERROR)
		return 0;

	if (l->size > l->capacity) {
		l->errNo = LIST_SIZE_ERROR;
		return 0;
	}

	if (!((l->head == 0 && l->tail == 0) ||
			(l->head > 0 && l->tail > 0))) {
		l->errNo = LIST_DATA_LIST_ERROR;
		return 0;
	}

	if (l->prev[l->head] != 0 || l->next[l->tail] != 0) {
		l->errNo = LIST_DATA_LIST_ERROR;
		return 0;
	}

	if (l->size > 1) {
		if (l->next[l->head] < 0 || l->prev[l->tail] < 0 ||
				l->prev[l->next[l->head]] != l->head ||
				l->next[l->prev[l->tail]] != l->tail) {
			l->errNo = LIST_DATA_LIST_ERROR;
			return 0;
		}
	}

	if (l->size > 2) {
		int it = l->next[l->head];
		for (int i = 1; i < l->size - 1; i++, it = l->next[it]) {
			if (l->prev[it] <= 0 || l->next[it] <= 0 ||
					l->prev[l->next[it]] != it ||
					l->next[l->prev[it]] != it) {
				l->errNo = LIST_DATA_LIST_ERROR;
				return 0;
			}
		}
		if (l->next[it] != l->tail) {
			l->errNo = LIST_DATA_LIST_ERROR;
			return 0;
		}
	}

	int it = l->free;
	for (int i = 0; i < l->capacity - l->size - 1; i++, it = l->next[it]) {
		if (l->prev[it] != -1 || l->next[it] <= 0) {
			l->errNo = LIST_FREE_LIST_ERROR;
			return 0;
		}
	}
	if (l->next[it] != 0) {
		l->errNo = LIST_FREE_LIST_ERROR;
		return 0;
	}

	if (l->sorted) {
		it = l->head;
		while (l->next[it] != 0) {
			if (l->next[it] != it + 1) {
				l->errNo = LIST_SORTED_ERROR;
				return 0;
			}
			it = l->next[it];
		}

		if (l->tail + 1 != l->free) {
			l->errNo = LIST_SORTED_ERROR;
			return 0;
		}

		it = l->free;
		while (l->next[it] != 0) {
			if (l->next[it] != it + 1) {
				l->errNo = LIST_SORTED_ERROR;
				return 0;
			}
			it = l->next[it];
		}
	}

	return 1;
}

void plotListGraph(list *l) {
	assert(l);

	FILE *out = fopen("list.dv", "wb");
	fprintf(out, "digraph list {\n");
	fprintf(out, "\trankdir=LR;\n");
	fprintf(out, "\te0 [color=grey, shape=box];\n");
	for (int i = 1; i <= l->capacity; i++) {
		fprintf(out, "\te%d [color=%s, shape=box", i, l->prev[i] == -1 ? "red" : "green");
		if (i == l->head) {
			fprintf(out, ", xlabel = \"head\"");
		}
		if (i == l->tail) {
			fprintf(out, ", xlabel = \"tail\"");
		}
		if (i == l->free) {
			fprintf(out, ", xlabel = \"free\"");
		}
		fprintf(out, "];\n");
	}
	for (int i = 0; i <= l->capacity; i++) {
		fprintf(out, "\te%d -> e%d [color=green];\n", i, l->next[i]);
		if (l->prev[i] != -1)
			fprintf(out, "\te%d -> e%d [color=blue];\n", i, l->prev[i]);
	}
	fprintf(out, "}\n");
	fclose(out);

	system("dotty list.dv");

	remove("list.dv");
}

void listDump(list *l) {
	assert(l);

	printf("list [%p] {\n", l);
	printf("\terrno = %d\n", l->errNo);
	printf("\tdata [%p]\n", l->data);
	printf("\tnext [%p]\n", l->next);
	printf("\tprev [%p]\n", l->prev);
	printf("\thead = %d, tail = %d\n", l->head, l->tail);
	printf("\tfree = %d\n", l->free);
	printf("\tsize = %d\n", l->size);
	printf("\tcapacity = %d\n", l->capacity);
	printf("\tdata {\n");
	for (int i = 0; i <= l->capacity; i++) {
		if (l->data[i] != NULL && l->data[i]->key != NULL)
			printf("\t%d: {%s, %d}, %d, %d\n", i, l->data[i]->key, l->data[i]->val, l->next[i], l->prev[i]);
	}
	printf("\t}\n");
	printf("\tsorted = %d\n", l->sorted);
	printf("}\n");

	plotListGraph(l);
}

int listInsert(list *l, int idx, list_data_t el) {
	assert(l);

	if (idx > l->capacity || idx < 0 || l->prev[idx] == -1) {
		l->errNo = LIST_INDEX_OUT_OF_BOUNDS;
		return l->errNo;
	}

	if (l->free == 0) {
		listResize(l, 2 * l->capacity);
		if (l->free == 0) {
			l->errNo = LIST_ALLOCATION_ERROR;
			return l->errNo;
		}
	}

	if (l->head == 0) {
		l->head = l->free;
		l->tail = l->free;
		l->free = l->next[l->free];
		l->data[l->head] = el;

		l->prev[l->head] = 0;
		l->next[l->head] = 0;

		l->size++;
		return l->errNo;
	}

	if (idx == 0) { // inserting before head
		int newHead = l->free;
		l->free = l->next[l->free];
		l->data[newHead] = el;

		l->prev[newHead] = 0;
		l->next[newHead] = l->head;
		l->prev[l->head] = newHead;
		l->head = newHead;

		l->size++;
		l->sorted = 0;
		return l->errNo;
	}

	if (idx == l->tail) { // inserting after tail
		int newTail = l->free;
		l->free = l->next[l->free];
		l->data[newTail] = el;

		l->next[newTail] = 0;
		l->prev[newTail] = l->tail;
		l->next[l->tail] = newTail;
		l->tail = newTail;

		l->size++;
		return l->errNo;
	}

	int newNode = l->free;
	l->free = l->next[l->free];
	l->data[newNode] = el;

	l->next[newNode] = l->next[idx];
	l->prev[l->next[idx]] = newNode;
	l->next[idx] = newNode;
	l->prev[newNode] = idx;

	l->size++;
	l->sorted = 0;

	return l->errNo;
}

int listRemove(list *l, int idx) {
	assert(l);

	if (idx > l->capacity || idx <= 0 || l->prev[idx] == -1) {
		l->errNo = LIST_INDEX_OUT_OF_BOUNDS;
		return l->errNo;
	}

	if (l->size == 1) {
		free(l->data[idx]->key);
		free(l->data[idx]);
		l->data[idx] = NULL;
		l->next[l->head] = l->free;
		l->prev[l->head] = -1;

		l->free = l->head;
		l->head = 0;
		l->tail = 0;

		l->size--;
		l->sorted = 1;
		return l->errNo;
	}

	if (idx == l->head) {
		l->head = l->next[l->head];
		l->prev[l->head] = 0;

		free(l->data[idx]->key);
		free(l->data[idx]);
		l->data[idx] = NULL;
		l->next[idx] = l->free;
		l->prev[idx] = -1;
		l->free = idx;

		l->size--;
		l->sorted = 0;
		return l->errNo;
	}

	if (idx == l->tail) {
		l->tail = l->prev[l->tail];
		l->next[l->tail] = 0;

		free(l->data[idx]->key);
		free(l->data[idx]);
		l->data[idx] = NULL;
		l->next[idx] = l->free;
		l->prev[idx] = -1;
		l->free = idx;

		l->size--;
		return l->errNo;
	}

	l->next[l->prev[idx]] = l->next[idx];
	l->prev[l->next[idx]] = l->prev[idx];

	l->data[idx] = 0;
	l->next[idx] = l->free;
	l->prev[idx] = -1;
	l->free = idx;

	l->size--;
	l->sorted = 0;

	return l->errNo;
}

int listClear(list *l) {
	assert(l);

	for (int i = 1; i < l->capacity; i++) {
		if (l->data[i] != NULL) {
			free(l->data[i]->key);
			free(l->data[i]);
			l->data[i] = NULL;
		}

	}

	l->data[0] = 0;
	l->next[0] = 0;
	l->prev[0] = 0;
	for (int i = 1; i < l->capacity; i++) { // free list
		l->next[i] = i + 1;
		l->prev[i] = -1;
	}
	l->next[l->capacity] = 0;
	l->prev[l->capacity] = -1;

	l->capacity = l->capacity;
	l->size = 0;
	l->head = 0;
	l->tail = 0;
	l->free = 1;

	l->errNo = LIST_NO_ERROR;

	l->sorted = 1;

	return l->errNo;
}

int listFindVerySlow(list *l, const char *key) {
	assert(l);

	int it = l->head;
	while (it != 0) {
		if (strcmp(key, l->data[it]->key) == 0)
			return it;
		it = l->next[it];
	}

	return 0;
}

int listNext(list *l, int idx) {
	assert(l);

	if (idx > l->capacity || idx <= 0) {
		l->errNo = LIST_INDEX_OUT_OF_BOUNDS;
		return -1;
	}

	return l->next[idx];
}

int listHasNext(list *l, int idx) {
	assert(l);

	if (idx > l->capacity || idx <= 0) {
		l->errNo = LIST_INDEX_OUT_OF_BOUNDS;
		return 0;
	}

	return l->next[idx] != 0;
}

int listPrev(list *l, int idx) {
	assert(l);

	if (idx > l->capacity || idx <= 0) {
		l->errNo = LIST_INDEX_OUT_OF_BOUNDS;
		return -1;
	}

	return l->prev[idx];
}

int listHasPrev(list *l, int idx) {
	assert(l);

	if (idx > l->capacity || idx <= 0) {
		l->errNo = LIST_INDEX_OUT_OF_BOUNDS;
		return 0;
	}

	return l->prev[idx] != 0;
}

int listHead(list *l) {
	assert(l);

	return l->head;
}

int listTail(list *l) {
	assert(l);

	return l->tail;
}

list_data_t listGet(list *l, int idx) {
	assert(l);

	if (idx > l->capacity || idx <= 0) {
		l->errNo = LIST_INDEX_OUT_OF_BOUNDS;
		return 0;
	}

	return l->data[idx];
}

int listErrno(list *l) {
	assert(l);

	return l->errNo;
}

void listResetErrno(list *l) {
	assert(l);

	l->errNo = 0;
}

unsigned int listSize(list *l) {
	assert(l);

	return l->size;
}

int listSort(list *l) {
	assert(l);

	int it = l->head, n = 1;
	while (it != 0) {
		if (it != n) {
			listSwap(l, it, n);
		}
		it = l->next[n];
		n++;
	}

	l->free = n;
	for (; n < l->capacity; n++) {
		l->data[n] = 0;
		l->next[n] = n + 1;
		l->prev[n] = -1;
	}
	l->data[l->capacity] = 0;
	l->next[l->capacity] = 0;
	l->prev[l->capacity] = -1;

	l->sorted = 1;

	return l->errNo;
}

int listSwap(list *l, int i, int j) {
	assert(l);

	if (i > l->capacity || i <= 0 || j > l->capacity || j <= 0) {
		l->errNo = LIST_INDEX_OUT_OF_BOUNDS;
		return -1;
	}

	if (l->prev[i] == -1 && l->prev[j] == -1) {
		return l->errNo;
	}

	if (l->prev[j] == -1 && l->prev[i] != -1) {
		int t = i;
		i = j;
		j = t;
	}

	if (l->prev[i] == -1 && l->prev[j] != -1) {
		int inext = l->next[i];
		int jnext = l->next[j], jprev = l->next[j];

		l->data[i] = l->data[j];
		if (j == l->tail) {
			l->tail = i;
		} else {
			l->prev[jnext] = i;
		}
		if (j == l->head) {
			l->head = i;
		} else {
			l->next[jprev] = i;
		}
		l->next[i] = jnext;
		l->prev[i] = jprev;

		l->data[j] = 0;
		l->next[j] = l->free;
		l->prev[j] = -1;

		int it = l->free;
		while (l->next[it] != i) {
			it = l->next[it];
		}
		l->next[it] = inext;
		l->free = j;

		l->sorted = 0;

		return l->errNo;
	}

	if (l->prev[i] != -1 && l->prev[j] != -1) {
		if (l->next[i] == j || l->next[j] == i) { // changing two subsequent elements
			if (l->next[j] == i) {
				int t = i;
				i = j;
				j = t;
			}

			list_data_t t = l->data[i];
			l->data[i] = l->data[j];
			l->data[j] = t;

			int jnext = l->next[j], iprev = l->prev[i];
			int newHead = 0, newTail = 0;

			if (j == l->tail) {
				newTail = i;
			} else {
				l->prev[jnext] = i;
			}
			l->prev[i] = j;
			l->next[i] = jnext;

			if (i == l->head) {
				newHead = j;
			} else {
				l->next[iprev] = j;
			}
			l->next[j] = i;
			l->prev[j] = iprev;

			if (newHead != 0) {
				l->head = newHead;
			}

			if (newTail != 0) {
				l->tail = newTail;
			}

			l->sorted = 0;

			return l->errNo;
		}

		int inext = l->next[i], iprev = l->prev[i];
		int jnext = l->next[j], jprev = l->prev[j];
		int newHead = 0, newTail = 0;

		list_data_t t = l->data[i];
		l->data[i] = l->data[j];
		l->data[j] = t;

		if (j == l->tail) {
			newTail = i;
		} else {
			l->prev[jnext] = i;
		}
		if (j == l->head) {
			newHead = i;
		} else {
			l->next[jprev] = i;
		}
		l->next[i] = jnext;
		l->prev[i] = jprev;

		if (i == l->tail) {
			newTail = j;
		} else {
			l->prev[inext] = j;
		}
		if (i == l->head) {
			newHead = j;
		} else {
			l->next[iprev] = j;
		}
		l->next[j] = inext;
		l->prev[j] = iprev;

		if (newHead != 0) {
			l->head = newHead;
		}

		if (newTail != 0) {
			l->tail = newTail;
		}

		l->sorted = 0;

		return l->errNo;
	}

	return l->errNo;
}

int listResize(list *l, unsigned int size) {
	assert(l);

	if (size <= l->capacity) {
		return 1;
	}

	list_data_t *newData = (list_data_t *) realloc(l->data, (size + 1) * sizeof(*newData));
	int *newNext = (int *) realloc(l->next, (size + 1) * sizeof(*newNext));
	int *newPrev = (int *) realloc(l->prev, (size + 1) * sizeof(*newPrev));
	if (newData == NULL || newNext == NULL || newPrev == NULL) {
		l->errNo = LIST_ALLOCATION_ERROR;
		return l->errNo;
	}

	l->data = newData;
	l->next = newNext;
	l->prev = newPrev;

	int newFreePos = l->capacity + 1;
	if (l->free == 0) {
		l->free = newFreePos++;
		l->data[l->free] = 0;
		l->next[l->free] = newFreePos;
		l->prev[l->free] = -1;
	} else {
		int it = l->free;
		for (; l->next[it] != 0; it = l->next[it]);
		l->next[it] = newFreePos;
	}
	for (int i = newFreePos; i < size; i++) {
		l->data[i] = 0;
		l->next[i] = i + 1;
		l->prev[i] = -1;
	}
	l->data[size] = 0;
	l->next[size] = 0;
	l->prev[size] = -1;

	l->capacity = size;

	return l->errNo;
}

void listIterate(list *l, void func(struct l_node *node)) {
	assert(l);
	assert(func);

	for (int it = l->head; it != 0; it = l->next[it]) {
		func(l->data[it]);
	}
}
