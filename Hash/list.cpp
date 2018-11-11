#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

///----------------------------------------------------------------------------
/** list data - stack information, includes info prev and next*/

/* which information will be in stack */
typedef int list_info;

struct myListData
{
  list_info info;
  size_t    prev;
  size_t    next;
};

typedef struct myListData list_data;

char listDataCtor (list_data *ld);
char listDataDtor (list_data *ld);

//-----------------------------------------------------------------------------

struct myList
{
  list_data *data;

  size_t     head;
  size_t     tail;

  size_t     size;
  size_t    count;

  size_t     free;
  char     sorted;
};

typedef struct myList list;


char listCtor (list *l);
char listDtor (list *l);

//-----------------------------------------------------------------------------

#include "list_dump.cpp"

char listDataCtor (list_data *ld, size_t size)
{
  assert (ld != 0);

  ld->prev = -1;
  ld->next = -1;
  /** change for different types */
  ld->info =  0;
}

char listDataDtor (list_data *ld)
{
  assert (ld != 0);

  ld->next = -1;
  ld->prev = -1;
  /** change for different types */
  ld->info =  0;
}

//----------------------------------------------------------------------------


char listCtor (list *l, size_t size)
{
  assert (l != NULL);


  l->size   = size;
  l->data   = (list_data *)calloc (l->size, sizeof (*(l->data)));

  l->free   = 0;
  for (int i = 0; i < size; i++)
  {
    ((l->data)[i]).info = 0;
    ((l->data)[i]).prev = i - 1;
    ((l->data)[i]).next = i + 1;
  }
  ((l->data)[0]).prev        = -1;
  ((l->data)[size - 1]).next = -1;

  l->head   = -1;
  l->tail   = -1;

  l->count  = 0;
  l->sorted = 1;
}

char listDtor (list *l)
{
  ;
}
