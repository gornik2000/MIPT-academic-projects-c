#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//-----------------------------------------------------------------------------

typedef tree_data node_data;

struct my_node
{
  node_data      *key;

  struct my_node *leftChild;
  struct my_node *parent;
  struct my_node *rightChild;

  int deepness;
};

typedef struct my_node node;

node *nodeCtor (void);
node *nodeDtor (node *n);

//-----------------------------------------------------------------------------

node *nodeCtor (void)
{
  node *n       = (node *)calloc (1, sizeof (*n));

  n->key   = (node_data *)calloc (1, sizeof (*(n->key)));
  n->leftChild  = NULL;
  n->parent     = NULL;
  n->rightChild = NULL;

  n->deepness   = 0;
  return n;
}

node *nodeDtor (node *n)
{
  assert (n != NULL);

  free        (n->key);
  n->key        = NULL;

  n->leftChild  = NULL;
  n->parent     = NULL;
  n->rightChild = NULL;

  n->deepness   = 0;

  free (n);
  n = NULL;

  return n;
}

//-----------------------------------------------------------------------------

struct my_tree
{
  node *rootNode;
  int  edgeCount;
};
typedef struct my_tree tree;

tree *treeCtor (void);

tree *treeDtor    (tree *t);
node *treeSubDtor (node *n);

node *treeFind    (tree *t, node_data data);
node *treeSubFind (node *n, node_data data);

char *nodePrintToRoot   (node *n);
char *nodePrintLeafs    (node *n);
char *nodePrintNonLeafs (node *n);
char *nodesPrintHistory (node *n1, node *n2);

//char treeAddData (tree *t, node_data data);

//-----------------------------------------------------------------------------

tree *treeCtor (void)
{
  tree *t = (tree *)calloc (1, sizeof (*t));

  t->edgeCount = 0;
  t->rootNode  = nodeCtor();
}

node *treeSubDtor  (node *n)
{
  if (n->leftChild == NULL && n->rightChild == NULL)
  {
    free (n);
    n = NULL;
    return n;
  }
  else if (n->leftChild  != NULL)
  {
    n->leftChild  = treeSubDtor (n->leftChild);
    free (n);
    n = NULL;
    return n;
  }
  else if (n->rightChild != NULL)
  {
    n->rightChild = treeSubDtor (n->rightChild);
    free (n);
    n = NULL;
    return n;
  }
  else
  {
    n->leftChild  = treeSubDtor (n->leftChild);
    n->rightChild = treeSubDtor (n->rightChild);
    free (n);
    n = NULL;
    return n;
  }

  n = nodeDtor (n);
}

tree *treeDtor (tree *t)
{
  assert (t != NULL);

  t->edgeCount = -1;
  t->rootNode = treeSubDtor (t->rootNode);

  free (t);
  t = NULL;

  return t;
}

node *treeFind    (tree *t, node_data data)
{
  assert (t != NULL);

  return treeSubFind (t->rootNode, data);
}

node *treeSubFind (node *n, node_data data)
{
  //printf ("1");
  if (n == NULL)
  {
    return n;
  }
  if (strcmp (*n->key, data) == 0)
  {
    return n;
  }
  return max (treeSubFind (n->leftChild,  data), treeSubFind (n->rightChild, data));
}

char *nodePrintToRoot   (node *n)
{
  if (n == NULL)
  {
    printf (" # ERROR! Incorrect input");
    return 0;
  }

  node_data *data = n->key;
  while (n->parent != NULL)
  {
    if (n == n->parent->leftChild)
      printf (" - %s %s\n", *data, *n->parent->key);
    if (n == n->parent->rightChild)
      printf (" - not %s %s\n", *data, *n->parent->key);
    n = n->parent;
  }
  return 0;
}

char *nodePrintLeafs    (node *n)
{
  if (n != NULL)
  {
    if (n->leftChild  != NULL)
      nodePrintLeafs (n->leftChild);
    if (n->rightChild != NULL)
      nodePrintLeafs (n->rightChild);
    if (n->leftChild == NULL && n->rightChild == NULL)
      printf ("- %s\n", *n->key);
  }
  return 0;
}

char *nodePrintNonLeafs (node *n)
{
  if (n != NULL)
  {
    if (n->leftChild  != NULL)
      nodePrintNonLeafs (n->leftChild);
    if (n->rightChild != NULL)
      nodePrintNonLeafs (n->rightChild);
    if (n->leftChild != NULL && n->rightChild != NULL)
      printf ("- %s\n", *n->key);
  }
  return 0;
}

char *nodesPrintHistory (node *n1, node *n2)
{
  ;
}
