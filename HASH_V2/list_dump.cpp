#include <stdio.h>

//-----------------------------------------------------------------------------

const char *LOG_FILE_NAME =  "list_logs.txt";
FILE *LOG_FILE = fopen (LOG_FILE_NAME, "w");

//-----------------------------------------------------------------------------

char listVerification (list *l);
char myListIsOk (list *l, int line, const char *funcName, const char* fileName);
void errorDecoder (char error, int line,
                   const char *funcName, const char *fileName);

#define listIsOk(l) myListIsOk ((l), __LINE__, __func__, __FILE__);

//-----------------------------------------------------------------------------

void errorDecoder (char error, int line,
                   const char *funcName, const char *fileName)
{
  const char *errorDescr = "";
  #define ERR_DEF(num, name, description)                    \
  {                                                          \
    if (num == error) errorDescr = description;              \
  }
  #include "errors.h"
  #undef ERR_DEF

  fprintf (LOG_FILE,                                                    \
          " # Error %s occured in: \n"                                  \
          " | file %s\n"                                                \
          " | function %s\n"                                            \
          " | line %d\n"                                                \
          " | date %s %s\n\n"                                           \
          , errorDescr, fileName, funcName, line, __TIME__, __DATE__);
}

char myListIsOk (list *l, int line, const char *funcName, const char* fileName)
{
  assert (l != NULL);
  listPrint (l);

  fprintf (LOG_FILE, " #-------------------------------------------------\n\n");
  if (l->count < 0)
  {
    errorDecoder (ERR_NEG_COUNT, line, funcName, fileName);
  }

  if (l->head == NULL)
  {
    errorDecoder (ERR_INV_HEAD,  line, funcName, fileName);
  }

  if (l->tail == NULL)
  {
    errorDecoder (ERR_INV_TAIL,  line, funcName, fileName);
  }
  fprintf (LOG_FILE, " #--------------------------------------------------\n");

  return ERR_NO_ERROR;
}

char listVerification (list *l)
{
  assert (l != NULL);

  return ERR_NO_ERROR;
}
