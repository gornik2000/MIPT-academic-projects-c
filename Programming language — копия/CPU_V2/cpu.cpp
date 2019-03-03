typedef elem_cpu data_cpu;

/* typedef for stack */
typedef data_cpu data_t;

#include "C:\Users\Lenovo\Documents\GitHub\safe-morning\Stack\Stack.cpp"

const int RAM_MEMORY_SIZE = 8192;

const int      REG_NUMBER = 255;

struct myCpu
{
  stack_t   mem;
  stack_t   fun;
  data_cpu *reg;
  data_cpu *ram;
};

typedef struct myCpu cpu_t;

//=============================================================================

void cpuCtor (cpu_t *c);
void cpuDtor (cpu_t *c);

void implementation (const char *byteCodeFileName);

#define CPU_DEF_CMD(name, num, par, func) \
void cpu_##name(cpu_t *cpu, int *ipCmd, int *ipPar, char *cmdBuf, data_cpu *parBuf) func

#include "commands.h"

#undef CPU_DEF_CMD

//=============================================================================

void implementation (const char *byteCodeFileName)
{
  assert (byteCodeFileName);

  FILE *byteCodeFile = fopen (byteCodeFileName, "rb");
  assert (byteCodeFile != NULL);

  /* reading statistics info */
  int statBuf[3];
  fread (statBuf, sizeof (int), 3, byteCodeFile);

  /* reading commands */
  char         *cmdBuf = (char *)calloc (statBuf[ASM_STAT_CMD_NUM], sizeof (*cmdBuf));
  fread  (cmdBuf, sizeof (char), statBuf[ASM_STAT_CMD_NUM], byteCodeFile);

  /* reading parameters */
  data_cpu    *parBuf = (data_cpu *)calloc (statBuf[ASM_STAT_PAR_NUM], sizeof (*parBuf));
  fread (parBuf, sizeof (data_cpu), statBuf[ASM_STAT_PAR_NUM], byteCodeFile);

  int ipPar = 0;
  int ipCmd = 0;
  char  cmd = 0;

  cpu_t cpu = {0};
  cpuCtor (&cpu);

  while (ipCmd < statBuf[ASM_STAT_CMD_NUM])
  {
    cmd = cmdBuf[ipCmd];
    #define CPU_DEF_CMD(name, num, par, func)                     \
    {                                                             \
      if (cmd == num)                                             \
      {                                                           \
        /*printf (#name"\n");                                     \
        printf (" %d \n\n", ipCmd);*/                             \
        cpu_##name(&cpu, &ipCmd, &ipPar, cmdBuf, parBuf);         \
      }                                                           \
    }
    #include "commands.h"
    #undef CPU_DEF_CMD
  }

  cpuDtor (&cpu);

  free(cmdBuf);
  free(parBuf);

  printf (" # Implementation was successful \n");
}

void cpuCtor (cpu_t *c)
{
  assert (c);

  c->mem = {0};

  stackCtor (&(c->mem));
  stackCtor (&(c->fun));

  c->reg = (data_cpu *)calloc (REG_NUMBER, sizeof (*(c->reg)));
  c->ram = (data_cpu *)calloc (RAM_MEMORY_SIZE, sizeof (*(c->ram)));
}

void cpuDtor (cpu_t *c)
{
    /* delete CPU and all its information */
    stackDtor (&(c->mem));
    stackDtor (&(c->fun));

    free(c->ram);
    free(c->reg);

    c->ram = NULL;
    c->reg = NULL;
    c = NULL;
}
