//---------------------------------------------------------------------------*/
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
//---------------------------------------------------------------------------*/
/** list data - stack information, includes info prev and next*/
/* which information will be in stack */
typedef char *list_value;
const list_value POISON_DESTRUCTED_VAL = 0;
/* how this information should be inputed */
#define VALUE_OUT "%s"
//---------------------------------------------------------------------------*/
struct myListData
{
  list_value       *value;
  struct myListData *prev;
  struct myListData *next;
};
typedef struct myListData list_data;
//---------------------------------------------------------------------------*/
list_data *listDataCtor (void);
list_data *listDataCtor (list_value value, myListData *prev, myListData *next);
list_data *listDataDtor (list_data *ld);
//---------------------------------------------------------------------------*/
/* data of the list constructor */
list_data *listDataCtor (void)
{
  list_data *ld = (list_data *)calloc (1, sizeof (*ld));
  assert (ld != NULL);

  ld->value = (list_value *)calloc (1, sizeof (*(ld->value)));
  ld->prev  = 0;
  ld->next  = 0;

  //uses_ldctor += 1;
  return ld;
}
//---------------------------------------------------------------------------*/
/* data of the list constructor */
list_data *listDataCtor (list_value value, myListData *prev, myListData *next)
{
  list_data *ld = (list_data *)calloc (1, sizeof (*ld));
  assert (ld != NULL);

  ld->value = (list_value *)calloc (1, sizeof (*(ld->value)));
  *(ld->value) = value;
  ld->prev     = prev;
  ld->next     = next;

  //uses_ldctor += 1;
  return ld;
}
//---------------------------------------------------------------------------*/
/* data of the list destructor */
list_data *listDataDtor (list_data *ld)
{
  assert (ld != NULL);

  *(ld->value) = POISON_DESTRUCTED_VAL;
  free (ld->value);
  (ld->value) = NULL;

  free (ld);
  ld = NULL;

  //uses_lddtor += 1;
  return ld;
}
//---------------------------------------------------------------------------*/
struct myList
{
  list_data *head;
  list_data *tail;

  int       count;
};
typedef struct myList list;
//---------------------------------------------------------------------------*/
list *listCtor (void);
list *listDtor (list *l);

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

char listVerification (list *l);
//---------------------------------------------------------------------------*/
#include "list_dump.cpp"
//---------------------------------------------------------------------------*/
/* list constructor */
list *listCtor (void)
{
  list *l = (list *)calloc (1, sizeof (*l));
  assert (l != NULL);

  l->head  = 0;
  l->tail  = 0;
  l->count = 0;

  //uses_lctor += 1;
  return l;
}
//---------------------------------------------------------------------------*/
/* list destructor */
list *listDtor (list *l)
{
  assert (l != NULL);

  listErase (l);

  l->head  = NULL;
  l->tail  = NULL;
  l->count = -1;

  free (l);
  l = NULL;

  //uses_ldtor += 1;
  return l;
}
//---------------------------------------------------------------------------*/
/* find element with given value */
list_data *listFindValue (list *l, list_value value)
{
  assert (l != NULL);
  if (l->count == 0 ) return 0;

  list_data *elem = l->head;
  for (int i = 0; i < l->count; i++)
  {
    if (*(elem->value) == value)
      return elem;
    elem = elem->next;
  }
}
//---------------------------------------------------------------------------*/
/* finds element with given num */
list_data *listFindNum   (list *l, int num)
{
  assert (l != NULL);
  if (l->count == 0 ) return 0;

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
//---------------------------------------------------------------------------*/
/* insert element before given num */
char listEmplaceFront (list *l, list_value value, int num)
{
  assert (l != NULL);

  if (l->count == 0)
  {
    return listPushFirst (l, value);
  }

  list_data *currentElem = listFindNum (l, num);

  list_data *elem = listDataCtor ();
  elem->next      = currentElem;
  *(elem->value)  = value;
  elem->prev      = currentElem->prev;

  if (elem->next == l->head)
  {
    l->head = elem;
  }

  (elem->prev)->next = elem;
  (elem->next)->prev = elem;

  (l->count)++;

  return ERR_NO_ERROR;
}
//---------------------------------------------------------------------------*/
/* insert element after given num */
char listEmplaceBack  (list *l, list_value value, int num)
{
  assert (l != NULL);

  if (l->count == 0)
  {
    return listPushFirst (l, value);
  }

  list_data *currentElem = listFindNum (l, num);

  list_data *elem = listDataCtor ();

  elem->next     = currentElem->next;
  *(elem->value) = value;
  elem->prev     = currentElem;

  if (elem->prev == l->tail)
  {
    l->tail = elem;
  }

  (elem->prev)->next = elem;
  (elem->next)->prev = elem;

  (l->count)++;

  return ERR_NO_ERROR;
}
//---------------------------------------------------------------------------*/
/* push first element to the list */
char listPushFirst (list *l, list_value value)
{
  list_data *elem = listDataCtor ();

  elem->next     = elem;
  *(elem->value) = value;
  elem->prev     = elem;

  (l->count)++;
  l->head = elem;
  l->tail = elem;

  return ERR_NO_ERROR;
}
//---------------------------------------------------------------------------*/
/* push element to front of the list */
char listPushFront (list *l, list_value value)
{
  assert (l != NULL);

  if (l->count == 0)
  {
    return listPushFirst (l, value);
  }

  list_data *elem = listDataCtor (value ,l->tail, l->head);

  (elem->next)->prev = elem;
  (elem->prev)->next = elem;

  ++(l->count);
  l->head = elem;

  //uses_lpush += 1;
  return ERR_NO_ERROR;
}
//---------------------------------------------------------------------------*/
/* push element to back of the list */
char listPushBack  (list *l, list_value value)
{
  assert (l != NULL);

  if (l->count == 0)
  {
    return listPushFirst (l, value);
  }

  list_data *elem = listDataCtor ();

  elem->next     = l->head;
  *(elem->value) = value;
  elem->prev     = l->tail;

  (elem->prev)->next = elem;
  (elem->next)->prev = elem;

  (l->count)++;
  l->tail = elem;

  return ERR_NO_ERROR;
}
//---------------------------------------------------------------------------*/
/* remove element from list */
char listRemove (list *l, int num)
{
  assert (l != NULL);

  if (l->count <= 0)
  {
    return ERR_NEG_COUNT;
  }

  if (l->count == 1)
  {
    return listErase (l);
  }

  list_data *elem = listFindNum (l, num);
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
  elem = listDataDtor (elem);

  l->count--;

  return ERR_NO_ERROR;
}
//---------------------------------------------------------------------------*/
/* erase list data */
char listErase  (list *l)
{
  assert (l != NULL);

  list_data *elem = l->head;

  for (int i = 1; i < l->count; i++)
  {
    elem = elem->next;
    elem->prev = listDataDtor (elem->prev);
  }

  l->count = 0;
  l->head  = 0;
  l->tail  = 0;

  return ERR_NO_ERROR;
}
//---------------------------------------------------------------------------*/
/* returns list head */
list_data *listFront (list *l)
{
  assert (l != NULL);

  return l->head;
}
//---------------------------------------------------------------------------*/
/* returns list tail */
list_data *listBack  (list *l)
{
  assert (l != NULL);

  return l->tail;
}
//---------------------------------------------------------------------------*/
/* returns next list element */
list_data *listNext     (list_data *ld)
{
  assert (ld != NULL);

  return ld->next;
}
//---------------------------------------------------------------------------*/
/* returns previous list element */
list_data *listPrevious (list_data *ld)
{
  assert (ld != NULL);

  return ld->prev;
}
//---------------------------------------------------------------------------*/
/* verification of the created list */
char listVerification (list *l)
{
  assert (l != NULL);
  listIsOk (l);

  list_data *elem = l->head;

  for (int i = 1; i < l->count; i++)
  {
    elem = elem->next;
  }
  if (elem != l->tail)              return ERR_PTR;

  for (int i = 1; i < l->count; i++)
  {
    elem = elem->prev;
  }
  if (elem != l->head)              return ERR_PTR;
  if ((l->tail)->next != (l->head)) return ERR_HEAD_TAIL;
  if ((l->tail) != (l->head)->prev) return ERR_HEAD_TAIL;

  printf (" # VERIFICATION finished correctly \n");

  return ERR_NO_ERROR;
}
//---------------------------------------------------------------------------*/
//               © Gorbachev Nikita, November 2018 - April 2019              //
//---------------------------------------------------------------------------*/

