#ifndef S21_GREP_H
#define S21_GREP_H
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF 4096

typedef struct {
  int v;  // -v
  int i;  // -i
  int c;  // -c
  int l;  // -l
  int n;  // -n
  int h;  // -h
  int s;  // -s
  int o;  // -o
  int e;  // -e
  int f;  // -f
  int MoreThanTwoFiles;
  int FileNamesPrint;
  int PatternTrue;
  char pattern[BUFF];
  char* f_file;
} flags;

void gpep(int argc, char* argv[]);
int ParseOptions(int argc, char* argv[], flags* options);
void FileReader(char* argv[], flags options);
void output(char* line_cur, char* argv[], flags options, int* match_found,
            int line_count);
void parseE(flags* options);
void FilePattern(flags* options, char* optarg);
void optO(char* string, regex_t compiled, flags options, int line_count,
          char* argv[]);
#endif