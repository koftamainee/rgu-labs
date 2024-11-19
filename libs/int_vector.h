#ifndef INT_VECTOR_H_
#define INT_VECTOR_H_

#include <stdlib.h>

#define INT_VECTOR_INITAL_CAPACITY 16
#define INT_VECTOR_GROWTH_FACTOR 2

typedef struct {
  size_t size;
  size_t capacity;
} int_vec_metadata_t;

typedef int *int_vector;

#define __int_vector_vec_to_base(vec) (&((int_vec_metadata_t *)(vec))[-1])
#define __int_vector_base_to_vec(vec) ((int *)&((int_vec_metadata_t *)(vec))[1])

#define int_vector_size(vec) (vec ? __int_vector_vec_to_base(vec)->size : 0)
#define int_vector_cap(vec) (vec ? __int_vector_vec_to_base(vec)->capacity : 0)

int_vector int_vector_init();
int_vector int_vector_from(int *src, size_t len);

void int_vector_free(int_vector);

int int_vector_grow(int_vector *vec, size_t new_size);

int int_vector_insert(int_vector *vec, int elem, size_t pos);
int int_vector_push_back(int_vector *vec, int elem);

int int_vector_pop(int_vector *vec, int *elem, size_t pos);
int int_vector_pop_back(int_vector *vec, int *elem);

#endif