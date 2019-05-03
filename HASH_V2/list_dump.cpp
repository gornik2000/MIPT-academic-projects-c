//---------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
//---------------------------------------------------------------------------*/
const char *LOG_FILE_NAME = "data_logs/list_logs.txt";
FILE       *LOG_FILE      = fopen (LOG_FILE_NAME, "w");
const int LOG_MESSAGE_LEN = 150;
const int LOG_MAX_SIZE    = 250000000;
//---------------------------------------------------------------------------*/
char  listPrint    (list *l);
char *listPrintBuf (list *l, char *buf);

char *strcat_imp (char *buf, const char *s);

char myListIsOk (list *l, int line, const char *funcName, const char* fileName);
void errorDecoder (char error, int line,
                   const char *funcName, const char *fileName);
//---------------------------------------------------------------------------*/
#define listIsOk(l) myListIsOk ((l), __LINE__, __func__, __FILE__);
//---------------------------------------------------------------------------*/
/* decodes errror */
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
//---------------------------------------------------------------------------*/
/* checks list correction */
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
//---------------------------------------------------------------------------*/
/* print list data */
char listPrint (list *l)
{
  assert (l != NULL);

  fprintf (LOG_FILE, "\n # list %8p head %8p tail %8p count %3d\n",
                              l, l->head, l->tail, l->count);

  list_data *elem = l->head;
  for (int i = 0; i < l->count; i++)
  {
    fprintf (LOG_FILE, " # value "VALUE_OUT" prev %8p current %8p next %8p\n",
                   *(elem->value),     elem->prev, elem,    elem->next);
    elem = elem->next;
  }
}
//---------------------------------------------------------------------------*/
char *listPrintBuf (list *l, char *buf)
{
  assert (l   != NULL);
  assert (buf != NULL);

  char s[LOG_MESSAGE_LEN] = {0};
  sprintf (s, "\n # list %8p head %8p tail %8p count %3d\n",
                       l, l->head, l->tail, l->count);
  buf = strcat_imp (buf, s);

  list_data *elem = l->head;
  for (int i = 0; i < l->count; i++)
  {
    memset  (s, 0, LOG_MESSAGE_LEN);
    sprintf (s, " # value "VALUE_OUT" prev %8p current %8p next %8p\n",
            *(elem->value),     elem->prev, elem,    elem->next);
    buf = strcat_imp (buf, s);

    elem = elem->next;
  }

  return buf;
}
//---------------------------------------------------------------------------*/
char *strcat_imp (char *buf, const char *s)
{
  /*
  __asm__ ("strcat_circle_start:     \n" \
           "movl (%%ebx), %%ecx      \n" \
           "movl  %%ecx, (%%eax)     \n" \
           "inc %%eax                \n" \
           "inc %%ebx                \n" \
           "cmpb $0, (%%ebx)         \n" \
           "jnz  strcat_circle_start \n" \
          :"=a"(buf)                     \
          :"a" (buf), "b"(s)             \
          );
  //*/
  //*
  strcat (buf, s);
  buf += strlen (s);
  //*/
  return buf;
}
//---------------------------------------------------------------------------*/
//               © Gorbachev Nikita, November 2018 - April 2019              //
//---------------------------------------------------------------------------*/
