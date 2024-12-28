#include <ctype.h>
#include <stdlib.h>

#include "../../../libc/cstring.h"
#include "../../../libc/hash_table.h"

int string_keys_comparer(const void *a, const void *b);

void strnig_string_bucket_destructor(void *b);

size_t base62_hash(const void *key, size_t key_size, size_t capacity);
size_t base62_to_decimal(const String str);

int program_04_9(int argc, char *argv[]) {
    err_t err = 0;
    hash_table *defines = NULL;

    err = hash_table_init(&defines, string_keys_comparer, base62_hash,
                          sizeof(String *), sizeof(String *),
                          strnig_string_bucket_destructor);

    String str = string_from("Hello world!");
    String key = string_from("Greet");

    err = hash_table_set(defines, &key, &str);
    if (err) {
        return err;
    }

    String *ret = NULL;

    err = hash_table_get(defines, &key, (void **)&ret);
    if (err) {
        return err;
    }

    string_print(*ret);

    hash_table_free(defines);

    return EXIT_SUCCESS;
}

int string_keys_comparer(const void *a, const void *b) {
    const hash_table_bucket *b1 = a, *b2 = b;
    String s1 = *(String *)(b1->key);
    String s2 = *(String *)(b2->key);
    return string_cmp(s1, s2);
}

void strnig_string_bucket_destructor(void *b) {
    hash_table_bucket *bucket = b;

    string_free(*(String *)bucket->value);
    string_free(*(String *)bucket->key);

    free(bucket->value);
    free(bucket->key);

    free(bucket);
}

size_t base62_hash(const void *key, size_t key_size, size_t capacity) {
    const String key_s = *(String *)key;
    size_t index = base62_to_decimal(key_s);
    return index % capacity;
}

size_t base62_to_decimal(const String str) {
    size_t base = 62;
    size_t result = 0;
    size_t value = 0;
    size_t i = 0;
    char c = 0;

    for (i = 0; i < string_len(str); ++i) {
        c = str[i];
        value = 0;
        if (isdigit(c)) {
            value = c - '0';
        } else if (isupper(c)) {
            value = c - 'A' + 10;
        } else if (islower(c)) {
            value = c - 'a' + 36;
        } else {
            continue;
        }
        result = result * base + value;
    }
    return result;
}
