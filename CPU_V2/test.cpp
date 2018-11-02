#include <stdio.h>

#define CPU_DEF_CMD(name, num, par, func) \
void cpu_##name(int *i) func;
CPU_DEF_CMD ( add,  1, 0, {(*i) ++;} )
#undef CPU_DEF_CMD

int main()
{
  int i = 0;
  cpu_add (&i);
  cpu_add (&i);
  printf("%d",i);
}
