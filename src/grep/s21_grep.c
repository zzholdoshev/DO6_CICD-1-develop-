#define _GNU_SOURCE
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lines_struct.h"
#include "parse_arguments.h"

void parse_arguments(int *flags, int argc, char **argv);
void grep_magick(int *flags, char **patern, char **filename, int j, int k);
void realloc_memory(char ***result_line, int *result_number_lines);
int flag_o(char *string_to_compare, char ***result_line,
           int *result_number_lines, int file_line_number, char *filename,
           regex_t *regex, regmatch_t *matches);
void flag_vef(char *string_to_compare, char ***result_line,
              int *result_number_lines, int file_line_number, char *filename);
void realloc_structure(lines **line, int result_number_lines);
void print_result(char **filename, int k, int result_number_lines, int *flags,
                  int file_results[k]);
int grep_regcomp(regex_t *regex, int *flags, char *pattern);
void free_mem(char ***result_line, int result_number_lines);

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("ERROR usage: -flag [patterns] [file]");
    return 1;
  }
  int flags[10] = {0};
  parse_arguments(flags, argc, argv);
}

// e, f, i, v, o flags processed here:
void grep_magick(int *flags, char **pattern, char **filename, int j, int k) {
  int file_line_number = 0;
  regex_t regex;
  int v_count = 0;
  size_t len;
  FILE *fp = NULL;
  char *string_to_compare = NULL;
  char **result_line = NULL;
  int result_number_lines = 0;
  regmatch_t matches;
  int prev_line = -1;
  int result_per_filename = 0;
  int file_results[k];
  for (int x = 0; x < k; x++) {
    if ((fp = fopen(filename[x], "r")) == NULL) {
      file_results[x] = -1;
    } else {
      ssize_t read;
      while ((read = getline(&string_to_compare, &len, fp)) != -1) {
        ++file_line_number;
        for (int i = 0; i < j; i++) {
          int check = grep_regcomp(&regex, flags, pattern[i]);
          if (check != 0) {
            regfree(&regex);
            break;
          }
          if (flags[O] > 0 && flags[V] == 0) {
            if (flag_o(string_to_compare, &result_line, &result_number_lines,
                       file_line_number, filename[x], &regex, &matches) &&
                prev_line != file_line_number) {
              prev_line = file_line_number;
              result_per_filename++;
            }
          } else {
            int regex_res = regexec(&regex, string_to_compare, 1, &matches, 0);
            if (flags[V] > 0 && flags[O] == 0) {
              if (!regex_res) v_count = 1;
              if (i == j - 1) {
                if (v_count == 0) {
                  flag_vef(string_to_compare, &result_line,
                           &result_number_lines, file_line_number, filename[x]);
                  result_per_filename++;
                }
                v_count = 0;
              }
            } else {
              if (flags[O] > 0 && flags[V] > 0) {
                break;
              }
              if (regex_res == 0) {
                if (prev_line != file_line_number) {
                  flag_vef(string_to_compare, &result_line,
                           &result_number_lines, file_line_number, filename[x]);
                  result_per_filename++;
                  prev_line = file_line_number;
                }
              }
            }
          }
          regfree(&regex);
        }
      }
      file_line_number = 0;
      file_results[x] = result_per_filename;
    }
    result_per_filename = 0;
    if (fp != NULL) {
      fclose(fp);
    }
  }

  if (string_to_compare) {
    free(string_to_compare);
  }
  print_result(filename, k, result_number_lines, flags, file_results);
  free_mem(&result_line, result_number_lines);
}

void print_result(char **filename, int k, int result_number_lines, int *flags,
                  int file_results[k]) {
  if (flags[L] > 0) {
    for (int i = 0; i < k; i++) {
      if (file_results[i] > 0) {
        printf("%s\n", filename[i]);
      }
    }
  } else {
    if (flags[C] > 0) {
      if (k > 1) {
        for (int i = 0; i < k; i++) {
          if (file_results[i] >= 0) {
            if (!flags[H]) {
              printf("%s:", filename[i]);
            }
            printf("%d\n", file_results[i]);
          }
        }
      } else {
        if (result_number_lines > 0) {
          printf("%d\n", file_results[0]);
        } else {
          printf("0\n");
        }
      }
    } else {
      for (int i = 0; i < result_number_lines; i++) {
        if (k > 1 && !flags[H]) {
          printf("%s:", line[i].filename);
        }
        if (flags[N] > 0) {
          printf("%d:", line[i].number_line);
        }
        printf("%s\n", line[i].line);
      }
    }
  }

  for (int i = 0; i < k; i++) {
    if (file_results[i] < 0 && !flags[S]) {
      fprintf(stderr, "grep: %s: %s", filename[i],
              "No such file or directory\n");
    }
  }
}

void realloc_memory(char ***result_line, int *result_number_lines) {
  char **temp = (char **)realloc(*result_line,
                                 (*result_number_lines + 1) * sizeof(char *));
  if (temp != NULL) {
    *result_line = temp;
  } else {
    printf("ERROR allcoating memory!!!");
  }
}
void realloc_structure(lines **line, int result_number_lines) {
  lines *temp = realloc(*line, (result_number_lines + 1) * sizeof(lines));
  if (temp == NULL) {
    printf("error allocating memory!");
  } else {
    *line = temp;
  }
}

int flag_o(char *string_to_compare, char ***result_line,
           int *result_number_lines, int file_line_number, char *filename,
           regex_t *regex, regmatch_t *matches) {
  size_t offset = 0;
  int eflags = 0;
  int match = 0;
  size_t length = strlen(string_to_compare);
  while (regexec(regex, string_to_compare + offset, 1, matches, eflags) == 0) {
    eflags = REG_NOTBOL;
    match = 1;
    int start = matches->rm_so + offset;
    int end = matches->rm_eo + offset;
    offset += matches->rm_eo;
    if (matches->rm_eo == matches->rm_so) {
      offset += 1;
    }
    if (offset > length) {
      break;
    }

    realloc_memory(result_line, result_number_lines);
    (*result_line)[*result_number_lines] =
        malloc(sizeof(char) * (end - start + 1));
    strncpy((*result_line)[*result_number_lines], string_to_compare + start,
            end - start);
    (*result_line)[*result_number_lines][end - start] = 0;
    realloc_structure(&line, *result_number_lines);
    line[*result_number_lines].number_line = file_line_number;
    line[*result_number_lines].filename = filename;
    line[*result_number_lines].line = (*result_line)[*result_number_lines];
    (*result_number_lines)++;
  }
  return match;
}

void flag_vef(char *string_to_compare, char ***result_line,
              int *result_number_lines, int file_line_number, char *filename) {
  realloc_memory(result_line, result_number_lines);
  int size = strlen(string_to_compare);
  (*result_line)[*result_number_lines] = malloc(sizeof(char) * (size) + 1);
  strcpy((*result_line)[*result_number_lines], string_to_compare);
  if ((*result_line)[*result_number_lines][size - 1] == '\n') {
    (*result_line)[*result_number_lines][size - 1] = 0;
  }
  realloc_structure(&line, *result_number_lines);
  line[*result_number_lines].number_line = file_line_number;
  line[*result_number_lines].filename = filename;
  line[*result_number_lines].line = (*result_line)[*result_number_lines];
  (*result_number_lines)++;
}

void free_mem(char ***result_line, int result_number_lines) {
  for (int i = 0; i < result_number_lines; i++) {
    free((*result_line)[i]);
  }
  if (*result_line != NULL) {
    free(*result_line);
  }
  free(line);
}

int grep_regcomp(regex_t *regex, int *flags, char *pattern) {
  int check;
  if (flags[I] == 0) {
    check = regcomp(regex, pattern, REG_EXTENDED);
  } else {
    check = regcomp(regex, pattern, REG_ICASE);
  }
  return check;
}
