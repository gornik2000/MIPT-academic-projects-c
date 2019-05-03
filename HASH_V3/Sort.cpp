//---------------------------------------------------------------------------*/
#include <ctype.h>
#include <string.h>
//---------------------------------------------------------------------------*/
int strLength (char *str);

inline int strCompare (const void *lineOne, const void *lineTwo);

inline int strCompareFromEnd (const void *lineOne, const void *lineTwo);
//---------------------------------------------------------------------------*/
inline int strLength (char *str)
{
  int length = 0;
  for (; *str != '\0'; length ++, str ++);

  return length;
}
//---------------------------------------------------------------------------*/
inline int strCompare (const void *lineOne, const void *lineTwo)
{
  /* makes letters */
  register char *str1 = *(char **)lineOne;
  register char *str2 = *(char **)lineTwo;

  /* finds the first letters */
  for (; !isalpha (*str1) && *str1 != '\0'; ++ str1);
  for (; !isalpha (*str2) && *str2 != '\0'; ++ str2);

  /* compares till first non equal letters */
  while (*str1 == *str2 && *str1 != '\0')
  {
    for (; !isalpha (*str1++) && *str1 != '\0';);
    for (; !isalpha (*str2++) && *str2 != '\0';);
  }

  return *str1 - *str2;
}
//---------------------------------------------------------------------------*/
inline int strCompareFromEnd(const void *lineOne, const void *lineTwo)
{
  /* makes strings */
  register char *str1 = *(char **)lineOne;
  register char *str2 = *(char **)lineTwo;

  /* remove pointers to the end */
  str1 += strLength (str1);
  str2 += strLength (str2);

  /* finds the first letter */
  for (; !isalpha (*--str1) && *str1 != '\0'; );
  for (; !isalpha (*--str2) && *str2 != '\0'; );

  /* compares till first non equal letters */
  while (*str1 == *str2 && *str1 != '\0')
  {
    for (; !isalpha (*--str1) && *str1 != '\0'; );
    for (; !isalpha (*--str2) && *str2 != '\0'; );
  }

  return *str1 - *str2;
}
//---------------------------------------------------------------------------*/
//               © Gorbachev Nikita, November 2018 - April 2019              //
//---------------------------------------------------------------------------*/
