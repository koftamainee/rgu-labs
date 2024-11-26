#include <stdio.h>
#include <stdlib.h>

#include "../../../libs/int_vector.h"

int sums_decomposition(int value, int_vector **result_decompositions,
                       size_t *result_decompositions_count,
                       int allowed_equal_sum_components);

int program_03_13(int argc, char *argv[]) {
  int_vector *res = NULL;
  size_t len = 0, i = 0, j = 0;
  int err = 0;

  err = sums_decomposition(52, &res, &len, 1);
  for (i = 0; i < len; ++i) {
    for (j = 0; j < int_vector_size(res[i]); ++i) {
      printf("%d ", res[i][j]);
    }
    int_vector_free(res[i]);
    printf("\n");
  }
  free(res);
  return err;
}

int sums_decomposition(int value, int_vector **result_decompositions,
                       size_t *result_decompositions_count,
                       int allowed_equal_sum_components) {
  return EXIT_SUCCESS;
}