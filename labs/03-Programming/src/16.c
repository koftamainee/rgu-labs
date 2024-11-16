#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// add vilka pls
#include "../../../libs/memory.h"

#define ITEMS_COUNT (5)

// Errors
#define OK (0)
#define ITEMS_NULL (1)
#define RESULT_PERMUTATIONS_NULL (2)
#define RESULT_PERMUTATIONS_COUNT_NULL (3)
#define EQUALITY_COMPARER_NULL (4)
#define MEMORY_ALLOCATE_ERROR (5)
#define EQUAL_ELEMENTS_FOUND (6)
#define ERROR_OPENING_THE_FILE (7)
#define CURRENT_PERMUTATION_NULL (8)
#define NOT_INSERTED_ITEMS_NULL (9)
#define RESULT_PERMUTATIONS_INDEX_TO_INSERT_PERMUTATION_NULL (10)

int permutations(int *items, size_t items_count, int ***result_permutations,
                 size_t *result_permutations_count,
                 int (*equality_comparer)(int const *, int const *));

int __permutations_inner(
    size_t items_count, int *current_permutation,
    size_t current_permutation_to_insert_index, int *not_inserted_items,
    int not_inserted_items_count, int *const *result_permutations,
    size_t *result_permutations_index_to_insert_permutation);

int compare_int_numbers(int const *a, int const *b);

int program_03_16(int argc, char *argv[]) {
  int i, j, err;
  size_t result_permutations_count = 0;
  int **result_permutations = NULL;
  int items[ITEMS_COUNT];
  FILE *fptr = NULL;
  char const file_path[] = "result.txt";

  fptr = fopen(file_path, "w");
  if (fptr == NULL) {
    return ERROR_OPENING_THE_FILE;
  }

  printf("Inital array: { ");
  srand((unsigned int)time(NULL));
  for (i = 0; i < ITEMS_COUNT; ++i) {
    items[i] = rand() % 20001 - 10000;
    printf("%d%s", items[i], i == ITEMS_COUNT - 1 ? "" : ", ");
  }
  printf(" }\n");

  err = permutations(items, ITEMS_COUNT, &result_permutations,
                     &result_permutations_count, compare_int_numbers);
  switch (err) {
  case OK:
    for (i = 0; i < result_permutations_count; ++i) {
      fprintf(fptr, "Permutation №%d: { ", i + 1);
      for (j = 0; j < ITEMS_COUNT; ++j) {
        fprintf(fptr, "%d%s", result_permutations[i][j],
                j == ITEMS_COUNT - 1 ? "" : ",");
      }
      fprintf(fptr, " }\n");

      free(result_permutations[i]);
    }
    printf("Permutations have been written to the %s file\n", file_path);
    break;
  case ITEMS_NULL:
    printf("Items ptr is NULL\n");
    break;
  case RESULT_PERMUTATIONS_NULL:
    printf("Result permutation ptr is NULL\n");
    break;
  case RESULT_PERMUTATIONS_COUNT_NULL:
    printf("Result permutation count is NULL\n");
    break;
  case EQUALITY_COMPARER_NULL:
    printf("Equality comparer ptr is NULL\n");
    break;
  case MEMORY_ALLOCATE_ERROR:
    printf("Memory allocation error\n");
    break;
  case EQUAL_ELEMENTS_FOUND:
    printf("There are equal elements in array\n");
    break;
  case CURRENT_PERMUTATION_NULL:
    printf("Current permutation ptr is NULL\n");
    break;
  case NOT_INSERTED_ITEMS_NULL:
    printf("Not inserted items ptr is NULL\n");
    break;
  case RESULT_PERMUTATIONS_INDEX_TO_INSERT_PERMUTATION_NULL:
    printf("Result permutations index to insert permutation ptr is NULL\n");
    break;
  default:
    printf("Undefined behavior 0_o Error code: %d\n", err);
    break;
  }

  vilka("cf", fptr, result_permutations);
  return err;
}

int compare_int_numbers(int const *a, int const *b) { return *a - *b; }

int permutations(int *items, size_t items_count, int ***result_permutations,
                 size_t *result_permutations_count,
                 int (*equality_comparer)(int const *, int const *)) {

  int i, j, err;
  int *current_permutation = NULL;
  int *not_inserted_items = NULL;
  int **target_permutation = NULL;
  size_t permutation_count = 1;
  size_t result_permutations_index_to_insert_permutation = 0;

  // Parametrs validation
  if (items == NULL) {
    return ITEMS_NULL;
  }
  if (result_permutations == NULL) {
    return RESULT_PERMUTATIONS_NULL;
  }
  if (result_permutations_count == NULL) {
    return RESULT_PERMUTATIONS_COUNT_NULL;
  }
  if (equality_comparer == NULL) {
    return EQUALITY_COMPARER_NULL;
  }

  for (i = 0; i < items_count; ++i) {
    for (j = i + 1; j < items_count; ++j) {
      if (!equality_comparer(items + i, items + j)) {
        return EQUAL_ELEMENTS_FOUND;
      }
    }
  }

  // Memory allocation without realloc :-|
  if (!(current_permutation = (int *)malloc(items_count * sizeof(int)))) {
    return MEMORY_ALLOCATE_ERROR;
  }
  if (!(not_inserted_items = (int *)malloc(items_count * sizeof(int)))) {
    return MEMORY_ALLOCATE_ERROR;
  }

  // O(n!) memory usage, inital permutation_count == 1
  for (i = 2; i <= items_count; ++i) {
    permutation_count *= i;
  }

  // Another memory allocation 0_0
  // Alocating <int **>array for all permutations
  if (!(target_permutation =
            (int **)malloc(permutation_count * sizeof(int *)))) {
    vilka("ff", current_permutation, not_inserted_items);
    return MEMORY_ALLOCATE_ERROR;
  }
  for (i = 0; i < permutation_count; ++i) {
    // so big so huge
    if (!(target_permutation[i] = (int *)malloc(items_count * sizeof(int)))) {
      for (j = 0; j < i; ++j) {
        free(target_permutation[j]);
      }
      vilka("fff", target_permutation, not_inserted_items, current_permutation);

      return MEMORY_ALLOCATE_ERROR;
    }
  }

  memcpy(not_inserted_items, items, items_count * sizeof(int));

  // Recursion
  err = __permutations_inner(
      items_count, current_permutation, 0, not_inserted_items, items_count,
      target_permutation, &result_permutations_index_to_insert_permutation);

  if (err) {
    vilka("ff", current_permutation, not_inserted_items);
    return err;
  }

  *result_permutations = target_permutation;
  *result_permutations_count = permutation_count;

  vilka("ff", current_permutation, not_inserted_items);

  return OK;
}

int __permutations_inner(
    size_t items_count, int *current_permutation,
    size_t current_permutation_to_insert_index, int *not_inserted_items,
    int not_inserted_items_count, int *const *result_permutations,
    size_t *result_permutations_index_to_insert_permutation) {

  int i, j, not_inserted_item_index, err;

  // Parametrs validation
  if (current_permutation == NULL) {
    return CURRENT_PERMUTATION_NULL;
  }
  if (not_inserted_items == NULL) {
    return NOT_INSERTED_ITEMS_NULL;
  }
  if (result_permutations == NULL) {
    return RESULT_PERMUTATIONS_NULL;
  }
  if (result_permutations_index_to_insert_permutation == NULL) {
    return RESULT_PERMUTATIONS_INDEX_TO_INSERT_PERMUTATION_NULL;
  }

  // equal to permutation is ready (Base case)
  if (not_inserted_items_count == 0) {
    memcpy(
        result_permutations[*result_permutations_index_to_insert_permutation],
        current_permutation, items_count * sizeof(int));
    ++*result_permutations_index_to_insert_permutation;

    return OK;
  }

  for (not_inserted_item_index = 0;
       not_inserted_item_index < not_inserted_items_count;
       ++not_inserted_item_index) {
    current_permutation[current_permutation_to_insert_index] =
        not_inserted_items[not_inserted_item_index];

    // worst string in world's history
    memcpy(not_inserted_items + not_inserted_item_index,
           not_inserted_items + not_inserted_item_index + 1,
           (not_inserted_items_count - not_inserted_item_index - 1) *
               sizeof(int));

    err = __permutations_inner(items_count, current_permutation,
                               current_permutation_to_insert_index + 1,
                               not_inserted_items, not_inserted_items_count - 1,
                               result_permutations,
                               result_permutations_index_to_insert_permutation);
    if (err) {
      return err;
    }

    // second worst string in world's history
    memcpy(not_inserted_items + not_inserted_item_index + 1,
           not_inserted_items + not_inserted_item_index,
           (not_inserted_items_count - not_inserted_item_index - 1) *
               sizeof(int));

    not_inserted_items[not_inserted_item_index] =
        current_permutation[current_permutation_to_insert_index];
  }

  return OK;
}