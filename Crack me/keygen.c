#include <stdio.h>
#include <string.h>

const int MAX_LENGTH = 10;

int GetCode(char *name, int length)
{
  int code = 0;
  for (int i = 0; i < length; ++i)
  {
    if (name[i] <  'A') return -1;
    if (name[i] >= 'Z') code -= 32;
    code += name[i];
  }
  return code ^ 22136 ^ 4660;
}

int main()
{
  char name[MAX_LENGTH];
  printf("Enter NAME\n");
  scanf ("%s", name);

  int length = strlen(name);
  if (length > MAX_LENGTH)
  {
    printf("Incorrect name length, try length less then 10\n");
    return 0;
  }

  int code = GetCode(name, length);
  if (code == -1)
  {
    printf("Incorrect name, try another letters\n");
    return 0;
  }

  printf("Your code is %d\n",   code);
  return 0;
}
