//-----------------------------------------------------------------------------
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include "fileIO.cpp"
#include "CPU_V2/cpuMain.cpp"
//-----------------------------------------------------------------------------
struct myFunction
{
  const char *name;
  char        argNum;
  char        varNum;
};
typedef struct myFunction cmd_fun;

cmd_fun *funCtor (void)
{
  cmd_fun *fun = (cmd_fun *)calloc (1, sizeof (*fun));

  fun->argNum = 0;
  fun->varNum = 0;
  fun->name   = NULL;

  return fun;
}

cmd_fun *funDtor (cmd_fun *fun)
{
  fun->argNum = -1;
  fun->varNum = -1;
  fun->name   = NULL;

  free (fun);
  fun = NULL;

  return fun;
}
//-----------------------------------------------------------------------------
const char   MAX_COMMAND_SIZE = 20;
const double PI               = 3.141592654;
int FLAG                      = 0;

enum node_types
{
  PGM  = 0,
  CNST = 1,
  OP   = 2,
  VAR  = 3,
  CMD  = 4,
  FUN  = 5,
};

/* type of diff_data value */
typedef double p_data_value;

struct prm_data
{
  char         type;
  char         priority;
  p_data_value value;
};
typedef struct prm_data p_data;

typedef p_data tree_data;

#include "tree.cpp"

p_data *pdataCtor   (void);

p_data *pdataDtor   (p_data *d);

p_data *pdataCreate (char type, char priority, p_data_value value);

//-----------------------------------------------------------------------------
p_data *pdataCtor (void)
{
  p_data *d = (p_data *)calloc (1, sizeof (*d));

  d->type     = 0;
  d->priority = 0;
  d->value    = 0;// (d_data_value *)calloc (1, sizeof (*d->value));

  return d;
}
//-----------------------------------------------------------------------------
p_data *pdataDtor (p_data *d)
{
  assert (d != NULL);

  d->type     = 0;
  d->priority = 0;
  d->value    = 0;

  free (d);
  d = NULL;

  return d;
}
//-----------------------------------------------------------------------------
p_data *pdataCreate (char type, char priority, p_data_value value)
{
  p_data *d = pdataCtor ();

  d->type     = type;
  d->priority = priority;
  d->value    = value;

  return d;
}
//-----------------------------------------------------------------------------
char programCompilation    (const char *fileInName, const char *fileOutName);
char codeTextToByteCode    (char **codeText, int codeSize, int codeLines,
                            const char *fileOutName);

char treeToAsm             (tree *t, const char *fileName);
char subTreeToAsm          (node *n, FILE *f);

char programImplementation (const char *fileAsmName);

char *getCmd               (char *str, char *cmd);

node *nodeCreateCopy       (node *n);
node *nodeCreate           (p_data *key, node *leftChild, node *rightChild);

char isSep                 (const char c);

char treeToFile            (tree *t, const char *fileName);

#include "recursiveDescent.cpp"
//-----------------------------------------------------------------------------
char programCompilation    (const char *fileInName, const char *fileOutName)
{
  FILE *pgmFile = fopen (fileInName, "r");
  int countSize = fileSize (pgmFile);

  char *pgmCode = (char *)calloc (countSize + 10, sizeof (*pgmCode));
  int pgmSize   = fread (pgmCode, sizeof (char), countSize, pgmFile);
  /* we need pgmSize because it does not content \r\n, only \n */

  fclose (pgmFile);
  printf (" # read size %d, found size %d, data: \n%s\n\n", pgmSize, countSize, pgmCode);

  tree *pgmTree = getTreeFromCode (pgmCode);
  return treeToAsm (pgmTree, fileOutName);
}
//-----------------------------------------------------------------------------
char treeToAsm (tree *t, const char *fileName)
{
  FILE *f = fopen (fileName, "w");

  fprintf (f, "push 1\n");
  fprintf (f, "pop r0\n");

  for (int i = funNumber; i < t->rootNode->childrenNum; i++)
  {
    printf (" # CodeStr start with \"%lf\"\n", t->rootNode->child[i]->key->value);
    subTreeToAsm (t->rootNode->child[i], f);
  }

  fprintf (f, "end\n");

  for (int i = 0; i < funNumber; i++)
  {
    printf (" # CodeStr start with fun \"%lf\"\n", t->rootNode->child[i]->key->value);

    fprintf (f, "\n\n:fn_%s\n", funArr[i]->name);
    fprintf (f, "push 20\n");
    fprintf (f, "push r0\n");
    fprintf (f, "add\n");
    fprintf (f, "pop r0\n\n");

    for (int j =  funArr[i]->argNum - 1; j >= 0; j--)
    {
      fprintf (f, "pop +%d\n", j);
    }

    for (int j = 0; j < t->rootNode->child[i]->childrenNum; j ++)
    {
      subTreeToAsm (t->rootNode->child[i]->child[j], f);
    }
    fprintf (f, "\npush r0\n");
    fprintf (f, "push -20\n");
    fprintf (f, "add\n");
    fprintf (f, "pop r0\n");
    fprintf (f, "ret\n");
  }
  return 0;
}
//-----------------------------------------------------------------------------
#define DEF_DIFF(name, val, tp, pt, arg, funD)                                \
{                                                                             \
  if ((val == n->key->value) && (tp == n->key->type))                         \
  {                                                                           \
    funD;                                                                     \
  }                                                                           \
}
char subTreeToAsm (node *n, FILE *f)
{
  #include "operations.h"

  if (n->key->type == VAR)
  {
    fprintf (f, "push +%d\n", (int)n->key->value);
  }

  if (n->key->type == CNST)
  {
    fprintf (f, "push %lf\n", n->key->value);
  }

  if (n->key->type == FUN)
  for (int i = 0; i < funNumber; i++)
  {
    if (i == n->key->value)
    {
      for (int j = 0; j < funArr[i]->argNum; j++)
      {
        subTreeToAsm (n->child[j], f);
      }

      fprintf (f, "call :fn_%s\n", funArr[i]->name);
    }
  }
}
#undef DEF_DIFF
//-----------------------------------------------------------------------------
char programImplementation (const char *fileAsmName)
{
  assert (fileAsmName != NULL);

  const char *fileBytecodeName = "bytecode.txt";

  compilation        (fileAsmName, fileBytecodeName);
  implementation     (fileBytecodeName);
}
//-----------------------------------------------------------------------------
node *nodeCreateCopy (node *n)
{
  assert (n != NULL);

  node *nCopy =  nodeCtor ();
  nCopy->key  = pdataCtor ();

  nCopy->key->type     = n->key->type;
  nCopy->key->priority = n->key->priority;
  nCopy->key->value    = n->key->value;

  nCopy->deepness = n->deepness;
  nCopy->parent   = n->parent;

  if (n->leftChild != NULL)
  {
    nCopy->leftChild  = nodeCreateCopy (n->leftChild);
  }
  if (n->rightChild != NULL)
  {
    nCopy->rightChild = nodeCreateCopy (n->rightChild);
  }

  return nCopy;
}
//-----------------------------------------------------------------------------
node *nodeCreate (p_data *key, node *leftChild, node *rightChild)
{
  node *n = nodeCtor ();

  n->child[0] = leftChild;
  n->child[1] = rightChild;
  n->key      = key;

  return n;
} //*/
//-----------------------------------------------------------------------------
char isSep (const char c)
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
