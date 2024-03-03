#include "file_reader.h"

#include <stdlib.h>

FileStr ReadFile(FILE* file) {
  FileStr file_str = {0};
  while (!feof(file)) {
    char c = fgetc(file);
    if (!feof(file)) {
      AddChar(&file_str, c);
    }
  }
  return file_str;
}
void AddChar(FileStr* file_str, char c) {
  if (file_str->content == NULL) {
    file_str->content = (char*)malloc(sizeof(char));
    file_str->length = 0;
  } else {
    file_str->content = (char*)realloc(file_str->content, file_str->length + 1);
  }
  file_str->content[file_str->length] = c;
  file_str->length++;
}
