#include "C:\Users\Lenovo\Documents\GitHub\safe-morning\Stack\Stack.cpp"
#include "C:\Users\Lenovo\Documents\GitHub\safe-morning\Encyclopedia\FileIO.cpp"

struct myCPU
{
    stack_t stack;
    data_t *reg;
    data_t *ram;
};

typedef struct myCPU CPU;

void cpuCtr (CPU *c)
{
  stackCtor(&(c->stack));

  c->reg = (data_t *)calloc (ASM_REG_NUMBER, sizeof (*(c->reg)));
  c->ram = (data_t *)calloc (ASM_RAM_SIZE, sizeof (*(c->reg)));
}

void cpuDtr (CPU *c)
{
  stackDtor(&(c->stack));

  free(c->reg);
  c->reg = NULL;

  free(c->ram);
  c->ram = NULL;
}

void cpuProgrammImplementation (const char *byteCodeFileName)
{
  assert (byteCodeFileName);

  FILE *byteCodeFile = fopen (byteCodeFileName, "rb");
  assert (byteCodeFile);

  int byteCodeSize = fileSize (byteCodeFile);
  printf ("%d", byteCodeSize);

  int *byteCode = (int *)calloc(byteCodeSize, sizeof (*byteCode));
  fread(byteCode, byteCodeSize, sizeof(int), byteCodeFile);

  //for (int i = 0; i<byteCodeSize; i++) printf("byteCode %d \n", byteCode[i]);
}
