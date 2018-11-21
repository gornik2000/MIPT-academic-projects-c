#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "fileIO.cpp"
typedef char *tree_data;
#include "tree.cpp"

#define speak(s) system ("balcon -n \"Zira\" -t \""s"\" -s 5 -v 100");
//-----------------------------------------------------------------------------

const int MAX_FILE_STR_LINE = 256;

char akinator   (const char *dataFileName = "new.txt");
char akinatorDialogue (tree *akinatorTree);
char akinatorExtra    (tree *akinatorTree);

char textToTree (char **text, tree *t, int lines);
char treeTofile (tree *t, const char *fileName);
char subTreeToFile (node *n, FILE *file);

//-----------------------------------------------------------------------------

char akinator (const char *dataFileName)
{
  assert (dataFileName != NULL);

  printf (" # AKINATOR v0.5\n\n"
          " # Would you like to play with me?\n"
          "\t  1 - 0  - 2\n"
          "\tYes - No - Extra\n");
  speak  ("Would you like to play with me?");

  char mode = getchar ();
  getchar ();

  if (mode == '0')
  {
    speak  ("Thanks for patience");
    printf (" # Thanks for patience \n");
    return 0;
  }
  if (mode != '1' && mode != '2')
  {
    speak  ("ERROR! Inputted incorrect data");
    printf (" # ERROR! Inputted incorrect data: %c \n", mode);
    return 3;
  }
  system ("cls");
  speak  ("Imagine an object and i will guess it. Ready? Go!");
  printf (" # Imagine an object and i will guess it. Ready? Go!\n\n\n");

  tree *akinatorTree = treeCtor ();
  int lines = 0;
  int size  = 0;
  char **text = createTextFromFile (dataFileName, &size, &lines);

  textToTree  (text, akinatorTree, lines);

  if (mode == '2')
  {
    akinatorExtra (akinatorTree);
    return 2;
  }

  akinatorDialogue (akinatorTree);

  treeTofile (akinatorTree, dataFileName);

  free (text[0]);
  text[0] = NULL;
  free (text);
  text    = NULL;

  akinatorTree = treeDtor (akinatorTree);
  speak  ("Thanks for playing with me");
  printf (" # Thanks for playing with me \n");

  return 1;
}

char akinatorExtra    (tree *akinatorTree)
{
  char action = 1;
  while (action != 0)
  {
    speak  ("What information do you need?");
    printf (" # What information do you need?\n");
    printf (" all pars - all objs - obj description - obj cmp - exit\n"
            "     p    -     o    -     d           -     c   - e\n");
    action = getchar ();
    getchar ();

    switch (action)
    {
      case 'p' :
        nodePrintNonLeafs (akinatorTree->rootNode);
        break;
      case 'o' :
        nodePrintLeafs    (akinatorTree->rootNode);
        break;
      case 'd' :
        {
        speak  ("What object's characteristics do you want?");
        printf (" # What object's characteristics do you want?\n");

        char data[MAX_FILE_STR_LINE] = {0};
        gets (data);

        nodePrintToRoot (treeFind (akinatorTree, data));
        break;
        }
      case 'c' :
        {
        speak  ("Print two interested objects");
        printf (" # Print two interested objects\n");

        char str1[MAX_FILE_STR_LINE] = {0};
        gets (str1);
        char str2[MAX_FILE_STR_LINE] = {0};
        gets (str2);

        nodesPrintHistory (treeFind (akinatorTree, str1), treeFind (akinatorTree, str2));
        break;
        }
      case 'e' :
        speak  ("Thanks for patience");
        printf (" # Thanks for patience \n");
        return 0;
      default  :
        speak  ("ERROR! Inputted incorrect data:");
        printf (" # ERROR! Inputted incorrect data: %c\n", action);
        break;
    }
  }
  return 0;
}

char akinatorDialogue (tree *akinatorTree)
{
  char answer = 0;
  node *currentNode = akinatorTree->rootNode;
  node *lastNode    = NULL;

  while (currentNode != NULL)
  {
    if ((currentNode->leftChild && currentNode->rightChild) == NULL)
    {
      speak  ("You guessed");
      //speak  (*currentNode->key);
      printf (" # You guessed %s?\n", getNonSpace(*currentNode->key));
      printf ("\t  1 - 0\n"
              "\tYes - No\n");
    }
    else
    {
      speak  ("It");
      //speak  (*currentNode->key);
      printf (" # It %s?\n", getNonSpace(*currentNode->key));
      printf ("\t  1 - 0\n"
              "\tYes - No\n");
    }

    answer = getchar ();
    getchar ();

    if (answer == '1')
    {
      lastNode    = currentNode;
      currentNode = currentNode->leftChild;
    }
    if (answer == '0')
    {
      lastNode    = currentNode;
      currentNode = currentNode->rightChild;
    }
  }

  /* end of guess part */
  if (answer == '1')
  {
    speak  ("GG EZ");
    printf (" # GG EZ\n");
  }


  /* adding new elements */
  if (answer == '0')
  {
    speak  ("I will beat you next time!");
    printf (" # I will beat you next time!\n");
    speak  ("And what is the object?");
    printf (" # And what is the object?\n"
            " [It is] ");
    char *obj = (char *)calloc(MAX_FILE_STR_LINE, sizeof (*obj));
    gets (obj);

    speak  ("And how does it differs from last one?");
    printf (" # And how does it differs from last one?\n"
            " [It] ");
    char *par = (char *)calloc(MAX_FILE_STR_LINE, sizeof (*par));
    gets (par);

    node *newNode = nodeCtor ();
    node *newObj  = nodeCtor ();

    if      (lastNode->parent->leftChild == lastNode)
      lastNode->parent->leftChild  = newNode;
    else if (lastNode->parent->rightChild == lastNode)
      lastNode->parent->rightChild = newNode;

    newNode->parent     = lastNode->parent;
    newNode->rightChild = lastNode;
    newNode->leftChild  = newObj;
    newNode->key        = &par;
    newNode->deepness   = newNode->parent->deepness + 1;

    newObj->parent   = newNode;
    newObj->key      = &obj;
    newObj->deepness = newObj->parent->deepness + 1;

    lastNode->parent = newNode;
    akinatorTree->edgeCount += 2;
  }
  return 0;
}

char textToTree (char **text, tree *t, int lines)
{
  assert (text != NULL);
  assert (t    != NULL);

  node *currentNode = t->rootNode;
  char bktsDifference = 0;

  for (int i = 0; i < lines - 1; i++)
  {
    if (*getNonSpace(text[i]) == '{')
    {
      bktsDifference ++;
      if (currentNode->leftChild == NULL)
      {
        currentNode->leftChild         = nodeCtor ();
        currentNode->leftChild->parent = currentNode;
        currentNode =           currentNode->leftChild;
        currentNode->deepness = currentNode->parent->deepness + 1;
      }
      else
      {
        currentNode->rightChild         = nodeCtor ();
        currentNode->rightChild->parent = currentNode;
        currentNode =           currentNode->rightChild;
        currentNode->deepness = currentNode->parent->deepness + 1;
      }
      continue;
    }
    if (*getNonSpace(text[i]) == '}')
    {
      bktsDifference --;
      currentNode = currentNode->parent;
      continue;
    }

    *currentNode->key = getNonSpace (text[i]);
    t->edgeCount += 1;
  }

  return bktsDifference;
}

char treeTofile (tree *t, const char *fileName)
{
  assert (t        != NULL);
  assert (fileName != NULL);

  FILE *file = fopen (fileName, "w");

  subTreeToFile (t->rootNode, file);
}

char subTreeToFile (node *n, FILE *file)
{
  assert (n    != NULL);
  assert (file != NULL);

  if (n->key != NULL)
  {
    tabFprint (file, n->deepness /* tabcount */);
    fprintf   (file, "%s\n", *n->key);
  }

  if (n->leftChild != NULL)
  {
    tabFprint     (file, n->deepness /* tabcount */);
    fprintf       (file, "{\n");
    subTreeToFile (n->leftChild, file);
    tabFprint     (file, n->deepness /* tabcount */);
    fprintf       (file, "}\n");
  }

  if (n->rightChild != NULL)
  {
    tabFprint     (file, n->deepness /* tabcount */);
    fprintf       (file, "{\n");
    subTreeToFile (n->rightChild, file);
    tabFprint     (file, n->deepness /* tabcount */);
    fprintf       (file, "}\n");
  }
}

//-----------------------------------------------------------------------------
