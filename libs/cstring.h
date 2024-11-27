#ifndef CSTRING_H_
#define CSTRING_H_

#include <stdlib.h>

#define STRING_GROWTH_FACTOR (2)
#define STRING_BASE_CAPACITY (16)

typedef char *String;

typedef struct {
  size_t length;
  size_t capacity;
} String_metadata_t;

#define __cstring_string_to_base(str) (&((String_metadata_t *)(str))[-1])
#define __cstring_base_to_string(str) ((char *)&((String_metadata_t *)(str))[1])

#define string_len(str) (str ? __cstring_string_to_base(str)->length : 0)
#define string_cap(str) (str ? __cstring_string_to_base(str)->capacity : 0)

String string_init();
String string_from(const char *str);

void string_free(const String str);

int string_add(String *str, char c);

void string_print(const String str);

int string_cmp(const String str1, const String str2);
int string_lex_cmp(const String str1, const String str2);

int string_cpy(String *dest, const String *src);
int string_cpy_c(String *dest, const char *src);
int string_cat(String *dest, const String *src);
int string_cat_c(String *dest, const char *src);
int string_str(const String haystack, const String needle);
int string_str_c(const String haystack, const char *needle);

int string_grow(String *str, size_t new_size);

#endif