#include "../cvector.h"

#include <stdio.h>
#include <stdlib.h>

#include "../errors.h"

int cvector_init(Vector *vec) {
  vec->size = 0;
  vec->capacity = INITIAL_CAPACITY;
  vec->data = (int *)malloc(vec->capacity * sizeof(int));
  if (vec->data == NULL) {
    return MEMORY_ALLOCATE_ERROR;
  }
  return 0;
}

int __memory_allocate(Vector *vec) {
  int *for_realloc;
  for_realloc = (int *)realloc(vec->data, vec->capacity * sizeof(int));
  if (for_realloc == NULL) {
    return MEMORY_ALLOCATE_ERROR;
  }
  vec->data = for_realloc;
  return 0;
}

int cvector_push_back(Vector *vec, int elem) {
  if (vec->size >= vec->capacity) {
    vec->capacity *= GROWTH_FACTOR;
    if (__memory_allocate(vec) == 1) {
      return MEMORY_ALLOCATE_ERROR;
    }
  }
  vec->data[vec->size] = elem;
  vec->size++;
  return 0;
}

int cvector_pop(Vector *vec, int index) {
  if (index < 0 || index >= vec->size) {
    return INDEX_OUT_OF_BOUNDS;
  }

  for (int i = index; i < vec->size - 1; i++) {
    vec->data[i] = vec->data[i + 1];
  }

  vec->size--;

  if (vec->size > 0 && vec->size <= vec->capacity / (GROWTH_FACTOR * 2)) {
    vec->capacity /= GROWTH_FACTOR;
    __memory_allocate(vec);
  }

  return 0;
}

void cvector_free(Vector *vec) {
  free(vec->data);
  vec->data = NULL;
}

void cvector_print(Vector *vec) {
  int i;
  for (i = 0; i < vec->size; ++i) {
    printf("%d ", vec->data[i]);
  }
  printf("\n");
}

int cvector_pop_front(Vector *vec) { return cvector_pop(vec, 0); }

int cvector_pop_back(Vector *vec) { return cvector_pop(vec, vec->size - 1); }

int cvector_insert(Vector *vec, size_t index, int value) {
  if (index > vec->size) {
    return INDEX_OUT_OF_BOUNDS;
  }

  if (vec->size >= vec->capacity) {
    vec->capacity *= 2;
    int *new_data = realloc(vec->data, vec->capacity * sizeof(int));
    if (new_data == NULL) {
      return MEMORY_ALLOCATE_ERROR;
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