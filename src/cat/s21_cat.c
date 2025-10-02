#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <unistd.h>

void simple_cat(FILE *fp, int flag);
void et_flag(FILE *fp, int flag);
void nb_flag(FILE *fp, int flag);

int main(int argc, char *argv[]) {
  FILE *fp;
  int opt;
  int flag;
  int mode = -1;
  int option_index;
  if (argc < 2) {
    printf("ERROR usage: cat -opt [FILE]");
    return 1;
  }
  const char *short_options = "bEensTt";

  const struct option long_options[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {NULL, 0, NULL, 0}};

  while (optind < argc) {
    if ((opt = getopt_long(argc, argv, short_options, long_options,
                           &option_index)) != -1 &&
        mode < 0) {
      switch (opt) {
        case 'b':
        case 'n':
          flag = opt;
          mode = 1;
          break;
        case 'e':
        case 't':
        case 'E':
        case 'T':
          mode = 69;
          flag = opt;
          break;
        case 's':
          mode = 0;
          flag = opt;
          break;
        default:
          return 1;
      }
    } else {
      optind++;
    }
  }

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] != '-') {
      if ((fp = fopen(argv[i], "r")) == NULL) {
        printf("ERROR: %s no such file or directory!\n", argv[i]);
      } else {
        switch (mode) {
          case 69:
            et_flag(fp, flag);
            break;
          case 1:
            nb_flag(fp, flag);
            break;
          case 0:
          case -1:
            simple_cat(fp, flag);
            break;
        }
      }
    }
  }
  fclose(fp);
}

void et_flag(FILE *fp, int flag) {
  int charrr;
  int et_mode;
  int non_printable_mode = 0;
  if (flag == 'E') {
    et_mode = 1;
  } else {
    if (flag == 'T') {
      et_mode = 2;
    } else {
      if (flag == 'e') {
        et_mode = 3;
      } else {
        et_mode = 4;
      }
    }
  }
  while ((charrr = fgetc(fp)) != EOF) {
    if (isprint(charrr)) {
      putchar(charrr);
    } else {
      if (et_mode == 3 || et_mode == 1) {
        if (charrr == '\n') {
          putchar('$');
          putchar(charrr);
        } else {
          if (charrr == '\t') {
            putchar(charrr);
          } else {
            if (et_mode == 3) {
              non_printable_mode = 1;
            } else {
              putchar(charrr);
            }
          }
        }
      } else {
        if (et_mode == 2 || et_mode == 4) {
          if (charrr == '\t') {
            printf("^I");
          } else {
            if (charrr == '\n') {
              putchar(charrr);
            } else {
              if (et_mode == 4) {
                non_printable_mode = 1;
              } else {
                putchar(charrr);
              }
            }
          }
        }
      }
      if (non_printable_mode == 1) {
        if (charrr == 127) {
          printf("^?");
        } else {
          if (charrr < 32) {
            printf("^%c", charrr + 64);
          } else {
            if (charrr - 64 < 95) {
              printf("M-^%c", charrr - 64);
            } else {
              (charrr - 127 != 127 ? printf("M-%c", charrr - 128)
                                   : printf("M-^?"));
            }
          }
        }
        non_printable_mode = 0;
      }
    }
  }
}

void nb_flag(FILE *fp, int flag) {
  int nb_mode;
  int line = 1;
  int prev_charrr = '\n';
  int charrr;
  if (flag == 'n') {
    nb_mode = 1;
  } else {
    if (flag == 'b') {
      nb_mode = 2;
    }
  }
  if (nb_mode == 1) {
    while ((charrr = fgetc(fp)) != EOF) {
      if (prev_charrr == '\n') {
        printf("%6d\t", line);
      }
      putchar(charrr);
      prev_charrr = charrr;
      if (charrr == '\n') {
        line++;
      }
    }

  } else {
    if (nb_mode == 2) {
      while ((charrr = fgetc(fp)) != EOF) {
        if (charrr == '\n') {
          putchar(charrr);
          prev_charrr = charrr;
        } else {
          if (prev_charrr == '\n' && charrr != '\n') {
            printf("%6d\t", line);
            line++;
            putchar(charrr);
            prev_charrr = charrr;
          } else {
            putchar(charrr);
            prev_charrr = charrr;
          }
        }
      }
    }
  }
}

void simple_cat(FILE *fp, int flag) {
  int charrr;
  int start = 0;
  int newline_chars = 0;
  while ((charrr = fgetc(fp)) != EOF) {
    if (charrr == '\n') {
      newline_chars++;
      if (flag == 's') {
        if (newline_chars > 2 || start == 0) {
          if (newline_chars > 0 && start == 0) {
            putchar(10);
            start = 1;
          }
        } else {
          putchar(charrr);
        }
      } else {
        putchar(charrr);
      }
    } else {
      start = 1;
      newline_chars = 0;
      putchar(charrr);
    }
  }
}
