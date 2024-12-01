/*-------------------- This is EXTREMLY outdated --------------------*/
/*--------- PLS implement it with new cvector or int_vector ---------*/
/*
//
//
*/

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../libs/errors.h"
#include "../../../libs/types.h"

/*------------------------------- old cvector ------------------------------- */
#define ____INITIAL_CAPACITY 16
#define ____GROWTH_FACTOR 2

typedef struct {
  size_t size;
  size_t capacity;
  int *data;
} ____Vector;

int ______memory_allocate(____Vector *vec);

int ______cvector_init(____Vector *vec);
int ______cvector_push_back(____Vector *vec, int elem);
void ______cvector_free(____Vector *vec);
int ______cvector_pop(____Vector *vec, int index);
int ______cvector_pop_front(____Vector *vec);
int ______cvector_pop_back(____Vector *vec);
void ______cvector_print(____Vector *vec);
int ______cvector_insert(____Vector *vec, size_t index, int value);
/*----------------------------------------------------------------------------*/
int ______cvector_init(____Vector *vec) {
  vec->size = 0;
  vec->capacity = ____INITIAL_CAPACITY;
  vec->data = (int *)malloc(vec->capacity * sizeof(int));
  if (vec->data == NULL) {
    return MEMORY_ALLOCATION_ERROR;
  }
  return 0;
}

int ______memory_allocate(____Vector *vec) {
  int *for_realloc;
  for_realloc = (int *)realloc(vec->data, vec->capacity * sizeof(int));
  if (for_realloc == NULL) {
    return MEMORY_ALLOCATION_ERROR;
  }
  vec->data = for_realloc;
  return 0;
}

int ______cvector_push_back(____Vector *vec, int elem) {
  if (vec->size >= vec->capacity) {
    vec->capacity *= ____GROWTH_FACTOR;
    if (______memory_allocate(vec) == 1) {
      return MEMORY_ALLOCATION_ERROR;
    }
  }
  vec->data[vec->size] = elem;
  vec->size++;
  return 0;
}

int ______cvector_pop(____Vector *vec, int index) {
  if (index < 0 || index >= vec->size) {
    return INDEX_OUT_OF_BOUNDS;
  }

  for (int i = index; i < vec->size - 1; i++) {
    vec->data[i] = vec->data[i + 1];
  }

  vec->size--;

  if (vec->size > 0 && vec->size <= vec->capacity / (____GROWTH_FACTOR * 2)) {
    vec->capacity /= ____GROWTH_FACTOR;
    ______memory_allocate(vec);
  }

  return 0;
}

void ______cvector_free(____Vector *vec) {
  free(vec->data);
  vec->data = NULL;
}

void ______cvector_print(____Vector *vec) {
  int i;
  for (i = 0; i < vec->size; ++i) {
    printf("%d ", vec->data[i]);
  }
  printf("\n");
}

int ______cvector_pop_front(____Vector *vec) {
  return ______cvector_pop(vec, 0);
}

int ______cvector_pop_back(____Vector *vec) {
  return ______cvector_pop(vec, vec->size - 1);
}

int ______cvector_insert(____Vector *vec, size_t index, int value) {
  if (index > vec->size) {
    return INDEX_OUT_OF_BOUNDS;
  }

  if (vec->size >= vec->capacity) {
    vec->capacity *= 2;
    int *new_data = realloc(vec->data, vec->capacity * sizeof(int));
    if (new_data == NULL) {
      return MEMORY_ALLOCATION_ERROR;
    }
    vec->data = new_data;
  }

  for (size_t i = vec->size; i > index; i--) {
    vec->data[i] = vec->data[i - 1];
  }

  vec->data[index] = value;
  vec->size++;

  return 0;
}
/*----------------------------------------------------------------------------*/
/*
//
//
//
//
//
//
//
*/

#define NUMBER_COUNT 5

int find_kaprekar_numbers(____Vector *ans, int *answers_count,
                          int numbers_count, int base, ...);
int is_kaprekar(int *ans, char const *number, int base);

int program_03_7(int argc, char *argv[]) {
  int err, answers_count, i;
  ____Vector vec;

  int const base = 10;

  err = find_kaprekar_numbers(&vec, &answers_count, 4, base, "4879", "52", "F",
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
  case MEMORY_ALLOCATION_ERROR:
    printf("Memory allocation error\n");
    break;
  case INVALID_NUMERIC_BASE:
    printf("Invalid numeric base\n");
    break;
  default:
    printf("Undefined behavior 0_o. Exit code %d", err);
    break;
  }

  ______cvector_free(&vec);
  return err;
}

int find_kaprekar_numbers(____Vector *ans, int *answers_count,
                          int numbers_count, int base, ...) {
  int err, i, inner_ans;
  char *str_num = NULL;
  va_list valist;

  if (ans == NULL || answers_count == NULL) {
    return DEREFERENCING_NULL_PTR;
  }
  if (base < 2 || base > 36) {
    return INVALID_NUMERIC_BASE;
  }

  err = ______cvector_init(ans);
  if (err) {
    return MEMORY_ALLOCATION_ERROR;
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
      err = ______cvector_push_back(ans, inner_ans);
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
