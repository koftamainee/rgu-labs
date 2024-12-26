#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>

#include "errors.h"

int is_file_empty(FILE *fp);
err_t swap(void *first, void *second, size_t len);

err_t binary_search(void const *values, void const *value_to_find, size_t count,
                    size_t value_size,
                    int (*comparer)(void const *, void const *),
                    void **result_placement);
#endif
