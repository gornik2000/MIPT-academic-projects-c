#ifndef LIST_H

#define LIST_H

typedef struct list list;
typedef struct l_node *list_data_t;
typedef char *m_key_t;
typedef int val_t;

enum listError {
	LIST_NO_ERROR,
	LIST_ALLOCATION_ERROR,
	LIST_DATA_LIST_ERROR,
	LIST_FREE_LIST_ERROR,
	LIST_SORTED_ERROR,
	LIST_SIZE_ERROR,
	LIST_INDEX_OUT_OF_BOUNDS
};

struct list {
	list_data_t *data;
	int *next;
	int *prev;
	unsigned int capacity;
	int head;
	int tail;
	int free;
	unsigned int size;
	int sorted;
	int errNo;
};

struct l_node {
	m_key_t key;
	val_t val;
};

int listCtor(list *l, unsigned int capacity);
void listDtor(list *l);

int listOk(list *l);

void listDump(list *l);

int listInsert(list *l, int idx, list_data_t el);
int listRemove(list *l, int idx);
int listClear(list *l);

int listFindVerySlow(list *l, const char *key);

int listNext(list *l, int idx);
int listHasNext(list *l, int idx);
int listPrev(list *l, int idx);
int listHasPrev(list *l, int idx);

int listHead(list *l);
int listTail(list *l);

list_data_t listGet(list *l, int idx);

int listErrno(list *l);
void listResetErrno(list *l);

unsigned int listSize(list *l);

int listSort(list *l);
int listSwap(list *l, int i, int j);

int listResize(list *l, unsigned int size);

void listIterate(list *l, void func(struct l_node *node));

#include "list.c"

#endif
