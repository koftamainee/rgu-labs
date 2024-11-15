#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../libs/cvector.h"
#include "../../../libs/errors.h"
#include "../../../libs/types.h"

#define NUMBER_COUNT 5

int find_kaprekar_numbers(Vector *ans, int *answers_count, int numbers_count,
                          int base, ...);
int is_kaprekar(int *ans, char const *number, int base);

int program_03_7(int argc, char *argv[]) {
  int err, answers_count, i;
  Vector vec;

  int const base = 16;

  err = find_kaprekar_numbers(&vec, &answers_count, 4, base, "1", "52", "F",
                              "AB");
  switch (err) {
  case OK:
    if (vec.size > 0) {
      printf("Kaprekar numbers in %d counting system: ", base);
      for (i = 0; i < vec.size; ++i) {
        printf("%d ", vec.data[i]);
      }
      printf("\n");
    } else {
      printf("There is no Kaprekar numbers :(\n");
    }
    break;
  case DEREFERENCING_NULL_PTR:
    printf("Dereferencing NULL ptr\n");
    break;
  case MEMORY_ALLOCATE_ERROR:
    printf("Memory allocation error\n");
    break;
  case INVALID_NUMERIC_BASE:
    printf("Invalid numeric base\n");
    break;
  default:
    printf("Undefined behavior 0_o. Exit code %d", err);
    break;
  }

  cvector_free(&vec);
  return err;
}

int find_kaprekar_numbers(Vector *ans, int *answers_count, int numbers_count,
                          int base, ...) {
  int err, i, inner_ans;
  char *str_num = NULL;
  va_list valist;

  if (ans == NULL || answers_count == NULL) {
    return DEREFERENCING_NULL_PTR;
  }
  if (base < 2 || base > 36) {
    return INVALID_NUMERIC_BASE;
  }

  err = cvector_init(ans);
  if (err) {
    return MEMORY_ALLOCATE_ERROR;
  }

  va_start(valist, base);

  for (i = 0; i < numbers_count; ++i) {
    str_num = va_arg(valist, char *);
    err = is_kaprekar(&inner_ans, str_num, base);
    if (err) {
      return err;
    }
    if (inner_ans) {
      err = catoi(str_num, base, &inner_ans);
      if (err) {
        return err;
      }
      err = cvector_push_back(ans, inner_ans);
      if (err) {
        return err;
      }
    }
  }

  va_end(valist);
  return OK;
}

int is_kaprekar(int *ans, char const *number, int base) {
  int err, int_number, i, div, left_part, right_part;
  double square;
  size_t number_digits_count, square_digits_count;

  if (ans == NULL || number == NULL) {
    return DEREFERENCING_NULL_PTR;
  }
  if (base < 2 || base > 36) {
    return INVALID_NUMERIC_BASE;
  }

  err = catoi(number, base, &int_number);
  if (err) {
    return err;
  }

  if (int_number == 1) {
    *ans = 1;
    return OK;
  }

  square = int_number * int_number;

  number_digits_count = strlen(number);
  square_digits_count = (size_t)log10(square) + 1;

  for (i = 1; i < square_digits_count; ++i) {
    div = (int)pow(base, i);

    left_part = ((int)square) / div;
    right_part = ((int)square) % div;

    if (left_part + right_part == int_number) {
      *ans = 1;
      return OK;
    }
  }

  *ans = 0;
  return OK;
}