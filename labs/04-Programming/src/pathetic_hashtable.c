#include "../pathetic_hashtable.h"

#include <ctype.h>
#include <string.h>

int pathetic_hash_fn(int c) {
    if (!isalpha(c)) {
        return -1;  // invalid key for our pathetic goals
    }
    return toupper(c) - 'A';  // returns index 0..25
}

err_t p_hash_init(p_hash **t) {
    if (t == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    p_hash *table = *t;

    table = (p_hash *)malloc(sizeof(p_hash));
    if (table == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }
    memset(table->data, 0,
           sizeof(int) *
               ALPHABET_SIZE);  // all vars are init by default, and set to 0
    table->hash = pathetic_hash_fn;

    *t = table;

    return EXIT_SUCCESS;
}

void p_hash_free(p_hash *t) { free(t); }

err_t p_hash_get(p_hash *t, int key, int *value_holder) {
    if (t == NULL || value_holder == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    int index = t->hash(key);
    if (index == -1) {  // invalid key passed
        return KEY_NOT_FOUND;
    }
    *value_holder = t->data[index];

    return EXIT_SUCCESS;
}

err_t p_hash_set(p_hash *t, int key, int new_value) {
    if (t == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    int index = t->hash(key);
    if (index == -1) {
        return KEY_NOT_FOUND;
    }

    t->data[index] = new_value;

    return EXIT_SUCCESS;
}
