/*
  to do от Дединского (переписанный)
  -юнит тесты
  -режимы
  -повышение понижение в одну функцию
  -изменить дамп
  -деф PRINT
  -лог инфо при ошибке
  -глобальные переменные для увеличения и понижения
  -отключаемые ошибки при вызове функций
*/

//------------------------------------------------------------------------------
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

#include "Stack.cpp"

/// RELEASE defines version of the project \
    if defined erases supporting debug functions \
    if not defined many supporting functions will be able to work
#define RELEAS

#if defined RELEASE
  #define iPrint(inf);
  #define stackTesting(tests);
#else
  #define iPrint(inf) printf ((inf));
  #include "StackUnitTesting.cpp";
#endif

int main()
{
  iPrint (" # This program includes functions for working with stack\n");

  stackTesting ();
  stack_t myfirststack = {};

  //stackCtor (&myfirststack);
  //(&myfirststack)->capacity = -100;
  //printf(" %d ",(&myfirststack)->capacity);
  //stackIsOk (&myfirststack);
  //stackPop (&myfirststack);

  return 1;
}
