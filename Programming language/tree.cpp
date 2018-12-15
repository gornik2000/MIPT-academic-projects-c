//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
//-----------------------------------------------------------------------------
const char MAX_CHILDREN_NUMBER = 255;
typedef tree_data node_data;

struct my_node
{
  node_data      *key;

  struct my_node *leftChild;
  struct my_node *parent;
  struct my_node *rightChild;
  struct my_node **child;

  int children_num;
  int deepness;
};

typedef struct my_node node;

node *nodeCtor (void);
node *nodeDtor (node *n);
//-----------------------------------------------------------------------------
node *nodeCtor (void)
{
  node *n       = (node *)calloc (1, sizeof (*n));

  n->key        = (node_data *)calloc (1, sizeof (*(n->key)));
  n->child      = (node **)calloc (MAX_CHILDREN_NUMBER, sizeof (*(n->child)));
  n->parent     = NULL;
  n->rightChild = NULL;
  n->leftChild  = NULL;

  n->children_num = 0;
  n->deepness     = 0;
  return n;
}
//-----------------------------------------------------------------------------
node *nodeDtor (node *n)
{
  assert (n != NULL);

  free        (n->key);
  n->key        = NULL;
  free (n->child);
  n->child      = NULL;

  n->leftChild  = NULL;
  n->parent     = NULL;
  n->rightChild = NULL;


  n->children_num = 0;
  n->deepness     = 0;

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
//-----------------------------------------------------------------------------
tree *treeCtor          (void);

tree *treeDtor          (tree *t);
node *treeSubDtor       (node *n);

char  nodeKillChildren  (node *n);
//-----------------------------------------------------------------------------
tree *treeCtor (void)
{
  tree *t = (tree *)calloc (1, sizeof (*t));

  t->edgeCount = 1;
  t->rootNode  = nodeCtor();
}
//-----------------------------------------------------------------------------
node *treeSubDtor  (node *n)
{
  /*
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
  }*/

  for (int i = 0; i < n->children_num; i++)
  {
    n->child[i] = treeSubDtor (n->child[i]);
  }

  n->children_num = 0;
  free (n);
  n = NULL;

  //n = nodeDtor (n);
  return n;
}
//-----------------------------------------------------------------------------
tree *treeDtor (tree *t)
{
  assert (t != NULL);

  t->edgeCount = -1;
  t->rootNode = treeSubDtor (t->rootNode);

  free (t);
  t = NULL;

  return t;
}
//-----------------------------------------------------------------------------
char nodeKillChildren (node *n)
{
  if (n->leftChild != NULL)
  {
    nodeDtor (n-> leftChild);
    n->leftChild  = NULL;
  }
  if (n->rightChild != NULL)
  {
    nodeDtor (n->rightChild);
    n->rightChild = NULL;
  }
  return 0;
}
//-----------------------------------------------------------------------------
