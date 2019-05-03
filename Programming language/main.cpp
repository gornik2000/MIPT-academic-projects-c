#include "stdio.h"
#include "language.cpp"
//-----------------------------------------------------------------------------
int main ()
{
  //const char*    fileCodeName = "factorial.txt";
  //const char*    fileCodeName = "quadratic.txt";
  const char*    fileCodeName = "code.txt";
  const char* fileAsmName  = "myasm.txt";

  printf (" # Language 1.50\n"
          " # This program is used to compile programs on my language \n"
          " # Input code in \"code.txt\""
          " # Output can be found in \"out.txt\". Good luck! \n\n");

  printf (" # Chose mode: [c]ompilation - [i]mplementation\n");

  char mode = 0;
  scanf ("%c", &mode);

  switch (mode)
  {
    case 'c':
      programCompilation    (fileCodeName, fileAsmName);
      printf (" # Code was successfully compiled\n");
      break;

    case 'i':
      programImplementation (fileAsmName);
      printf (" # Code was successfully implemented\n");
      break;

    default:
      printf (" # Unknown symbol\n");
      break;
  }

  printf (" # Thanks for using my language\n");

  return 0;
}
//-----------------------------------------------------------------------------
