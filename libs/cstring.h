#ifndef CSTRING_H_
#define CSTRING_H_

#include <stdlib.h>

#define STRING_GROWTH_FACTOR (2)
#define STRING_BASE_CAPACITY (16)

typedef char *String;

typedef struct {
  size_t length;
  size_t capacity;
} String_metadata;

#define __string_to_base(str) (&((String_metadata *)(str))[-1])
#define __base_to_string(str) ((char *)&((String_metadata *)(str))[1])

#define string_len(str) (str ? __string_to_base(str)->length : 0)
#define string_cap(str) (str ? __string_to_base(str)->capacity : 0)

String string_init();
String string_from(char *str);

void string_free(String str);

int string_add(String *str, char c);

void string_print(String str);

int string_cmp(String str1, String str2);
int string_lex_cmp(String str1, String str2, int(comp)(char, char));

int string_cpy(String *dest, String *src);
int string_cpy_c(String *dest, char *src);
int string_cat(String *dest, String *src);
int string_cat_c(String *dest, char *src);
int string_str(String needle, String haystack);

int string_grow(String *str, size_t new_size);

#endif