#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "fileIO.cpp"
typedef char *tree_data;
#include "tree.cpp"

//-----------------------------------------------------------------------------

const int MAX_FILE_STR_LINE = 100;

char akinator   (const char *dataFileName = "new.txt");

char textToTree (char **text, tree *t, int lines);

char treeTofile (tree *t, const char *fileName);

char subTreeToFile (node *n, FILE *file);

//-----------------------------------------------------------------------------

char akinator (const char *dataFileName)
{
  assert (dataFileName != NULL);

  printf (" # AKINATOR v0.5\n\n"
          " # Would you like to play with me?\n"
          "\t  1 - 0\n"
          "\tYes - No\n");

  char start = getchar ();
  getchar ();

  if (start == '0')
  {
    printf (" # Thanks for patience \n");
    return 0;
  }
  system ("cls");

  printf (" # Imagine an object and i will guess it. Ready? Go!\n\n\n");
  Sleep (500);

  tree *akinatorTree = treeCtor ();
  int lines = 0;
  int size = 0;
  char **text = createTextFromFile (dataFileName, &size, &lines);

  //printf (" = %p - head -%s\n",  akinatorTree->rootNode, text[0]);
  //printf (" = %p - left -%s\n",  akinatorTree->rootNode->leftChild, *akinatorTree->rootNode->leftChild->key);
  //printf (" = %p - right -%s \n", akinatorTree->rootNode->rightChild, *akinatorTree->rootNode->rightChild->key);

  textToTree (text, akinatorTree, lines);
  foutputText(text, "text.txt");
  //printf (" = %p - head -%s\n",  akinatorTree->rootNode, *akinatorTree->rootNode->key);
  //printf (" = %p - left -%s\n",  akinatorTree->rootNode->leftChild, *akinatorTree->rootNode->leftChild->key);
  //printf (" = %p - right -%s \n", akinatorTree->rootNode->rightChild, *akinatorTree->rootNode->rightChild->key);

  char answer = 0;
  node *currentNode = akinatorTree->rootNode;
  node *lastNode = NULL;

  while (currentNode != NULL)
  {
    if ((currentNode->leftChild && currentNode->rightChild) == NULL)
    {
      printf (" # Is it %s?\n", *currentNode->key);
      printf ("\t  1 - 0\n"
              "\tYes - No\n");
    }
    else
    {
    printf (" # It %s?\n", *currentNode->key);
    printf ("\t  1 - 0\n"
            "\tYes - No\n");
    }

    answer = getchar ();
    getchar ();

    if (answer == '1')
    {
      lastNode = currentNode;
      currentNode = currentNode->leftChild;
    }
    if (answer == '0')
    {
      lastNode = currentNode;
      currentNode = currentNode->rightChild;
    }
  }
  Sleep (500);
  if (answer == '1')
  {
    printf (" # GG EZ\n");
  }

  if (answer == '0')
  {
    printf (" # I will beat you next time!\n");
    Sleep (1000);

    printf (" # And what is the object?\n"
            " [It is] ");
    char *obj = (char *)calloc(MAX_FILE_STR_LINE, sizeof (*obj));
    gets (obj);

    printf (" # And how does it differs from last one?\n"
            " [It] ");
    char *par = (char *)calloc(MAX_FILE_STR_LINE, sizeof (*par));
    gets (par);

    node *newNode = nodeCtor ();
    node *newObj  = nodeCtor ();
    //printf ("%s", *lastNode->key);

    if      (lastNode->parent->leftChild == lastNode)
    {
      lastNode->parent->leftChild = newNode;
    }
    else if (lastNode->parent->rightChild == lastNode)
    {
      lastNode->parent->rightChild = newNode;
    }
    newNode->parent = lastNode->parent;
    newNode->key    = &par;
    newNode->rightChild = lastNode;
    //printf ("2");

    newNode->leftChild  = newObj;
    newObj->key = &obj;
    //printf ("3");

    lastNode->parent = newNode;
    akinatorTree->edgeCount += 2;
    //printf ("4");
  }
  //printf ("%s", *akinatorTree->rootNode->leftChild->key);

  //printf ("%s", *akinatorTree->rootNode->leftChild->rightChild->key);

  treeTofile (akinatorTree, dataFileName);

  //printf ("- %s -", *treeFind (akinatorTree, "a dog")->key);
  //system ("cls");

  free (text[0]);
  text[0] = NULL;
  free (text);
  text = NULL;

  akinatorTree = treeDtor (akinatorTree);
  printf (" # Thanks for playing with me \n");
}

char textToTree (char **text, tree *t, int lines)
{
  assert (text != NULL);
  assert (t    != NULL);

  node *currentNode = t->rootNode;
  char bktsDifference = 0;

  for (int i = 0; i < lines - 1; i++)
  {
    //printf ("char %c \n", *text[i]);
    //printf ("str %s \n", text[i]);
    if (*text[i] == '{')
    {
      bktsDifference ++;
      //printf ("{ \n");
      if (currentNode->leftChild == NULL)
      {
        currentNode->leftChild = nodeCtor ();
        currentNode->leftChild->parent = currentNode;
        currentNode = currentNode->leftChild;
      }
      else
      {
        currentNode->rightChild = nodeCtor ();
        currentNode->rightChild->parent = currentNode;
        currentNode = currentNode->rightChild;
      }
      continue;
    }
    if (*text[i] == '}')
    {
      bktsDifference --;
      //printf ("} \n");
      currentNode = currentNode->parent;
      continue;
    }
    currentNode->key = &(text[i]);
    t->edgeCount += 1;
    //printf ("data %s\n", *currentNode->key);
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
  if (n->key != NULL)
  {
    fprintf (file, "%s\n", *n->key);
    /*
    int len = strlen (*n->key) - 1;
    for (int i = 0; i < len; i++)
    {
      putc((*n->key)[i], file);
    }
    putc ('\n', file); */
  }

  if (n->leftChild != NULL)
  {
    fprintf       (file, "{\n");
    subTreeToFile (n->leftChild, file);
    fprintf       (file, "}\n");
  }
  if (n->rightChild != NULL)
  {
    fprintf       (file, "{\n");
    subTreeToFile (n->rightChild, file);
    fprintf       (file, "}\n");
  }
}

//-----------------------------------------------------------------------------

int main()
{
  akinator ("data.txt");
}
