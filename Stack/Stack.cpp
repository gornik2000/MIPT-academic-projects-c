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

void stackCtor (stack_t *s)
{
  assert (s);

  s->canaryOne = CANARY;

  s->size = 0;
  s->capacity = MINSTACKCAPACITY;
  s->data = (data_t *)calloc (s->capacity, sizeof (*(s->data)));

  s->canaryTwo = CANARY;

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

    s->size = 0;
    s->capacity = 0;

    s->data = NULL;
    s = NULL;
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
  if (file == NULL)
  {
    file = STACK_LOGS;
  }

  if (file != NULL)
  {
    fprintf (file, " # Stack data:\n | ");

    for (elem_t i = 0; i < s->size; i++)
    {
      fprintf (file, DATA_ST_TYPE" _ ", (s->data)[i]);
    }

    fprintf (file, ";\n\n");
  }
}

