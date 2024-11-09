#include <stdio.h>
#include <stdlib.h>

#include "../../../libs/matrix.h"

#define MATRIX_SIZE 2

int program_00_19() {
  double **answer = NULL;
  double **matrix = NULL;
  random_matrix_generate(&matrix, MATRIX_SIZE, MATRIX_SIZE, 9, 0);

  if (matrix == NULL) {
    printf("Memory alocation error. Exit code 1.");
    return 1;
  }
  for (int i = 0; i < MATRIX_SIZE; ++i) {
    if (matrix[i] == NULL) {
      printf("Memory alocation error. Exit code 1.");
      return 1;
    }
  }

  random_matrix_generate(&answer, MATRIX_SIZE, MATRIX_SIZE, 100, -100);

  if (answer == NULL) {
    printf("Memory alocation error. Exit code 1.");
    return 1;
  }
  for (int i = 0; i < MATRIX_SIZE; ++i) {
    if (answer[i] == NULL) {
      printf("Memory alocation error. Exit code 1.");
      return 1;
    }
  }

  printf("Old matrix:\n");
  matrix_print(&matrix, MATRIX_SIZE, MATRIX_SIZE);

  if ((inverse_matrix(&matrix, &answer, MATRIX_SIZE)) != 0) {
    printf("Division by 0 error. Aborting.\n");
    return 1;
  }

  printf("New matrix:\n");
  matrix_print(&answer, MATRIX_SIZE, MATRIX_SIZE);

  matrix_free(&matrix, MATRIX_SIZE, MATRIX_SIZE);
  matrix_free(&answer, MATRIX_SIZE, MATRIX_SIZE);
  return 0;
}
