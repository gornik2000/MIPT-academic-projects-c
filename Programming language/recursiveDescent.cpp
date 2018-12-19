#include <ctype.h>
//-----------------------------------------------------------------------------
///   getG in circle
///   G::=L'\0'
///   L::=A...\n
///   A::=S=C
///   C::=E{[<=...]E}*
///   E::=T{[+-]T}*
///   T::=D{[*/]D}*
///   D::=P{^P}*
///   P::=(E)|N|S|V|F|def
///
///   N::=double number
///   S::=string operation from base
///   V::=variable from base
///   F::=function from base
//-----------------------------------------------------------------------------
const int   MAX_COMMAND_LENGTH    = 15;
const char *TREE_STRING_FOR_INPUT = NULL;
const char  MAX_FUN_NUMBER        = 100;
const char  MAX_VAR_NUMBER        = 100;

cmd_fun *funArr[MAX_FUN_NUMBER] = {0};
char funNumber = 0;
int    curLine = 1;
#define s TREE_STRING_FOR_INPUT

tree *getTreeFromCode (const char *str);
node *getDef     (void); // def function
node *getLine    (char **varNames); // code line

node *getE       (char **varNames); // +-
node *getT       (char **varNames); // */
node *getD       (char **varNames); // ^
node *getP       (char **varNames); // () ...
node *getC       (char **varNames); // <= >= != ==
node *getA       (char **varNames); // =

node *getS       (char **varNames); // string operation
node *getN       (void); // double number

char *getName    (void); // function name
char skipSpace   (void);
char skipComment (void);
int  getVarCount (char **varNames, const char *str, int varNum);
//-----------------------------------------------------------------------------
tree *getTreeFromCode (const char *str)
{
  assert (str != NULL);

  printf ("\n # starting descent \n");
  s              = str;
  tree *t        = treeCtor ();
  char *varNames[MAX_VAR_NUMBER] = {0};

  if (*s == '\0')
  {
    printf (" # ERRPR! Empty inputted data\n");
    return t;
  }

  t->rootNode = nodeCtor ();
  t->rootNode->key = pdataCreate (PGM, 0, 0);

  int i = 0;
  while (*s == 'd' && *(s + 1) == 'e' && *(s + 2) == 'f' && *(s + 3) == ' ')
  {
    t->rootNode->child[i] = getDef ();
    i++;
  }

  int varNumber = 0;
  varNumber = getVarCount (varNames, s, varNumber);

  while (*s != '\0')
  {
    t->rootNode->child[i] = getLine (varNames);
    i++;
  }
  t->rootNode->childrenNum = i;

  printf (" # Functions count in program %d\n", funNumber);
  printf (" # Program's functions: \n");     \
  for (int i = 0; i < funNumber; i++)        \
    printf ("\t\t%s\n", funArr[i]->name);    \
  printf (" # Variable count in program %d\n", varNumber);
  printf (" # Program's variables: \n");     \
  for (int i = 0; varNames[i] != NULL; i++)        \
    printf ("\t\t%s\n", varNames[i]);        \
  printf ("\n");

  printf (" # ending descent \n");
  //nodeOptimization (t->rootNode);
  return t;
}
//-----------------------------------------------------------------------------
node *getLine    (char **varNames)
{
  assert (varNames != NULL);

  while (*s == '\t')
  {
    s++;
  }

  node *n = getA (varNames);

  skipSpace   ();
  skipComment ();

  if (*s != '\n' && *s != '\0')
  {
    printf (" # ERROR. Wrong data was found in line %d : [%c]\n", curLine, *s);
    return n;
  }
  s++;

  curLine ++;

  return n;
}
//-----------------------------------------------------------------------------
/* starts from not separator */
node *getDef  (void)
{
  char *varNames[MAX_VAR_NUMBER] = {0};
  char vNum = 0;
  s += 4;

  funArr[funNumber]       = funCtor ();
  funArr[funNumber]->name = getName ();

  node *n = nodeCtor ();
  n->key  = pdataCreate (FUN, 0, funNumber);

  if (*s != '(')
  {
    printf (" # ERROR. \"(\" was not found in line %d\n", curLine);
    return n;
  }
  s++;
  while (*s != ')')
  {
    varNames[vNum] = getName ();
    vNum++;
    if (*s != ',' && *s != ')')
    {
      printf (" # ERROR. \",\" was not found in line %d\n", curLine);
      return n;
    }
    if (*s == ',')
    {
      s++;
    }
  }
  s++;
  if (*s != ':')
  {
    printf (" # ERROR. \":\" was not found in line %d\n", curLine);
    return n;
  }
  s++;
  skipSpace   ();
  skipComment ();
  curLine++;
  if (*s != '\n' && *s != '\0')
  {
    printf (" # ERROR. Wrong data was found in line %d after \":\"\n", curLine);
    return n;
  }
  s++;

  funArr[funNumber]->argNum = vNum;
  funArr[funNumber]->varNum = getVarCount (varNames, s, vNum);

  printf ("\n # Found function \"%s\" \n"                                     \
          " # It's arguments: \n", funArr[funNumber]->name);                  \
  for (int i = 0; i < funArr[funNumber]->argNum; i++)                         \
    printf ("\t\t%s\n", varNames[i]);                                         \
  printf (" # It's variables: \n");                                           \
  for (int i = funArr[funNumber]->argNum; i < funArr[funNumber]->varNum; i++) \
    printf ("\t\t%s\n", varNames[i]);                                         \
  printf ("\n");

  while (*s == '\t')
  {
    *s ++;
  }

  for (int i = 0; *s != 'r'; i++)
  {
    n->child[i] = getLine (varNames);
  }

  funNumber ++;
  return n;
}
//-----------------------------------------------------------------------------
/* starts from not separator */
char *getName (void)
{
  int len = 0;
  while (!isSep (s[len]))
  {
    len ++;
  }

  char *buf = (char *)calloc (len + 1, sizeof (*buf));
  for (int i = 0; i < len; i++)
  {
    buf[i] = *s;
    s++;
  }

  return buf;
}
//-----------------------------------------------------------------------------
node *getA (char **varNames)
{
  assert (varNames != NULL);

  node *nLeft = getC (varNames);

  if (*s == '=')
  {
    printf ("---->GOT = <----\n");
    int op = 1; // "=" value

    s++;
    if (*s == ' ')
    {
      s++;
    }

    node *nRight = getC (varNames);
    nLeft = nodeCreate (pdataCreate (OP, 15, op),
                        nLeft,
                        nRight
                        );
  }

  return nLeft;
}
//-----------------------------------------------------------------------------
node *getC (char **varNames)
{
  assert (varNames != NULL);

  node *nLeft = getE (varNames);

  while (*s == '<' && *(s + 1) == '=' || *s == '>' && *(s + 1) == '=' ||
         *s == '=' && *(s + 1) == '=' || *s == '!' && *(s + 1) == '=')
  {
    printf ("---->GOT %c%c <----\n", *s, *(s + 1));

    int op = 0;
    if (*s == '<' && *(s + 1) == '=' ) op = 14;
    if (*s == '>' && *(s + 1) == '=' ) op = 15;
    if (*s == '=' && *(s + 1) == '=' ) op = 13;
    if (*s == '!' && *(s + 1) == '=' ) op = 32;

    s += 2;
    if (*s == ' ')
    {
      s++;
    }

    node *nRight = getE (varNames);
    nLeft = nodeCreate (pdataCreate (OP, 15, op),
                        nLeft,
                        nRight
                        );
  }

  while (*s == '<' || *s == '>')
  {
    printf ("---->GOT %c <----\n", *s);

    int op = 0;
    if (*s == '<') op = 11;
    if (*s == '>') op = 12;

    s ++;
    if (*s == ' ')
    {
      s++;
    }

    node *nRight = getP (varNames);
    nLeft = nodeCreate (pdataCreate (OP, 15, op),
                        nLeft,
                        nRight
                        );
  }

  return nLeft;
}
//-----------------------------------------------------------------------------
node *getE (char **varNames)
{
  assert (varNames != NULL);

  node *nLeft = getT (varNames);

  while (*s == '+' || *s == '-')
  {
    printf ("---->GOT %c <----\n", *s);

    int op = 0;
    if (*s == '+') op = 30;
    if (*s == '-') op = 31;

    s++;
    if (*s == ' ')
    {
      s++;
    }

    node *nRight = getT (varNames);
    nLeft = nodeCreate (pdataCreate (OP, 30, op),
                        nLeft,
                        nRight
                        );
  }

  return nLeft;
}
//-----------------------------------------------------------------------------
node *getT (char **varNames)
{
  assert (varNames != NULL);

  node *nLeft = getD (varNames);

  while (*s == '*' || *s == '/')
  {
    printf ("---->GOT %c <----\n", *s);

    int op = 0;
    if (*s == '*') op = 34;
    if (*s == '/') op = 35;

    s++;
    if (*s == ' ')
    {
      s++;
    }

    node *nRight = getD (varNames);
    nLeft = nodeCreate (pdataCreate (OP, 20, op),
                        nLeft,
                        nRight
                        );
  }

  return nLeft;
}
//-----------------------------------------------------------------------------
node *getD (char **varNames)
{
  assert (varNames != NULL);

  node *nLeft = getP (varNames);

  while (*s == '^')
  {
    printf ("---->GOT ^ <----\n");

    int op = 33;

    s++;
    if (*s == ' ')
    {
      s++;
    }

    node *nRight = getP (varNames);
    nLeft = nodeCreate (pdataCreate (OP, 15, op),
                        nLeft,
                        nRight
                        );
  }

  return nLeft;
}
//-----------------------------------------------------------------------------
node *getP (char **varNames)
{
  assert (varNames != NULL);

  if (*s == '(')
  {
    s++;
    node *n = getE (varNames);
    if (*s != ')')
    {
      printf (" ERROR! ) was not found\n");
      return n;
    }
    s++;
    return n;
  }
  if (isdigit (*(s)))
  {
    /* think about it */
    return getN ();
  }
  //if (isalpha (*(s)))
  //{
    /* think about it */
    return getS (varNames);
  //}

  //s++;
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

  if (*s == ' ')
  {
    s++;
  }
  printf ("---->GOT %lf <----\n", atof(number));

  return nodeCreate (pdataCreate (CNST, 255, atof (number)),
                     NULL,
                     NULL
                    );
}
//-----------------------------------------------------------------------------
#define DEF_DIFF(name, val, tp, pt, arg, funD)                                \
{                                                                             \
  if (strcmp (name, buf) == 0)                                                \
  {                                                                           \
    switch (arg)                                                              \
    {                                                                         \
      case 1:                                                                 \
        return nodeCreate (pdataCreate (tp, pt, val),                         \
                           NULL,                                              \
                           NULL                                               \
                          );                                                  \
      case 2:                                                                 \
        return nodeCreate (pdataCreate (tp, pt, val),                         \
                           getP (varNames),                                   \
                           NULL                                               \
                          );                                                  \
      case 3:                                                                 \
        return nodeCreate (pdataCreate (tp, pt, val),                         \
                           getP (varNames),                                   \
                           NULL                                               \
                          );                                                  \
      case 4:                                                                 \
        return nodeCreate (pdataCreate (tp, pt, val),                         \
                           NULL,                                              \
                           NULL                                               \
                          );                                                  \
      case 5:                                                                 \
        return nodeCreate (pdataCreate (tp, pt, val),                         \
                           NULL,                                              \
                           NULL                                               \
                          );                                                  \
      default:                                                                \
        printf (" # ERROR. Arg for constant was not inputted");               \
    }                                                                         \
  }                                                                           \
}
node *getS (char **varNames)
{
  assert (varNames != NULL);

  //char buf[MAX_COMMAND_LENGTH] = {0};
  int i = 0;

  char *buf = getName ();
  printf ("---->GOT %s <----\n", buf);
  if (*s == ' ')
  {
    s++;
  }
  //printf (" ended with %c -\n", *s);
  // op check
  #include "operations.h"
  // function check
  for (int i = 0; i < funNumber + 1; i++)
  {
    if (strcmp (funArr[i]->name, buf) == 0)
    {
      printf (" fun found %s\n", buf);
      /* different number of vars */
      return nodeCreate (pdataCreate(FUN, 0, i),
                         NULL,
                         NULL);
    }
  }
  // var check
  for (int i = 0; varNames[i] != NULL; i++)
  {
    if (strcmp (varNames[i], buf) == 0)
    {
      printf (" var found %s\n", buf);
      return nodeCreate (pdataCreate(VAR, 0, i),
                         NULL,
                         NULL);

    }
  }

  printf (" # Command \"%s\" has not been read\n", buf);

  return NULL;
}
#undef DEF_DIFF
//-----------------------------------------------------------------------------
/* starts space */
char skipSpace   (void)
{
  while (*s == ' ')
  {
    s++;
  }
  return 0;
}

/* starts from comment, ends with \n or \0 */
char skipComment (void)
{
  if (*s == '#')
  {
    while (*s != '\n' && *s != '\0')
    {
      s++;
    }
  }
  return 0;
}
//-----------------------------------------------------------------------------
#define DEF_DIFF(name, val, tp, pt, arg, fun)                                 \
{                                                                             \
  if (strcmp(command, name) == 0)                                             \
  {                                                                           \
    goto getVarCount_skip_iteration;                                          \
  }                                                                           \
}
int getVarCount (char **varNames, const char *str, int varNum)
{
  assert (varNames != NULL);
  assert (str      != NULL);

  printf (" GET COUNT STARTED\n");

  for (;*str == '\t'; str++);
  //char *command = getName ();
  char *command = (char *)calloc (MAX_COMMAND_LENGTH, sizeof (*command));
  for (int i = 0; !isSep (*str); i++)
  {
    command[i] = *str;
    str++;
  }//*/

  while (strcmp (command, "return") != 0)
  {
    // op check
    #include "operations.h"
    // function check
    for (int i = 0; i < funNumber; i++)
    {
      if (strcmp (funArr[i]->name, command) == 0)
      {
        goto getVarCount_skip_iteration;
      }
    }
    // var check
    for (int i = 0; i < varNum; i++)
    {
      if (strcmp (varNames[i], command) == 0)
      {
        goto getVarCount_skip_iteration;
      }
    }

    varNames[varNum] = command;
    varNum++;
    command = (char *)calloc (MAX_COMMAND_LENGTH, sizeof (*command));

    getVarCount_skip_iteration:

    while (*str != '\n')
    {
      str++;
    }
    str++;

    for (;*str == '\t'; str++);
    //memset (command, 0, MAX_COMMAND_LENGTH);
    //command = getName ();

    for (int i = 0; !isSep (*str); i++)
    {
      command[i] = *str;
      str++;
    }
  }

  //for (int i = 0; i < varNum; i++)
    //printf (" )))) var %s\n", varNames[i]);

  return varNum;
}
#undef DEF_DIFF

#undef s


