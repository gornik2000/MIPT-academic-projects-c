/*!
//  \file Stack.cpp
//
//  Contains definition of structure stack and functions for working with stack
*/

#include <stdio.h>
#include <cstdlib>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//------------------------------------------------------------------------------

/*!
//  \var const int MINSTACKAPACITY
//
//  Minimal size of stack capacity
*/

const int MINSTACKCAPACITY = 5;

/*!
//  \var const int POISON
//
//  Variable with which we clear memory
*/

const int POISON = 0;

/*!
// \var const float increaseCapacityCoefficient
//
//  Coefficient of capacity increasing
*/

const float increaseCapacityCoefficient = 2;

/*!
// \var const float decreaseCapacityCoefficient
//
//  Coefficient of capacity decreasing
*/

const float decreaseCapacityCoefficient = 0.5;

/*!
//  \typedef size_t data_t;
//
//  Typedefs size_t as data_t - type of data elements of stack
*/

typedef size_t data_t;

/*!
//  \typedef size_t elem_t;
//
//  Typedefs size_t as elem_t - type of stack arguments,
//  which should have integer type
//
//  \warning Elem_t should be defined as integer type
*/

typedef size_t elem_t;

/*!
//  \struct myStack
//
//  Structure stack, made with doctrine first in, last out
//  Has infromation about stack data, size, capacity and pointer
//  to file with logs
*/

struct myStack
{
  data_t *data;
  elem_t size;
  elem_t capacity;
  FILE  *logName;
};

/*!
//  \typedef struct myStack stack_t;
//
//  Typedefs  stack_t as structure stack
*/

typedef struct myStack stack_t;

//------------------------------------------------------------------------------

void stCtor (stack_t *s, char *fileLogName = "stackLogs.txt");
void stackDtor (stack_t *s);

void  stackPush (stack_t *s, elem_t number);
elem_t stackPop (stack_t *s);

void stackChangeCapacity (stack_t *s, float changeValue);

elem_t stackReturnCapacity (stack_t *s);
elem_t stackReturnSize     (stack_t *s);

/// RELEASE defines version of the project \
    if defined erases supporting debug functions \
    if not defined many supporting functions will be able to work
#if defined RELEASE
  #define errorDecoder(a, b, c, d, e);
  #define stIsOk(a, b, c ,d);
  #define fopenforlogs(a, b);
  #define fcloseforlogs(a);
#else
  #include "Dump.cpp"
  #define fopenforlogs(a, b) fopen((a), (b));
  #define fcloseforlogs(a) fclose((a));
#endif

/// defines stackCtor as function stCtor with already given name of file
#define stackCtor(st) \
{ \
  const char *name = "log" #st ".txt"; \
  stCtor(st, name); \
}

//------------------------------------------------------------------------------

/*!
//  \fn void stCtor (stack_t *s, const char *fileLogName)
//
//  Creates stack with MINSTACKCAPACITY capacity
//
//  \param s Pointer to stack
//  \param fileLogName Pointer to file where error logs should be putted
//  \warning Stack should be constructed
*/

void stCtor (stack_t *s, const char *fileLogName)
{
  assert (s);

  s->size = 0;

  s->capacity = MINSTACKCAPACITY;
  s->data = (data_t *)calloc (s->capacity, sizeof (*(s->data)));
  s->logName = fopenforlogs (fileLogName, "w");

  stackIsOk (s);
}

/*!
//  \fn void stackDtor (stack_t *s)
//
//   Stack destructor. Destroys information from stack.
//   Fill stack data with POISON
//
//  \param s Pointer to stack that should be destroyed
*/

void stackDtor (stack_t *s)
{
  const char *error = stackIsOk (s);
  if (error == 0)
  {
    /* delete information in stack */
    memset (s->data, POISON, (s->size) * sizeof (*(s->data)));

    free (s->data);

    s->data = NULL;
    s->size = 0;
    s->capacity = 0;

    fcloseforlogs (s->logName);
    s->logName = NULL;
  }
}

/*!
//  \fn void stackPush (stack_t *s, elem_t element)
//
//  Inputs element in stack
//
//  \param s Pointer to stack
//  \param element Element inputted in stack
*/

void stackPush (stack_t *s, elem_t element)
{
  const char *error = stackIsOk (s);
  if (error == 0)
  {
    /* if stack not full */
    if (s->capacity >= s->size + 1)
    {
      s->size ++;
      (s->data)[s->size - 1] = element;
    }
    else
    {
      stackChangeCapacity (s, increaseCapacityCoefficient);
      s->size ++;
      (s->data)[s->size - 1] = element;
    }
    stackIsOk (s);
  }
}

/*!
//  \fn elem_t stackPop (stack_t *s)
//
//  Gives the last element from stack and destroys it
//
//  \param s Pointer to stack
//  \return Last stack element
//  \warning returns 0 if stack size = 0
*/

elem_t stackPop (stack_t *s)
{
  (s->size) --;

  const char *error = stackIsOk (s);
  if (error == 0)
  {
    elem_t element = (s->data)[s->size];
    (s->data)[s->size] = POISON;

    if (((increaseCapacityCoefficient + 1) * s->size < s->capacity) \
                                                  && (s->size > 1))
    {
      stackChangeCapacity (s, decreaseCapacityCoefficient);
    }

    stackIsOk (s);

    return (element);
  }

  (s->size) ++;
  return 0;
}

/*!
//------------------------------------------------------------------------------
//  \fn elem_t stackReturnSize (stack_t *s)
//
//  Returns stack size
//
//  \param s Pointer to stack
//  \return Stack size
//------------------------------------------------------------------------------
*/

elem_t stackReturnSize (stack_t *s)
{
  const char *error = stackIsOk (s);
  if (error == 0)
  {
    return (s->size);
  }

  return (-1);
}

/*!
//  \fn elem_t stackReturnCapacity (stack_t *s)
//
//  Returns stack capacity
//
//  \param s Pointer to stack
//  \return Stack capacity
*/

elem_t stackReturnCapacity (stack_t *s)
{
  const char *error = stackIsOk (s);
  if (error == 0)
  {
    return (s->capacity);
  }

  return (-1);
}

/*!
//  \fn void stackChangeCapacity (stack_t *s, float changeValue)
//
//  Changes stack capacity in changeValue times
//
//  \param s Pointer to stack
//  \param changeValue In what times capacity is changed
*/

void stackChangeCapacity (stack_t *s, float changeValue)
{
  const char *error = stackIsOk (s);
  if (error == 0)
  {
    if ((changeValue > 1) || (s->capacity >= 1 / changeValue))
    {
      s->capacity = int (s->capacity * changeValue);
      s->data = (data_t *)realloc (s->data, s->capacity * sizeof (*(s->data)));

      stackIsOk (s);
    }
  }
}
