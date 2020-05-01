/*! \file stack.c
*
*/

#include "stack.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define START_SIZE 10

#ifndef NDEBUG

#define ASSERT_OK(s) \
	if (!stackOk(s)) { \
		stackDump(s); \
		assert(0); \
	}

#else

#define ASSERT_OK(s) ;

#endif
/*
int main() {
	stack s = {};
	stackCtor(&s);
	ASSERT_OK(&s);
	for (int i = 0; i < 20; i++) {
		stackPush(&s, i);
		printf("%d ", i);
	}
	printf("\n");

	stackDump(s);

	for (int i = 0; i < 20; i++) {
		printf("%g ", stackPop(&s));
	}
	printf("\n");

	stackDump(s);

	stackDtor(&s);
}
*/

/*!
* \brief stack creator
*/

void stackCtor(stack *s) {
	assert(s);

	s->errNo = STACK_NO_ERROR;
	s->capacity = 0;
	s->data = NULL;
	stackSetCapacity(s, START_SIZE);
	s->size = 0;
	s->hash = 0;
}

/*!
* \brief stack destructor
*/

void stackDtor(stack *s) {
	assert(s);

	memset(s->data, 0, s->capacity * sizeof(data_t));
	free(s->data);
	s->data = NULL;
	s->size = 0;
	s->capacity = 0;
	s->errNo = STACK_NO_ERROR;
	s->hash = 0;
}

/*!
* \brief change stack capacity to c
*/

void stackSetCapacity(stack *s, int c) {
	assert(s);
	assert(c);

	if (s->capacity == 0) {
		if ((s->data = (data_t *) calloc(c, sizeof(data_t))) == NULL) {
			s->errNo = STACK_ALLOCATION_ERROR;
		} else {
			s->capacity = c;
		}
		return;
	}

	data_t *tmp = (data_t *) realloc(s->data, c * sizeof(data_t));
	if (tmp == NULL) {
		s->errNo = STACK_REALLOCATION_ERROR;
	} else {
		s->capacity = c;
		s->data = tmp;
	}
}

/*!
* \brief push element with val to the top of the stck s
*/

int stackPush(stack *s, data_t val) {
	assert(s);
	ASSERT_OK(s);

	if (s->size == s->capacity) {
		stackSetCapacity(s, s->capacity * 2);
		ASSERT_OK(s);
	}

	s->data[s->size++] = val;

	#ifdef CHECK_HASH
	int x = *((int *) &val);
	s->hash += x * s->size;
	#endif

	return s->errNo;
}

/*!
* \brief pop top element from stack s
*/

data_t stackPop(stack *s) {
	assert(s);
	ASSERT_OK(s);

	if (s->size == 0) {
		s->errNo = STACK_UNDERFLOW;
		return 0;
	}

	data_t val = s->data[--(s->size)];
	s->data[s->size] = 0;
	if (s->size < s->capacity / 4) {
		stackSetCapacity(s, s->capacity / 2);
	}
	ASSERT_OK(s);

	#ifdef CHECK_HASH
	int x = *((int *) &val);
	s->hash -= x * (s->size + 1);
	#endif

	return val;
}

/*!
* \brief clears stack s
*/

void stackClear(stack *s) {
	assert(s);

	s->size = 0;
	memset(s->data, 0, s->capacity * sizeof(data_t));
	stackSetCapacity(s, START_SIZE);
	s->hash = 0;
}

/*!
* \return size of stack s
*/

unsigned int stackSize(stack *s) {
	assert(s);

	return s->size;
}

/*!
* \return capacity of stack s
*/

unsigned int stackCapacity(stack *s) {
	assert(s);

	return s->capacity;
}

/*!
* \return errno of stack s
*/

int stackErrno(stack *s) {
	assert(s);

	return s->errNo;
}

/*!
* \brief set stack's errno to zero
*/
void stackResetErrno(stack *s) {
	assert(s);

	s->errNo = 0;
}

/*!
* \brief check if stack s operable
* \return 0 if stack isn't operable, non-zero otherwise
*/

int stackOk(stack *s) {
	if (s == NULL)
		return 0;

	if (s->errNo != STACK_NO_ERROR) return 0;

	if (s->data == NULL || s->capacity == 0 || s->size >= s->capacity)
		return 0;

	return 1;
}

/*!
* \brief check precomputed hash with real hash
*/

#ifdef CHECK_HASH
int stackCheckHash(stack *s) {
	int realHash = 0;
	for (int i = 0; i < s->size; i++) {
		realHash += (i + 1) * *((int *)(s->data + i));
	}
	if (s->hash == realHash) {
		return 1;
	} else {
		return 0;
	}
}
#else
int stackCheckHash(stack *s) {
	return 1;
}
#endif

/*!
* \brief prints debug information
*/

void stackDump(stack *s) {
	printf("stack [%p] {\n", s);
	printf("\tcapacity = %d\n", s->capacity);
	printf("\tsize = %d\n", s->size);
	printf("\terrno = %d\n", s->errNo);
	printf("\thash = %d\n", s->hash);
	printf("\tdata[%d]:[%p] {\n", s->size, s->data);
	for (int i = 0; i < s->size; i++)
		printf("\t\t[%d] = %d\n", i, s->data[i]);
	printf("\t}\n");
	printf("}\n");
}
