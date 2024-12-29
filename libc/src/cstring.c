#include "../cstring.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../errors.h"

String string_init() {
    String_metadata_t *str_p = (String_metadata_t *)malloc(
        (sizeof(char) * STRING_BASE_CAPACITY) + (sizeof(String_metadata_t)));
    if (str_p == NULL) {
        return NULL;
    }
    str_p->length = 0;

    str_p->capacity = STRING_BASE_CAPACITY;
    return __cstring_base_to_string(str_p);
}

String string_from(const char *str) {
    int length = strlen(str);
    String_metadata_t *str_p = (String_metadata_t *)malloc(
        (sizeof(char) * length) + (sizeof(String_metadata_t)));
    if (str_p == NULL) {
        return NULL;
    }
    str_p->length = length;
    str_p->capacity = length;
    memcpy(__cstring_base_to_string(str_p), str, length * sizeof(char));
    return __cstring_base_to_string(str_p);
}

void string_free(const String str) {
    if (str == NULL) {
        return;
    }
    free((void *)__cstring_string_to_base(str));
}

int string_add(String *str, char c) {
    if (str == NULL || *str == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    if (string_len(*str) >= string_cap(*str)) {
        size_t new_capacity = (string_cap(*str) == 0)
                                  ? 1
                                  : string_cap(*str) * STRING_GROWTH_FACTOR;

        int err = string_grow(str, new_capacity);
        if (err) {
            return err;
        }
    }

    (*str)[string_len(*str)] = c;
    __cstring_string_to_base(*str)->length++;

    return EXIT_SUCCESS;
}

void string_print(String str) {
    int i;
    if (string_len(str) == 0) {
        return;
    }
    for (i = 0; i < string_len(str); ++i) {
        putc(str[i], stdout);
    }
}

int string_cmp(String str1, String str2) {
    size_t len1, len2;
    // printf("STRING.H compare: %s %s\n", str1, str2);
    int i;
    len1 = string_len(str1);
    len2 = string_len(str2);

    if (len1 != len2) {
        if (len1 >= len2) {
            return str1[len2];
        } else {
            return -str2[len1];
        }
    }
    for (i = 0; i < len1; ++i) {
        if (str1[i] != str2[i]) {
            return str1[i] - str2[i];
        }
    }
    return 0;
}
int string_lex_cmp(String str1, String str2) {
    size_t len1, len2, min;
    int i;
    len1 = string_len(str1);
    len2 = string_len(str2);

    min = (len1 < len2) ? len1 : len2;
    for (i = 0; i < min; ++i) {
        if (str1[i] != str2[i]) {
            return str1[i] - str2[i];
        }
    }
    return 0;
}

int string_cpy(String *dest, const String *src) {
    int err;
    int length;
    if (dest == NULL || src == NULL) {
        return DEREFERENCING_NULL_PTR;
    }
    length = string_len(*src);
    if (string_cap(*dest) < length) {
        err = string_grow(dest, length);
        if (err) {
            return err;
        }
    }
    memcpy(*dest, *src, length);
    __cstring_string_to_base(*dest)->length = length;
    return EXIT_SUCCESS;
}

int string_cpy_c(String *dest, const char *src) {
    int err;
    int length = strlen(src);
    if (dest == NULL || src == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    if (string_cap(*dest) < length) {
        err = string_grow(dest, length);
        if (err) {
            return err;
        }
    }
    memcpy(*dest, src, length);
    __cstring_string_to_base(*dest)->length = length;
    return EXIT_SUCCESS;
}

int string_cat(String *dest, const String *src) {
    int err;
    int length = string_len(*src) + string_len(*dest);
    if (dest == NULL || src == NULL) {
        return DEREFERENCING_NULL_PTR;
    }
    if (string_cap(*dest) < length) {
        err = string_grow(dest, length);
        if (err) {
            return err;
        }
    }
    memcpy(*dest + string_len(*dest), *src, string_len(*src));
    __cstring_string_to_base(*dest)->length = length;
    return EXIT_SUCCESS;
}

int string_cat_c(String *dest, const char *src) {
    int err;
    int length = strlen(src) + string_len(*dest);
    if (dest == NULL || src == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    if (string_cap(*dest) < length) {
        err = string_grow(dest, length);
        if (err) {
            return err;
        }
    }
    memcpy((*dest) + string_len(*dest), src, strlen(src));
    __cstring_string_to_base(*dest)->length = length;

    return EXIT_SUCCESS;
}

int string_str(String haystack, String needle) {
    size_t needle_len = string_len(needle);
    size_t haystack_len = string_len(haystack);

    if (needle_len > haystack_len) {
        return -1;
    }

    for (size_t i = 0; i <= haystack_len - needle_len; ++i) {
        int found = 1;
        for (size_t j = 0; j < needle_len; ++j) {
            if (haystack[i + j] != needle[j]) {
                found = 0;
                break;
            }
        }
        if (found) {
            return i;
        }
    }

    return -1;
}

int string_str_c(String haystack, const char *needle) {
    size_t needle_len = strlen(needle);
    size_t haystack_len = string_len(haystack);

    if (needle_len > haystack_len) {
        return -1;
    }

    for (size_t i = 0; i <= haystack_len - needle_len; ++i) {
        int found = 1;
        for (size_t j = 0; j < needle_len; ++j) {
            if (haystack[i + j] != needle[j]) {
                found = 0;
                break;
            }
        }
        if (found) {
            return i;
        }
    }

    return -1;
}

int string_c_str(const char *haystack, const String needle) {
    size_t needle_len = string_len(needle);
    size_t haystack_len = strlen(haystack);

    if (needle_len > haystack_len) {
        return -1;
    }

    for (size_t i = 0; i <= haystack_len - needle_len; ++i) {
        int found = 1;
        for (size_t j = 0; j < needle_len; ++j) {
            if (haystack[i + j] != needle[j]) {
                found = 0;
                break;
            }
        }
        if (found) {
            return i;
        }
    }

    return -1;
}

int string_grow(String *str, size_t new_size) {
    if (str == NULL || *str == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    String_metadata_t *for_realloc = NULL;
    size_t current_size = string_cap(*str);

    if (current_size == new_size) {
        return EXIT_SUCCESS;
    }
    for_realloc = (String_metadata_t *)realloc(
        __cstring_string_to_base(*str),
        (new_size * sizeof(char)) + sizeof(String_metadata_t));

    if (for_realloc == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }

    *str = __cstring_base_to_string(for_realloc);

    for_realloc->capacity = new_size;
    if (new_size < current_size) {
        for_realloc->length = new_size;
    }

    return EXIT_SUCCESS;
}

void string_fprint(FILE *fout, const String str) {
    size_t i;
    if (string_len(str) == 0) {
        return;
    }
    for (i = 0; i < string_len(str); ++i) {
        fputc(str[i], fout);
    }
}
