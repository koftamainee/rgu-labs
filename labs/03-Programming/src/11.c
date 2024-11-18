#include <stdio.h>

#include "../../../libs/cstring.h"
#include "../../../libs/errors.h"

int tokenize(char *inital, int (*detector)(int), int accept_empty_lexems,
             char ***lexems, size_t *lexems_count);

int program_03_11(int argc, char *argv[]) {
  int i, err;

  String str = string_from("Hello, world!");
  String str2 = string_init();
  String str3 = string_from("");
  String str4 = string_init();
  if (str == NULL || str2 == NULL || str3 == NULL) {
    return MEMORY_ALLOCATE_ERROR;
  }

  for (i = 0; i < 52; i++) {
    err = string_add(&str2, i + 50);
    if (err) {
      string_free(str);
      string_free(str2);
      string_free(str3);
      return err;
    }
  }
  printf("string_from(): ");
  string_print(str);
  printf("\nstring_init(): ");
  string_print(str2);
  printf("\n");

  string_grow(&str, 2);
  printf("string after -grow(): ");
  string_print(str);

  err = string_cpy_c(&str2, " string_cpy_c_test ");
  if (err) {
    string_free(str);
    string_free(str2);
    string_free(str3);
    return err;
  }
  printf("\nstring_cpy_c(): ");
  string_print(str2);

  err = string_cat_c(&str2, " string_cat_c_test ");
  if (err) {
    string_free(str);
    string_free(str2);
    string_free(str3);
    return err;
  }
  printf("\nstring_cat_c(): ");
  string_print(str2);

  printf("\nnow str3 == str2 (via string_cpy()): ");

  err = string_cpy(&str3, &str2);
  if (err) {
    string_free(str);
    string_free(str2);
    string_free(str3);
    return err;
  }

  string_print(str3);

  printf("\nlet's double the content of str2 (via string_cat()): ");

  err = string_cat(&str2, &str3);
  if (err) {
    string_free(str);
    string_free(str2);
    string_free(str3);
    return err;
  }

  string_print(str2);

  string_free(str);
  string_free(str2);
  string_free(str3);
  return 0;
}