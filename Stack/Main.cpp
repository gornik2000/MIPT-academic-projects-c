/*!
//  \file Main.cpp
//
//  Main file which contains description of the program
*/
/*!
//  \mainpage Description
//
//  \author Nikita Gorbachev
//  \version 1.0
//  \date October 2018
//
//  This project includes functions for using structure stack
*/
//------------------------------------------------------------------------------
/// RELEASE defines version of the project \
    if defined erases supporting debug functions \
    if not defined many supporting functions will be able to work
//#define RELEASE
typedef double data_t;

#include "Stack.cpp"

#if defined RELEASE
  #define iPrint(inf);
  #define stackTesting(tests);
#else
  #define iPrint(inf) printf ((inf));
#endif


int main()
{
  iPrint (" # Created by Nikita Gorbachev\n");
  iPrint (" # This program includes functions for working with stack\n");

  stackTesting ();
  stack_t myfirststack = {};

  stackCtor (&myfirststack);
  stackDtor (&myfirststack);

  return 1;
}
