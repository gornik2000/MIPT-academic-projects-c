#include <stdio.h>

const char *LOG_FILE_NAME =  "list_logs.txt";
FILE *LOG_FILE = fopen (LOG_FILE_NAME, "w");

char listVerification (list *l);
char listPrint  (list *l);
char myListIsOk (list *l, int line, const char *funcName, const char* fileName);
void errorDecoder (char error, int line,
                   const char *funcName, const char *fileName);

#define listIsOk(l) myListIsOk ((l), __LINE__, __func__, __FILE__);

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
  //assert (0);
}

char myListIsOk (list *l, int line, const char *funcName, const char* fileName)
{
  listPrint (l);

  if (l->count < 0)
  {
    errorDecoder (ERR_NEG_COUNT, line, funcName, fileName);
  }

  if (l->head == 0)
  {
    errorDecoder (ERR_INV_HEAD,  line, funcName, fileName);
  }

  if (l->tail == 0)
  {
    errorDecoder (ERR_INV_TAIL,  line, funcName, fileName);
  }
}

char listPrint (list *l)
{
  assert (l != NULL);

  fprintf (LOG_FILE, "\n # list %8p head %8p tail %8p count %3d\n",
                              l, l->head, l->tail, l->count);

  list_data *elem = l->head;
  for (int i = 0; i < l->count; i ++)
  {
    fprintf (LOG_FILE, " # value %8d prev %8p current %8p next %8p\n",
                     elem->value, elem->prev, elem, elem->next);
    elem = elem->next;
  }
}
