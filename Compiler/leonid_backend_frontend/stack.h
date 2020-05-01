#ifndef STACK_H

#define STACK_H

typedef char *data_t;
typedef struct stack stack;

enum stackError {
	STACK_NO_ERROR,
	STACK_ALLOCATION_ERROR,
	STACK_REALLOCATION_ERROR,
	STACK_UNDERFLOW,
};

struct stack {
	data_t *data;
	unsigned int size;
	unsigned int capacity;
	int errNo;
	int hash;
};

void stackCtor(stack *s);
void stackDtor(stack *s);

void stackSetCapacity(stack *s, int c);

int stackOk(stack *s);

int stackPush(stack *s, data_t val);
data_t stackPop(stack *s);

unsigned int stackSize(stack *s);
unsigned int stackCapacity(stack *s);
int stackErrno(stack *s);
void stackResetErrno(stack *s);

void stackClear(stack *s);

int stackCheckHash(stack *s);

void stackDump(stack *s);

#include "stack.c"

#endif
