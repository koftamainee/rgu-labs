#include "../hash_table.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

err_t hash_table_init(
    hash_table **ht, int (*keys_comparer)(const void *, const void *),
    size_t (*hash)(const void *key, size_t key_size, size_t capacity),
    size_t key_size, size_t value_size, void (*bucket_destructor)(void *)) {
    if (ht == NULL || keys_comparer == NULL || hash == NULL ||
        bucket_destructor == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    hash_table *table = *ht;
    err_t err = 0;
    size_t i = 0, j = 0;

    table = (hash_table *)malloc(sizeof(hash_table));
    if (table == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }

    table->buckets = (u_list **)calloc(  // all ptr's should be NULL
        HASHSIZE, sizeof(u_list *));
    if (table->buckets == NULL) {
        free(table);
        return MEMORY_ALLOCATION_ERROR;
    }

    for (i = 0; i < HASHSIZE; ++i) {
        err = u_list_init(&(table->buckets[i]), sizeof(hash_table_bucket),
                          bucket_destructor);
        if (err) {
            for (j = 0; j < i; ++j) {
                u_list_free(table->buckets[j]);
            }
            free(table->buckets);
            free(table);
            return err;
        }
    }

    table->size = 0;
    table->min_chain_length = 0;
    table->max_chain_length = 0;
    table->capacity = HASHSIZE;
    table->key_size = key_size;
    table->value_size = value_size;
    table->keys_comparer = keys_comparer;
    table->hash = hash;
    table->bucket_destructor = bucket_destructor;

    *ht = table;

    return EXIT_SUCCESS;
}

void hash_table_free(hash_table *ht) {
    if (ht == NULL) {
        return;
    }

    size_t i = 0;
    hash_table_bucket *bucket = NULL, *temp = NULL;

    for (i = 0; i < ht->capacity; ++i) {
        u_list_free(ht->buckets[i]);
    }
    free(ht->buckets);
    free(ht);
}

err_t hash_table_set(hash_table *ht, const void *key, const void *value) {
    if (ht == NULL || key == NULL || value == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    size_t index = ht->hash(key, ht->key_size, ht->capacity);
    u_list *bucket = ht->buckets[index];
    hash_table_bucket *existing_bucket = NULL, new_bucket;
    hash_table_bucket search;
    u_list_node *node = NULL;
    err_t err = 0;
    double load_factor = 0, chain_length_factor = 0;

    search.key = (void *)key;

    err = u_list_get_node_by_value(bucket, &search, ht->keys_comparer, &node);
    if (err != EXIT_SUCCESS && err != NO_SUCH_ENTRY_IN_COLLECTION) {
        return err;
    }
    if (err == EXIT_SUCCESS) {
        existing_bucket = (hash_table_bucket *)node->data;
        memcpy(existing_bucket->value, value, ht->value_size);
        return EXIT_SUCCESS;
    }
    // key not found
    new_bucket.key = malloc(ht->key_size);
    if (new_bucket.key == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }
    new_bucket.value = malloc(ht->value_size);
    if (new_bucket.value == NULL) {
        free(new_bucket.key);
        return MEMORY_ALLOCATION_ERROR;
    }

    memcpy(new_bucket.key, key, ht->key_size);
    memcpy(new_bucket.value, value, ht->value_size);

    err = u_list_insert(bucket, 0, &new_bucket);
    if (err) {
        ht->bucket_destructor(&new_bucket);
        return err;
    }

    ht->size++;

    if (bucket->size > ht->max_chain_length) {
        ht->max_chain_length = bucket->size;
    }

    err = hash_table_get_load_factor(ht, &load_factor);
    if (err) {
        return err;
    }
    err = hash_table_get_chain_length_factor(ht, &chain_length_factor);
    if (err) {
        return err;
    }
    if (load_factor > 0.75 || chain_length_factor >= 2.0) {
        err = hash_table_resize(ht, HASH_TABLE_GROWTH_FACTOR);
        if (err) {
            return err;
        }
    }

    return EXIT_SUCCESS;
}

err_t hash_table_get(hash_table *ht, const void *key,
                     void **value_placeholder) {
    if (ht == NULL || key == NULL || value_placeholder == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    size_t index = ht->hash(key, ht->key_size, ht->capacity);
    u_list *bucket = ht->buckets[index];
    hash_table_bucket search;
    u_list_node *node = NULL;
    err_t err;

    if (bucket->size == 1) {  // good case
        *value_placeholder = ((hash_table_bucket *)bucket->first->data)->value;
        return EXIT_SUCCESS;
    }
    // bad case :(
    search.key = (void *)key;
    err = u_list_get_node_by_value(bucket, &search, ht->keys_comparer, &node);
    if (err != EXIT_SUCCESS && err != NO_SUCH_ENTRY_IN_COLLECTION) {
        return err;
    }

    if (err == NO_SUCH_ENTRY_IN_COLLECTION) {
        return KEY_NOT_FOUND;
    }

    *value_placeholder = ((hash_table_bucket *)node->data)->value;

    return EXIT_SUCCESS;
}

err_t hash_table_dispose(hash_table *ht, const void *key) {
    if (ht == NULL || key == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    size_t index = ht->hash(key, ht->key_size, ht->capacity);
    err_t err;
    hash_table_bucket search;
    u_list_node *node = NULL;
    u_list *bucket = ht->buckets[index];
    double load_factor = 0;

    search.key = (void *)key;

    if (bucket->size == 1) {
        err = u_list_delete_by_index(bucket, 0);
        if (err != EXIT_SUCCESS || err != NO_SUCH_ENTRY_IN_COLLECTION) {
            return err;
        }
        if (err == NO_SUCH_ENTRY_IN_COLLECTION) {
            return KEY_NOT_FOUND;
        }
    } else {
        err = u_list_delete_by_value(bucket, &search, ht->keys_comparer);
        if (err != EXIT_SUCCESS || err != NO_SUCH_ENTRY_IN_COLLECTION) {
            return err;
        }
        if (err == NO_SUCH_ENTRY_IN_COLLECTION) {
            return KEY_NOT_FOUND;
        }
    }

    ht->size--;

    if (bucket->size < ht->min_chain_length) {
        ht->min_chain_length = bucket->size;
    }

    err = hash_table_get_load_factor(ht, &load_factor);
    if (err) {
        return err;
    }
    if (load_factor < 0.25) {
        err = hash_table_resize(ht, -HASH_TABLE_GROWTH_FACTOR);
        if (err) {
            return err;
        }
    }

    return EXIT_SUCCESS;
}

err_t hash_table_resize(hash_table *ht, int size_modifier) {
    if (ht == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    size_t new_capacity = ht->capacity * size_modifier, new_index = 0;
    u_list **new_buckets = NULL, *old_bucket = NULL;
    hash_table_bucket *entry = NULL;
    u_list_node *node = NULL, *father = NULL;
    size_t i = 0, j = 0;
    err_t err = 0;

    new_buckets = (u_list **)calloc(new_capacity, sizeof(u_list *));
    if (new_buckets == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }

    for (i = 0; i < new_capacity; ++i) {
        err = u_list_init(&(new_buckets[i]), sizeof(hash_table_bucket),
                          ht->bucket_destructor);
        if (err) {
            for (j = 0; j < i; ++j) {
                u_list_free(new_buckets[j]);
            }
            free(new_buckets);
            return err;
        }
    }

    for (i = 0; i < ht->capacity; ++i) {
        old_bucket = ht->buckets[i];
        node = old_bucket->first;
        while (node != NULL) {
            entry = node->data;
            new_index = ht->hash(entry->key, ht->key_size, new_capacity);
            err = u_list_insert(new_buckets[i], 0, entry);
            if (err) {
                for (j = 0; j < new_capacity; ++j) {
                    u_list_free(new_buckets[j]);
                }
                free(new_buckets);
                return err;
            }
            father = node;
            node = node->next;
            free(father->data);
            free(father);
        }
        free(old_bucket);
    }
    free(ht->buckets);
    ht->buckets = new_buckets;
    ht->capacity = new_capacity;

    return EXIT_SUCCESS;
}

err_t hash_table_get_load_factor(hash_table *ht,
                                 double *load_factor_placeholder) {
    if (ht == NULL || load_factor_placeholder == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    if (ht->capacity == 0) {
        return ZERO_DIVISION;
    }

    *load_factor_placeholder = (double)ht->size / (double)ht->capacity;

    return EXIT_SUCCESS;
}

err_t hash_table_get_chain_length_factor(
    hash_table *ht, double *chain_length_factor_placeholder) {
    if (ht == NULL || chain_length_factor_placeholder == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    if (ht->min_chain_length == 0) {
        *chain_length_factor_placeholder = 1;
        return EXIT_SUCCESS;
    }
    *chain_length_factor_placeholder =
        (double)ht->max_chain_length / (double)ht->min_chain_length;

    return EXIT_SUCCESS;
}
