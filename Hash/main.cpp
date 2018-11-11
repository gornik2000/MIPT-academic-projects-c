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

*/
//-----------------------------------------------------------------------------

#include <stdio.h>
#include "list.cpp"

int main ()
{
  list l = {0};
  listCtor (&l, 10);
  listPrint (&l);
  printf (" # Hash functions program\n");

  return 0;
}
