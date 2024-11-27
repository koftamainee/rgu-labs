#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "../errors.h"

void vilka(char const *restrict _format, ...) {
  void *arg;
  va_list valist;
  va_start(valist, _format);
  while (*_format) {
    arg = va_arg(valist, void *);
    if (*_format == 'f') {
      free(arg);
    } else if (*_format == 'c') {
      fclose((FILE *)arg);
    }
    _format++;
  }
  va_end(valist);
  return;
}

int rerealloc(void **ptr, size_t size) {
  void *for_realloc = NULL;
  int err;

  for_realloc = realloc(*ptr, size);
  if (for_realloc == NULL) {
    return MEMORY_ALLOCATION_ERROR;
  }
  *ptr = for_realloc;
  return 0;
}
