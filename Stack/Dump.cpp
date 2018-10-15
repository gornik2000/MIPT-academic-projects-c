/*!
//  \file Dump.cpp
//
//  Contains functions for checking stack for correction
*/

/// Error variables names
const char *ERRSTCAPACITY = "Invalid stack capacity";
const char *ERRSTNULL     = "Missing stack pointer";
const char *ERRSTSIZE     = "Invalid stack size";
const char *ERRSTDATA     = "Missing stack data";
const char *ERRSTMEMORY   = "Size is bigger then capacity";

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
    errorDecoder (s->logName, ERRSTNULL, line, funcName, fileName);
    return ERRSTNULL;
  }

  /* stack capacity < 0 */
  if (s->capacity < 0)
  {
    errorDecoder (s->logName, ERRSTCAPACITY, line, funcName, fileName);
    return ERRSTCAPACITY;
  }

  /* stack size < 0 */
  if (s->size < 0)
  {
    errorDecoder (s->logName, ERRSTSIZE, line, funcName, fileName);
    return ERRSTSIZE;
  }

  /* pointer to data is NULL, data does not exist */
  if (s->data == NULL)
  {
    errorDecoder (s->logName, ERRSTDATA, line, funcName, fileName);
    return ERRSTDATA;
  }

  /* stack capability is smaller than stack size */
  if (s->capacity  < s->size)
  {
    errorDecoder (s->logName, ERRSTMEMORY, line, funcName, fileName);
    return ERRSTMEMORY;
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
  fprintf (file, \
           " # Error '%s' occured in: \n" \
           " | file %s\n" \
           " | function %s\n" \
           " | line %d\n" \
           " | date %s %s\n\n" \
           , error, fileName, funcName, line, __TIME__, __DATE__);
}
