#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>

int read_str(char** str);
int read_number(int* number);
int read_binary_number(int* number);

int read_array_from_file(FILE* fptr, int* array);

#endif