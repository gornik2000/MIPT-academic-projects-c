/* headers start */
#define CPU_DEF_CMD(name, num, par, mode, func) \
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
  for (elem_t i = 0; i < (&(cpu->mem))->size; i++)
  {
    printf (ELEM_CPU_PRINT" _ ", ((&(cpu->mem))->data)[i]);
  }

  *ipCmd += 1;
  printf (";\n\n");
}

void cpu_push (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  *ipCmd += 2;
}

void cpu_pop  (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  *ipCmd += 2;
}

void cpu_add  (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  *ipCmd += 1;
}

void cpu_sub  (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  *ipCmd += 1;
}

void cpu_mul  (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  *ipCmd += 1;
}

void cpu_div  (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  *ipCmd += 1;
}


void cpu_jmp  (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  *ipCmd += 1;
}

void cpu_ja   (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  *ipCmd += 1;
}

void cpu_jb   (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  *ipCmd += 1;
}

void cpu_jae  (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  *ipCmd += 1;
}


void cpu_sin  (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  printf (" %d ", *ipCmd);
  *ipCmd += 1;
  printf (" %d ", *ipCmd);
  printf (" 0 ");
}

void cpu_cos  (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  *ipCmd += 1;
}

void cpu_tan  (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  *ipCmd += 1;
}

void cpu_ln   (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  *ipCmd += 1;
}

void cpu_exp  (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  *ipCmd += 1;
}

void cpu_sqrt (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  *ipCmd += 1;
}

void cpu_sqr  (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  *ipCmd += 1;
}

void cpu_pwr  (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  *ipCmd += 1;
}


void cpu_call (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  *ipCmd += 1;
}

void cpu_ret  (cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf)
{
  *ipCmd += 1;
}

