const char *FILE_DATA_NAME = "data.txt";
const char *FILE_OUT_NAME = "out.CSV";


#include <stdio.h>
#include "errors.cpp"
#include "hash_table.cpp"
#include "FileIO.cpp"
#include "hashFunctions.cpp"

//-----------------------------------------------------------------------------

int main ()
{
  printf (" # Start\n");

  int fileSize  = 0;
  int fileLines = 0;
  char **text = createTextFromFile (FILE_DATA_NAME, &fileSize, &fileLines);

  map *m1 = mapCtor ();
  map *m2 = mapCtor ();
  map *m3 = mapCtor ();
  map *m4 = mapCtor ();
  map *m5 = mapCtor ();
  map *m6 = mapCtor ();

  for (int i = 0; i < fileLines - 1; i++)
  {
    mapAdd (m1, text[i], hashFunction1 (text[i], m1->maxSize));
    mapAdd (m2, text[i], hashFunction2 (text[i], m2->maxSize));
    mapAdd (m3, text[i], hashFunction3 (text[i], m3->maxSize));
    mapAdd (m4, text[i], hashFunction4 (text[i], m4->maxSize));
    mapAdd (m5, text[i], hashFunction5 (text[i], m5->maxSize));
    mapAdd (m6, text[i], hashFunction6 (text[i], m6->maxSize));
  }

  mapPrint (m1);
  mapPrint (m2);
  mapPrint (m3);
  mapPrint (m4);
  mapPrint (m5);
  mapPrint (m6);

  FILE *fileOut = fopen (FILE_OUT_NAME, "w");
  //mapToFile (m1, fileOut);
  //mapToFile (m2, fileOut);
  //mapToFile (m3, fileOut);
  //mapToFile (m4, fileOut);
  //mapToFile (m5, fileOut);
  mapToFile (m6, fileOut);
  fclose (fileOut);

  //m1 = mapDtor (m1);
  m2 = mapDtor (m2);
  m3 = mapDtor (m3);
  m4 = mapDtor (m4);
  m5 = mapDtor (m5);
  m6 = mapDtor (m6);

  printf (" # Hash functions program\n");

  return 0;
  //*/
}
