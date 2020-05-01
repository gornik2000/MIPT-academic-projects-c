//---------------------------------------------------------------------------*/
#define DEF_BC(name, len, ...) const unsigned char name##_CODE[] = {__VA_ARGS__};
#include "bytecode.h"
#undef  DEF_BC
//---------------------------------------------------------------------------*/
#define DEF_BC(name, len, ...) const char name##_LEN = len;
#include "bytecode.h"
#undef  DEF_BC
//---------------------------------------------------------------------------*/
#include "fileIO.h"
#include "programTree.h"
//---------------------------------------------------------------------------*/
const int  MAX_ASMCODE_SIZE = 10000;
const int  BLOCK_SIZE       = 256;

const char MAX_FUN_NUMBER   = 100;
const char MAX_VAR_NUMBER   = 10;
//---------------------------------------------------------------------------*/
/// need to be filed while getting tree from txt
cmd_fun *funArr[MAX_FUN_NUMBER] = {0};
char funNumber                  = 0;
//---------------------------------------------------------------------------*/
tree *textToStdTree (char **text, int lines);
char *nonTab        (char *s);

unsigned char  *putInBuf (unsigned char *buf, const unsigned char *str, char num);
unsigned char *treeToBuf (const tree *t, unsigned char *buf);
unsigned char *nodeToBuf (const node *n, unsigned char *buf);
//---------------------------------------------------------------------------*/
int main (int argc, char *argv[])
{
  if (argc < 2)
  {
		printf(" # Missing file\n");
		return 0;
	}

  char *fileInName  = argv[1];
  char *fileOutName = argv[2];

  /* create code text */
  int lines = 0;
  int size  = 0;
  char **treeCode = createTextFromFile (fileInName, &size, &lines);

  tree *pgmTree   = textToStdTree (treeCode, lines);
  //treeToFile (pgmTree, "fromfile.txt");

  unsigned char *byteCode = (unsigned char *)calloc (MAX_ASMCODE_SIZE, sizeof (*byteCode));
  unsigned char *endBuf   = treeToBuf (pgmTree, byteCode);
  FILE          *outFile  = fopen (fileOutName, "wb");
  fwrite (byteCode, sizeof (char), endBuf - byteCode, outFile);

  treeDtor (pgmTree);
  fclose   (outFile);
  free     (byteCode);

  printf (" # Tree was compiled\n");
  return 0;
}
//---------------------------------------------------------------------------*/
#define DEF_OP(std, val)                                                      \
{                                                                             \
  if (value == std)                                                           \
  {                                                                           \
    if (val == MAIN_CODE)                                                     \
    {                                                                         \
      i ++;                                                                   \
    }                                                                         \
    else                                                                      \
    {                                                                         \
      currentNode->key = pdataCreate (type, val);                             \
    }                                                                         \
  }                                                                           \
}
tree *textToStdTree (char **text, int lines)
{
  assert (text != NULL);

  tree *t           = treeCtor ();
  node *currentNode = t->rootNode;
  t->rootNode->key = pdataCreate (PGM, 0);

  int type  = 0;
  int value = 0;
  int dump  = 0;
  int aNum  = 0;
  int i     = 0;

  /* -1 because starts from 0 and -1 because of compatibility with main */
  while (i < lines - 2)
  {
    if      (*nonTab(text[i]) == '(')
    {
      currentNode->childrenNum += 1;

      currentNode->child[currentNode->childrenNum - 1] = nodeCtor ();
      currentNode->child[currentNode->childrenNum - 1]->parent = currentNode;

      currentNode = currentNode->child[currentNode->childrenNum - 1];
    }
    else if (*nonTab(text[i]) == ')')
    {
      currentNode = currentNode->parent;
    }
    else
    {
      sscanf (text[i], "%d %d", &type, &value);
      switch (type)
      {
        case OP  :
          #include "stdOperations.h"
          break;

        case FUND:
          currentNode->key = pdataCreate (FUN,  value);
          funArr[funNumber] = funCtor ();
          sscanf (text[i + 2], "%d %d", &dump, &aNum);
          funArr[funNumber]->argNum = aNum;
          funNumber ++;
          /* skip compatibility data */
          i += 3;
          break;

        default  :
          currentNode->key = pdataCreate (type, value);
          break;
      }
    }
    i++;
  }

  return t;
}
#undef DEF_OP
//---------------------------------------------------------------------------*/
/* for commands     */
#define $(cmd)   buf   = putInBuf (buf, cmd##_CODE, cmd##_LEN);
/* for 1b constants */
#define c(cnst) *buf++ = (unsigned char)(cnst);
/* for 2b constants */
#define dc(cnst)                                                              \
{                                                                             \
  c((cnst) % 256)                                                             \
  c((cnst) / 256)                                                             \
}
unsigned char *treeToBuf (const tree *t, unsigned  char *buf)
{
  assert (t   != NULL);
  assert (buf != NULL);

  unsigned char *jmpStart = 0;

  /* program */
  $(SUBD_SP); dc(BLOCK_SIZE);

  /* functions */
  for (int i = 0; i < funNumber; i++)
  {
    $(JMP);
    jmpStart = buf;
    buf++;

    funArr[i]->start = buf;

    $(PUSH_BP);
    $(R16_MOV); $(BP_SP);
    $(ADD_BP ); c(4    );
    for (int j = 0; j < t->rootNode->child[i]->childrenNum; j++)
    {
      buf = nodeToBuf (t->rootNode->child[i]->child[j], buf);
    }
    $(RET);
    $(NOP);
    $(NOP);
    $(NOP);

    *jmpStart = buf - jmpStart - 1;
  }

  /* main program */
  $(ENTER);
  for (int i = funNumber; i < t->rootNode->childrenNum; i++)
  {
    buf = nodeToBuf (t->rootNode->child[i], buf);
  }
  $(EXIT);

  return buf;
}
//---------------------------------------------------------------------------*/
#define DEF_OP(name, code, tp, arg, var, funD)                                \
{                                                                             \
  if ((code == n->key->value) && (tp == n->key->type))                        \
  {                                                                           \
    funD                                                                      \
  }                                                                           \
}
unsigned char *nodeToBuf (const node *n, unsigned  char *buf)
{
  assert (n   != NULL);
  assert (buf != NULL);

  #include "operations.h"

  if (n->key->type == VAR)
  {
    $(R16_MOV); $(AX_1BP1); c(n->key->value * 2); // mov  ax, [bp + value]
    $(PUSH_AX);                                   // push ax
  }

  if (n->key->type == CNST)
  {
    $(MOV_AX ); dc(n->key->value); // mov ax, value
    $(PUSH_AX);                    // push ax
  }

  if (n->key->type == FUN)
  for (int i = 0; i < funNumber; i++)
  {
    if (i == n->key->value)
    {
      $(SUBD_SP); dc(BLOCK_SIZE);
      for (int j = funArr[i]->argNum - 1; j >= 0; j--)
      {
        buf = nodeToBuf (n->child[j], buf);
      }
      $(CALL);    dc(65535 - (buf - funArr[i]->start));
      $(ADDD_SP); dc(BLOCK_SIZE + funArr[i]->argNum * 2);
      $(PUSH_AX);
    }
  }

  return buf;
}
#undef dc
#undef c
#undef $
//---------------------------------------------------------------------------*/
unsigned char *putInBuf (unsigned char *buf, const unsigned char *str, char num)
{
  for (int i = 0; i < num; i++, str++, buf++)
  {
    *buf = *str;
  }

  return buf;
}
//---------------------------------------------------------------------------*/
char *nonTab (char *s)
{
  while (*s == '\t') s++;

  return s;
}
//---------------------------------------------------------------------------*/
//               © Gorbachev Nikita, May 2019                                //
//---------------------------------------------------------------------------*/
