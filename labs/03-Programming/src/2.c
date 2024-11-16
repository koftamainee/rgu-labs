#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define MEMORY_ALLOCATE_ERROR 1
#define DEREFERENCING_NULL_PTR 2

// custom errors

#define NOT_ENOUGH_ARGUMENTS -1
#define INVALID_FLAG_FORMAT -2

int cstrlen(const char *str, int *ans);
int creverse(const char *str, char **ans);
int ctoupper(const char *str, char **ans);
int creorder_string(const char *str, char **ans);
int cstrcat(char **strings, char **ans, int count, int seed);
int create_string_array(int argc, char *argv[], char ***str_arr);
int cstrcpy(char *to, const char *from);
void swap_char(char **a, char **b);
int cstrcatd(char *str1, const char *str2);

int program_03_2(int argc, char *argv[]) {
  int err, seed, i;
  int int_ans;
  char *str_ans = NULL;
  char **str_arr = NULL;

  if (argc < 3) {
    printf("Not enough arguments\n");
    return NOT_ENOUGH_ARGUMENTS;
  } else if (argv[1][0] == '-') {
    err = cstrlen(argv[1], &int_ans);
    if (err == DEREFERENCING_NULL_PTR) {
      printf("NULL ptr dereferencing\n");
      return err;
    }
    if (int_ans != 2) {
      printf("Invalid flag format\n");
      return INVALID_FLAG_FORMAT;
    }
    switch (argv[1][1]) {

    case 'l':
      err = cstrlen(argv[2], &int_ans);
      if (err == DEREFERENCING_NULL_PTR) {
        printf("NULL ptr dereferencing\n");
        return err;
      }
      printf("Length of the string: %d\n", int_ans);
      break;

    case 'r':
      err = creverse(argv[2], &str_ans);
      if (err == 0) {
        printf("Reversed string: %s\n", str_ans);
        free(str_ans);
      } else {
        printf("Error occurred\n");
        return err;
      }
      break;

    case 'u':
      err = ctoupper(argv[2], &str_ans);
      if (err == 0) {
        printf("Uppercase string: %s\n", str_ans);
        free(str_ans);
      } else {
        printf("Error occurred\n");
        return err;
      }
      break;

    case 'n':
      err = creorder_string(argv[2], &str_ans);
      if (err == 0) {
        printf("Reordered string: %s\n", str_ans);
        free(str_ans);
      } else {
        printf("Error occurred\n");
        return err;
      }
      break;

    case 'c':
      if (argc < 4) {
        printf("Not enough arguments\n");
        return 1;
      }
      seed = atoi(argv[3]);
      err = create_string_array(argc, argv, &str_arr);
      if (err != 0) {
        printf("Error creating string array\n");
        return err;
      }
      err = cstrcat(str_arr, &str_ans, argc - 3, seed);
      if (err == 0) {
        printf("Concatenated string: %s\n", str_ans);
        free(str_ans);
      } else {
        printf("Error concatenating strings\n");
        return err;
      }
      for (i = 0; i < argc - 3; ++i) {
        free(str_arr[i]);
      }
      free(str_arr);
      break;

    default:
      printf("Invalid flag\n");
      return 2;
    }
  }

  return 0;
}

int cstrlen(const char *str, int *ans) {
  if (str == NULL || ans == NULL) {
    return DEREFERENCING_NULL_PTR;
  }

  int len = 0;
  const char *ptr = str;
  while (*ptr++) {
    len++;
  }

  *ans = len;
  return 0;
}

int creverse(const char *str, char **ans) {
  if (str == NULL) {
    return DEREFERENCING_NULL_PTR;
  }
  int len, i;
  cstrlen(str, &len);
  *ans = (char *)malloc((len + 1) * sizeof(char));
  if (!*ans) {
    return MEMORY_ALLOCATE_ERROR;
  }
  (*ans)[len] = 0;
  for (i = 0; i < len; ++i) {
    (*ans)[len - i - 1] = str[i];
  }
  return 0;
}

int ctoupper(const char *str, char **ans) {
  if (str == NULL) {
    return DEREFERENCING_NULL_PTR;
  }
  int len, i;
  cstrlen(str, &len);
  printf("%d\n", len);
  *ans = (char *)malloc((len + 1) * sizeof(char));
  if (!*ans) {
    return MEMORY_ALLOCATE_ERROR;
  }
  (*ans)[len] = 0;

  for (i = 0; i < len; ++i) {
    if (i % 2 == 0) {
      (*ans)[i] = toupper(str[i]);
    } else {
      (*ans)[i] = str[i];
    }
  }
  return 0;
}

int creorder_string(const char *str, char **ans) {
  if (str == NULL)
    return 1;
  int len;
  cstrlen(str, &len);

  char *digits = malloc(len + 1), *letters = malloc(len + 1),
       *others = malloc(len + 1);
  if (!digits || !letters || !others) {
    free(digits);
    free(letters);
    free(others);
    return 1;
  }

  int digits_cnt = 0, letters_cnt = 0, others_cnt = 0;
  const char *ptr = str;

  while (*ptr) {
    if (isdigit(*ptr))
      digits[digits_cnt++] = *ptr;
    else if (isalpha(*ptr))
      letters[letters_cnt++] = *ptr;
    else
      others[others_cnt++] = *ptr;
    ptr++;
  }

  digits[digits_cnt] = 0;
  letters[letters_cnt] = 0;
  others[others_cnt] = 0;

  *ans = malloc(digits_cnt + letters_cnt + others_cnt + 1);
  if (!*ans) {
    free(digits);
    free(letters);
    free(others);
    return MEMORY_ALLOCATE_ERROR;
  }

  cstrcpy(*ans, digits);
  cstrcatd(*ans, letters);
  cstrcatd(*ans, others);

  free(digits);
  free(letters);
  free(others);
  return 0;
}

int cstrcat(char **strings, char **ans, int count, int seed) {
  int total_len = 0, int_ans;
  srand(seed);

  for (int i = count - 1; i > 0; --i) {
    int j = rand() % (i + 1);
    swap_char(&strings[i], &strings[j]);
  }

  for (int i = 0; i < count; ++i) {
    cstrlen(strings[i], &int_ans);
    total_len += int_ans;
  }

  *ans = (char *)malloc(total_len + 1);
  if (*ans == NULL) {
    return MEMORY_ALLOCATE_ERROR;
  }

  (*ans)[0] = '\0';
  for (int i = 0; i < count; ++i) {
    cstrcatd(*ans, strings[i]);
  }

  return 0;
}

int create_string_array(int argc, char *argv[], char ***str_arr) {
  *str_arr = (char **)malloc((argc - 3) * sizeof(char *));
  if (*str_arr == NULL) {
    return MEMORY_ALLOCATE_ERROR;
  }

  for (int i = 4; i < argc; ++i) {
    int len;
    cstrlen(argv[i], &len);
    (*str_arr)[i - 4] = (char *)malloc((len + 1) * sizeof(char));
    if ((*str_arr)[i - 4] == NULL) {
      return MEMORY_ALLOCATE_ERROR;
    }
    cstrcpy((*str_arr)[i - 4], argv[i]);
  }

  return 0;
}

int cstrcpy(char *to, const char *from) {
  while (*from) {
    *to++ = *from++;
  }
  *to = '\0';
  return 0;
}

void swap_char(char **a, char **b) {
  char *temp = *a;
  *a = *b;
  *b = temp;
}

int cstrcatd(char *str1, const char *str2) {
  while (*str1)
    str1++;
  while (*str2)
    *str1++ = *str2++;
  *str1 = '\0';
  return 0;
}
