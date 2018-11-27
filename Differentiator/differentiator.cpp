#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include "fileIO.cpp"
//-----------------------------------------------------------------------------
const char MAX_COMMAND_SIZE = 20;
const double PI = 3.141592654;

enum DIFF_VAR_TYPES
{
  CNST = 1, /* const can't be used */
  OP   = 2,
  VAR  = 3,
};

/* type of diff_data value */
typedef double d_data_value;

struct diff_data
{
  char         type;
  char         priority;
  d_data_value value;
};
typedef struct diff_data d_data;

typedef d_data tree_data;

#include "tree.cpp"

d_data *ddataCtor   (void);

d_data *ddataDtor   (d_data *d);

d_data *ddataCreate (char type, char priority, d_data_value value);

//-----------------------------------------------------------------------------
d_data *ddataCtor (void)
{
  d_data *d = (d_data *)calloc (1, sizeof (*d));

  d->type     = 0;
  d->priority = 0;
  d->value    = 0;// (d_data_value *)calloc (1, sizeof (*d->value));

  return d;
}
//-----------------------------------------------------------------------------
d_data *ddataDtor (d_data *d)
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
d_data *ddataCreate (char type, char priority, d_data_value value)
{
  d_data *d = ddataCtor ();

  d->type     = type;
  d->priority = priority;
  d->value    = value;

  return d;
}
//-----------------------------------------------------------------------------
char differentiator     (const char* fileInName,
                         const char *fileOutName, int diffNumber);

node *nodeDerivative    (node *n, node *dN);

char  treeOptimization  (node *n);

char  nodeOptimization  (node *n);

tree *createTreeFromStr (char *str);

char  treeToFile        (tree *t, const char *fileName);

char  subTreeToFile     (node *n, FILE *file);

char *getCmd            (char *str, char *cmd);

node *nodeCreateCopy    (node *n);
//-----------------------------------------------------------------------------
char differentiator (const char* fileInName,
                     const char *fileOutName, int diffNumber)
{
  int length = 0;
  char *dStr = createStrFromFile ("data.txt", &length);
  //printf (" %s %d %c - first\n", dStr, length, dStr[length]);

  tree *t  = createTreeFromStr (dStr);
  tree *dT = treeCtor ();
  //printf (" line %s \n\n", dStr);
  //printf (" %d \n", dTree->rootNode->key->value);
  //printf (" %d \n", dTree->edgeCount);
  for (int i = 0; i < diffNumber; i++)
  {

    dT->rootNode = nodeDerivative (t->rootNode, dT->rootNode);
    treeOptimization (dT->rootNode);
  }

  treeToFile (dT, fileOutName);
  //dT = treeDtor (dT);
  //t  = treeDtor (t);
  return 0;
}

//-----------------------------------------------------------------------------
#define DEF_DIFF(name, val, tp, pt, funD)                                     \
{                                                                             \
  if (n->key->type == tp && n->key->type != CNST && n->key->value == val)     \
  {                                                                           \
    funD                                                                      \
    return dN;                                                                \
  }                                                                           \
}
/* derivative of n putted in dN*/
node *nodeDerivative (node *n, node *dN)
{
  #include "operations.h"
  dN->key->type     = CNST;
  dN->key->value    = 0.0;
  dN->key->priority = 255;
  return dN;
}
#undef DEF_DIFF
//-----------------------------------------------------------------------------
char treeOptimization (node *n)
{
  printf ("t");
  nodeOptimization (n);

  if (n->leftChild  != NULL)
  {
    treeOptimization (n->leftChild);
  }

  if (n->rightChild != NULL)
  {
    treeOptimization (n->rightChild);
  }

  nodeOptimization (n);

  return 0;
}
//-----------------------------------------------------------------------------
char nodeOptimization   (node *n)
{
  printf ("n");
  if (n->key->type == OP)
  {
    /* if both exist and right = 0 */
    if (n-> leftChild != NULL && n->rightChild != NULL &&
        n->rightChild->key->value == 0)
    {
      if (n->key->value == '*')
      {
        n->key->type     = CNST;
        n->key->priority = 255;
        n->key->value    = 0;
        nodeKillChildren (n);
        return 0;
      }
      if (n->key->value == '^')
      {
        n->key->type     = CNST;
        n->key->priority = 255;
        n->key->value    = 1;
        nodeKillChildren (n);
        return 0;
      }
    }
    /* if both exist and left = 0 */
    if (n-> leftChild != NULL && n->rightChild != NULL &&
        n-> leftChild->key->value == 0)
    {
      if (n->key->value == '*')
      {
        n->key->type     = CNST;
        n->key->priority = 255;
        n->key->value    = 0;
        nodeKillChildren (n);
        return 0;
      }
      if (n->key->value == '/')
      {
        n->key->type     = CNST;
        n->key->priority = 255;
        n->key->value    = 0;
        nodeKillChildren (n);
        return 0;
      }
      if (n->key->value == '^')
      {
        n->key->type     = CNST;
        n->key->priority = 255;
        n->key->value    = 0;
        nodeKillChildren (n);
        return 0;
      }
    }
    /* both exist and const */
    if (n-> leftChild            != NULL && n->rightChild            != NULL &&
        n-> leftChild->key->type == CNST && n->rightChild->key->type == CNST)
    {
      if (n->key->value == '+')
      {
        n->key->type     = CNST;
        n->key->priority = 255;
        n->key->value    = n->leftChild->key->value + n->rightChild->key->value;
        nodeKillChildren (n);
        return 0;
      }
      if (n->key->value == '-')
      {
        n->key->type     = CNST;
        n->key->priority = 255;
        n->key->value    = n->leftChild->key->value - n->rightChild->key->value;
        nodeKillChildren (n);
        return 0;
      }
      if (n->key->value == '*')
      {
        n->key->type     = CNST;
        n->key->priority = 255;
        n->key->value    = n->leftChild->key->value * n->rightChild->key->value;
        nodeKillChildren (n);
        return 0;
      }
      if (n->key->value == '/')
      {
        n->key->type     = CNST;
        n->key->priority = 255;
        n->key->value    = n->leftChild->key->value / n->rightChild->key->value;
        nodeKillChildren (n);
        return 0;
      }
      if (n->key->value == '^')
      {
        n->key->type     = CNST;
        n->key->priority = 255;
        n->key->value    = pow (n->leftChild->key->value, n->rightChild->key->value);
        nodeKillChildren (n);
        return 0;
      }
      return 0;
    }
    /* if left exist and const */
    if (n->leftChild != NULL && n->leftChild->key->type == CNST)
    {
      if (n->key->value == 's')
      {
        n->key->type     = CNST;
        n->key->priority = 255;
        n->key->value    = sin (n->leftChild->key->value);
        nodeKillChildren (n);
        return 0;
      }
      if (n->key->value == 'c')
      {
        n->key->type     = CNST;
        n->key->priority = 255;
        n->key->value    = cos (n->leftChild->key->value);
        nodeKillChildren (n);
        return 0;
      }
      if (n->key->value == 't')
      {
        n->key->type     = CNST;
        n->key->priority = 255;
        n->key->value    = tan (n->leftChild->key->value);
        nodeKillChildren (n);
        return 0;
      }
      if (n->key->value == '1')
      {
        n->key->type     = CNST;
        n->key->priority = 255;
        n->key->value    = 1 / tan (n->leftChild->key->value);
        nodeKillChildren (n);
        return 0;
      }
      if (n->key->value == '2')
      {
        n->key->type     = CNST;
        n->key->priority = 255;
        n->key->value    = asin (n->leftChild->key->value);
        nodeKillChildren (n);
        return 0;
      }
      if (n->key->value == '3')
      {
        n->key->type     = CNST;
        n->key->priority = 255;
        n->key->value    = acos (n->leftChild->key->value);
        nodeKillChildren (n);
        return 0;
      }
      if (n->key->value == '4')
      {
        n->key->type     = CNST;
        n->key->priority = 255;
        n->key->value    = atan (n->leftChild->key->value);
        nodeKillChildren (n);
        return 0;
      }
      if (n->key->value == '5')
      {
        n->key->type     = CNST;
        n->key->priority = 255;
        n->key->value    = PI / 2.0 - (n->leftChild->key->value);
        nodeKillChildren (n);
        return 0;
      }
    }
  }
  return 0;
}
//-----------------------------------------------------------------------------
#define DEF_DIFF(name, val, tp, pt, funD)                                     \
{                                                                             \
  if (strcmp (name, buf) == 0)                                                \
  {                                                                           \
    printf ("->%s\n", name);                                                  \
    currentNode->key->type     = tp;                                          \
    currentNode->key->priority = pt;                                          \
    currentNode->key->value    = (double) val;                                \
                                                                              \
    memset (buf, '\0', sizeof (buf));                                         \
    continue;                                                                 \
  }                                                                           \
}
tree *createTreeFromStr (char *str)
{
  printf ("start str to tree \n");
  tree *t = treeCtor ();
  node *currentNode = t->rootNode;

  char *buf = (char *)calloc (MAX_COMMAND_SIZE, sizeof (*buf));
  int  bufPos = 0;

  for (; *str != '\0'; str++)
  {
    switch (*str)
    {
      case '(' :
        if (currentNode->leftChild == NULL)
        {
          currentNode->leftChild          = nodeCtor ();
          currentNode->leftChild->parent  = currentNode;
          currentNode                     = currentNode->leftChild;
        }
        else
        {
          currentNode->rightChild         = nodeCtor ();
          currentNode->rightChild->parent = currentNode;
          currentNode                     = currentNode->rightChild;
        }
        currentNode->deepness = currentNode->parent->deepness + 1;
        t->edgeCount += 1;
        break;
      case ')' :
        currentNode = currentNode->parent;
        break;
      default  :
        str = getCmd (str, buf);
        #include "operations.h"

        currentNode->key->type     = CNST;
        currentNode->key->priority = 255; //max one
        currentNode->key->value    = atof (buf);

        memset (buf, '\0', sizeof (buf));
        break;
    }
  }
  free (buf);

  printf ("end str to tree \n\n");
  treeOptimization (t->rootNode);
  printf ("end of optimization\n\n");
  return t;
}
#undef DEF_DIFF
//-----------------------------------------------------------------------------
#define DEF_DIFF(name, val, tp, pt, funD)                                     \
{                                                                             \
  if (n->key->type == tp && n->key->value == val)                             \
  {                                                                           \
    fprintf (file, "%s", name);                                               \
    printf ("found operation %s\n", name);                                    \
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
      printf ("found value %lf\n", n->key->value);
      fprintf (file, "%lf", n->key->value);
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
  printf ("start tree to file\n");

  assert (t        != NULL);
  assert (fileName != NULL);

  FILE *file = fopen (fileName, "w");

  subTreeToFile (t->rootNode, file);

  printf ("end tree to file\n\n");
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
  node *nCopy =  nodeCtor ();
  nCopy->key  = ddataCtor ();

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
