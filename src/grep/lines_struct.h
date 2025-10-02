#ifndef LINES_STRUCT_H
#define LINES_STRUCT_H

typedef struct {
  char *filename;
  char *line;
  int number_line;
} lines;

lines *line = NULL;

#endif
