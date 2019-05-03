//---------------------------------------------------------------------------*/
#include "Sort.cpp"
#include <math.h>
#include <ctype.h>
//---------------------------------------------------------------------------*/
static const unsigned char sTable[256] =
{
  0xa3,0xd7,0x09,0x83,0xf8,0x48,0xf6,0xf4,0xb3,0x21,0x15,0x78,0x99,0xb1,0xaf,0xf9,
  0xe7,0x2d,0x4d,0x8a,0xce,0x4c,0xca,0x2e,0x52,0x95,0xd9,0x1e,0x4e,0x38,0x44,0x28,
  0x0a,0xdf,0x02,0xa0,0x17,0xf1,0x60,0x68,0x12,0xb7,0x7a,0xc3,0xe9,0xfa,0x3d,0x53,
  0x96,0x84,0x6b,0xba,0xf2,0x63,0x9a,0x19,0x7c,0xae,0xe5,0xf5,0xf7,0x16,0x6a,0xa2,
  0x39,0xb6,0x7b,0x0f,0xc1,0x93,0x81,0x1b,0xee,0xb4,0x1a,0xea,0xd0,0x91,0x2f,0xb8,
  0x55,0xb9,0xda,0x85,0x3f,0x41,0xbf,0xe0,0x5a,0x58,0x80,0x5f,0x66,0x0b,0xd8,0x90,
  0x35,0xd5,0xc0,0xa7,0x33,0x06,0x65,0x69,0x45,0x00,0x94,0x56,0x6d,0x98,0x9b,0x76,
  0x97,0xfc,0xb2,0xc2,0xb0,0xfe,0xdb,0x20,0xe1,0xeb,0xd6,0xe4,0xdd,0x47,0x4a,0x1d,
  0x42,0xed,0x9e,0x6e,0x49,0x3c,0xcd,0x43,0x27,0xd2,0x07,0xd4,0xde,0xc7,0x67,0x18,
  0x89,0xcb,0x30,0x1f,0x8d,0xc6,0x8f,0xaa,0xc8,0x74,0xdc,0xc9,0x5d,0x5c,0x31,0xa4,
  0x70,0x88,0x61,0x2c,0x9f,0x0d,0x2b,0x87,0x50,0x82,0x54,0x64,0x26,0x7d,0x03,0x40,
  0x34,0x4b,0x1c,0x73,0xd1,0xc4,0xfd,0x3b,0xcc,0xfb,0x7f,0xab,0xe6,0x3e,0x5b,0xa5,
  0xad,0x04,0x23,0x9c,0x14,0x51,0x22,0xf0,0x29,0x79,0x71,0x7e,0xff,0x8c,0x0e,0xe2,
  0x0c,0xef,0xbc,0x72,0x75,0x6f,0x37,0xa1,0xec,0xd3,0x8e,0x62,0x8b,0x86,0x10,0xe8,
  0x08,0x77,0x11,0xbe,0x92,0x4f,0x24,0xc5,0x32,0x36,0x9d,0xcf,0xf3,0xa6,0xbb,0xac,
  0x5e,0x6c,0xa9,0x13,0x57,0x25,0xb5,0xe3,0xbd,0xa8,0x3a,0x01,0x05,0x59,0x2a,0x46
};
//---------------------------------------------------------------------------*/
/* 0                                   */
int hashFunction1  (char *str, int size);
/* str[0]                              */
int hashFunction2  (char *str, int size);
/* str Length                          */
int hashFunction3  (char *str, int size);
/* str sum                             */
int hashFunction4  (char *str, int size);
/* average char                        */
int hashFunction5  (char *str, int size);
/* ded function                        */
int hashFunction6  (char *str, int size);
/* gnu                                 */
int hashFunction7  (char *str, int size);
/* internet extra                      */
int hashFunction8  (char *str, int size);
/* own function                        */
int hashFunction9  (char *str, int size);
/* binary optimized own function       */
int hashFunction10 (char *str, int size);
/* binary optimized gnu                */
int hashFunction11 (char *str, int size);
//---------------------------------------------------------------------------*/
int hashFunction1 (char *str, int size)
{
  assert (str != NULL);

  return 0;
}
//---------------------------------------------------------------------------*/
int hashFunction2 (char *str, int size)
{
  assert (str != NULL);

  return *str % size;
}
//---------------------------------------------------------------------------*/
int hashFunction3 (char *str, int size)
{
  assert (str != NULL);

  return strLength (str) % size;
}
//---------------------------------------------------------------------------*/
int hashFunction4 (char *str, int size)
{
  assert (str != NULL);

  long int sum = 0;

  for (int i = 0; *str != '\0'; i++, str++)
  {
    if (isalpha(*str))
      sum = (sum + *str - 'a');
    else
      sum = (sum + *str);
  }
  return sum % size;
}
//---------------------------------------------------------------------------*/
int hashFunction5 (char *str, int size)
{
  assert (str != NULL);

  long int sum = 0;
  int i   = 1;

  while (*str != '\0')
  {
    sum = (sum + *str);
    str++;
    i++;
  }

  return (sum / i) % size;
}
//---------------------------------------------------------------------------*/
/* random one */
int hashFunction6 (char *str, int size)
{
  assert (str != NULL);

  int seed = 131313;
  int hash = 0;

  for (int i = 1; *str != '\0'; i++, str++)
  {
     hash = ((hash * seed) + *str + i) % size;
  }

  return hash;
}
//---------------------------------------------------------------------------*/
/* gnu */
int hashFunction7 (char *str, int size)
{
  assert (str != NULL);

  unsigned long int count = 5381;
  //int strSize = strLength (str);

  for(; *str != 0; str++)
  {
    count = (*str + (count << 5) + count);
  }
  return (count % size);
}
//---------------------------------------------------------------------------*/
/* extra one from internet */
int hashFunction8 (char *str, int size)
{
  unsigned int hash = 0;
  unsigned int rotate = 2;
  unsigned int seed = 0x1A4E41U;

  int i = 0;
  while (*str != '\0')
  {

    hash += sTable[(*str + i) & 255];
    hash = (hash << (32 - rotate) ) | (hash >> rotate);
    hash = (hash + i ) * seed;
    i++;
    str++;
  }
  return (hash + i) * seed % size;
}
//---------------------------------------------------------------------------*/
/* own function */
int hashFunction9 (char *str, int size)
{
  unsigned long int hash = 0;

  while (*str != 0)
  {
    hash = (*str) ^ ((hash << 31) | (hash >> 1));
    str ++;
  }

  return hash % size;
}
//---------------------------------------------------------------------------*/
/* binary optimized own function */
int hashFunction10 (char *str, int size)
{
  unsigned int hash = 0;//---------------------------------------------------------------------------*/

  __asm__ ("hash10_circle_start:     \n"
           "ror  $1, %%eax           \n"
           "xor (%%ebx), %%al        \n"
           "inc  %%ebx               \n"
           "cmpb $0, (%%ebx)         \n"
           "jnz  hash10_circle_start \n"
          :"=a"(hash)
          :"a" (hash), "b"(str)
          );

  return hash % size;
}
//---------------------------------------------------------------------------*/
/* binary optimized gnu */
int hashFunction11 (char *str, int size)
{
  assert (str != NULL);

  unsigned long int hash = 5381;

  for(; *str != 0; str++)
  {
    __asm__ ("movl %%eax, %%ecx  \n"
             "sal  $5,    %%eax  \n"
             "addl %%ecx, %%eax  \n"
             "addl %%ebx, %%eax  \n"
            :"=a"(hash)
            :"a" (hash), "b"(*str)
            );
  }
  return (hash % size);
}
//---------------------------------------------------------------------------*/
//               © Gorbachev Nikita, November 2018 - April 2019              //
//---------------------------------------------------------------------------*/
