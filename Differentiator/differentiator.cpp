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
char differentiator        (const char* fileInName,
                            const char *fileOutName, int diffNumber);

char *getCmd               (char *str, char *cmd);
tree *createTreeFromStr    (char *str);
int   isNodesNotSame       (node *n1, node *n2);
node *nodeCreateDerivative (node *n);

char  treeOptimization     (node *n);
char  nodeOptimization     (node *n);

char  treeToFile           (tree *t, const char *fileName);
char  subTreeToFile        (node *n, FILE *file);

char  treeRepair           (tree *t);
int   subTreeRepair        (node *n);

node *nodeCreateCopy       (node *n);
node *nodeCreate           (node *leftChild, node *rightChild, d_data *key);
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
  dT->rootNode = nodeCreateCopy (t->rootNode);
  //treeOptimization (dT->rootNode);
  for (int i = 0; i < diffNumber; i++)
  {
    dT->rootNode = nodeCreateDerivative (dT->rootNode);
    treeOptimization (dT->rootNode);
  }

  treeRepair (dT);
  //printf ("edges %d \n", dT->edgeCount);
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
    node *dN = 0;                                                             \
    funD                                                                      \
    return dN;                                                                \
  }                                                                           \
}
/* derivative of n */
node *nodeCreateDerivative (node *n)
{
  //node *dN = nodeCtor ();
  #include "operations.h"

  node *dN = nodeCtor ();
  dN->key = ddataCreate (CNST, 255, 0.0);
  return dN;
}
#undef DEF_DIFF
//-----------------------------------------------------------------------------
char treeOptimization (node *n)
{
  //printf ("t");
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
  //printf ("n");
  if (n->key->type != OP)
  {
    return 0;
  }
  /* if both exist and right = 0 */
  if (n-> leftChild != NULL && n->rightChild != NULL &&
      n->rightChild->key->value == 0)
  {
    switch ((char) n->key->value)
    {
    case '*' :
      n->key = ddataCreate (CNST, 255, 0);
      nodeKillChildren (n);
      return 0;
    case '^' :
      n->key = ddataCreate (CNST, 255, 1);
      nodeKillChildren (n);
      return 0;
    case '+' :
      nodeDtor (n->rightChild);
      n->rightChild = n->leftChild->rightChild;
      n->key        = n->leftChild->key;
      n->leftChild  = n->leftChild->leftChild;
      return 0;
    case '-' :
      nodeDtor (n->rightChild);
      n->rightChild = n->leftChild->rightChild;
      n->key        = n->leftChild->key;
      n->leftChild  = n->leftChild->leftChild;
      return 0;
    }
  }
  /* if both exist and left = 0 */
  if (n-> leftChild != NULL && n->rightChild != NULL &&
      n-> leftChild->key->value == 0)
  {
    switch ((char) n->key->value)
    {
    case '*' :
      n->key = ddataCreate (CNST, 255, 0);
      nodeKillChildren (n);
      return 0;
    case '/' :
      n->key = ddataCreate (CNST, 255, 0);
      nodeKillChildren (n);
      return 0;
    case '^' :
      n->key = ddataCreate (CNST, 255, 0);
      nodeKillChildren (n);
      return 0;
    case '+' :
      nodeDtor (n->leftChild);
      n->leftChild  = n->rightChild->leftChild;
      n->key        = n->rightChild->key;
      n->rightChild = n->rightChild->rightChild;
      return 0;
    }
  }
  /* if both exist and right = 1 */
  if (n-> leftChild != NULL && n->rightChild != NULL &&
      n->rightChild->key->value == 1)
  {
    switch ((char) n->key->value)
    {
    case '*' :
      nodeDtor (n->rightChild);
      n->rightChild = n->leftChild->rightChild;
      n->key        = n->leftChild->key;
      n->leftChild  = n->leftChild->leftChild;
      return 0;
    case '^' :
      nodeDtor (n->rightChild);
      n->rightChild = n->leftChild->rightChild;
      n->key        = n->leftChild->key;
      n->leftChild  = n->leftChild->leftChild;
      return 0;
    case '/' :
      nodeDtor (n->rightChild);
      n->rightChild = n->leftChild->rightChild;
      n->key        = n->leftChild->key;
      n->leftChild  = n->leftChild->leftChild;
      return 0;
    }
  }
  /* if both exist and left = 1 */
  if (n-> leftChild != NULL && n->rightChild != NULL &&
      n-> leftChild->key->value == 1)
  {
    switch ((char) n->key->value)
    {
    case '*' :
      nodeDtor (n->leftChild);
      n->leftChild  = n->rightChild->leftChild;
      n->key        = n->rightChild->key;
      n->rightChild = n->rightChild->rightChild;
      return 0;
    case '^' :
      n->key = ddataDtor   (n->key);
      n->key = ddataCreate (CNST, 255, 1);
      nodeKillChildren (n);
      return 0;
    }
  }
  /* both exist and const */
  if (n-> leftChild            != NULL && n->rightChild            != NULL &&
      n-> leftChild->key->type == CNST && n->rightChild->key->type == CNST)
  {
    switch ((char) n->key->value)
    {
    case '+' :
      n->key = ddataCreate (CNST, 255, n->leftChild->key->value + n->rightChild->key->value);
      nodeKillChildren (n);
      return 0;
    case '-' :
      n->key = ddataCreate (CNST, 255, n->leftChild->key->value - n->rightChild->key->value);
      nodeKillChildren (n);
      return 0;
    case '*' :
      n->key = ddataCreate (CNST, 255, n->leftChild->key->value * n->rightChild->key->value);
      nodeKillChildren (n);
      return 0;
    case '/' :
      n->key = ddataCreate (CNST, 255, n->leftChild->key->value / n->rightChild->key->value);
      nodeKillChildren (n);
      return 0;
    case '^' :
      n->key = ddataCreate (CNST, 255, pow (n->leftChild->key->value, n->rightChild->key->value));
      nodeKillChildren (n);
      return 0;
    }
    return 0;
  }
  /* if left and right exist and not const and same */
  if (n-> leftChild != NULL && n->rightChild != NULL &&
      isNodesNotSame (n->leftChild, n->rightChild) == 0)
  {
    switch ((char) n->key->value)
    {
    case '-' :
      n->key = ddataDtor (n->key);
      n->key = ddataCreate (CNST, 255, 0);
      nodeKillChildren (n);
      return 0;
    case '/' :
      n->key = ddataDtor (n->key);
      n->key = ddataCreate (CNST, 255, 1);
      nodeKillChildren (n);
      return 0;
    case '+' :
      n->leftChild = nodeDtor   (n->leftChild);
      n->leftChild = nodeCreate (NULL, NULL, ddataCreate (CNST, 255, 2));
      n->key = ddataDtor (n->key);
      n->key = ddataCreate (OP, 20, '*');
      return 0;
    case '*' :
      n->rightChild = nodeDtor   (n->rightChild);
      n->rightChild = nodeCreate (NULL, NULL, ddataCreate (CNST, 255, 2));
      n->key = ddataDtor (n->key);
      n->key = ddataCreate (OP, 20, '^');
      return 0;
    }
  }
  /* if left exist and const and right do not exist */
  if (n->leftChild != NULL && n->leftChild->key->type == CNST)
  {
    switch ((char) n->key->value)
    {
    case 's' :
      n->key = ddataDtor   (n->key);
      n->key = ddataCreate (CNST, 255, sin (n->leftChild->key->value));
      nodeKillChildren (n);
      return 0;
    case  'c' :
      n->key = ddataDtor   (n->key);
      n->key = ddataCreate (CNST, 255, cos (n->leftChild->key->value));
      nodeKillChildren (n);
      return 0;
    case 't' :
      n->key = ddataDtor   (n->key);
      n->key = ddataCreate (CNST, 255, tan (n->leftChild->key->value));
      nodeKillChildren (n);
      return 0;
    case  '1' :
      n->key = ddataDtor   (n->key);
      n->key = ddataCreate (CNST, 255, 1 / tan (n->leftChild->key->value));
      nodeKillChildren (n);
      return 0;
    case '2' :
      n->key = ddataDtor   (n->key);
      n->key = ddataCreate (CNST, 255, asin (n->leftChild->key->value));
      nodeKillChildren (n);
      return 0;
    case '3' :
      n->key = ddataDtor   (n->key);
      n->key = ddataCreate (CNST, 255, acos (n->leftChild->key->value));
      nodeKillChildren (n);
      return 0;
    case '4' :
      n->key = ddataDtor   (n->key);
      n->key = ddataCreate (CNST, 255, atan (n->leftChild->key->value));
      nodeKillChildren (n);
      return 0;
    case '5' :
      n->key = ddataDtor   (n->key);
      n->key = ddataCreate (CNST, 255, PI / 2.0 - (n->leftChild->key->value));
      nodeKillChildren (n);
      return 0;
    }
  }
}
//-----------------------------------------------------------------------------
#define DEF_DIFF(name, val, tp, pt, funD)                                     \
{                                                                             \
  if (strcmp (name, buf) == 0)                                                \
  {                                                                           \
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
  //printf ("start str to tree \n");
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

  //printf ("end str to tree \n\n");
  treeOptimization (t->rootNode);
  //printf ("end of optimization\n\n");
  return t;
}
#undef DEF_DIFF
//-----------------------------------------------------------------------------
#define DEF_DIFF(name, val, tp, pt, funD)                                     \
{                                                                             \
  if (n->key->type == tp && n->key->value == val)                             \
  {                                                                           \
    fprintf (file, "%s", name);                                               \
    /*printf ("found operation %s\n", name);*/                                \
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
  //printf ("start tree to file\n");

  assert (t        != NULL);
  assert (fileName != NULL);

  FILE *file = fopen (fileName, "w");

  subTreeToFile (t->rootNode, file);

  //printf ("end tree to file\n\n");

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
//-----------------------------------------------------------------------------
node *nodeCreate (node *leftChild, node *rightChild, d_data *key)
{
  node *n = nodeCtor ();

  n->leftChild  = leftChild;
  n->rightChild = rightChild;
  n->key        = key;

  return n;
}
//-----------------------------------------------------------------------------
int isNodesNotSame (node *n1, node *n2)
{
  if (n1->key->type     == n2->key->type &&
      n1->key->priority == n2->key->priority &&
      n1->key->value    == n2->key->value)
  {
    if (n1->leftChild  != NULL && n2->leftChild  != NULL &&
        n1->rightChild != NULL && n2->rightChild != NULL)
    {
      return isNodesNotSame (n1->leftChild, n2->leftChild) +
             isNodesNotSame (n1->rightChild, n2->rightChild);
    }
    if (n1->leftChild  != NULL && n2->leftChild  != NULL)
    {
      return isNodesNotSame (n1->leftChild, n2->leftChild);
    }
    if (n1->rightChild != NULL && n2->rightChild != NULL)
    {
      return isNodesNotSame (n1->rightChild, n2->rightChild);
    }
    if (n1->leftChild  == NULL && n2->leftChild  == NULL &&
        n1->rightChild == NULL && n2->rightChild == NULL)
    {
      return 0;
    }
  }

  return 1;
}

char  treeRepair           (tree *t)
{
  assert (t != NULL);

  t->rootNode->parent   = NULL;
  t->rootNode->deepness = 0;

  t->edgeCount = subTreeRepair (t->rootNode);

  return 0;
}

int  subTreeRepair        (node *n)
{
  assert (n != NULL);

  if (n->leftChild != NULL && n->rightChild != NULL)
  {
    n->leftChild->parent   = n;
    n->leftChild->deepness = n->deepness + 1;
    n->rightChild->parent   = n;
    n->rightChild->deepness = n->deepness + 1;
    return subTreeRepair (n->rightChild) +
           subTreeRepair (n->leftChild) + 2;
  }
  if (n->leftChild != NULL)
  {
    n->leftChild->parent   = n;
    n->leftChild->deepness = n->deepness + 1;
    return subTreeRepair (n->leftChild) + 1;
  }
  if (n->rightChild != NULL)
  {
    n->rightChild->parent   = n;
    n->rightChild->deepness = n->deepness + 1;
    return subTreeRepair (n->rightChild) + 1;
  }

  return 0;
}
