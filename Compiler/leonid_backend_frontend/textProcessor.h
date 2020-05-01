#ifndef TEXTPROCESSOR_H

#define TEXTPROCESSOR_H

char **readTextFromFile(const char *file);
int sizeofFile(const char *file);
int countLines(char **lines);
void writeToFile(const char *file, const char **lines);
int countCharsInStr(const char c, const char *str);
int getLine(char buff[], int maxSize);
void sort(char *lines[], int left, int right, int (*comp)(const char *, const char *));
int strComp(const char *first, const char *second);
int strCompFromEnd(const char *first, const char *second);
char *skipSpaces(char *ptr);

#include "textProcessor.c"

#endif
