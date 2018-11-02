/* headers start */
#define CPU_DEF_CMD(name, num, par, func) \
void func(cpu_t *c, int *ipPar, int *ipCmd, char *cmdBuf, data_cpu *parBuf);

#include "commands.h"

#undef CPU_DEF_CMD
/* headers end   */

void cpu_end  (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  *ipCmd += 1000000000;
}

void cpu_in   (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  *ipCmd += 1;
}

void cpu_out  (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  printf (" # ");

  for (elem_t i = 0; i < (&(cpu->mem))->size; i++)
  {
    printf (ELEM_CPU_PRINT" _ ", ((&(cpu->mem))->data)[i]);
  }

  *ipCmd += 1;
  printf (";\n");
}

void cpu_push (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  switch (cmdBuf[*ipCmd + 1])
  {
    case (type_val):
      stackPush (&(cpu->mem), parBuf[*ipPar]);
      break;

    case (type_reg):
      stackPush (&(cpu->mem), (cpu->reg)[parBuf[*ipPar]]);
      break;

    case (type_ram):
      stackPush (&(cpu->mem), (cpu->ram)[parBuf[*ipPar]]);
      break;

    case (type_lbl):
      /* work for sure */
      stackPush(&(cpu->mem), parBuf[*ipPar]);
      break;
  }

  *ipPar += 1;
  *ipCmd += 2;
}

void cpu_pop  (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  switch (cmdBuf[*ipCmd + 1])
  {
    case (type_val):
      stackPop (&(cpu->mem));
      break;

    case (type_reg):
      (cpu->reg)[parBuf[*ipPar]] = stackPop (&(cpu->mem));
      break;

    case (type_ram):
      (cpu->ram)[parBuf[*ipPar]] = stackPop (&(cpu->mem));
      break;

    case (type_lbl):
      stackPop (&(cpu->mem));
      break;
  }

  *ipPar += 1;
  *ipCmd += 2;
}

void cpu_add  (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  data_cpu a = stackPop (&(cpu->mem));
  data_cpu b = stackPop (&(cpu->mem));

  stackPush (&(cpu->mem), b + a);
  *ipCmd += 1;
}

void cpu_sub  (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  data_cpu a = stackPop (&(cpu->mem));
  data_cpu b = stackPop (&(cpu->mem));

  stackPush (&(cpu->mem), b * a);
  *ipCmd += 1;
}

void cpu_mul  (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  data_cpu a = stackPop (&(cpu->mem));
  data_cpu b = stackPop (&(cpu->mem));

  stackPush (&(cpu->mem), b - a);
  *ipCmd += 1;
}

void cpu_div  (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  data_cpu a = stackPop (&(cpu->mem));
  data_cpu b = stackPop (&(cpu->mem));

  stackPush (&(cpu->mem), b / a);
  *ipCmd += 1;
}


void cpu_jmp  (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  *ipCmd  = parBuf[*ipPar];
  *ipPar += 1;
}

void cpu_ja   (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  data_cpu a = stackPop (&(cpu->mem));
  data_cpu b = stackPop (&(cpu->mem));

  if (a > b) *ipCmd  = parBuf[*ipPar];
  else       *ipCmd += 1;

  *ipPar += 1;
}

void cpu_jb   (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  data_cpu a = stackPop (&(cpu->mem));
  data_cpu b = stackPop (&(cpu->mem));

  if (a < b) *ipCmd  = parBuf[*ipPar];
  else       *ipCmd += 1;

  *ipPar += 1;
}

void cpu_je   (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  data_cpu a = stackPop (&(cpu->mem));
  data_cpu b = stackPop (&(cpu->mem));

  if (a == b) *ipCmd  = parBuf[*ipPar];
  else        *ipCmd += 1;

  *ipPar += 1;
}

void cpu_jae  (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  data_cpu a = stackPop (&(cpu->mem));
  data_cpu b = stackPop (&(cpu->mem));

  if (a >= b) *ipCmd  = parBuf[*ipPar];
  else        *ipCmd += 1;

  *ipPar += 1;
}

void cpu_jbe  (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  data_cpu a = stackPop (&(cpu->mem));
  data_cpu b = stackPop (&(cpu->mem));

  if (a <= b) *ipCmd  = parBuf[*ipPar];
  else        *ipCmd += 1;

  *ipPar += 1;
}

void cpu_jne  (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  data_cpu a = stackPop (&(cpu->mem));
  data_cpu b = stackPop (&(cpu->mem));

  if (a != b) *ipCmd  = parBuf[*ipPar];
  else        *ipCmd += 1;

  *ipPar += 1;
}

void cpu_sin  (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  data_cpu a = stackPop (&(cpu->mem));

  stackPush (&(cpu->mem), sin (a));
  *ipCmd += 1;
}

void cpu_cos  (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  data_cpu a = stackPop (&(cpu->mem));

  stackPush (&(cpu->mem), cos (a));
  *ipCmd += 1;
}

void cpu_tan  (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  data_cpu a = stackPop (&(cpu->mem));

  stackPush (&(cpu->mem), tan (a));
  *ipCmd += 1;
}

void cpu_ln   (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  data_cpu a = stackPop (&(cpu->mem));

  stackPush (&(cpu->mem), log (a));
  *ipCmd += 1;
}

void cpu_exp  (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  data_cpu a = stackPop (&(cpu->mem));

  stackPush (&(cpu->mem), exp (a));
  *ipCmd += 1;
}

void cpu_sqrt (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  data_cpu a = stackPop (&(cpu->mem));

  stackPush (&(cpu->mem), sqrt (a));
  *ipCmd += 1;
}

void cpu_sqr  (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  data_cpu a = stackPop (&(cpu->mem));

  stackPush (&(cpu->mem), a * a);
  *ipCmd += 1;
}

void cpu_asin (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  data_cpu a = stackPop (&(cpu->mem));

  stackPush (&(cpu->mem), asin (a));
  *ipCmd += 1;
}

void cpu_acos (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  data_cpu a = stackPop (&(cpu->mem));

  stackPush (&(cpu->mem), acos (a));
  *ipCmd += 1;
}

void cpu_atan (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  data_cpu a = stackPop (&(cpu->mem));

  stackPush (&(cpu->mem), atan (a));
  *ipCmd += 1;
}

void cpu_call (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  /* put in function stack next command pointer */
  stackPush (&(cpu->fun), (*ipCmd + 1));

  /* jump to label */
  *ipCmd  = parBuf[*ipPar];
  *ipPar += 1;
}

void cpu_ret  (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  /* returns last position */
  *ipCmd  = stackPop (&(cpu->fun));
}


