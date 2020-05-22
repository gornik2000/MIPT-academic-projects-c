//---------------------------------------------------------------------------*/
#include <stdio.h>
#include "list.cpp"
//---------------------------------------------------------------------------*/
const char *MAP_LOG_FILE_NAME =  "data_logs/map_logs.txt";
FILE       *MAP_LOG_FILE      = fopen (MAP_LOG_FILE_NAME, "w");
//---------------------------------------------------------------------------*/
const int MAX_MAP_SIZE = 500;

struct my_map
{
  list **data;
  int maxSize;
  int (*hash)(char *, int);
};
typedef struct my_map map;
//---------------------------------------------------------------------------*/

map *mapCtor (int (*hashFunction)(char *, int));
map *mapDtor (map *m);

char mapAdd    (map *m, list_value elem);
char mapDelete (map *m, int num);
char mapErase  (map *m);

char mapPrint  (map *m);
char myMapIsOk (map *m, int line, const char *funcName, const char* fileName);

char mapToFile (map *m, FILE *file);
//---------------------------------------------------------------------------*/
#define mapIsOk(l) myMapIsOk ((m), __LINE__, __func__, __FILE__);
//---------------------------------------------------------------------------*/
/* constructor */
map *mapCtor (int (*hashFunction)(char *, int))
{
  map *m = (map *)calloc (1, sizeof (*m));
  m->maxSize = MAX_MAP_SIZE;

  m->data = (list **)calloc (MAX_MAP_SIZE, sizeof (*(m->data)));

  m->hash = hashFunction;

  for (int i = 0; i < m->maxSize; i++) (m->data[i]) = listCtor ();


  return m;
}
//---------------------------------------------------------------------------*/
/* destructor */
map *mapDtor (map *m)
{
  assert (m != NULL);

  for (int i = 0; i < m->maxSize; i++) listDtor ((m->data)[i]);

  m->maxSize = -1;
  m->hash = NULL;

  free (m->data);
  m->data = NULL;

  free (m);
  m = NULL;

  return m;
}
//---------------------------------------------------------------------------*/
/* add element to map's list */
char mapAdd    (map *m, list_value elem)
{
  assert (m != NULL);

  int num = m->hash (elem, m->maxSize);

  if ((m->data)[num]->count < 0) return ERR_SIZE_PROBLEM;

  if (((m->data)[num]->count == 0) || !(listIsStrInList ((m->data)[num], elem)))
  {
    listPushFront ((m->data)[num], elem);
  }

  return ERR_NO_ERROR;
}
//---------------------------------------------------------------------------*/
/* delete element (list) from stack */
char mapDelete (map *m, int num)
{
  assert (m != NULL);

  if (m->maxSize <= num) return ERR_SIZE_PROBLEM;

  listErase ((m->data)[num]);

  return ERR_NO_ERROR;
}
//---------------------------------------------------------------------------*/
/* erase map data */
char mapErase  (map *m)
{
  assert (m != NULL);

  for (int i = 0; i < m->maxSize; i++) listErase ((m->data)[i]);

  return ERR_NO_ERROR;
}
//---------------------------------------------------------------------------*/
/* print map data to file */
char mapPrint (map *m)
{
  assert (m != NULL);
  char *buf = (char *) calloc (1, LOG_MAX_SIZE);
  char *buf_start = buf;

  fprintf (LOG_FILE, "\n map: max size %8d\n", m->maxSize);
  for (int i = 0; i < m->maxSize; i++)
  {
    buf  = listPrintBuf ((m->data)[i], buf);
  }

  fwrite (buf_start, buf - buf_start, 1, LOG_FILE);
  return ERR_NO_ERROR;
}
//---------------------------------------------------------------------------*/
/* checks if map is correct */
char myMapIsOk (map *m, int line, const char *funcName, const char* fileName)
{
  assert (m != NULL);

  fprintf (MAP_LOG_FILE, " #-------------------------------------------------\n\n");
  if (m->maxSize < 0)
  {
    errorDecoder (ERR_INV_SIZE, line, funcName, fileName);
  }

  if (m->data == NULL)
  {
    errorDecoder (ERR_NULL_PTR,  line, funcName, fileName);
  }
  fprintf (MAP_LOG_FILE, " #--------------------------------------------------\n");

  return ERR_NO_ERROR;
}
//---------------------------------------------------------------------------*/
/* create file for graphics from map */
char mapToFile (map *m, FILE *file)
{
  for (int i = 0; i < m->maxSize; i++)
  {
    fprintf (file, "%d\t%d\n", i, (m->data)[i]->count);
  }
}
//---------------------------------------------------------------------------*/
//               © Gorbachev Nikita, November 2018 - April 2019              //
//---------------------------------------------------------------------------*/
