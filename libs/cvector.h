#ifndef CVECTOR_H
#define CVECTOR_H

#include <stdlib.h>

#define INITIAL_CAPACITY 1
#define GROWTH_FACTOR 2

typedef struct dynamic_array {
    size_t size;
    size_t capacity;
    int* data;
} Vector;

int __memory_allocate(Vector *vec);

int cvector_init(Vector *vec);
int cvector_push_back(Vector *vec, int elem);
void cvector_free(Vector *vec);
int cvector_pop(Vector *vec, int index);
int cvector_pop_front(Vector *vec);
int cvector_pop_back(Vector *vec);
void cvector_print(Vector *vec);
int cvector_insert(Vector *vec, size_t index, int value);

#endif