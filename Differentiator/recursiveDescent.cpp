#include <ctype.h>
//-----------------------------------------------------------------------------
const int   MAX_COMMAND_LENGTH     = 8;
const char *TREE_STRING_FOR_INPUT = NULL;
#define s TREE_STRING_FOR_INPUT

tree *getTree (const char *str);
node *getE    (void);
node *getT    (void);
node *getD    (void);
node *getP    (void);
node *getN    (void);
node *getS    (void);
//-----------------------------------------------------------------------------
tree *getTree (const char *str)
{
  s = str;
  tree *t = treeCtor ();

  t->rootNode = getE ();
  if (*s != '\0')
  {
    printf (" # ERROR! Incorrect data, 0 was not found in the end ");
    return t;
  }
  assert (s != str);

  nodeOptimization (t->rootNode);
  return t;
}
//-----------------------------------------------------------------------------
node *getE (void)
{
  node *nLeft = getT ();

  while (*s == '+' || *s == '-')
  {
    int op = *s;
    s++;

    node *nRight = getT ();
    if (op == '+')
    {
      nLeft = nodeCreate (nLeft,
                          nRight,
                          ddataCreate (OP, 30, '+')
                          );
    }
    else
    {
      nLeft = nodeCreate (nLeft,
                          nRight,
                          ddataCreate (OP, 30, '-')
                          );
    }
  }

  return nLeft;
}
//-----------------------------------------------------------------------------
node *getT (void)
{
  node *nLeft = getD ();

  while (*s == '*' || *s == '/')
  {
    int op = *s;
    s++;

    node *nRight = getD ();
    if (op == '*')
    {
      nLeft = nodeCreate (nLeft,
                          nRight,
                          ddataCreate (OP, 20, '*')
                          );
    }
    else
    {
      nLeft = nodeCreate (nLeft,
                          nRight,
                          ddataCreate (OP, 20, '/')
                          );
    }
  }

  return nLeft;
}
//-----------------------------------------------------------------------------
node *getD (void)
{
  node *nLeft = getP ();

  while (*s == '^')
  {
    s++;

    node *nRight = getP ();
    nLeft = nodeCreate (nLeft,
                        nRight,
                        ddataCreate (OP, 15, '^')
                        );
  }

  return nLeft;
}
//-----------------------------------------------------------------------------
node *getP (void)
{
  if (*s == '(')
  {
    s++;
    node *n = getE ();
    assert (*s == ')');
    s++;
    return n;
  }
  if (isdigit (*(s)))
  {
    /* think about it */
    return getN ();
  }
  if (isalpha (*(s)))
  {
    /* think about it */
    return getS ();
  }

  printf (" # ERROR. Got not digit and not alpha\n");
}
//-----------------------------------------------------------------------------
node *getN (void)
{
  char number[MAX_COMMAND_LENGTH] = {0};
  int i = 0;

  while (isdigit (*s) || *s == '.')
  {
    number[i] = *s;
    s++;
    i++;
  }

  return nodeCreate (NULL,
                     NULL,
                     ddataCreate (CNST, 255, atof (number))
                    );
}
//-----------------------------------------------------------------------------
#define DEF_DIFF(name, val, tp, pt, arg, funD)                                \
{                                                                             \
  if (strcmp (name, buf) == 0)                                                \
  {                                                                           \
    switch (arg)                                                              \
    {                                                                         \
      case 0:                                                                 \
        return nodeCreate (NULL,                                              \
                           NULL,                                              \
                           ddataCreate (tp, pt, val)                          \
                          );                                                  \
      case 1:                                                                 \
        return nodeCreate (getP (),                                           \
                           NULL,                                              \
                           ddataCreate (tp, pt, val)                          \
                          );                                                  \
      default:                                                                \
        printf (" # ERROR. Arg for constant was not inputted");               \
    }                                                                         \
  }                                                                           \
}
node *getS (void)
{
  char buf[MAX_COMMAND_LENGTH] = {0};
  int i = 0;

  while (i < MAX_COMMAND_LENGTH)
  {
    buf[i] = *s;
    s++;
    i++;
    #include "operations.h"
  }

  printf (" # Command has not been readen\n");
  return NULL;
}
#undef DEF_DIFF
//-----------------------------------------------------------------------------
