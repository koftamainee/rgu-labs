#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>
#include <stdio.h>

void vilka(char const *restrict_format, ...);
int rerealloc(void *src, void **dest, size_t size);

#endif