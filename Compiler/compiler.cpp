//---------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//---------------------------------------------------------------------------*/
const int MAX_FILE_NAME_SIZE = 30;
const int MAX_CMD_SIZE       = 100;

const char *TREE_FILE_NAME   = "a.tree";
//---------------------------------------------------------------------------*/
const char *getExtension (const char *str);
//---------------------------------------------------------------------------*/
int main ()
{
  printf (" # Compiler 3.00\n"
          " # This program compiles programs from one of the available"
          " programming language to one of the available assemblers\n\n"
          " # Available extensions :\n"
          " # input     output      \n"
          " # .ng       .com        \n"
          " # .cb       .s          \n"
          " #           .asm        \n\n");
  //-------------------------------------------*/
  char code_in [MAX_FILE_NAME_SIZE] = {0};
  char code_out[MAX_FILE_NAME_SIZE] = {0};

  scanf ("%s%s", code_in, code_out);

  char  firstCmd[MAX_CMD_SIZE] = {0};
  char secondCmd[MAX_CMD_SIZE] = {0};
  //-------------------------------------------*/
  const char *ext_in  = getExtension (code_in );
  if      (strcmp (ext_in, "ng") == 0)
  {
    sprintf (firstCmd, "start ng_tree.exe %s %s", code_in, TREE_FILE_NAME);
    system  (firstCmd);
  }
  else if (strcmp (ext_in, "cb") == 0)
  {
    sprintf (firstCmd, "start cb_tree.exe %s %s", code_in, TREE_FILE_NAME);
    system  (firstCmd);
  }
  else
  {
    printf (" # Unknown input extension \"%s\"\n", ext_in);
    return 1;
  }
  //-------------------------------------------*/
  const char *ext_out = getExtension (code_out);
  if      (strcmp (ext_out, "com") == 0)
  {
    sprintf (secondCmd, "start tree_com.exe %s %s", TREE_FILE_NAME, code_out);
    system  (secondCmd);
  }
  else if (strcmp (ext_out, "s"  ) == 0)
  {
    sprintf (secondCmd, "start tree_s.exe %s %s", TREE_FILE_NAME, code_out);
    system  (secondCmd);
  }
  else if (strcmp (ext_out, "asm") == 0)
  {
    sprintf (secondCmd, "start tree_asm.exe %s %s", TREE_FILE_NAME, code_out);
    system  (secondCmd);
  }
  else
  {
    printf (" # Unknown output extension \"%s\"\n", ext_out);
    return 2;
  }
  //-------------------------------------------*/
  printf (" # Code was successfully compiled\n"
          " # Thanks for using our compiler! \n");

  return 0;
}
//---------------------------------------------------------------------------*/
const char *getExtension (const char *str)
{
  while (*str != '.' && *str != '\0') str++;

  return ++str;
}
//---------------------------------------------------------------------------*/
//                Gorbachev Nikita & Kondrashov  Leonid
//                              May 2019
//---------------------------------------------------------------------------*/
