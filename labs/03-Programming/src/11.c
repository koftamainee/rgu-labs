#include <stdio.h>
#include <string.h>

#include "../../../libs/cstring.h"
#include "../../../libs/errors.h"
#include "../../../libs/memory.h"

#define GROWTH_FACTOR (2)

int tokenize(const String inital, int (*detector)(int), int accept_empty_lexems,
             String **lexems, size_t *lexems_count);

int detector_t(int c);

int program_03_11(int argc, char *argv[]) {
  int err, i;
  size_t len = 0;
  String inital = string_from("abc      abc abc");
  String *ans;
  err = tokenize(inital, detector_t, 1, &ans, &len);
  if (err) {
    for (i = 0; i < len; ++i) {
      string_free(ans[i]);
    }
    free(ans);
    return err;
  }

  printf("Total len: %zu\n", len);
  for (i = 0; i < len; ++i) {
    string_print((ans)[i]);
    printf("\n");
  }

  for (i = 0; i < len; ++i) {
    string_free(ans[i]);
  }
  free(ans);
  return EXIT_SUCCESS;
}

int tokenize(const String inital, int (*detector)(int), int accept_empty_lexems,
             String **lexems, size_t *lexems_count) {
  size_t i, j, capacity;
  int err;
  String lexem = string_init();

  const int initial_capacity = 2;

  capacity = initial_capacity;

  *lexems = (String *)malloc(sizeof(String) * initial_capacity);
  if (*lexems == NULL) {
    return MEMORY_ALLOCATE_ERROR;
  }

  if (inital == NULL || detector == NULL || lexems == NULL ||
      lexems_count == NULL) {
    return DEREFERENCING_NULL_PTR;
  }

  *lexems_count = 0;
  for (i = 0; i < string_len(inital); ++i) {
    if (detector(inital[i])) {
      if (string_len(lexem) > 0 && accept_empty_lexems == 0 ||
          accept_empty_lexems) {
        if (*lexems_count + 1 >= capacity) {
          err = rerealloc((void **)lexems,
                          capacity * GROWTH_FACTOR * sizeof(String));
          if (err) {
            return err;
          }
          capacity *= GROWTH_FACTOR;
        }

        if (string_len(lexem) > 0) {
          (*lexems)[*lexems_count] = string_init();
          if ((*lexems)[*lexems_count] == NULL) {
            return MEMORY_ALLOCATE_ERROR;
          }
          err = string_cpy(&((*lexems)[*lexems_count]), &lexem);
          if (err) {
            return err;
          }
          (*lexems_count)++;
          err = string_grow(&lexem, 0);
          if (err) {
            return err;
          }
        } else {
          (*lexems)[*lexems_count] = string_from("");
          if ((*lexems)[*lexems_count] == NULL) {
            return MEMORY_ALLOCATE_ERROR;
          }
          (*lexems_count)++;
          err = string_grow(&lexem, 0);
          if (err) {
            return err;
          }
        }
      }
    } else {
      err = string_add(&lexem, inital[i]);
      if (err) {
        return err;
      }
    }
  }

  if (string_len(lexem) > 0) {
    (*lexems)[*lexems_count] = string_init();
    if ((*lexems)[*lexems_count] == NULL) {
      return MEMORY_ALLOCATE_ERROR;
    }
    err = string_cpy(&((*lexems)[*lexems_count]), &lexem);
    if (err) {
      return err;
    }
    (*lexems_count)++;
    err = string_grow(&lexem, 0);
    if (err) {
      return err;
    }
  }

  string_free(lexem);
  return EXIT_SUCCESS;
}

int detector_t(int c) { return (c == ' ') ? 1 : 0; }
