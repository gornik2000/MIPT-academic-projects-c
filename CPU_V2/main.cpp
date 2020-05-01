typedef int elem_cpu;

#define ELEM_CPU_PRINT "%d"

#include <math.h>
#include "constants.cpp"
#include "asm.cpp"
#include "disasm.cpp"
#include "cpu.cpp"

//#define taskTest
#ifdef  taskTest
  const char *CMD_FILE_NAME     = "Test.txt";
#else
  const char *CMD_FILE_NAME       = "SolveQuadratic.txt";
#endif

const char *BYTECODE_FILE_NAME  = "ByteCode.txt";
const char *IN_COMP_FILE_NAME   = "BackInCmd.txt";

//=============================================================================

int main ()
{
  char mode = 0;
  printf (" # Enter 1 for compilation, "
                   "2 for inverse compilation, "
                   "3 for implementation \n");
  scanf ("%d", &mode);

//  switch (mode)
  {
//  case 1:
    compilation        (CMD_FILE_NAME,      BYTECODE_FILE_NAME);
//    break;

//  case 2:
    inverseCompilation (BYTECODE_FILE_NAME, IN_COMP_FILE_NAME);
//    break;

//  case 3:
    implementation     (BYTECODE_FILE_NAME);
//    break;

//  default:
    printf (" # Incorrect inputed number \n");
//    break;
  }
}
