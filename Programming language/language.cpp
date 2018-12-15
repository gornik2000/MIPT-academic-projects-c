//-----------------------------------------------------------------------------
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include "fileIO.cpp"
#include "CPU_V2/cpuMain.cpp"
#include <list>
//-----------------------------------------------------------------------------
int MAX_FUN_NUMBER = 100;
int MAX_VAR_NUMBER = 100;

struct myFunction
{
  const char *name;
  int         pos;
  char        argNum;
};
typedef struct myFunction cmd_fun;

cmd_fun *funCtor (void)
{
  cmd_fun *fun = (cmd_fun *)calloc (1, sizeof (*fun));

  fun->argNum = -1;
  fun->name   = NULL;
  fun->pos    = -1;

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

enum LANG_VAR_TYPES
{
  CNST = 1, /* const can't be used */
  OP   = 2,
  VAR  = 3,
  CMD  = 4,
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
char *createWord (char *str);
char *nextWord (char *str);

char programImplementation (const char *fileInName, const char *fileOutName);

tree *codeToTree (char **text, int size, int lines);

char *getCmd               (char *str, char *cmd);

char  treeToFile           (tree *t, const char *fileName);
char  subTreeToFile        (node *n, FILE *file);

node *nodeCreateCopy       (node *n);
node *nodeCreate           (p_data *key, node *leftChild, node *rightChild);

#include "recursiveDescent.cpp"
//-----------------------------------------------------------------------------
char programCompilation    (const char *fileInName, const char *fileOutName)
{
  int codeSize  = 0;
  int codeLines = 0;
  char **codeText = createTextFromFile (fileInName, &codeSize, &codeLines);

  printf (" # code size %d lines %d\n", codeSize, codeLines);

  char err = codeTextToByteCode (codeText, codeSize, codeLines, fileOutName);

  free (codeText[0]);
  free (codeText);
  codeText = NULL;

  return err;
}

#define DEF_DIFF(name, val, tp, pt, arg, fun)                                 \
{                                                                             \
  if (strContent(word, name))                                                 \
  {                                                                           \
    printf ("op found %s\n", name);                                           \
    continue;                                                                 \
  }                                                                           \
}
char codeTextToByteCode (char **codeText, int codeSize, int codeLines, const char *fileOutName)
{
  cmd_fun *funArr[MAX_FUN_NUMBER] = {};
  cmd_var *varArr[MAX_VAR_NUMBER] = {};
  char *word = NULL;
  int arrNum = 0;
  int varNum = 0;
  char skip = 0;

  for (int i = 0; i < codeLines; i++)
  {
    word = codeText[i];
    if (strContent(word, "def"))
    {
      //printf ("\nstarted\n");
      word = nextWord (word);
      funArr[arrNum] = funCtor ();
      funArr[arrNum]->name = createWord (word);
      while (word[0] != '\0')
      {
        //printf ("circle for arguments word %s\n", word);
        word = nextWord (word);
        //printf ("circle for arguments word %s\n", word);
        funArr[arrNum]->argNum = funArr[arrNum]->argNum + 1;
        //printf ("\nok\n");
      }
      arrNum ++;
      continue;
      //printf ("it ended \n");
    }
    for (; *word == '\t'; word++);

    #include "operations.h"

    skip = 0;
    for (int j = 0; j < varNum; j++)
    {
      if (strContent (word, varArr[j]->name))
      {
        printf ("-- %s - %s \n", word, varArr[j]->name);
        skip = 1;
        break;
      }
    }
    if (skip)
    {
      continue;
    }

    varArr[varNum] = varCtor ();
    varArr[varNum]->name = createWord (word);
    varNum ++;
  }
  for (int i = 0; i < arrNum; i++)
  {
    printf ("-> function %s arguments %d\n", funArr[i]->name, funArr[i]->argNum);
  }

  for (int i = 0; i < varNum; i++)
  {
    printf ("-> variable %s\n", varArr[i]->name);
  }
}
#undef DEF_DIFF

char *nextWord (char *str)
{
  /* skip previous name */
  while (*str != ' ' && *str != ',' &&
         *str != '(' && *str != ')' &&
         *str != '\0')
  {
    str++;
  }

  /* skip to next name */
  while (*str == ':' || *str == ' ' ||
         *str == '(' || *str == ')' ||
         *str == ',')
  {
    str++;
  }

  //printf ("%s\n", str);
  return str;
}

char *createWord (char *str)
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
//-----------------------------------------------------------------------------
#define DEF_DIFF(name, val, tp, pt, arg, funD)                                \
{                                                                             \
  if (n->key->type == tp && n->key->value == val)                             \
  {                                                                           \
    fprintf (file, "%s", name);                                               \
    skip = 1;                                                                 \
  }                                                                           \
}
char subTreeToFile (node *n, FILE *file)
{
  assert (n    != NULL);
  assert (file != NULL);

  if (n->key != NULL && n->rightChild == NULL)
  {
    char skip = 0;
    #include "operations.h"

    if (skip == 0)
    {
      fprintf (file, "%.2lf", n->key->value);
    }
  }

  if (n->leftChild != NULL)
  {
    if (n->leftChild->key->priority > n->key->priority)
    {
      fprintf       (file, "(");
      subTreeToFile (n->leftChild, file);
      fprintf       (file, ")");
    }
    else
    {
      subTreeToFile (n->leftChild, file);
    }
  }

  if (n->key != NULL && n->rightChild != NULL)
  {
    char skip = 0;
    #include "operations.h"

    if (skip == 0)
    {
      printf ("found value %lf\n", n->key->value);
      fprintf (file, "%lf", n->key->value);
    }
  }

  if (n->rightChild != NULL)
  {
    if (n->rightChild->key->priority > n->key->priority)
    {
      fprintf       (file, "(");
      subTreeToFile (n->rightChild, file);
      fprintf       (file, ")");
    }
    else
    {
      subTreeToFile (n->rightChild, file);
    }
  }
}
#undef DEF_DIFF
//-----------------------------------------------------------------------------
char treeToFile (tree *t, const char *fileName)
{
  assert (t        != NULL);
  assert (fileName != NULL);

  FILE *file = fopen (fileName, "w");

  subTreeToFile (t->rootNode, file);

  return 0;
}
//-----------------------------------------------------------------------------
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

  n->leftChild  = leftChild;
  n->rightChild = rightChild;
  n->key        = key;

  return n;
}
//-----------------------------------------------------------------------------
