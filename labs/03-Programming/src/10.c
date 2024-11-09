#include <stdio.h>

#include "../../../libs/errors.h"

int __overvscanf(char const *restrict data, ...);

int overscanf(char const *restrict _format, ...);
int overfscanf(FILE *restrict stream, char const *restrict _format, ...);
int oversscanf(char const *restrict s, char const *restrict _format, ...);

int program_03_10() {
  int err, x;

  err = overscanf("%d", &x);

  return err;
}

int overscanf(char const *restrict _format, ...) { return OK; }

int overfscanf(FILE *restrict stream, char const *restrict _format, ...) {
  return OK;
}

int oversscanf(char const *restrict s, char const *restrict _format, ...) {
  return OK;
}