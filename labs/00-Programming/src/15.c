#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../libs/input.h"

int reorder_string(char **str) {
  char *ptr = *str;
  char *digits;
  char *letters;
  char *others;
  int digits_cnt = 0, letters_cnt = 0, others_cnt = 0;
  int len = strlen(*str);

  if (!(digits = (char *)malloc((len + 1) * sizeof(char)))) {
    return 1;
  }
  if (!(letters = (char *)malloc((len + 1) * sizeof(char)))) {
    free(digits);
    return 1;
  }
  if (!(others = (char *)malloc((len + 1) * sizeof(char)))) {
    free(digits);
    free(letters);
    return 1;
  }

  while (*ptr) {
    if (isdigit(*ptr)) {
      digits[digits_cnt++] = *ptr;
    } else if (isalpha(*ptr)) {
      letters[letters_cnt++] = *ptr;
    } else {
      others[others_cnt++] = *ptr;
    }
    ptr++;
  }

  digits[digits_cnt] = 0;
  letters[letters_cnt] = 0;
  others[others_cnt] = 0;

  strcpy(*str, digits);
  strcat(*str, letters);
  strcat(*str, others);

  free(digits);
  free(letters);
  free(others);

  return 0;
}

int program_00_15() {
  char *str = NULL;

  if (read_str(&str) != 0) {
    printf("Error reading string.\n");
    return 1;
  }

  if (reorder_string(&str) != 0) {
    printf("Error reordering string.\n");
    free(str);
    return 1;
  }

  printf("ans: %s\n", str);
  free(str);
  return 0;
}
