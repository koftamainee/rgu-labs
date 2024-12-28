#ifndef PATHETIC_HASHTABLE_H_

#include <stdlib.h>

#include "../../libc/errors.h"

#define ALPHABET_SIZE 26  // A..Z

typedef struct {
    int data[26];
    int (*hash)(int c);
} pathetic_hashtable, p_hash;

err_t p_hash_init(p_hash **t);
void p_hash_free(p_hash *t);

err_t p_hash_get(p_hash *t, int key, int *value_holder);
err_t p_hash_set(p_hash *t, int key, int new_value);

#endif  // PATHETIC_HASHTABLE_H_
