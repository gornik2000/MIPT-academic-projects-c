//------------------------------------------------------------------------------
/*!
//  \file FileIO.cpp
//
//  File which contains functions for file input and output
*/
//------------------------------------------------------------------------------

#include <io.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int fileSize (FILE *file);

int textInLines (char **text, int textSize);

int foutputText (char **text, const char *name = "out.txt");

char **createTextCopy (char **text, int lines);

char **createText (char *buf, int lines, int textSize);

char **createTextFromFile (const char *fileName, int *currentFileSize = 0, \
                                                 int *currentFileLines = 0);

//------------------------------------------------------------------------------
/*!
//  \fn int fileSize (FILE *file)
//
//  Function used to find the file length in bytes
//
//  \param *file Pointer to file which length should be find
//  \return file Length of the file in bytes
*/
//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------
/*!
//  \fn int bufInLines (char *buf, int textSize)
//
//  Function used to change all \n to \0 in buffer and to find the amount of
//  lines in it
//
//  \param *buf Pointer to buffer that should be changed
//  \param textsize Size of the buffer
//  \return Number of lines in buffer
*/
//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------
/*!
//  \fn void foutputText (char **text, FILE *file)
//
//  Print text into file, with \n on places where \0 are
//
//  \param **text Pointer to text (array of strings)
//  \param *name The name of file in which we should print text
//  \return 1 if function worked correctly
*/
//------------------------------------------------------------------------------

int foutputText (char **text, const char *name)
{
  assert (text);
  assert (name);

  /* opens file */
  FILE *file = fopen (name, "w");
  assert (file);

  /* inputs text into file by lines */
  for (int lines = 0; text[lines] != 0; lines ++)
  {
    fprintf (file, "%s\n", text[lines]);
  }

  fclose (file);

  return 0;
}

//------------------------------------------------------------------------------
/*!
//  \fn char **createTextCopy (char **text, int lines)
//
//  Create the copy of the text (array of strings)
//
//  \param **text Pointer to text (array of strings)
//  \param lines The amount of lines in text (array length)
//  \warning The text copy memory should be freed
//  \return Text copy
*/
//------------------------------------------------------------------------------

char **createTextCopy (char **text, int lines)
{
  assert (text);

  /* creates memory for text copy */
  char **textCopy = (char **)calloc (lines + 1, sizeof (textCopy));
  assert (textCopy);

  /*  makes a copy */
  for (int i = 0; i <= lines; i++)
  {
      textCopy[i] = text[i];
  }

  return textCopy;
}


//------------------------------------------------------------------------------
/*!
//  \fn char **createText (char *buf, int lines, int textSize)
//
//  Create text (array of strings) from buf
//
//  \param *buf Pointer to buf from witch text should created
//  \param lines The amount of lines in future text (array length)
//  \param textSize The size of the buf
//  \warning The text memory should be freed
//  \return Created text
*/
//------------------------------------------------------------------------------

char **createText (char *buf, int lines, int textSize)
{
  assert (buf);

  /* creates memory for text */
  char **text = (char **)calloc(lines + 1, sizeof (text));
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


//------------------------------------------------------------------------------
/*!
//  \fn char **createTextFromFile (const char *fileName, int *currentFileSize = 0, int *currentFileLines = 0)
//
//  Create text (array of strings) from .txt file and gives fileSize and
//  fileLines (amount of lines) if needed
//
//  \param *fileName The name of file from witch we should get text
//  \param currentFileSize The address of var where FileSize should be putted
//  \param currentTextSize The address of var where FileLines should be putted
//  \warning The text memory and text[0] memory should be freed
//  \return Created text
*/
//------------------------------------------------------------------------------

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
  FILE *file = fopen (fileName, "r");
  assert (file);

  /* finds file size */
  *currentFileSize = fileSize (file);

  /* creates memory for text buff with \0 in the beginning*/
  char *buf = (char *)calloc(*currentFileSize + 1, sizeof (buf));
  assert (buf);
  buf ++;

  /* puts text from file into buff */
  int readSize = fread (buf, sizeof (char), *currentFileSize, file);
  //assert (readSize == *currentFileSize);
  //printf ("read %d count %d", readSize, *currentFileSize);
  *currentFileSize = readSize;

  fclose (file);

  /* finds number of lines in buf and change \n to \0 */
  *currentFileLines = bufInLines (buf, *currentFileSize);

  /* returns array of line pointers, connecting buff and text */
  return createText (buf, *currentFileLines, *currentFileSize);
}
