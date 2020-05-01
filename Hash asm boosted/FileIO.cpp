//---------------------------------------------------------------------------*/
#include <io.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
//---------------------------------------------------------------------------*/
int fileSize (FILE *file);

int textInLines (char **text, int textSize);

int foutputText (char **text, const char *name = "out.txt");

char **createTextCopy (char **text, int lines);

char **createText (char *buf, int lines, int textSize);

char **createTextFromFile (const char *fileName, int *currentFileSize = 0, \
                                                 int *currentFileLines = 0);
//---------------------------------------------------------------------------*/
int fileSize (FILE *file)
{
  assert (file);

  /* saves the cursor position before any changing */
  int currentPosition = ftell (file);
  assert (currentPosition != -1L);

  /* moves the cursor to last position */
  int check = fseek (file, 0, SEEK_END);
  assert (check == 0);

  /* puts this position to fileSize */
  int fileSize = ftell (file);
  assert (fileSize != -1L);

  /* moves the cursor to the position before any changing */
  fseek (file, currentPosition, SEEK_SET);

  return fileSize;
}
//---------------------------------------------------------------------------*/
int bufInLines (char *buf, int textSize)
{
  assert (buf);

  /* changes \n to \0 and find the amount of changes */
  int lines = 0;
  for (char* s = strchr (buf, '\n'); s != NULL; \
                                     s = strchr (s + 1, '\n'), lines ++)
  {
    *s = '\0';
  }

  return lines + 1;
}
//---------------------------------------------------------------------------*/
int foutputText (char **text, const char *name)
{
  assert (text);
  assert (name);

  /* opens file */
  FILE *file = fopen (name, "wb");
  assert (file);

  /* inputs text into file by lines */
  for (int lines = 0; text[lines] != 0; lines ++)
  {
    fprintf (file, "%s\n", text[lines]);
  }

  fclose (file);

  return 0;
}
//---------------------------------------------------------------------------*/
char **createTextCopy (char **text, int lines)
{
  assert (text);

  /* creates memory for text copy */
  char **textCopy = (char **)calloc (lines + 1, sizeof (*textCopy));
  assert (textCopy);

  /*  makes a copy */
  for (int i = 0; i <= lines; i++)
  {
      textCopy[i] = text[i];
  }

  return textCopy;
}
//---------------------------------------------------------------------------*/
char **createText (char *buf, int lines, int textSize)
{
  assert (buf);

  /* creates memory for text */
  char **text = (char **)calloc(lines + 1, sizeof (*text));
  assert (text);

  /* puts the first pointer in text - our buffer */
  text[0] = buf;

  /* puts the other pointers */
  int i = 1;
  for (char* s = strchr (buf, '\0'); s < buf + textSize; \
                                     s = strchr (s + 1, '\0'), i++)
  {
    text[i] = s + 1;
  }

  return text;
}
//---------------------------------------------------------------------------*/
char **createTextFromFile (const char *fileName, int *currentFileSize, \
                                                 int *currentFileLines)
{
  assert (fileName);

  /* if currentFileLines was not given creates it */
  if (currentFileLines == NULL)
  {
    int cFL = 0;
    currentFileLines = &cFL;
  }

  /* if currentFileSize was not given creates it */
  if (currentFileSize == NULL)
  {
    int cFS = 0;
    currentFileSize = &cFS;
  }

  /* opens file with text */
  FILE *file = fopen (fileName, "rb");
  assert (file);

  /* finds file size */
  *currentFileSize = fileSize (file);

  /* creates memory for text buff with \0 in the beginning*/
  char *buf = (char *)calloc(*currentFileSize + 1, sizeof (*buf));
  assert (buf);
  buf ++;

  /* puts text from file into buff */
  int readSize = fread (buf, sizeof (char), *currentFileSize, file);
  assert (readSize == *currentFileSize);

  fclose (file);

  /* finds number of lines in buf and change \n to \0 */
  *currentFileLines = bufInLines (buf, *currentFileSize);

  /* returns array of line pointers, connecting buff and text */
  return createText (buf, *currentFileLines, *currentFileSize);
}
//---------------------------------------------------------------------------*/
//               © Gorbachev Nikita, November 2018 - April 2019              //
//---------------------------------------------------------------------------*/
