//---------------------------------------------------------------------------*/
#include <ctype.h>
//---------------------------------------------------------------------------*/
/*    getG in circle        |
      G::=L'\0'             |
      L::=A...\n            |
      A::=S=C               |
      C::=E{[<=...]E}*      |
      E::=T{[+-]T}*         |
      T::=D{[/*]D}*         |
      D::=P{^P}*            |
      P::=(E)|N|S|V|F|def   |

      N::=double number
      S::=string operation from base
      V::=variable from base
      F::=function from base
//---------------------------------------------------------------------------*/
const int   MAX_COMMAND_LENGTH    = 30;
const char *TREE_STRING_FOR_INPUT = NULL;
const char  MAX_FUN_NUMBER        = 100;
const char  MAX_VAR_NUMBER        = 10;
//---------------------------------------------------------------------------*/
cmd_fun *funArr[MAX_FUN_NUMBER] = {0};

char funNumber = 0;
int    curLine = 1;
int    curTabs = 0;
//---------------------------------------------------------------------------*/
#define s TREE_STRING_FOR_INPUT
//---------------------------------------------------------------------------*/
tree *getTreeFromCode (const char *str);
node *getDef     (void);            // def function
node *getLine    (char **varNames); // code line

node *getA       (char **varNames); // =
node *getC       (char **varNames); // <= >= != ==
node *getE       (char **varNames); // +-
node *getT       (char **varNames); // */%
node *getD       (char **varNames); // ^
node *getP       (char **varNames); // () ...

node *getS       (char **varNames); // string operation
node *getN       (void);            // double number

char getTab      (void);

char checkTabs   (void);
char *getName    (void);            // function name

char skipSpace   (void);
char skipComment (void);

char isSubStringIn (const char *str1, const char *str2);
int  getVarCount   (char **varNames, const char *str, int varNum);

char isSep  (const char c);

char treeToStdFile    (tree *t, const char *fileName);
char subTreeToStdFile (node *n, FILE *file, int deepness);
//---------------------------------------------------------------------------*/
tree *getTreeFromCode (const char *str)
{
  assert (str != NULL);

  //printf ("\n # starting descent \n");
  s              = str;
  tree *t        = treeCtor ();
  char *varNames[MAX_VAR_NUMBER] = {0};

  if (*s == '\0')
  {
    printf (" # ERRPR! Empty inputted data\n");
    return t;
  }

  t->rootNode      = nodeCtor ();
  t->rootNode->key = pdataCreate (PGM, 0);

  int i = 0;

  while (isSubStringIn (s, "def"))
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

  //nodeOptimization (t->rootNode);
  return t;
}
//---------------------------------------------------------------------------*/
node *getLine    (char **varNames)
{
  assert (varNames != NULL);

  curTabs = getTab ();

  node *n = getA (varNames);

  skipSpace   ();
  skipComment ();

  if (*s != '\n' && *s != '\0')
  {
    return n;
  }
  s++;

  curLine ++;

  return n;
}
//---------------------------------------------------------------------------*/
/* starts from not separator */
node *getDef  (void)
{
  char *varNames[MAX_VAR_NUMBER] = {0};
  char vNum = 0;
  s += 4;

  funArr[funNumber]       = funCtor ();
  funArr[funNumber]->name = getName ();

  node *n = nodeCtor    ();
  n->key  = pdataCreate (FUND, funNumber);

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
    if (*s == ',') s++;
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
  funNumber ++;

  while (*s == '\t') *s ++;

  int i    = 0;
  int skip = 0;
  while (skip == 0)
  {
    n->child[i] = getLine (varNames);
    if (n->child[i]->key->value == RETURN_CODE)
    {
      skip = 1;
    }
    i++;
  }

  n->childrenNum = i;

  return n;
}
//---------------------------------------------------------------------------*/
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
//---------------------------------------------------------------------------*/
node *getA (char **varNames)
{
  assert (varNames != NULL);

  node *nLeft = getC (varNames);

  if (*s == '=')
  {
    int op = ASSIGN_CODE; // "=" value

    s++;
    if (*s == ' ')
    {
      s++;
    }

    node *nRight = getC (varNames);
    nLeft = nodeCreate (pdataCreate (OP, op),
                        nLeft,
                        nRight
                        );
  }

  return nLeft;
}
//---------------------------------------------------------------------------*/
node *getC (char **varNames)
{
  assert (varNames != NULL);

  node *nLeft = getE (varNames);

  if (isSubStringIn (s, "<=") || isSubStringIn (s, ">=") ||
      isSubStringIn (s, "==") || isSubStringIn (s, "!="))
  {
    int op = 0;
    if (isSubStringIn(s, "<=")) op =    EQ_LESS_CODE;
    if (isSubStringIn(s, ">=")) op = EQ_GREATER_CODE;
    if (isSubStringIn(s, "==")) op =      EQUAL_CODE;
    if (isSubStringIn(s, "!=")) op =  NOT_EQUAL_CODE;

    s++;
    s++;
    if (*s == ' ') s++;

    node *nRight = getP (varNames);
    nLeft = nodeCreate (pdataCreate (OP, op),
                        nLeft,
                        nRight
                        );
  }
  else if (*s == '<' || *s == '>')
  {
    int op = 0;
    if (*s == '<') op = LESS_CODE;
    if (*s == '>') op = GREATER_CODE;

    s ++;
    if (*s == ' ') s++;

    node *nRight = getP (varNames);
    nLeft = nodeCreate (pdataCreate (OP, op),
                        nLeft,
                        nRight
                        );
  }

  return nLeft;
}
//---------------------------------------------------------------------------*/
node *getE (char **varNames)
{
  assert (varNames != NULL);

  node *nLeft = getT (varNames);

  while (*s == '+' || *s == '-')
  {
    int op = 0;
    if (*s == '+') op = ADD_CODE;
    if (*s == '-') op = SUB_CODE;

    s++;
    if (*s == ' ') s++;

    node *nRight = getT (varNames);
    nLeft = nodeCreate (pdataCreate (OP, op),
                        nLeft,
                        nRight
                        );
  }

  return nLeft;
}
//---------------------------------------------------------------------------*/
node *getT (char **varNames)
{
  assert (varNames != NULL);

  node *nLeft = getD (varNames);

  while (*s == '*' || *s == '/' || *s == '%')
  {
    int op = 0;
    if (*s == '*') op = MUL_CODE;
    if (*s == '/') op = DIV_CODE;
    if (*s == '%') op = MOD_CODE;

    s++;
    if (*s == ' ') s++;

    node *nRight = getD (varNames);
    nLeft = nodeCreate (pdataCreate (OP, op),
                        nLeft,
                        nRight
                        );
  }

  return nLeft;
}
//---------------------------------------------------------------------------*/
node *getD (char **varNames)
{
  assert (varNames != NULL);

  node *nLeft = getP (varNames);

  while (*s == '^')
  {
    int op = PWR_CODE;

    s++;
    if (*s == ' ') s++;

    node *nRight = getP (varNames);
    nLeft = nodeCreate (pdataCreate (OP, op),
                        nLeft,
                        nRight
                        );
  }

  return nLeft;
}
//---------------------------------------------------------------------------*/
node *getP (char **varNames)
{
  assert (varNames != NULL);

  if (*s == '(')
  {
    s++;
    node *n = getC (varNames);
    if (*s != ')')
    {
      printf (" ERROR! ) was not found\n");
      printf ("      %c was found\n", *s);
      return n;
    }
    s++;
    return n;
  }
  if (isdigit (*(s)))
  {
    return getN ();
  }

  return getS (varNames);
}
//---------------------------------------------------------------------------*/
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

  if (*s == ' ') s++;

  return nodeCreate (pdataCreate (CNST, atof (number)),
                     NULL,
                     NULL
                    );
}
//---------------------------------------------------------------------------*/
#define DEF_OP(name, code, tp, arg, var, funD)                                \
{                                                                             \
  if (strcmp (name, buf) == 0)                                                \
  {                                                                           \
    switch (arg)                                                              \
    {                                                                         \
      case 1:                                                                 \
        return nodeCreate (pdataCreate (tp, code),                            \
                           NULL,                                              \
                           NULL                                               \
                          );                                                  \
      case 2:                                                                 \
        return nodeCreate (pdataCreate (tp, code),                            \
                           getP (varNames),                                   \
                           NULL                                               \
                          );                                                  \
      case 3:                                                                 \
      {                                                                       \
        node *n = nodeCtor ();                                                \
        n->key  = pdataCreate (tp, code);                                     \
        if (*s != '(')                                                        \
        {                                                                     \
            printf (" # ERROR No ( in line %d got \"%c\"\n", curLine, *s);    \
        }                                                                     \
        n->child[0] = getA (varNames);                                        \
        if (*s != ':')                                                        \
        {                                                                     \
            printf (" # ERROR No : in line %d got \"%c\"\n", curLine, *s);    \
        }                                                                     \
        s++;                                                                  \
        skipSpace   ();                                                       \
        /*skipComment ();*/                                                   \
        s++;                                                                  \
        curLine++;                                                            \
                                                                              \
        int startTabs = curTabs;                                              \
        int i = 1;                                                            \
        int skip = 0;                                                         \
        while (checkTabs() - 1 == startTabs)                                  \
        {                                                                     \
          n->child[i] = getLine (varNames);                                   \
          i++;                                                                \
        }                                                                     \
        n->childrenNum = i;                                                   \
        return n;                                                             \
      }                                                                       \
      case 4:                                                                 \
        return nodeCreate (pdataCreate (tp, code),                            \
                           NULL,                                              \
                           NULL                                               \
                          );                                                  \
      case 5:                                                                 \
        return nodeCreate (pdataCreate (tp, code),                            \
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

  int i = 0;

  char *buf = getName ();
  skipSpace  ();

  // op check
  #include "operations.h"

  // function check
  for (int i = 0; i < funNumber; i++)
  {
    if (strcmp (funArr[i]->name, buf) == 0)
    {
      /* different number of vars */
      node *n = nodeCtor ();
      n->key  = pdataCreate (FUN, i);

      if (*s != '(')
      {
        printf (" # ERROR. \"(\" was not found in line %d\n", curLine);
        printf ("   %c was found\n", *s);
        return n;
      }
      s++;
      int j = 0;
      while (*s != ')')
      {
        n->child[j] = getE (varNames);
        j++;

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

      return n;
    }
  }

  // var check
  for (int i = 0; varNames[i] != NULL; i++)
  {
    if (strcmp (varNames[i], buf) == 0)
    {
      return nodeCreate (pdataCreate(VAR, i),
                         NULL,
                         NULL);

    }
  }

  printf (" # Command \"%s\" has not been read on line %d\n", buf, curLine);

  return NULL;
}
#undef DEF_OP
//---------------------------------------------------------------------------*/
/* starts with space */
char skipSpace   (void)
{
  while (*s == ' ') s++;

  return 0;
}
//---------------------------------------------------------------------------*/
/* starts from comment, ends with \n or \0 */
char skipComment (void)
{
  if (*s == '#')
  {
    while (*s != '\n' && *s != '\0') s++;
    return 1;
  }

  return 0;
}
//---------------------------------------------------------------------------*/
#define DEF_OP(name, val, tp, arg, var, fun)                                  \
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

  for (;*str == '\t'; str++);
  char *command = (char *)calloc (MAX_COMMAND_LENGTH, sizeof (*command));
  for (int i = 0; !isSep (*str); i++)
  {
    command[i] = *str;
    str++;
  }

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

    while (*str != '\n') str++;
    str++;

    for ( ;*str == '\t'; str++);
    memset (command, 0, MAX_COMMAND_LENGTH);

    for (int i = 0; !isSep (*str); i++)
    {
      command[i] = *str;
      str++;
    }
  }

  return varNum;
}
#undef DEF_OP
//---------------------------------------------------------------------------*/
char getTab (void)
{
  int i = 0;
  while (*s == '\t')
  {
    s++;
    i++;
  }
  return i;
}
//---------------------------------------------------------------------------*/
char checkTabs (void)
{
  int i = 0;
  while (s[i] == '\t')
  {
    i++;
  }
  return i;
}
//---------------------------------------------------------------------------*/
char isSubStringIn (const char *str1, const char *str2)
{
  assert (str1 != NULL);
  assert (str2 != NULL);

  for (;*str2 != '\0'; ++str2, ++str1)
  {
    if (*str2 != *str1) return 0;
  }

  return 1;
}
//---------------------------------------------------------------------------*/
char isSep (const char c)
{
  /* can be done better */
  switch (c)
  {
    case ' ' :
    case '(' :
    case ')' :
    case '\n':
    case '\0':
    case ':' :
    case '\t':
    case ',' :
    case ';' :
    case '[' :
    case ']' :
      return 1;
    default:
      return 0;
  }
}
//---------------------------------------------------------------------------*/
#define DEF_OP(std, val)                                                      \
{                                                                             \
  if (val == MAIN_CODE)                                                       \
  {                                                                           \
    fprintf (file, "\t%d %d\n", OP, std);                                     \
  }                                                                           \
}
char treeToStdFile (tree *t, const char *fileName)
{
  assert (t        != NULL);
  assert (fileName != NULL);

  FILE *file = fopen (fileName, "w");

  for (int i = 0; i < funNumber; i ++)
  {
    subTreeToStdFile (t->rootNode->child[i], file, 0);
  }

  /* for compatibility */
  fprintf (file, "(\n");
  #include "stdOperations.h"
  for (int i = funNumber; i < t->rootNode->childrenNum; i++)
  {
    subTreeToStdFile (t->rootNode->child[i], file, 1);
  }
  fprintf (file, ")\n");

  return 0;
}
#undef DEF_OP
//---------------------------------------------------------------------------*/
#define DEF_OP(std, val)                                                      \
{                                                                             \
  if (n->key->value == val)                                                   \
  {                                                                           \
    fprintf (file, "%d\n", std);                                              \
  }                                                                           \
}
char subTreeToStdFile (node *n, FILE *file, int deepness)
{
  assert (n    != NULL);
  assert (file != NULL);

  /* data out */
  tabFprint (file, deepness);
  fprintf (file, "(\n");

  tabFprint (file, deepness + 1);
  fprintf (file, "%d ", n->key->type);
  if (n->key->type != OP)
  {
    fprintf (file, "%d\n", n->key->value);
  }
  else
  {
    #include "stdOperations.h"
  }

  /* for compatibility argnum */
  if ((n->key->type) == FUND)
  {
    fprintf (file, "\t(\n");
    fprintf (file, "\t\t%d %d\n", CNST, funArr[n->key->value]->argNum);
    fprintf (file, "\t)\n");
  }

  /* child out */
  for (int i = 0; n->child[i] != NULL; i++)
  {
    subTreeToStdFile (n->child[i], file, deepness + 1);
  }
  tabFprint (file, deepness);
  fprintf (file, ")\n");

  return 0;
}
#undef DEF_OP
//---------------------------------------------------------------------------*/
#undef s
//---------------------------------------------------------------------------*/
//               © Gorbachev Nikita, December 2018 + April 2019              //
//---------------------------------------------------------------------------*/
