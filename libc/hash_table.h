#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include "cstring.h"
#include "u_list.h"

#define HASHSIZE (128)
#define HASH_TABLE_GROWTH_FACTOR (2)
#define HASH_TABLE_SHRINK_FACTOR (2)

typedef struct hash_table_bucket {
    void *key;
    void *value;
} hash_table_bucket;

typedef struct {
    u_list **buckets;
    size_t size;
    size_t capacity;
    size_t key_size;
    size_t value_size;
    size_t max_chain_length;
    size_t min_chain_length;
    int (*keys_comparer)(const void *, const void *);
    size_t (*hash)(const void *key, size_t key_size, size_t capacity);
    void (*bucket_destructor)(void *);
} hash_table;

err_t hash_table_init(
    hash_table **ht, int (*keys_comparer)(const void *, const void *),
    size_t (*hash)(const void *key, size_t key_size, size_t capacity),
    size_t key_size, size_t value_size, void (*bucket_destructor)(void *));

void hash_table_free(hash_table *ht);

err_t hash_table_set(hash_table *ht, const void *key, const void *value);
err_t hash_table_get(hash_table *ht, const void *key, void **value_placeholder);
err_t hash_table_dispose(hash_table *ht, const void *key);

err_t hash_table_resize(hash_table *ht, int size_modifier);

err_t hash_table_get_load_factor(hash_table *ht,
                                 double *load_factor_placeholder);
err_t hash_table_get_chain_length_factor(
    hash_table *ht, double *chain_length_factor_placeholder);

/* --------------- EXAMPLE FUNCTIONS --------------- */
size_t djb2_hash(const void *key, size_t key_size, size_t capacity);
size_t murmur_hash(const void *key, size_t key_size, size_t capacity);
size_t sha256_hash(const void *key, size_t key_size, size_t capacity);

#endif  // HASH_TABLE_H_
