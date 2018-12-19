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
/*#define DEF_DIFF(name, val, tp, pt, arg, funD)  op_#name = (val),
enum operations
{
  #include "operations.h"
};
#undef DEF_DIFF //*/
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

struct myVariable
{
  const char *name;
  int         pos;
};
typedef struct myVariable cmd_var;

cmd_var *varCtor (void)
{
  cmd_var *var = (cmd_var *)calloc (1, sizeof (*var));

  var->name   = NULL;
  var->pos    = 0;

  return var;
}
//-----------------------------------------------------------------------------
const char   MAX_COMMAND_SIZE = 20;
const double PI               = 3.141592654;

enum node_types
{
  PGM  = 0,
  CNST = 1, /* const can't be used */
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
char codeTextToByteCode (char **codeText, int codeSize, int codeLines, const char *fileOutName);
char strContent(const char *strMain, const char *strMinor);
char *createWord (const char *str);
char *nextWord (char *str);

char programImplementation (const char *fileInName, const char *fileOutName);

tree *codeToTree (char **text, int size, int lines);

char *getCmd               (char *str, char *cmd);

char  treeToFile           (tree *t, const char *fileName);
char  subTreeToFile        (node *n, FILE *file);

node *nodeCreateCopy       (node *n);
node *nodeCreate           (p_data *key, node *leftChild, node *rightChild);

char isSep                 (const char c);

char treeToFile (tree *t, const char *fileName);

#include "recursiveDescent.cpp"
//-----------------------------------------------------------------------------
char programCompilation    (const char *fileInName, const char *fileOutName)
{
  /*
  int codeSize  = 0;
  int codeLines = 0;
  char **codeText = createTextFromFile (fileInName, &codeSize, &codeLines);

  printf (" # code size %d lines %d\n", codeSize, codeLines);

  char err = codeTextToByteCode (codeText, codeSize, codeLines, fileOutName);

  free (codeText[0]);
  free (codeText);
  codeText = NULL;

  return err; */

  FILE *pgmFile = fopen (fileInName, "r");
  int countSize = fileSize (pgmFile);

  char *pgmCode = (char *)calloc (countSize + 10, sizeof (*pgmCode));
  int pgmSize   = fread (pgmCode, sizeof (char), countSize, pgmFile);
  /* we need pgmSize because it does not content \r\n, only \n */

  fclose (pgmFile);
  printf (" # read size %d, found size %d, data: \n%s", pgmSize, countSize, pgmCode);

  tree *pgmTree = getTreeFromCode (pgmCode);

  treeToFile (pgmTree, "OUTTREE.txt");
  return 0;
}

char *nextWord (char *str)
{

  while (*str != ' ' && *str != ',' &&
         *str != '(' && *str != ')' &&
         *str != '\0')
  {
    str++;
  }

  while (*str == ':' || *str == ' ' ||
         *str == '(' || *str == ')' ||
         *str == ',')
  {
    str++;
  }

  //printf ("%s\n", str);
  return str;
}

char *createWord (const char *str)
{
  assert (str != NULL);

  int len = 0;
  while (str[len] != '\0' && str[len] != ' ' &&
         str[len] != '('  && str[len] != ')')
  {
    len++;
  }

  char *word = (char *)calloc (len + 1, sizeof (*word));
  for (int i = 0; i < len; i++)
  {
    word[i] = str[i];
  }

  //printf (" created word %s\n", word);
  return word;
}

char strContent(const char *strMain, const char *strMinor)
{
  assert (strMain  != NULL);
  assert (strMinor != NULL);

  for (int i = 0; strMain[i]  != '\0' && strMinor[i] != '\0' &&
                  strMinor[i] != ' '  && strMinor[i] != '('  &&
                  strMinor[i] != ')'  && strMinor[i] != ',';
                  i++)
  {
    //printf("1");
    if (strMain[i] != strMinor[i])
    {
      //printf("\n");
      return 0;
    }
  }
  return 1;
}

char programImplementation (const char *fileInName, const char *fileOutName)
{
  // open bytecode file
  // do it
  // print everything in outfile
}
tree *codeToTree (char **text, int size, int lines)
{
  return 0;
}

char *getCmd (char *str, char *cmd)
{
  int cmdPos = 0;
  while (*str != '(' && *str != ')')
  {
    cmd[cmdPos] = *str;
    cmdPos ++;
    str ++;
  }

  return str - 1;
}

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

char subTreeToFile (node *n, FILE *file);

char treeToFile (tree *t, const char *fileName)
{
  assert (t        != NULL);
  assert (fileName != NULL);

  FILE *file = fopen (fileName, "w");

  subTreeToFile (t->rootNode, file);
}

char subTreeToFile (node *n, FILE *file)
{
  assert (n    != NULL);
  assert (file != NULL);

  if (n->key != NULL)
  {
    tabFprint (file, n->deepness);
    fprintf   (file, "%s\n", *n->key);
  }

  if (n->leftChild != NULL)
  {
    tabFprint     (file, n->deepness + 1);
    fprintf       (file, "{\n");
    subTreeToFile (n->leftChild, file);
    tabFprint     (file, n->deepness + 1);
    fprintf       (file, "}\n");
  }

  if (n->rightChild != NULL)
  {
    tabFprint     (file, n->deepness + 1);
    fprintf       (file, "{\n");
    subTreeToFile (n->rightChild, file);
    tabFprint     (file, n->deepness + 1);
    fprintf       (file, "}\n");
  }
}

