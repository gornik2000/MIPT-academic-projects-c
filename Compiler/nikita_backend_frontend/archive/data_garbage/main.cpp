//---------------------------------------------------------------------------*/
#include "language.cpp"
//---------------------------------------------------------------------------*/
int main ()
{
  const char* fileCodeName = "data_in/code.ng";

  printf (" # Language 2.00\n"                                                \
          " # This program is used to compile programs on my language \n"     \
          " # Input code in \"code.txt\""                                     \
          " # Output can be found in \"a.com\". Good luck! \n\n");

  programCompilation (fileCodeName);

  printf (" # Code was successfully compiled\n"
          " # Thanks for using my language\n"  );

  return 0;
}
//---------------------------------------------------------------------------*/
//               © Gorbachev Nikita, December 2018 + April 2019              //
//---------------------------------------------------------------------------*/
