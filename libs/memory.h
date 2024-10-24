#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>
#include <stdio.h>

void vilka(char const *restrict_format, ...);
int rerealloc(void **ptr, size_t size);

#endif