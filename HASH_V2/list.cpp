#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------
/** list data - stack information, includes info prev and next*/

const int POISON_DESTRUCTED = 0;

/* which information will be in stack */
typedef int list_value;

struct myListData
{
  list_value        value;
  struct myListData *prev;
  struct myListData *next;
};

typedef struct myListData list_data;

char listDataCtor (list_data *ld);
char listDataDtor (list_data *ld);

//-----------------------------------------------------------------------------

char listDataCtor (list_data *ld)
{
  assert (ld != NULL);

  ld->value = (list_value )calloc (1,sizeof (*(ld->next)));
  if ( ld->value == NULL) return ERR_MEM_PROBLEM;

  ld->prev  = (list_data *)calloc (1,sizeof (*(ld->next)));
  if ( ld->prev == NULL) return ERR_MEM_PROBLEM;

  ld->next  = (list_data *)calloc (1,sizeof (*(ld->next)));
  if ( ld->next == NULL) return ERR_MEM_PROBLEM;

  return ERR_NO_ERROR;
}

char listDataDtor (list_data *ld)
{
  assert (ld != NULL);
  /*
  free (&(ld->value));
  ld->value = POISON_DESTRUCTED;

  free (ld);
  ld = POISON_DESTRUCTED;
  */
  ld->value = POISON_DESTRUCTED;
  free (&(ld->value));

  free (ld);
  ld = POISON_DESTRUCTED;

  return ERR_NO_ERROR;
}

//-----------------------------------------------------------------------------

struct myList
{
  list_data *head;
  list_data *tail;

  int       count;
};

typedef struct myList list;

char listCtor (list *l);
char listDtor (list *l);

list_data *listFront (list *l);
list_data *listBack  (list *l);

list_data *listNext     (list_data *ld);
list_data *listPrevious (list_data *ld);

list_data *listFindValue (list *l, list_value value);
list_data *listFindNum   (list *l, int num);

char listEmplaceFront (list *l, list_value value, int num);
char listEmplaceBack  (list *l, list_value value, int num);

char listPushFirst (list *l, list_value value);
char listPushFront (list *l, list_value value);
char listPushBack  (list *l, list_value value);

char listRemove (list *l, int num);
char listErase  (list *l);

#include "list_dump.cpp"
//-----------------------------------------------------------------------------

char listCtor (list *l)
{
  assert (l != NULL);

  l->head  = 0;
  l->tail  = 0;
  l->count = 0;

  return ERR_NO_ERROR;
}

char listDtor (list *l)
{
  assert (l != NULL);

  listErase (l);

  free (l->head);
  l->head  = POISON_DESTRUCTED;
  free (l->tail);
  l->tail  = POISON_DESTRUCTED;
  free (&(l->count));
  l->count = POISON_DESTRUCTED;

  return ERR_NO_ERROR;
}

list_data *listFront (list *l)
{
  assert (l != NULL);

  return (l->head);
}

list_data *listBack  (list *l)
{
  assert (l != NULL);

  return (l->tail);
}

list_data *listNext     (list_data *ld)
{
  assert (ld != NULL);

  return ld->next;
}

list_data *listPrevious (list_data *ld)
{
  assert (ld != NULL);

  return ld->prev;
}

list_data *listFindValue (list *l, list_value value)
{
  assert (l != NULL);

  list_data *elem = l->head;
  for (int i = 0; i < l->count; i++)
  {
    if (elem->value == value)
      return elem;
    elem = elem->next;
  }
}

list_data *listFindNum   (list *l, int num)
{
  assert (l != NULL);

  list_data *elem = l->head;
  if (num > 0)
  {
    for (int i = 0; i < num; i++)
      elem = elem->next;
    return elem;
  }
  if (num < 0)
  {
    for (int i = num; i < 0; i++)
      elem = elem->prev;
    return elem;
  }
  return elem;
}

char listEmplaceFront (list *l, list_value value, int num)
{
  assert (l != NULL);

  if (l->count == 0)
  {
    return listPushFirst (l, value);
  }

  list_data *currentElem = listFindNum (l, num);

  list_data *elem = (list_data *)calloc (1, sizeof (*elem));
  listDataCtor (elem);

  elem->next  = currentElem;
  elem->value = value;
  elem->prev  = currentElem->prev;

  if (elem->next == l->head)
  {
    l->head = elem;
  }

  (elem->prev)->next = elem;
  (elem->next)->prev = elem;

  (l->count) ++;

  return ERR_NO_ERROR;
}

char listEmplaceBack  (list *l, list_value value, int num)
{
  assert (l != NULL);

  if (l->count == 0)
  {
    return listPushFirst (l, value);
  }

  list_data *currentElem = listFindNum (l, num);

  list_data *elem = (list_data *)calloc (1, sizeof (*elem));
  listDataCtor (elem);

  elem->next  = currentElem->next;
  elem->value = value;
  elem->prev  = currentElem;

  if (elem->prev == l->tail)
  {
    l->tail = elem;
  }

  (elem->prev)->next = elem;
  (elem->next)->prev = elem;

  (l->count) ++;

  return ERR_NO_ERROR;
}

char listPushFirst (list *l, list_value value)
{
  list_data *elem = (list_data *)calloc (1, sizeof (*elem));

  listDataCtor (elem);

  elem->next  = elem;
  elem->value = value;
  elem->prev  = elem;

  (l->count) ++;
  l->head = elem;
  l->tail = elem;

  return ERR_NO_ERROR;
}

char listPushFront (list *l, list_value value)
{
  assert (l != NULL);

  if (l->count == 0)
  {
    return listPushFirst (l, value);
  }

  list_data *elem = (list_data *)calloc (1, sizeof (*elem));
  listDataCtor (elem);

  elem->next  = l->head;
  elem->value = value;
  elem->prev  = l->tail;

  (elem->next)->prev = elem;
  (elem->prev)->next = elem;

  (l->count) ++;
  l->head = elem;

  return ERR_NO_ERROR;
}

char listPushBack  (list *l, list_value value)
{
  assert (l != NULL);

  if (l->count == 0)
  {
    return listPushFirst (l, value);
  }

  list_data *elem = (list_data *)calloc (1, sizeof (*elem));
  listDataCtor (elem);

  elem->next  = l->head;
  elem->value = value;
  elem->prev  = l->tail;

  (elem->prev)->next = elem;
  (elem->next)->prev = elem;

  (l->count) ++;
  l->tail = elem;

  return ERR_NO_ERROR;
}

char listRemove (list *l, int num)
{
  assert (l != NULL);

  list_data *elem = listFindNum (l, num);

  if (l->count == 0)
  {
    return ERR_NEG_COUNT;
  }

  if (l->count == 1)
  {
    return listErase (l);
  }

  (elem->next)->prev = (elem->prev);
  (elem->prev)->next = (elem->next);

  if (elem == l->head)
  {
    l->head = elem->next;
  }

  if (elem == l->tail)
  {
    l->tail = elem->prev;
  }
  listDataDtor (elem);
  /*
  elem->value = POISON_DESTRUCTED;
  free (&(elem->value));
  free (elem);
  elem = POISON_DESTRUCTED;
  */
  l->count --;

  return ERR_NO_ERROR;
}

char listErase  (list *l)
{
  assert (l != NULL);

  list_data *elem = l->head;

  for (int i = 0; i < l->count; i ++)
  {
    elem = elem->next;
    listDataDtor (elem->prev);
    /*
    elem->value = POISON_DESTRUCTED;
    free (&(elem->value));

    elem = elem->next;
    free (elem->prev); */
  }

  l->count = 0;
  l->head  = 0;
  l->tail  = 0;

  return ERR_NO_ERROR;
}

//----------------------------------------------------------------------------
