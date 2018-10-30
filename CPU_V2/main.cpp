typedef int elem_cpu;

#define ELEM_CPU_PRINT "%d"

#include <math.h>
#include "constants.cpp"
#include "asm.cpp"
#include "disasm.cpp"
#include "cpu.cpp"

const char *cmdFileName      = "CmdFile.txt";
const char *byteCodeFileName = "ByteCode.txt";
const char *inCompFileName   = "BackInCmd.txt";

int main ()
{
  //compilation        (cmdFileName, byteCodeFileName);

  //inverseCompilation (byteCodeFileName, inCompFileName);

  implementa
  tion     (byteCodeFileName);
}
