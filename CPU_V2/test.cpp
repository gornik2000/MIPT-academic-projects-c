#include <stdio.h>

void sum(int a)
{
  printf(" %d ", a*a);
}

#define def_cmd(function) \
{\
  function(15);\
}

int main ()
{
  def_cmd(sum);
}
