//---------------------------------------------------------------------------*/
//                      Recursive Descent Realization
//---------------------------------------------------------------------------*/
#include <ctype.h>
#define PRINT(s, ...) printf (" # " s "\n", ##__VA_ARGS__)
//---------------------------------------------------------------------------*/
//                      Global constant variables
//---------------------------------------------------------------------------*/
const int   MAX_COMMAND_LENGTH    = 30;
const char *TREE_STRING_FOR_INPUT = NULL;
const char  MAX_FUN_NUMBER        = 100;
const char  MAX_VAR_NUMBER        = 100;
//---------------------------------------------------------------------------*/
//                      Global variables (fastest solution)
//---------------------------------------------------------------------------*/
cmd_fun *funArr[MAX_FUN_NUMBER] = {0};

char funNumber = 0;
int    curLine = 1;
int    curTabs = 0;
#define s TREE_STRING_FOR_INPUT
//---------------------------------------------------------------------------*/
//                      Recursive descent functions
//---------------------------------------------------------------------------*/
/*    G::=def B{def B}*B"\0"
      B::=E{"\n"E}*
      E::=T{[+-]T}*
      T::=D{[/*]D}*
      D::=C{"^" C}*
      C::=A{[<>]A}*
      A::=P{"=" P}*

      P::=(E)|N|S|V|F

      N::=integer Number
      S::=String operation from base
      V::=Variable from base
      Ñ::=Function from base
      B::=function Body definition
//---------------------------------------------------------------------------*/
/* G */ tree *getTreeFromCode (const char *str);

/* B */ node *getBody       (char **varNames); // fun body / main body
/* E */ node *getExpresion  (char **varNames); // +-
/* T */ node *getTerm       (char **varNames); // */
/* D */ node *getDegree     (char **varNames); // ^
/* C */ node *getComparison (char **varNames); // <= >= != == ...
/* A */ node *getAssignment (char **varNames); // =

/* P */ node *getParameter  (char **varNames); // () ...

/* N */ node *getNumber (void);            // double number
/* S */ node *getString (char **varNames); // string operation
/* D */ node *getDef    (void);            // def function

char *getName   (void);            // function name

char checkTabs   (void);
char isSubStringIn (const char *str1, const char *str2);

int  getVarCount (char **varNames, const char *str, int varNum);
//---------------------------------------------------------------------------*/
tree *getTreeFromCode (const char *str)
{
  assert (str != NULL);

  PRINT ("starting descent");
  s       = str;
  tree *t = treeCtor ();
  char *varNames[MAX_VAR_NUMBER] = {0};

  if (*s == '\0')
  {
    PRINT ("ERRPR! Empty inputted data");
    return t;
  }

  t->rootNode      = nodeCtor ();
  t->rootNode->key = pdataCreate (PGM, 0, 0);

  int i = 0;
  for (; isSubStringIn (s, "def "); ++i, s += 4)
  {
    t->rootNode->child[i] = getBody ();
  }
  t->rootNode->childrenNum = i + 1;

  t->rootNode->child[i - 1] = getBody ();

  //*///nodeOptimization (t->rootNode);
  return t;
}
//---------------------------------------------------------------------------*/
node *getBody ()
{
  int varNumber = 0;
  varNumber = getVarCount (varNames, s, varNumber);
}
//---------------------------------------------------------------------------*/
node *getExpresion (char **varNames)
{
  ;
}
//---------------------------------------------------------------------------*/
node *getTerm (char **varNames)
{
  ;
}
//---------------------------------------------------------------------------*/
node *getDegree (char **varNames)
{
  ;
}
//---------------------------------------------------------------------------*/
node *getComparison (char **varNames)
{
  ;
}
//---------------------------------------------------------------------------*/
node *getAssignment (char **varNames)
{
  ;
}
//---------------------------------------------------------------------------*/
node *getParameter (char **varNames)
{
  ;
}
//---------------------------------------------------------------------------*/
node *getNumber (void)
{
  ;
}
//---------------------------------------------------------------------------*/
node *getString (char **varNames)
{
  ;
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
