#include <stdio.h>
#include <stdlib.h>

#include "../../../libs/input.h"

char *custom_strstr(char *main_str, char *str);

int program_00_12() {
  char c;
  char *str1;
  char *str2;
  printf("First string...\n");
  read_str(&str1);
  printf("Second string...\n");
  while ((c = getchar()) != '\n' && c != EOF)
    ;
  read_str(&str2);

  char *p = custom_strstr(str1, str2);
  printf("strstr result: %s", p);

  free(str1);
  free(str2);
  return 0;
}

char *custom_strstr(char *main_str, char *str) {
  char *i, n, i_temp;
  if (!*str) {
    return (char *)main_str;
  }

  for (i = main_str; *i; ++i) {
    char *i_temp = i;
    char *n = str;

    while (*i_temp && *n && *i_temp == *n) {
      ++i_temp;
      ++n;
    }

    if (!*n) {
      return (char *)i;
    }
  }

  return NULL;
}
