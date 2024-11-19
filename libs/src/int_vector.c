#include "../int_vector.h"

#include "../errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int_vector int_vector_init() {
  int_vec_metadata_t *vec = NULL;
  vec =
      (int_vec_metadata_t *)malloc((sizeof(int_vec_metadata_t)) +
                                   (INT_VECTOR_INITAL_CAPACITY * sizeof(int)));
  if (vec == NULL) {
    return NULL;
  }

  vec->capacity = INT_VECTOR_INITAL_CAPACITY;
  vec->size = 0;

  return __int_vector_base_to_vec(vec);
}

int_vector int_vector_from(int *src, size_t len) {
  int_vec_metadata_t *vec = NULL;
  vec = (int_vec_metadata_t *)malloc((sizeof(int_vec_metadata_t)) +
                                     (len * sizeof(int)));
  if (vec == NULL) {
    return NULL;
  }
  memcpy(__int_vector_base_to_vec(vec), src, len * sizeof(int));
  vec->capacity = len;
  vec->size = len;
  return __int_vector_base_to_vec(vec);
}

void int_vector_free(int_vector vec) {
  free(__int_vector_vec_to_base(vec));
  return;
}

int int_vector_grow(int_vector *vec, size_t new_size) {
  if (vec == NULL || *vec == NULL) {
    return DEREFERENCING_NULL_PTR;
  }

  int_vec_metadata_t *for_realloc = NULL;
  size_t current_size = int_vector_cap(vec);
  if (current_size == new_size) {
    return EXIT_SUCCESS;
  }

  for_realloc = (int_vec_metadata_t *)realloc(__int_vector_vec_to_base(*vec),
                                              (sizeof(int_vec_metadata_t)) +
                                                  (new_size * sizeof(int)));
  if (for_realloc == NULL) {
    return MEMORY_ALLOCATE_ERROR;
  }

  *vec = __int_vector_base_to_vec(for_realloc);

  for_realloc->capacity = new_size;
  if (new_size < current_size) {
    for_realloc->size = new_size;
  }

  return EXIT_SUCCESS;
}

int int_vector_insert(int_vector *vec, int elem, size_t pos);

int int_vector_push_back(int_vector *vec, int elem);

int int_vector_pop(int_vector *vec, int *elem, size_t pos);

int int_vector_pop_back(int_vector *vec, int *elem);