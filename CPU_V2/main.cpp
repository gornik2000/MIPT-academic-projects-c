//#include "commands.cpp"
#include "asm.cpp"
//#include "CPU.cpp"

const char *cmdFileName      = "CmdFile.txt" ;
const char *byteCodeFileName = "ByteCode.txt";

int main ()
{
  compilation  (cmdFileName, byteCodeFileName);
  //cpuProgrammImplementation (byteCodeFileName);
}
