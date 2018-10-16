/*!
//  \file Dump.cpp
//
//  Contains functions for checking stack for correction
*/

/// Error variables names
const char *ERR_ST_CAPACITY = "Invalid stack capacity";
const char *ERR_ST_NULL     = "Missing stack pointer";
const char *ERR_ST_SIZE     = "Invalid stack size";
const char *ERR_ST_DATA     = "Missing stack data";
const char *ERR_ST_MEMORY   = "Size is bigger then capacity";

const char *stIsOk (stack_t *s, int line = 0, \
                    const char *funcName = "not given", \
                    const char *fileName = "not given");

/// makes a stackIsOk function as stIsOk in witch automatically given \
    system information
#define stackIsOk(stack) stIsOk(stack, __LINE__, __func__, __FILE__);


void errorDecoder (FILE *file, const char *error, int line = 0, \
                   const char *funcName = "not given", \
                   const char *fileName = "not given");

//------------------------------------------------------------------------------

/*!
//  \fn const char *stIsOk (stack_t *s, int line, const char *funcName, const char *fileName)
//
//  Checks stack for correction. Returns pointer to name of error if it was
//
//  \param s Pointer to stack that should be checked
//  \param line Line from witch function was get
//  \param funcName Name of function from witch stIsOk was get
//  \param fileName Name of file from witch stIsOk was get
//
//  \return Pointer to the name of error if smth was wrong, else 0
*/

const char *stIsOk (stack_t *s, int line, \
                    const char *funcName, \
                    const char *fileName)
{
  /* stack pointer is NULL */
  if (s == NULL)
  {
    errorDecoder (s->logName, ERR_ST_NULL, line, funcName, fileName);
    return ERR_ST_NULL;
  }

  /* stack capacity < 0 or too big */
  if (((s->capacity) < 0) || ((s->capacity) > 1000000000))
  {
    errorDecoder (s->logName, ERR_ST_CAPACITY, line, funcName, fileName);
    return ERR_ST_CAPACITY;
  }

  /* stack size < 0 or too big */
  if ((s->size < 0) || ((s->size) > 1000000000))
  {
    errorDecoder (s->logName, ERR_ST_SIZE, line, funcName, fileName);
    return ERR_ST_SIZE;
  }

  /* pointer to data is NULL, data does not exist */
  if (s->data == NULL)
  {
    errorDecoder (s->logName, ERR_ST_DATA, line, funcName, fileName);
    return ERR_ST_DATA;
  }

  /* stack capability is smaller than stack size */
  if (s->capacity  < s->size)
  {
    errorDecoder (s->logName, ERR_ST_MEMORY, line, funcName, fileName);
    return ERR_ST_MEMORY;
  }

  /* if 0 errors */
  return 0;
}

/*!
//  \fn void errorDecoder (FILE *file, const char *error, int line, const char *funcName, const char *fileName)
//
//  Checks stack for correction. Returns pointer to name of error if it was
//
//  \param file Pointer to file where error information should be printed
//  \param error String name of the error that should be printed
//  \param line Line from witch error was get was get
//  \param funcName Name of function from witch error was get
//  \param fileName Name of file from witch error was get
*/

void errorDecoder (FILE *file, const char *error, int line, \
                   const char *funcName, \
                   const char *fileName)
{
  /* means that stack was not created */
  assert (file);

  fprintf (file, \
           " # Error '%s' occured in: \n" \
           " | file %s\n" \
           " | function %s\n" \
           " | line %d\n" \
           " | date %s %s\n\n" \
           , error, fileName, funcName, line, __TIME__, __DATE__);
}
