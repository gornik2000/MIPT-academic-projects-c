//---------------------------------------------------------------------------*/
#include "fileIO.h"
#include "programTree.h"
#include "recursiveDescent.cpp"
//---------------------------------------------------------------------------*/
const char MAX_FILE_NAME_SIZE = 30;
const char MAX_COMMAND_SIZE   = 80;
//---------------------------------------------------------------------------*/
char  programCompilation (const char *fileInName);
char *codeRefactoring    (const char *incode, char *outcode);
//---------------------------------------------------------------------------*/
int main (int argc, char *argv[])
{
	if (argc < 2)
  {
		printf(" # Missing file\n");
		return 0;
	}

  char *fileInName  = argv[1];
  char *fileOutName = argv[2];

  FILE *pgmFile = fopen (fileInName, "r");
  int countSize = fileSize (pgmFile);

  /* open code file */
  char *pgmAllCode = (char *)calloc (countSize, sizeof (*pgmAllCode));
  int   pgmSize    = fread (pgmAllCode, sizeof (char), countSize, pgmFile);
  fclose (pgmFile);

  /* code refactoring */
  char *pgmCode = (char *)calloc (countSize, sizeof (*pgmCode));
  pgmCode++;
  codeRefactoring (pgmAllCode, pgmCode);

  /* write refactored code for debugging */
  //FILE   *treeFile = fopen ("data_out/refactored.txt", "w");
  //fwrite (pgmCode, sizeof (char), countSize, treeFile);
  //fclose (treeFile);
  free   (pgmAllCode);

  /* code to tree + write it for debugging */
  tree *pgmTree = getTreeFromCode (pgmCode);
  pgmCode--;
  treeToStdFile (pgmTree, fileOutName);
  //treeToFile    (pgmTree, "tree_dump.txt");

  free   (pgmCode);

  printf (" # Tree was created\n");

  return 0;
}
//---------------------------------------------------------------------------*/
char *codeRefactoring (const char *incode, char *outcode)
{
  assert ( incode != NULL);
  assert (outcode != NULL);

  //printf (" # Code Refactoring\n");
  char *start = outcode;

  while (*incode != '\0')
  {
    if (*incode == '#')
    {
      while (*incode != '\n' && *incode != '\0') incode++;
      if (*(outcode - 1) != '\n' && *(outcode - 1) != '\0')
      {
        *outcode = *incode;
         outcode++;
      }
    }
    /* anti doubles */
    else if ((*incode != ' '  || *(outcode - 1) != ' ') &&
             (*incode != '\n' || *(outcode - 1) != '\n'))
    {
      *outcode = *incode;
       outcode++;
    }
    incode++;
  }

  return outcode;
}
//---------------------------------------------------------------------------*/
//               © Gorbachev Nikita, May 2019                                //
//---------------------------------------------------------------------------*/
