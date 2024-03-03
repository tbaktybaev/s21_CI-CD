#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_reader.h"

typedef struct options {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
  int h;
} opt;

void displayHelp(void);
void parseOptions(int argc, char* argv[], opt* options);
void ProcessFile(FileStr* file_str, opt options);

int main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("Usage: %s [OPTION]... [FILE]...\n", argv[0]);
    printf("Try '%s --help' for more information.\n", argv[0]);
    return 1;
  } else {
    opt options = {0};
    parseOptions(argc, argv, &options);
    for (int i = optind; i < argc; ++i) {
      FILE* file = fopen(argv[i], "r");
      if (file == NULL) {
        printf("\ncat: %s: No such file or directory \n", argv[i]);
        continue;
      }
      FileStr file_str = ReadFile(file);
      fclose(file);
      ProcessFile(&file_str, options);
      free(file_str.content);
    }
  }
  return 0;
}

void parseOptions(int argc, char* argv[], opt* options) {
  int opt;
  int option_index;
  static struct option long_options[] = {{"number-nonblank", 0, 0, 'b'},
                                         {"number", 0, 0, 'n'},
                                         {"squeeze-blank", 0, 0, 's'},
                                         {"help", 0, 0, 'h'},
                                         {0, 0, 0, 0}};

  while ((opt = getopt_long(argc, argv, "benstvTEh", long_options,
                            &option_index)) != -1) {
    switch (opt) {
      case 'b':
        options->b = 1;
        break;
      case 'e':
        options->e = 1;
        options->v = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 't':
        options->t = 1;
        options->v = 1;
        break;
      case 'E':
        options->e = 1;
        break;
      case 'T':
        options->t = 1;
        break;
      case 'v':
        options->v = 1;
        break;
      case 'h':
        displayHelp();
        exit(EXIT_SUCCESS);
        break;
      default:
        fprintf(stderr, "Usage: %s [-benstvh] [FILE]...\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if (options->b) {
      options->n = 0;
    }
  }
}

void ProcessFile(FileStr* file_str, opt options) {
  int newline = '\n';
  int line_number = 1;
  int last_was_blank = 0;

  for (int i = 0; i < file_str->length; i++) {
    int c = file_str->content[i];
    if (options.b) {
      if ((newline == '\n') && (c != '\n')) {
        printf("%6d\t", line_number++);
      }
    }
    if (options.s) {
      if (c == '\n' && newline == '\n') {
        last_was_blank++;
        if (last_was_blank > 1) {
          continue;
        }
      } else {
        last_was_blank = 0;
      }
    }
    if (options.n) {
      if (newline == '\n') {
        printf("%6d\t", line_number++);
      }
    }

    if (options.t) {
      if (c == '\t') {
        printf("^");
        c = 'I';
      }
    }
    if (options.e) {
      if (c == '\n') printf("$");
    }
    if (options.v) {
      if ((c >= 0 && c < 9) || (c > 10 && c < 32) || (c > 126 && c <= 160)) {
        printf("^");
        if (c > 126) {
          c -= 64;
        } else {
          c += 64;
        }
      }
    }
    printf("%c", c);
    newline = c;
  }
}

void displayHelp(void) {
  const char* helpMsg =
      "NAME:\n"
      "\ts21_cat - concatenate files and print on the standard output: \n"
      "SYNOPSIS"
      "\ts21_cat [OPTION]... [FILE]...\n"
      "DESCRIPTION"
      "\tConcatenate FILE(s) to standart output.\n"
      "\tWith no FILE, or when FILE is -, read standard input.\n"
      "\t-b (GNU: --number-nonblank)\n"
      "\t\tnumber nonempty output lines, overrides -n"
      "\t-e assumes and -v (GNU only: -E is the same, but without -v)\n"
      "\t\tdisplays the end-of-line characters as $\n"
      "\t-n (GNU: --number)\n"
      "\t\tnumber all output lines"
      "\t-s (GNU: --squeeze-blank)\n"
      "\t\tcompresses several adjacent blank lines"
      "\t-t assumes and -v (GNU: -T is the same, but without -v)\n"
      "\t\talso displays tabs as ^I"
      "\t-? (GNU: --help)\n"
      "\t\tdisplay this help and exit";
  printf("%s", helpMsg);
}
