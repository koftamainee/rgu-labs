#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>

#include "cstring.h"
#include "errors.h"

void clear_screen();

err_t read_string_from_user(String *str);
int read_str(char **str);
void read_number(int *number);
int read_binary_number(int *number);

int read_array_from_file(FILE *fptr, int *array);

#endif
