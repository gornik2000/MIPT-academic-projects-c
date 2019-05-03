#include <stdio.h>
#include <stdlib.h>

const char *file_name       = "data.txt";
const int   size            = 200000000;
const char  line_length_max = 20;

int main ()
{
  //printf ("%d %d\n", 'A', 'z');
  FILE *file = fopen (file_name, "w");

  char *buf = (char *)malloc (size);
  if   (buf == 0) return 1;
  char *buf_start = buf;

  char line_length = 0;
  int  i           = 0;
  while (i < size - line_length_max)
  {
    line_length = rand () % line_length_max + 1;
    for (int j = 0; j < line_length; ++j)
    {
      *buf = rand () % (122 - 65) + 65;
      ++buf;
    }
    i += line_length + 1;
    *buf = '\n';
    ++buf;
  }
  fwrite (buf_start, 1, size, file);
  free   (buf_start);
  fclose (file);

  printf (" # END\n");
  return 0;
}
