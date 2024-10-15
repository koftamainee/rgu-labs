#include <stdlib.h>
#include <stdio.h>

#include "../../../libs/matrix.h"

#define MATRIX_SIZE 5
#define MIN -10000
#define MAX 10000


int transpose_matrix(int** matrix, int height);


int program_00_18() {
    int** matrix = random_matrix_generate(MATRIX_SIZE, MATRIX_SIZE, 100, -100);

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

    transpose_matrix(matrix, MATRIX_SIZE);

    printf("New matrix:\n");
    matrix_print(matrix, MATRIX_SIZE, MATRIX_SIZE);

    matrix_free(matrix, MATRIX_SIZE, MATRIX_SIZE);
    return 0;
}


int transpose_matrix(int** matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            int temp = matrix[i][j];
            matrix[i][j] = matrix[j][i];
            matrix[j][i] = temp;
        }
    }
    return 0;
}