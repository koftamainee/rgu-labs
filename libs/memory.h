#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include <stdlib.h>

void vilka(char const *restrict_format, ...);
int rerealloc(void **ptr, size_t size);

#endif