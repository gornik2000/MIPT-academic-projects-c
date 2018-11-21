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
    printf (" 0 ");
    return n;
  }
  if (strcmp (*n->key, data) == 0)
  {
    printf (" 1 ");
    return n;
  }
  return max (treeSubFind (n->leftChild,  data), treeSubFind (n->rightChild, data));
}
