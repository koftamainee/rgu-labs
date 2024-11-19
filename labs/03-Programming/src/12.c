#include <stdio.h>

#include "../../../libs/int_vector.h"

int program_03_12(int argc, char *argv[]) {
  int i;
  int arr[] = {1, 2, 3, 4, 5};
  int_vector vec = int_vector_from(arr, 5);
  for (i = 0; i < int_vector_size(vec); ++i) {
    printf("%d ", vec[i]);
  }
  int_vector_free(vec);
  return 0;
}