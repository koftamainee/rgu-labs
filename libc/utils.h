#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>

#include "errors.h"

int is_file_empty(FILE *fp);
err_t swap(void *first, void *second, size_t len);

#endif
