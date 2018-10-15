/*!
//  \file StackUnitTesting.cpp
//
//  Contains function for testing stack
*/

/*!
//  \var const char *FILESTATISTICSNAME
//
//  Name of file where testing statistics should be printed
*/
const char *FILESTATISTICSNAME = "Testing_errors.txt";
FILE *fileStat = fopen(FILESTATISTICSNAME, "w");

void stackTesting ();

int stackTestPushPop (int testsNumber = 500);
int stackTestCreateDestruct(int testsNumber = 500);
int stackTestChangeCapacity (int testsNumber = 20);

//------------------------------------------------------------------------------

/*!
// \fn void stackTesting ()
//
//  Main testing function. All the information would be printed in files
*/

void stackTesting ()
{
  int errors = 0;

  errors += stackTestCreateDestruct();
  errors += stackTestPushPop ();
  errors += stackTestChangeCapacity ();

  fprintf (fileStat, "\n # %d ERRORS occur in the whole testing\n", errors);

  fclose (fileStat);
}

/*!
// \fn int stackTestCreateDestruct(int testsNumber)
//
//  Function witch tests creation and destruction of stack
//
//  \param testNumber Numbers of needed tests
//  \return Number of errors
*/

int stackTestCreateDestruct(int testsNumber)
{
  fprintf (fileStat, " # Starting Create/Destruct testing\n");

  int errors = 0;
  int maxErrors = testsNumber * 4;
  stack_t s = {};

  for (elem_t i = 0; i < testsNumber; i ++)
  {
    stackCtor (&s);
    stackPush (&s, 10);

    if ((&s)->data == NULL)
    {
      fprintf (fileStat, " | ERROR expected not data NULL pointer, got %d\n" \
                       , (&s)->data);
      errors += 1;
    }

    stackDtor (&s);

    if ((&s)->data != NULL)
    {
      fprintf (fileStat, " | ERROR expected NULL data pointer, got %d\n" \
                       , (&s)->data);
      errors += 1;
    }

    if ((&s)->size != 0)
    {
      fprintf (fileStat, " | ERROR expected 0 size, got %d\n" \
                       , (&s)->data);
      errors += 1;
    }

    if ((&s)->capacity != 0)
    {
      fprintf (fileStat, " | ERROR expected 0 capacity, got %d\n" \
                       , (&s)->data);
      errors += 1;
    }
  }

  fprintf (fileStat, " # %d/%d ERRORS occur in Create/Destruct testing\n\n", \
             errors, maxErrors);
  return (errors);
}

/*!
// \fn int stackTestPushPop (int testsNumber)
//
//  Function witch tests stack push and pop
//
//  \param testNumber Numbers of needed tests
//  \return Number of errors
*/

int stackTestPushPop (int testsNumber)
{
  fprintf (fileStat, " # Starting Push/Pop testing\n");

  int errors = 0;
  int maxErrors = (testsNumber - 1) * testsNumber / 2;

  stack_t s = {};
  stackCtor (&s);
  for (elem_t startingNumber = 0; startingNumber < testsNumber; startingNumber ++)
  {
    for (elem_t i = startingNumber; i < testsNumber; i ++)
    {
      stackPush (&s, i);
    }

    elem_t poped = 0;
    for (elem_t i = testsNumber - 1; i > startingNumber; i --)
    {
      if (poped = stackPop (&s) != i)
      {
        fprintf (fileStat, " | ERROR expected element %d, got %d\n" \
                         , i, poped);
        errors += 1;
      }
    }
  }

  stackDtor (&s);

  fprintf (fileStat, " # %d/%d ERRORS occur in Push/Pop testing\n\n", \
             errors, maxErrors);
  return (errors);
}

/*!
// \fn int stackTestChangeCapacity (int testsNumber)
//
//  Function witch tests changing stack capacity
//
//  \param testNumber Numbers of needed tests
//  \return Number of errors
*/

int stackTestChangeCapacity (int testsNumber)
{
  fprintf (fileStat, " # Starting Change capacity testing\n");

  int errors = 0;
  int maxErrors = testsNumber;

  stack_t s = {};
  stackCtor (&s);

  elem_t predictedCapacity = MINSTACKCAPACITY * 2;
  for (elem_t i = 0; i < testsNumber - 1; \
                  i ++, predictedCapacity *= increaseCapacityCoefficient)
  {
    stackChangeCapacity (&s, increaseCapacityCoefficient);
    if ((&s)->capacity != predictedCapacity)
    {
      fprintf (fileStat, " | ERROR expected %d capacity, got %d\n" \
                       , predictedCapacity, (&s)->capacity);
      errors += 1;
    }
  }

  for (elem_t i = 0; i < testsNumber * 2; i++)
  {
    stackChangeCapacity (&s, decreaseCapacityCoefficient);
  }

  if ((&s)->capacity != 1)
  {
    fprintf (fileStat, " | ERROR expected 1 capacity, got %d\n" \
                     , (&s)->capacity);
    errors += 1;
  }

  fprintf (fileStat, " # %d/%d ERRORS occur in Change capacity testing\n\n", \
             errors, maxErrors);
  return (errors);
}
