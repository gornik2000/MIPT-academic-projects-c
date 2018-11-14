#include "Sort.cpp"
#include <math.h>

//-----------------------------------------------------------------------------

/* 0 */
int hashFunction1 (char *str, int size);
/* str[0] */
int hashFunction2 (char *str, int size);
/* str Length */
int hashFunction3 (char *str, int size);
/* str sum */
int hashFunction4 (char *str, int size);
/* average char */
int hashFunction5 (char *str, int size);
/* ded function */
int hashFunction6 (char *str, int size);

//-----------------------------------------------------------------------------

int hashFunction1 (char *str, int size)
{
  assert (str != NULL);

  return 0;
}

int hashFunction2 (char *str, int size)
{
  assert (str != NULL);

  return str[0] % size;
}

int hashFunction3 (char *str, int size)
{
  assert (str != NULL);

  return strLength (str) % size;

}

int hashFunction4 (char *str, int size)
{
  assert (str != NULL);

  int sum = 0;

  for (int i = 0; str[i] != '\0'; i++)
  {
    sum = (sum + str[i]) % size;
  }

  return sum;
}

int hashFunction5 (char *str, int size)
{
  assert (str != NULL);

  int sum = 0;
  int i   = 0;
  while (str[i] != '\0')
  {
    sum = (sum + str[i]) % size;
    i ++;
  }

  return sum / i;
}

int hashFunction6 (char *str, int size)
{
  assert (str != NULL);

  int seed = 1580;
  int hash = 0;

  for (int i = 1; str[i] != '\0'; i++)
  {
     hash = ((hash * seed) + str[i] + i) % size;
  }

  return hash;
}
