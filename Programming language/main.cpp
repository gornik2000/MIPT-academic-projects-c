#include "stdio.h"
//-----------------------------------------------------------------------------
#include "language.cpp"
//-----------------------------------------------------------------------------
int main ()
{
  printf (" # Language 0.2\n"
          " # This program is used to compile programs on my language \n"
          " # Input code in \"code.txt\""
          " # Output can be found in \"out.txt\". Good luck! \n\n");

  printf (" # Chose mode: [c]ompilation - [i]mplementation - [b]oth \n");
  char mode = 0;
  scanf ("%c", &mode);
  switch (mode)
  {
    case 'c':
      programCompilation ("code.txt", "mybytecode.txt");
      printf (" # Code was successfully compiled\n");
      break;
    case 'i':
      //programImplementation ();
      printf (" # Code was successfully implementated\n");
      break;
    case 'b':
      programCompilation ("code.txt", "mybytecode.txt");
      //programImplementation ();
      printf (" # Code was successfully compiled\n");
      break;
    default:
      printf (" # Unknown symbol\n");
  }

  printf (" # Thanks for using my language\n");

  return 0;
}
//-----------------------------------------------------------------------------
// указатели на еще не написанные функции
