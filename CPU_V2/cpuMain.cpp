typedef double elem_cpu;

#define ELEM_CPU_PRINT "%f"

#include <math.h>
#include "constants.cpp"
#include "asm.cpp"
#include "disasm.cpp"
#include "cpu.cpp"

#define taskFact
#ifdef  taskFact
  const char *CMD_FILE_NAME     = "Factorial.txt";
#else
  const char *CMD_FILE_NAME       = "SolveQuadratic.txt";
#endif
//cpu_t cpu = {0};
const char *BYTECODE_FILE_NAME  = "ByteCode.txt";
const char *IN_COMP_FILE_NAME   = "BackInCmd.txt";

//=============================================================================
