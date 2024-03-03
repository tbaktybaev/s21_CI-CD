#include "s21_grep.h"

int main(int argc, char* argv[]) {
  if (argc > 2) gpep(argc, argv);

  return 0;
}

void gpep(int argc, char* argv[]) {
  flags options = {0};
  if (!ParseOptions(argc, argv, &options)) {
    if (options.PatternTrue == 0) {  // если нет флагов -e и -f
      sprintf(options.pattern, "%s", argv[optind]);
      optind++;
    }
    if (argc - optind > 2)
      options.MoreThanTwoFiles = 1;  // >= 2 файлов в поиске
    if (options.MoreThanTwoFiles && !options.h)
      options.FileNamesPrint = 1;  // вывод названия файла
    while (optind < argc) {
      FileReader(argv, options);
      optind++;
    }
  }
}

int ParseOptions(int argc, char* argv[], flags* options) {
  int opt = 0, flag_error = 0;

  const char* short_options = "e:ivclnhsf:o";
  while ((opt = getopt_long(argc, argv, short_options, 0, 0)) != -1) {
    switch (opt) {
      case 'e':  // Шаблон
        options->e = 1;
        parseE(options);
        options->PatternTrue = 1;
        break;
      case 'i':  // Игнор регистра
        options->i = 1;
        break;
      case 'v':  // Инвертация поиска
        options->v = 1;
        break;
      case 'c':  // Вывод кол-ва совп. строк
        options->c = 1;
        break;
      case 'l':  // Вывод совп. файлов
        options->l = 1;
        break;
      case 'n':  // Вывод номера строки в файле
        options->n = 1;
        break;
      case 'h':  // Без вывода имени файла
        options->h = 1;
        break;
      case 's':  // Без вывода ошибки чтения файла
        options->s = 1;
        break;
      case 'f':  // Читать шаблон поиска из файла
        options->f = 1;
        FilePattern(options, optarg);
        options->PatternTrue = 1;
        break;
      case 'o':  // Вывод только совпадающей части
        options->o = 1;
        break;
      default:
        flag_error = 1;
        fprintf(stderr, "usage: grep [options] template [file_name]\n");
        break;
    }
  }
  if (options->v || options->c || options->l) options->o = 0;
  return flag_error;
}

void FileReader(char* argv[], flags options) {
  FILE* fp = NULL;
  fp = fopen(argv[optind], "rb");
  if (fp) {
    int reg_flags = REG_EXTENDED;
    regex_t compiled;
    if (options.i) reg_flags = REG_ICASE | REG_EXTENDED;  // -i
    if (regcomp(&compiled, options.pattern, reg_flags) == 0) {
      char* line_cur = NULL;
      size_t line_len;
      int line_count = 0, error = 1, match_found = 0;
      while (getline(&line_cur, &line_len, fp) != -1) {
        line_count++;
        if (options.o) {
          optO(line_cur, compiled, options, line_count, argv);
        } else {
          if (line_cur) error = regexec(&compiled, line_cur, 0, 0, 0);
          if (!options.v && !error) {
            output(line_cur, argv, options, &match_found, line_count);
          } else if (options.v && error) {  // -v
            output(line_cur, argv, options, &match_found, line_count);
          }
        }
      }

      if (options.c) {  // -c
        if (options.FileNamesPrint) printf("%s:", argv[optind]);
        if (options.l && match_found) match_found = 1;  // -lc
        printf("%d\n", match_found);
      }

      if (options.l && match_found)  // -l
        printf("%s\n", argv[optind]);

      if (line_cur != NULL) free(line_cur);
      regfree(&compiled);
    }
    fclose(fp);
  } else if (!options.s) {  // -s
    fprintf(stderr, "%s: No such file or directory\n", argv[optind]);
  }
}

void output(char* line_cur, char* argv[], flags options, int* match_found,
            int line_count) {
  if (!options.l && !options.c) {
    if (options.FileNamesPrint) printf("%s:", argv[optind]);  // -h
    if (options.n) printf("%d:", line_count);                 // -n
    char* newline = strchr(line_cur, '\n');  // проверка на перевод строки
    if (newline) *newline = '\0';
    printf("%s\n", line_cur);
  }
  (*match_found)++;
}

void optO(char* string, regex_t compiled, flags options, int line_count,
          char* argv[]) {
  regmatch_t pmatch = {0};
  int opener = 0;
  while (regexec(&compiled, string, 1, &pmatch, 0) != REG_NOMATCH) {
    if (!opener) {
      if (options.FileNamesPrint) printf("%s:", argv[optind]);  // -h
      if (options.n) printf("%d:", line_count);                 // -n
    }
    for (int i = pmatch.rm_so; i < pmatch.rm_eo; i++) {
      printf("%c", string[i]);
      opener++;
    }
    printf("\n");
    string += pmatch.rm_eo;
  }
}

void parseE(flags* options) {
  if ((strlen(options->pattern) + strlen(optarg)) < (BUFF - 2)) {
    if (options->PatternTrue) strcat(options->pattern, "|");
    if (strlen(optarg) == 0)
      strcat(options->pattern, ".");
    else
      strcat(options->pattern, optarg);
  }
}

void FilePattern(flags* options, char* optarg) {
  FILE* fp = NULL;
  fp = fopen(optarg, "rb");
  if (fp) {
    char line_cur[1] = {'\0'};
    char cur = '\0', last = '\n';
    while ((cur = fgetc(fp)) != EOF) {
      if (strlen(options->pattern) < (BUFF - 2)) {
        if (options->PatternTrue && (last == '\n')) {
          strcat(options->pattern, "|");
        }
        if (cur != '\n') {
          line_cur[0] = cur;
          strcat(options->pattern, line_cur);
        }
        if (cur == '\n' && last == '\n') {
          strcat(options->pattern, ".");
        }
        if (cur == '\n') options->PatternTrue = 1;
        last = cur;
      }
    }
    fclose(fp);
  } else if (!options->s) {
    fprintf(stderr, "%s: No such file or directory\n", options->f_file);
  }
}
