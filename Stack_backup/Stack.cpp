/*!
//  \file Stack.cpp
//
//  Contains definition of structure stack and functions for working with stack
*/

#include "Stack_Header.cpp"

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

void stCtor (stack_t *s)
{
  assert (s);

  s->size = 0;
  s->capacity = MINSTACKCAPACITY;
  s->data = (data_t *)calloc (s->capacity, sizeof (*(s->data)));
}

/*!
//  \fn void stackDtor (stack_t *s)
//
//   Stack destructor. Destroys information from stack.
//   Fill stack data with POISON
//
//  \param s Pointer to stack that should be destroyed
*/

void stDtor (stack_t *s)
{
    /* delete information in stack */
    memset (s->data, POISON, (s->size) * sizeof (*(s->data)));

    free (s->data);

    s->size = 0;
    s->capacity = 0;

    s->data = NULL;
    s = NULL;
}

/*!
//  \fn void stackPush (stack_t *s, elem_t element)
//
//  Inputs element in stack
//
//  \param s Pointer to stack
//  \param element Element inputted in stack
*/

void stPush (stack_t *s, elem_t element)
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

elem_t stPop (stack_t *s)
{
  s->size --;

  elem_t element = (s->data)[s->size];
  (s->data)[s->size] = POISON;

  if (((increaseCapacityCoefficient + 1) * s->size < s->capacity) \
                                                && (s->size > 1))
  {
    stackChangeCapacity (s, decreaseCapacityCoefficient);
  }

  return (element);
}

elem_t stSafePop (stack_t *s)
{
  const char *error = stackIsOk (s)
  if (error == 0)
  {
    return stPop (s);
  }

  s->size ++;
  return 0;
}

/*!
//  \fn void stackChangeCapacity (stack_t *s, float changeValue)
//
//  Changes stack capacity in changeValue times
//
//  \param s Pointer to stack
//  \param changeValue In what times capacity is changed
*/

void stChangeCapacity (stack_t *s, float changeValue)
{
  if ((changeValue > 1) || (s->capacity >= 1 / changeValue))
  {
    s->capacity = int (s->capacity * changeValue);
    s->data = (data_t *)realloc (s->data, s->capacity * sizeof (*(s->data)));
  }
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

elem_t stReturnSize (stack_t *s)
{
  return (s->size);
}

/*!
//  \fn elem_t stackReturnCapacity (stack_t *s)
//
//  Returns stack capacity
//
//  \param s Pointer to stack
//  \return Stack capacity
*/

elem_t stReturnCapacity (stack_t *s)
{
  return (s->capacity);
}

/*!
//  \fn void stackFPrintData (stack_t *s, FILE *file)
//
//  Prints stack data into file
//
//  \param s Pointer to stack
//  \param file File in witch info will be inputed
//  \warning DATA_ST_TYPE should be defined
*/

void stackFPrintData (stack_t *s, FILE *file)
{
  assert (file);

  fprintf (file, " # Stack data:\n | ");

  for (elem_t i = 0; i < s->size; i++)
  {
    fprintf (file, DATA_ST_TYPE" _ ", (s->data)[i]);
  }

  fprintf (file, ";\n\n");

}

