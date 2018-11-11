#include <stdio.h>

const char *LOG_FILE_NAME =  "list_logs.txt";
FILE *LOG_FILE = fopen (LOG_FILE_NAME, "w");

void listPrint (list *l)
{
  fprintf (LOG_FILE, "\n # size %3d head %3d tail %3d free %3d count %1d sorted %d\n",
                        l->size, l->head, l->tail, l->free, l->count, l->sorted);

  for (int i = 0; i < l->size; i++)
  {
    fprintf (LOG_FILE, " # info %10d", ((l->data)[i]).info);
    fprintf (LOG_FILE, " prev %3d", ((l->data)[i]).prev);
    fprintf (LOG_FILE, " next %3d\n", ((l->data)[i]).next);
  }
}
