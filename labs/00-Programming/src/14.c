#include <stdio.h>
#include <stdlib.h>

#include "../../../libs/arrays.h"
#include "../../../libs/input.h"

#define ARRAY_SIZE 10000

int program_00_14() {

  FILE *fptr = fopen("14.txt", "r");
  if (fptr == NULL) {
    printf("Error opening file!\n");
    return 1;
  }
  int *array = (int *)malloc(sizeof(int) * ARRAY_SIZE);
  read_array_from_file(fptr, array);
  print_array(array, ARRAY_SIZE);
  int ans[4];
  find_max_and_min(array, ARRAY_SIZE, ans);
  printf("Max element: %d, index: %d\nMin element: %d, index: %d\n", ans[0],
         ans[1], ans[2], ans[3]);

  free(array);
  fclose(fptr);
  return 0;
}