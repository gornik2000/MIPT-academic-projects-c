/*!
//  \file Stack_Header.cpp
//
//  Contains headers of structure stack and functions for working with stack
*/

//------------------------------------------------------------------------------

#include <stdio.h>
#include <cstdlib>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------

FILE *ST_LOG_FILE = fopen("St_log_file.txt", "w");

/*!
//  \typedef size_t data_t;
//
//  Typedefs size_t as data_t - type of data elements of stack
*/
typedef size_t data_t;
/*!
//  \def DATA_ST_TYPE
//
//  Gives information about data_t type for outputting
*/
#define DATA_ST_TYPE "%d"
/*!
//  \typedef size_t elem_t;
//
//  Typedefs size_t as elem_t - type of stack arguments,
//  which should have integer type
//
//  \warning Elem_t should be defined as integer type
*/
typedef size_t elem_t;

/*!
//  \struct myStack
//
//  Structure stack, made with doctrine first in, last out
//  Has infromation about stack data, size, capacity and pointer
//  to file with logs
*/
struct myStack
{
  data_t canaryOne;

  data_t *data;
  elem_t size;
  elem_t capacity;

  data_t canaryTwo;
};
/*!
//  \typedef struct myStack stack_t;
//
//  Typedefs  stack_t as structure stack
*/
typedef struct myStack stack_t;

//------------------------------------------------------------------------------

/*!
//  \var const int MINSTACKAPACITY
//
//  Minimal size of stack capacity
*/
const int MINSTACKCAPACITY = 8;
/*!
//  \var const int POISON
//
//  Variable with which we clear memory
*/
const int POISON = 0;
/*!
// \var const float increaseCapacityCoefficient
//
//  Coefficient of capacity increasing
*/
const float increaseCapacityCoefficient = 2;
/*!
// \var const float decreaseCapacityCoefficient
//
//  Coefficient of capacity decreasing
*/
const float decreaseCapacityCoefficient = 0.5;
/*!
// \var data_t CANARY
//
//  A canary needed for checking stack correction
*/
const data_t CANARY = 5553535;

//------------------------------------------------------------------------------

void stCtor (stack_t *s);
void stDtor (stack_t *s);

void  stPush    (stack_t *s, elem_t number);
elem_t stPop (stack_t *s);
elem_t stSafePop (stack_t *s);

void stChangeCapacity (stack_t *s, float changeValue);

elem_t stReturnCapacity (stack_t *s);
elem_t stReturnSize     (stack_t *s);
void stackFPrintData    (stack_t *s, FILE *file = ST_LOG_FILE);

//------------------------------------------------------------------------------

#ifdef RELEASE
  #define stackCtor(s) \
    stCtor((s));

  #define stackDtor(s) \
    stDtor((s));

  #define stackPush(s, element) \
    stPush((s), (element));

  #define stackPop(s) \
    stPop((s));

  #define stackReturnCapacity(s) \
    stReturnCapacity((s));

  #define stackReturnSize(s) \
    stReturnSize((s));

  #define stackChangeCapacity(s, value) \
    stChangeCapacity((s), (value));
#else
  #include "Dump.cpp"

  #define stackCtor(s) \
  { \
    stCtor((s)); \
    (s)->canaryOne = CANARY; \
    (s)->canaryTwo = CANARY; \
    stackIsOk ((s)); \
  }

  #define stackDtor(s) \
  { \
    const char *error = stackIsOk ((s)); \
    if (error == 0) \
    { \
      stDtor ((s)); \
    } \
  }

  #define stackPush(s, element) \
  { \
    const char *error = stackIsOk ((s)); \
    if (error == 0) \
    { \
      stPush((s), (element)); \
      stackIsOk ((s)); \
    } \
  }

  #define stackPop(s) \
  { \
    stSafePop((s)) \
  }

  #define stackChangeCapacity(s, value)\
  { \
    const char *error = stackIsOk ((s)); \
    if (error == 0) \
    { \
      stChangeCapacity((s), (value)); \
      stackIsOk ((s)); \
    } \
  }

  #define stackReturnCapacity(s)\
  { \
    const char *error = stackIsOk ((s)); \
    if (error == 0) \
    { \
      stReturnCapacity((s)); \
    } \
  }

  #define stackReturnSize(s) \
  { \
    const char *error = stackIsOk ((s)); \
    if (error == 0) \
    { \
      stReturnSize((s)); \
    } \
  }
#endif


/// RELEASE defines version of the project \
    if defined erases supporting debug functions \
    if not defined many supporting functions will be able to work
#if defined RELEASE
  //#define errorDecoder(a, b, c, d, e);
  //#define stackIsOk(a) 0;
  //#define fopenforlogs(a, b) NULL;
  //#define fcloseforlogs(a);
#else
  //#define fopenforlogs(a, b) fopen(a, b);
  //#define fcloseforlogs(a) fclose(a);
#endif

