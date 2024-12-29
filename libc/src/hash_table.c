#include "../hash_table.h"

#include <stdint.h>
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

size_t djb2_to_decimal(const String str) {
    size_t hash = 5381;
    size_t len = string_len(str);

    for (size_t i = 0; i < len; i++) {
        hash = ((hash << 5) + hash) + str[i];  // hash * 33 + str[i]
    }

    return hash;
}

size_t djb2_hash(const void *key, size_t key_size, size_t capacity) {
    if (key == NULL || capacity == 0) {
        return 0;
    }

    const String *string_key = (const String *)key;
    size_t hash_value = djb2_to_decimal(*string_key);

    return hash_value % capacity;
}

size_t murmur_to_decimal(const String str, size_t seed) {
    size_t len = string_len(str);
    size_t hash = seed;
    size_t c1 = 0xcc9e2d51;
    size_t c2 = 0x1b873593;

    for (size_t i = 0; i < len; i++) {
        size_t k = (size_t)str[i];
        k *= c1;
        k = (k << 15) | (k >> (sizeof(size_t) * 8 - 15));  // ROTL(k, 15)
        k *= c2;

        hash ^= k;
        hash = (hash << 13) |
               (hash >> (sizeof(size_t) * 8 - 13));  // ROTL(hash, 13)
        hash = hash * 5 + 0xe6546b64;
    }

    hash ^= len;
    hash ^= (hash >> 16);
    hash *= 0x85ebca6b;
    hash ^= (hash >> 13);
    hash *= 0xc2b2ae35;
    hash ^= (hash >> 16);

    return hash;
}

size_t murmur_hash(const void *key, size_t key_size, size_t capacity) {
    if (key == NULL || capacity == 0) {
        return 0;
    }

    const String *string_key = (const String *)key;
    size_t hash_value = murmur_to_decimal(*string_key, 42);
    return hash_value % capacity;
}

void sha256_to_string(const String str, unsigned char output[32]) {
    static const uint32_t k[64] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
        0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
        0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
        0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
        0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
        0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
        0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
        0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
        0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

    uint32_t h[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
                     0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

    size_t len = string_len(str);
    size_t padded_len = ((len + 8) / 64 + 1) * 64;
    unsigned char *padded = calloc(padded_len, 1);
    memcpy(padded, str, len);
    padded[len] = 0x80;

    uint64_t bit_len = len * 8;
    for (size_t i = 0; i < 8; i++) {
        padded[padded_len - 1 - i] = (bit_len >> (i * 8)) & 0xFF;
    }

    for (size_t chunk = 0; chunk < padded_len; chunk += 64) {
        uint32_t w[64] = {0};
        for (size_t i = 0; i < 16; i++) {
            w[i] = (padded[chunk + i * 4] << 24) |
                   (padded[chunk + i * 4 + 1] << 16) |
                   (padded[chunk + i * 4 + 2] << 8) |
                   (padded[chunk + i * 4 + 3]);
        }

        for (size_t i = 16; i < 64; i++) {
            uint32_t s0 = ((w[i - 15] >> 7) | (w[i - 15] << (32 - 7))) ^
                          ((w[i - 15] >> 18) | (w[i - 15] << (32 - 18))) ^
                          (w[i - 15] >> 3);
            uint32_t s1 = ((w[i - 2] >> 17) | (w[i - 2] << (32 - 17))) ^
                          ((w[i - 2] >> 19) | (w[i - 2] << (32 - 19))) ^
                          (w[i - 2] >> 10);
            w[i] = w[i - 16] + s0 + w[i - 7] + s1;
        }

        uint32_t a = h[0];
        uint32_t b = h[1];
        uint32_t c = h[2];
        uint32_t d = h[3];
        uint32_t e = h[4];
        uint32_t f = h[5];
        uint32_t g = h[6];
        uint32_t h_var = h[7];

        for (size_t i = 0; i < 64; i++) {
            uint32_t S1 = ((e >> 6) | (e << (32 - 6))) ^
                          ((e >> 11) | (e << (32 - 11))) ^
                          ((e >> 25) | (e << (32 - 25)));
            uint32_t ch = (e & f) ^ ((~e) & g);
            uint32_t temp1 = h_var + S1 + ch + k[i] + w[i];
            uint32_t S0 = ((a >> 2) | (a << (32 - 2))) ^
                          ((a >> 13) | (a << (32 - 13))) ^
                          ((a >> 22) | (a << (32 - 22)));
            uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
            uint32_t temp2 = S0 + maj;

            h_var = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }

        h[0] += a;
        h[1] += b;
        h[2] += c;
        h[3] += d;
        h[4] += e;
        h[5] += f;
        h[6] += g;
        h[7] += h_var;
    }

    free(padded);

    for (size_t i = 0; i < 8; i++) {
        output[i * 4] = (h[i] >> 24) & 0xFF;
        output[i * 4 + 1] = (h[i] >> 16) & 0xFF;
        output[i * 4 + 2] = (h[i] >> 8) & 0xFF;
        output[i * 4 + 3] = h[i] & 0xFF;
    }
}

size_t sha256_hash(const void *key, size_t key_size, size_t capacity) {
    if (key == NULL || capacity == 0) {
        return 0;
    }

    const String *string_key = (const String *)key;
    unsigned char sha_output[32];
    sha256_to_string(*string_key, sha_output);

    size_t hash_value = 0;
    for (size_t i = 0; i < sizeof(size_t); i++) {
        hash_value = (hash_value << 8) | sha_output[i];
    }

    return hash_value % capacity;
}
