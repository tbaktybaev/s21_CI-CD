#ifndef FILE_READER_H
#define FILE_READER_H

#include <stdio.h>

typedef struct {
  char* content;
  int length;
} FileStr;

FileStr ReadFile(FILE* file);
void AddChar(FileStr* file_str, char c);
#endif /* FILE_READER_H */
