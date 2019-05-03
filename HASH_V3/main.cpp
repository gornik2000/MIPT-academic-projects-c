//---------------------------------------------------------------------------*/
//  -O0 -g -fno-omit-frame-pointer -gdwarf-2 -glevel
//---------------------------------------------------------------------------*/
const char *FILE_DATA_NAME = "data_in/improved_data.txt";
//const char *FILE_DATA_NAME = "data_in/data.txt";
const char *FILE_STAT_NAME = "data_out/stat_data.txt";
//---------------------------------------------------------------------------*/
#include <stdio.h>
//---------------------------------------------------------------------------*/
#include "errors.cpp"
#include "hash_table.cpp"
#include "FileIO.cpp"
#include "hashFunctions.cpp"
//---------------------------------------------------------------------------*/
const char *FILE_OUT_NAME = "data_out/out11.CSV";
//---------------------------------------------------------------------------*/
int main ()
{
  printf (" # Start\n");
  printf (" # Hash functions program\n");

  int fileSize  = 0;
  int fileLines = 0;

  char **text = createTextFromFile (FILE_DATA_NAME, &fileSize, &fileLines);
  if (text == NULL)
  {
    printf (" # Got NULL data ptr \n");
    return 0;
  }

  map *m = mapCtor (hashFunction7);

  for (int i = 0; i < fileLines; i++)
  {
    mapAdd (m, text[i]);
  }

  FILE *fileOut = fopen (FILE_OUT_NAME, "w");
  mapToFile (m, fileOut);
  fclose    (fileOut);

  free   (text[0]);
  memset (text, 0, fileLines);
  free   (text);
  text = NULL;

  mapDtor (m);

  printf (" # Program finished correctly\n");

  return 0;
}
//---------------------------------------------------------------------------*/
//               © Gorbachev Nikita, November 2018 - April 2019              //
//---------------------------------------------------------------------------*/
