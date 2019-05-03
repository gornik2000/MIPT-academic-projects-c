//---------------------------------------------------------------------------*/
#include "function.h"
//---------------------------------------------------------------------------*/
#define DEF_TYPE(name, num) name = num,
enum node_types
{
  #include "types.h"
};
#undef DEF_TYPE
//---------------------------------------------------------------------------*/
/* type of pgm_data value */
typedef int p_data_value;

struct prm_data
{
  char         type;
  p_data_value value;
};
typedef struct prm_data p_data;
typedef p_data tree_data;

#include "tree.cpp"

p_data *pdataCtor   (void);
p_data *pdataDtor   (p_data *d);
p_data *pdataCreate (char type, char priority, p_data_value value);

char treeToFile    (tree *t, const char *fileName);
char subTreeToFile (node *n, FILE *file, int deepness);

//---------------------------------------------------------------------------*/
p_data *pdataCtor (void)
{
  p_data *d = (p_data *)calloc (1, sizeof (*d));

  d->type  = 0;
  d->value = 0;

  return d;
}
//---------------------------------------------------------------------------*/
p_data *pdataDtor (p_data *d)
{
  assert (d != NULL);

  d->type  = 0;
  d->value = 0;

  free (d);
  d = NULL;

  return d;
}
//---------------------------------------------------------------------------*/
p_data *pdataCreate (char type, p_data_value value)
{
  p_data *d = pdataCtor ();

  d->type  = type;
  d->value = value;

  return d;
}
//---------------------------------------------------------------------------*/
char treeToFile (tree *t, const char *fileName)
{
  assert (t        != NULL);
  assert (fileName != NULL);

  FILE *file = fopen (fileName, "w");

  printf (" TREE TO FILE START\n");
  subTreeToFile (t->rootNode, file, 0);
}
//---------------------------------------------------------------------------*/
#define DEF_TYPE(name, num)                                                   \
{                                                                             \
  if (n->key->type == num)                                                    \
  {                                                                           \
    fprintf (file, "%s ", #name);                                             \
  }                                                                           \
}
#define DEF_OP(name, num, tp, arg, funD)                                      \
{                                                                             \
  if (n->key->type == tp && n->key->value == num)                             \
  {                                                                           \
    fprintf (file, "%s ", name);                                              \
  }                                                                           \
}
char subTreeToFile (node *n, FILE *file, int deepness)
{
  assert (n    != NULL);
  assert (file != NULL);

  if (n->key != NULL)
  {
    tabFprint (file, deepness);
    #include "types.h"
    #include "operations.h"
    fprintf   (file, "%d\n", n->key->value);
  }

  for (int i = 0; n->child[i] != NULL; i++)
  {
    //printf ("child %d\n", i);

    subTreeToFile (n->child[i], file, deepness + 1);
  }
}
#undef DEF_OP
#undef DEF_TYPE
//---------------------------------------------------------------------------*/
//               © Gorbachev Nikita, December 2018 + April 2019              //
//---------------------------------------------------------------------------*/
