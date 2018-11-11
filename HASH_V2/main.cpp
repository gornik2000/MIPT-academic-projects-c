/**
  ~~~to do list~~~
  struct list_t
    list data
    head
    tail
    free
    count
    ~sorted
    functions:
      ctor
      dtor
      add
      remove
      ~sort
  struct listData_t
    previous element
    next element
    information
    functions:
      ctor
      dtor
  truth checker
  dumper
    list is ok
  array of lists for hash tables, maybe structure
    6 different hash functions

  поставить пробел перед ++

*/
//-----------------------------------------------------------------------------

#define ERR_DEF(num, name, decription) ERR_##name = (num),
enum ERRORS
{
  #include "errors.h"
};
#undef ERR_DEF

#include <stdio.h>
#include "list.cpp"

int main ()
{
  printf ("start\n");
  list l = {0};
  listPrint (&l);
  listCtor (&l);
  listErase (&l);
  listEmplaceFront (&l, 10 ,10);
  listPushFront (&l, 4);
  listPrint (&l);
  listRemove (&l, 0);
  listRemove (&l, 0);
  listRemove (&l, 0);
  listPrint (&l);
  listEmplaceFront (&l, 20 ,10);
  listPrint (&l);
  printf ("1\n");

  listPushFront(&l, 1);
  listPushBack (&l, 2);
  listPushFront(&l, 3);
  listPrint (&l);

  printf ("2\n");
  listErase (&l);
  listPrint (&l);

  printf ("3\n");
  listPushFront (&l, 4);
  listPushFront (&l, 5);
  listPrint (&l);
  listEmplaceFront (&l, 15, 0);
  listEmplaceFront (&l, 20, 0);
  listPrint (&l);
  printf ("4\n");

  listRemove (&l, 0);
  listRemove (&l, 0);
  listRemove (&l, 0);
  listPrint (&l);
  printf ("5\n");

  listErase (&l);
  listPrint (&l);
  printf ("6\n");
  //*/
  listDtor  (&l);
  listPrint (&l);
  (&l)->count = -1;
  (&l)->head = 0;
  (&l)->tail = 0;
  listIsOk (&l);

  printf (" # Hash functions program\n");

  return 0;
  //*/
}
