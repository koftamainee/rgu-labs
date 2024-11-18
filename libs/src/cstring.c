#include "../cstring.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../errors.h"

String string_init() {
  String_metadata *str_p = (String_metadata *)malloc(
      (sizeof(char) * STRING_BASE_CAPACITY) + (sizeof(String_metadata)));
  if (str_p == NULL) {
    return NULL;
  }
  str_p->length = 0;
  str_p->capacity = 16;
  return __base_to_string(str_p);
}

String string_from(char *str) {
  int length = strlen(str);
  String_metadata *str_p = (String_metadata *)malloc(
      (sizeof(char) * STRING_BASE_CAPACITY) + (sizeof(String_metadata)));
  if (str_p == NULL) {
    return NULL;
  }
  str_p->length = length;
  str_p->capacity = length;
  memcpy(__base_to_string(str_p), str, length);
  return __base_to_string(str_p);
}

void string_free(String str) {
  if (str == NULL) {
    return;
  }
  free((void *)__string_to_base(str));
}

int string_add(String *str, char c) {
  if (str == NULL || *str == NULL) {
    return DEREFERENCING_NULL_PTR;
  }

  if (string_len(*str) >= string_cap(*str)) {
    size_t new_capacity =
        (string_cap(*str) == 0) ? 1 : string_cap(*str) * STRING_GROWTH_FACTOR;

    int err = string_grow(str, new_capacity);
    if (err) {
      return err;
    }
  }

  (*str)[string_len(*str)] = c;
  __string_to_base(*str)->length++;

  return EXIT_SUCCESS;
}

void string_print(String str) {
  int i;
  for (i = 0; i < string_len(str); ++i) {
    putc(str[i], stdout);
  }
}

int string_cmp(String str1, String str2);
int string_lex_cmp(String str1, String str2, int(comp)(char, char));

int string_cpy(String *dest, String *src) {
  int err;
  int length = string_len(*src);
  if (dest == NULL || src == NULL) {
    return DEREFERENCING_NULL_PTR;
  }
  if (string_cap(*dest) < length) {
    err = string_grow(dest, length);
    if (err) {
      return err;
    }
  }
  memcpy(*dest, *src, length);
  __string_to_base(*dest)->length = length;
  return EXIT_SUCCESS;
}

int string_cpy_c(String *dest, char *src) {
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
  __string_to_base(*dest)->length = length;
  return EXIT_SUCCESS;
}

int string_cat(String *dest, String *src) {
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
  __string_to_base(*dest)->length = length;
  return EXIT_SUCCESS;
}

int string_cat_c(String *dest, char *src) {
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
  __string_to_base(*dest)->length = length;

  return EXIT_SUCCESS;
}

int string_str(String needle, String haystack);

int string_str_c(String needle, char *haystack);

int string_grow(String *str, size_t new_size) {
  if (str == NULL || *str == NULL) {
    return DEREFERENCING_NULL_PTR;
  }

  String_metadata *for_realloc = NULL;
  size_t current_size = string_len(*str);

  if (current_size == new_size) {
    return EXIT_SUCCESS;
  }
  for_realloc = (String_metadata *)realloc(__string_to_base(*str),
                                           (new_size * sizeof(char)) +
                                               sizeof(String_metadata));

  if (for_realloc == NULL) {
    return MEMORY_ALLOCATE_ERROR;
  }

  *str = __base_to_string(for_realloc);

  for_realloc->capacity = new_size;
  if (new_size < current_size) {
    for_realloc->length = new_size;
  } else {
    for_realloc->length = current_size;
  }

  return EXIT_SUCCESS;
}