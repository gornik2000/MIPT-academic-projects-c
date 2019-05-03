//---------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
//---------------------------------------------------------------------------*/
const char MAX_CHILDREN_NUMBER = 30;
//---------------------------------------------------------------------------*/
typedef tree_data node_data;

struct my_node
{
  node_data      *key;

  struct my_node *parent;
  struct my_node **child;

  int childrenNum;
  int deepness;
};

typedef struct my_node node;
//---------------------------------------------------------------------------*/
node *nodeCtor (void);
node *nodeDtor (node *n);
//---------------------------------------------------------------------------*/
node *nodeCtor (void)
{
  node *n       = (node *)calloc (1, sizeof (*n));

  n->key        = (node_data *)calloc (1, sizeof (*(n->key)));
  n->child      = (node **)calloc (MAX_CHILDREN_NUMBER, sizeof (*(n->child)));
  n->parent     = NULL;

  n->childrenNum = 0;
  n->deepness    = 0;
  return n;
}
//---------------------------------------------------------------------------*/
node *nodeDtor (node *n)
{
  assert (n != NULL);

  free        (n->key);
  n->key        = NULL;

  free (n->child);
  n->child      = NULL;

  n->parent     = NULL;

  n->childrenNum = 0;
  n->deepness    = 0;

  free (n);
  n = NULL;

  return n;
}
//---------------------------------------------------------------------------*/
struct my_tree
{
  node *rootNode;
  int  edgeCount;
};
typedef struct my_tree tree;
//---------------------------------------------------------------------------*/
tree *treeCtor (void);

tree *treeDtor    (tree *t);
node *treeSubDtor (node *n);

//---------------------------------------------------------------------------*/
tree *treeCtor (void)
{
  tree *t = (tree *)calloc (1, sizeof (*t));

  t->edgeCount = 1;
  t->rootNode  = nodeCtor();
}
//---------------------------------------------------------------------------*/
node *treeSubDtor  (node *n)
{
  for (int i = 0; i < n->childrenNum; i++)
  {
    n->child[i] = treeSubDtor (n->child[i]);
  }

  n->childrenNum = 0;
  free (n);
  n = NULL;

  return n;
}
//---------------------------------------------------------------------------*/
tree *treeDtor (tree *t)
{
  assert (t != NULL);

  t->edgeCount = -1;
  t->rootNode = treeSubDtor (t->rootNode);

  free (t);
  t = NULL;

  return t;
}
//---------------------------------------------------------------------------*/
//               © Gorbachev Nikita, December 2018 + April 2019              //
//---------------------------------------------------------------------------*/
