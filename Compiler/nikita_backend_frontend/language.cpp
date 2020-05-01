//---------------------------------------------------------------------------*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>
//---------------------------------------------------------------------------*/
#include "fileIO.h"
//---------------------------------------------------------------------------*/
#define DEF_BC(name, len, ...) const unsigned char name##_CODE[] = {__VA_ARGS__};
#include "bytecode.h"
#undef  DEF_BC
//---------------------------------------------------------------------------*/
#define DEF_BC(name, len, ...) const char name##_LEN = len;
#include "bytecode.h"
#undef  DEF_BC
//---------------------------------------------------------------------------*/
#include "programTree.h"
//---------------------------------------------------------------------------*/
const char MAX_COMMAND_SIZE = 80;
const int  MAX_ASMCODE_SIZE = 10000;
const int  BLOCK_SIZE       = 256;
//---------------------------------------------------------------------------*/
char  programCompilation (const char *fileInName);
char *codeRefactoring    (const char *incode, char *outcode);

char  isSep  (const char c);

unsigned char * putInBuf (unsigned char *buf, const unsigned char *str, char num);
unsigned char *treeToBuf (const tree *t, unsigned char *buf);
unsigned char *nodeToBuf (const node *n, unsigned char *buf);

node *nodeCreate (p_data *key, node *leftChild, node *rightChild);

#include "recursiveDescent.cpp"
//---------------------------------------------------------------------------*/
char programCompilation (const char *fileInName)
{
  FILE *pgmFile = fopen (fileInName, "r");
  int countSize = fileSize (pgmFile);

  /* open code file */
  char *pgmAllCode = (char *)calloc (countSize, sizeof (*pgmAllCode));
  int   pgmSize    = fread (pgmAllCode, sizeof (char), countSize, pgmFile);
  printf (" # data before refactoring: \n %s \n", pgmAllCode);
  fclose (pgmFile);

  /* code refactoring */
  char *pgmCode = (char *)calloc (countSize, sizeof (*pgmCode));
  pgmCode++;
  codeRefactoring (pgmAllCode, pgmCode);
  printf (" # data after refactoring: \n %s \n", pgmCode);

  /* write refactored code for debugging */
  FILE   *treeFile = fopen ("data_out/refactored.txt", "w");
  fwrite (pgmCode, sizeof (char), countSize, treeFile);
  fclose (treeFile);
  free   (pgmAllCode);

  /* code to tree + write it for debugging */
  tree *pgmTree = getTreeFromCode (pgmCode);
  pgmCode--;
  treeToFile (pgmTree, "data_out/tree_file.txt");

  printf (" # tree was built\n");

  /* tree to byte code buffer */
  unsigned char *byteCode = (unsigned char *)calloc (MAX_ASMCODE_SIZE, sizeof (*byteCode));
  unsigned char *endBuf   = treeToBuf (pgmTree, byteCode);
  FILE          *outFile  = fopen ("S:/Doc/a.com", "wb");
  fwrite (byteCode, sizeof (char), endBuf - byteCode, outFile);

  fclose (outFile);
  free   (pgmCode);
  free   (byteCode);

  return 0;
}
//---------------------------------------------------------------------------*/
/* for commands     */
#define $(cmd)   buf   = putInBuf (buf, cmd##_CODE, cmd##_LEN);
/* for 1b constants */
#define c(cnst) *buf++ = (unsigned char)(cnst);
/* for 2b constants */
#define dc(cnst)                                                              \
{                                                                             \
  c((cnst) % 256)                                                             \
  c((cnst) / 256)                                                             \
}
unsigned char *treeToBuf (const tree *t, unsigned  char *buf)
{
  assert (t   != NULL);
  assert (buf != NULL);

  unsigned char *jmpStart = 0;

  /* program */
  $(SUBD_SP); dc(BLOCK_SIZE);

  /* functions */
  for (int i = 0; i < funNumber; i++)
  {
    $(JMP);
    jmpStart = buf;
    buf++;

    funArr[i]->start = buf;

    $(PUSH_BP);
    $(R16_MOV); $(BP_SP);
    $(ADD_BP ); c(4    );
    for (int j = 0; j < t->rootNode->child[i]->childrenNum; j++)
    {
      buf = nodeToBuf (t->rootNode->child[i]->child[j], buf);
    }
    $(RET);
    $(NOP);
    $(NOP);
    $(NOP);

    *jmpStart = buf - jmpStart - 1;
  }

  /* main program */
  $(ENTER);
  for (int i = funNumber; i < t->rootNode->childrenNum; i++)
  {
    buf = nodeToBuf (t->rootNode->child[i], buf);
  }
  $(EXIT);

  return buf;
}
//---------------------------------------------------------------------------*/
#define DEF_OP(name, code, tp, arg, var, funD)                                \
{                                                                             \
  if ((code == n->key->value) && (tp == n->key->type))                        \
  {                                                                           \
    funD                                                                      \
  }                                                                           \
}
unsigned char *nodeToBuf (const node *n, unsigned  char *buf)
{
  assert (n   != NULL);
  assert (buf != NULL);

  #include "operations.h"

  if (n->key->type == VAR)
  {
    $(R16_MOV); $(AX_1BP1); c(n->key->value * 2); // mov  ax, [bp + value]
    $(PUSH_AX);                                   // push ax
  }

  if (n->key->type == CNST)
  {
    $(MOV_AX ); dc(n->key->value); // mov ax, value
    $(PUSH_AX);                    // push ax
  }

  if (n->key->type == FUN)
  for (int i = 0; i < funNumber; i++)
  {
    if (i == n->key->value)
    {
      $(SUBD_SP); dc(BLOCK_SIZE);
      for (int j = funArr[i]->argNum - 1; j >= 0; j--)
      {
        buf = nodeToBuf (n->child[j], buf);
      }
      $(CALL);    dc(65535 - (buf - funArr[i]->start));
      $(ADDD_SP); dc(BLOCK_SIZE + funArr[i]->argNum * 2);
      $(PUSH_AX);
    }
  }

  return buf;
}
#undef dc
#undef c
#undef $
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
unsigned char *putInBuf (unsigned char *buf, const unsigned char *str, char num)
{
  for (int i = 0; i < num; i++, str++, buf++)
  {
    *buf = *str;
  }

  return buf;
}
//---------------------------------------------------------------------------*/
/*node *nodeCreate (p_data *key, node *leftChild, node *rightChild)
{
  node *n = nodeCtor ();

  n->child[0] = leftChild;
  n->child[1] = rightChild;
  n->key      = key;

  return n;
}
//---------------------------------------------------------------------------*/
/*char isSep (const char c)
{
  if (c == ' ' ) return 1;
  if (c == '(' ) return 1;
  if (c == ')' ) return 1;
  if (c == '\n') return 1;
  if (c == '\0') return 1;
  if (c == ':' ) return 1;
  if (c == '\t') return 1;
  if (c == ',' ) return 1;
  if (c == ';' ) return 1;
  if (c == '[' ) return 1;
  if (c == ']' ) return 1;

  return 0;
}
//---------------------------------------------------------------------------*/
//               © Gorbachev Nikita, December 2018 + April 2019              //
//---------------------------------------------------------------------------*/
