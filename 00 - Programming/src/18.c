#include <stdlib.h>
#include <stdio.h>

#include "../../libs/matrix.h"

#define MATRIX_SIZE 5
#define MIN -10000
#define MAX 10000


int transpose_matrix(int** matrix, int height, int width);


int main() {
    int** matrix = random_matrix_generate(MATRIX_SIZE, MATRIX_SIZE);

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

    printf("Old matrix:\n");
    matrix_print(matrix, MATRIX_SIZE, MATRIX_SIZE);

    transpose_matrix(matrix, MATRIX_SIZE, MATRIX_SIZE);

    printf("New matrix:\n");
    matrix_print(matrix, MATRIX_SIZE, MATRIX_SIZE);

    matrix_free(matrix, MATRIX_SIZE, MATRIX_SIZE);
    return 0;
}


int transpose_matrix(int** matrix, int height, int width) {
    return 0;
}