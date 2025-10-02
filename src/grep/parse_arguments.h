#ifndef COMMON_CODE_H
#define COMMON_CODE_H
enum GREP_FLAGS { E, I, V, C, L, N, H, S, F, O };
void grep_magick(int *flags, char **patern, char **filename, int j, int k);
void parse_dance(int argc, char **argv, int *flags, char *e_arg[],
                 char *f_arg[], int e_count, int f_count);
void realloc_for_copybara(char ***destination, int increase_mem);

void parse_arguments(int *flags, int argc, char **argv) {
  char *e_arg[argc];
  char *f_arg[argc];
  for (int i = 0; i < argc; i++) {
    f_arg[i] = NULL;
    e_arg[i] = NULL;
  }
  int f_count = 0;
  int e_count = 0;
  int opt, option_index = 0;
  while ((opt = getopt_long(argc, argv, "e:ivcnlhsof:", NULL, 0)) != -1) {
    switch (opt) {
      case 'e':
        flags[E]++;
        e_arg[e_count] = optarg;
        e_count++;
        break;
      case 'i':
        flags[I]++;
        break;
      case 'v':
        flags[V]++;
        break;
      case 'c':
        flags[C]++;
        break;
      case 'l':
        flags[L]++;
        break;
      case 'n':
        flags[N]++;
        break;
      case 'h':
        flags[H]++;
        break;
      case 's':
        flags[S]++;
        break;
      case 'o':
        flags[O]++;
        break;
      case 'f':
        flags[F]++;
        f_arg[f_count] = optarg;
        f_count++;
        break;
      default:
        break;
    }
    option_index++;
  }
  parse_dance(argc, argv, flags, e_arg, f_arg, e_count, f_count);
}
void parse_dance(int argc, char **argv, int *flags, char *e_arg[],
                 char *f_arg[], int e_count, int f_count) {
  char **pattern = (char **)malloc(sizeof(char *));
  char **filename = (char **)malloc(sizeof(char *));
  int j = 0;  // number of patterns
  int k = 0;  // number of filenames
  int start = 0;
  int end = 0;
  FILE *fp = NULL;
  if (flags[E] == 0 && flags[F] == 0) {
    for (int i = 1; i < argc; i++) {
      if (argv[i][0] != '-') {
        if (j == 0) {
          realloc_for_copybara(&pattern, j);
          pattern[j] = argv[i];
          j++;
        } else {
          realloc_for_copybara(&filename, k);
          filename[k] = argv[i];
          k++;
        }
      }
    }
  } else {
    if (flags[E] > 0 || flags[F] > 0) {
      start = j;
      for (int i = 0; i < e_count; i++) {
        realloc_for_copybara(&pattern, j);
        pattern[j] = e_arg[i];
        j++;
      }
      end = j;
      for (int i = 0; i < f_count; i++) {
        if ((fp = fopen(f_arg[i], "r")) != NULL) {
          while (1) {
            ssize_t read = 0;
            size_t len = 0;
            realloc_for_copybara(&pattern, j);
            pattern[j] = NULL;
            read = getline(&pattern[j], &len, fp);
            if (read != -1) {
              int e = strlen(pattern[j]);
              if (pattern[j][e - 1] == '\n') {
                pattern[j][e - 1] = 0;
              }
            } else {
              if (pattern[j]) {
                free(pattern[j]);
              }
              break;
            }

            j++;
          }
        }
      }
      if (fp != NULL) {
        fclose(fp);
      }

      for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i - 1], "-e") != 0 && strcmp(argv[i - 1], "-f") != 0 &&
            argv[i][0] != '-') {
          realloc_for_copybara(&filename, k);
          filename[k] = argv[i];
          k++;
        }
      }
    }
  }

  grep_magick(flags, pattern, filename, j, k);
  if (flags[F] > 0) {
    for (int i = 0; i < j; i++) {
      if (i >= start && i < end) {
        ;
      } else {
        if (pattern[i] != NULL) {
          free(pattern[i]);
        }
      }
    }
  }
  if (pattern) free(pattern);
  if (filename) free(filename);
}

void realloc_for_copybara(char ***destination, int increase_mem) {
  char **temp = NULL;
  temp = (char **)realloc(*destination, (increase_mem + 1) * sizeof(char *));
  if (temp != NULL) {
    *destination = temp;
  } else {
    printf("error allocating memory!");
  }
}

#endif
