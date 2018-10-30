typedef elem_cpu data_cpu;

/* typedef for stack */
typedef data_cpu data_t;

#include "C:\Users\Lenovo\Documents\GitHub\safe-morning\Stack\Stack.cpp"

const int RAM_MEMORY_SIZE = 512;

const int      REG_NUMBER = 4;

struct myCpu
{
  stack_t     mem;
  data_cpu *reg;
  int        *ram;
};

typedef struct myCpu cpu_t;

void cpuCtor (cpu_t *c)
{
  assert (c);

  c->mem = {};
  stackCtor (&(c->mem));

  c->reg = (data_cpu *)calloc (REG_NUMBER, sizeof (*(c->reg)));
  c->ram = (data_cpu *)calloc (RAM_MEMORY_SIZE, sizeof (*(c->ram)));
}

void cpuDtor (cpu_t *c)
{
    /* delete CPU and all its information */
    stackDtor (&(c->mem));

    free(c->ram);
    free(c->reg);

    c->ram = NULL;
    c->reg = NULL;
    c = NULL;
}

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

  while (ipCmd < statBuf[ASM_STAT_CMD_NUM])
  {
    cmd = cmdBuf[ipCmd];
    #define CPU_DEF_CMD(name, num, par, mode, func)               \
    {                                                             \
      if (cmd == num)                                             \
      {                                                           \
        printf(#func"\n");                                        \
        ipCmd++;                                                  \
        switch (par)                                              \
        {                                                         \
        case 0:                                                   \
          break;                                                  \
          /* func(&cpu); */                                       \
        case 1:                                                   \
          break;                                                  \
          /* ipCmp = parBuf[ipPar]; */                            \
        case 2:                                                   \
          ipCmd ++;                                               \
          break;                                                  \
        }                                                         \
      }                                                           \
    }
    #include "commands.h"
    #undef CPU_DEF_CMD
  }

  free(cmdBuf);
  free(parBuf);
}
