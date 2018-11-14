#include <stdio.h>
#include "list.cpp"

//-----------------------------------------------------------------------------

const char *MAP_LOG_FILE_NAME =  "map_logs.txt";
FILE *MAP_LOG_FILE = fopen (MAP_LOG_FILE_NAME, "w");

//-----------------------------------------------------------------------------

const int MAX_MAP_SIZE = 256;

struct my_map
{
  list **data;
  //int *elemSize;
  int maxSize;
};
typedef struct my_map map;

map *mapCtor (void);
map *mapDtor (map *m);

char mapAdd    (map *m, list_value elem, int num);
char mapDelete (map *m, int num);
char mapErase  (map *m);

char mapPrint  (map *m);
char myMapIsOk (map *m, int line, const char *funcName, const char* fileName);

char mapToFile (map *m, FILE *file);

char mapVerification (map *m);

#define mapIsOk(l) myMapIsOk ((m), __LINE__, __func__, __FILE__);

//-----------------------------------------------------------------------------

map *mapCtor (void)
{
  map *m = (map *)calloc (1, sizeof (*m));

  //m->elemSize = (list_value *)calloc (MAX_MAP_SIZE, sizeof (*(m->elemSize)));
  //printf (" map size %p \n", m->elemSize);

  m->maxSize = MAX_MAP_SIZE;

  m->data = (list **)calloc (MAX_MAP_SIZE, sizeof (*(m->data)));
  printf (" map data %p \n", m->data);

  for (int i = 0; i < m->maxSize; i++)
  {
    (m->data[i]) = listCtor ();
  }

  return m;
}

map *mapDtor (map *m)
{
  assert (m != NULL);

  for (int i = 0; i < m->maxSize; i++)
  {
    listDtor ((m->data)[i]);
    //(m->elemSize)[i] = -1;
  }
  m->maxSize = -1;

  free (m->data);
  m->data = NULL;

  //free (m->elemSize);
  //m->elemSize = NULL;

  free (m);
  m = NULL;

  return m;
}

char mapAdd    (map *m, list_value elem, int num)
{
  assert (m != NULL);

  //printf (" SIZE %p", (m->elemSize));

  if ((m->data)[num]->count < 0)
  {
    return ERR_SIZE_PROBLEM;
  }

  listPushFront ((m->data)[num], elem);

  return ERR_NO_ERROR;
}

char mapDelete (map *m, int num)
{
  assert (m != NULL);

  if (m->maxSize <= num)
  {
    return ERR_SIZE_PROBLEM;
  }

  listErase ((m->data)[num]);

  return ERR_NO_ERROR;
}

char mapErase  (map *m)
{
  assert (m != NULL);

  for (int i = 0; i < m->maxSize; i++)
  {
    listErase ((m->data)[i]);
  }

  return ERR_NO_ERROR;
}

char mapPrint (map *m)
{
  assert (m != NULL);

  fprintf (LOG_FILE, "\n map: \n");
  for (int i = 0; i < m->maxSize; i++)
  {
    listPrint ((m->data)[i]);
  }

  return ERR_NO_ERROR;
}

char myMapIsOk (map *m, int line, const char *funcName, const char* fileName)
{
  assert (m != NULL);

  fprintf (MAP_LOG_FILE, " #-------------------------------------------------\n\n");
  if (m->maxSize < 0)
  {
    errorDecoder (ERR_INV_SIZE, line, funcName, fileName);
  }

  /* if (m->elemSize == NULL)
  {
    errorDecoder (ERR_NULL_PTR,  line, funcName, fileName);
  } */

  if (m->data == NULL)
  {
    errorDecoder (ERR_NULL_PTR,  line, funcName, fileName);
  }
  fprintf (MAP_LOG_FILE, " #--------------------------------------------------\n");

  return ERR_NO_ERROR;
}

char mapToFile (map *m, FILE *file)
{
  for (int i = 0; i < m->maxSize; i++)
  {
    fprintf (file, "%d\n", (m->data)[i]->count);
  }
  fprintf (file, "\n");
}

char mapVerification (map *m)
{
  assert (m != NULL);

  return ERR_NO_ERROR;
}
