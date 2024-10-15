#include <stdlib.h>
#include <stdio.h>

#include "../../../libs/matrix.h"

#define MATRIX_HEIGHT_1 12
#define MATRIX_WIDTH_1 10
#define MATRIX_HEIGHT_2 10
#define MATRIX_WIDTH_2 5

int** multiplicate_matrix(int** matrix1, int** matrix2, int rows1, int cols1, int cols2);

int program_00_10() {
    printf("MATRIX 1:\n");
    int** matrix1 = random_matrix_generate(MATRIX_HEIGHT_1, MATRIX_WIDTH_1, 100, -100);
    matrix_print(matrix1, MATRIX_HEIGHT_1, MATRIX_WIDTH_1);
    printf("MATRIX 2:\n");
    int** matrix2 = random_matrix_generate(MATRIX_HEIGHT_2, MATRIX_WIDTH_2, 100, -100);
    matrix_print(matrix2, MATRIX_HEIGHT_2, MATRIX_WIDTH_2);

    if (MATRIX_WIDTH_1 != MATRIX_HEIGHT_2) {
        printf("Matrix multiplication not possible: Matrix1's width (%d) doesn't match Matrix2's height (%d).\n", MATRIX_WIDTH_1, MATRIX_HEIGHT_2);
        return -1;
    }

    int** new_matrix = multiplicate_matrix(matrix1, matrix2, MATRIX_HEIGHT_1, MATRIX_WIDTH_1, MATRIX_WIDTH_2);
    
    printf("MATRIX 1 @ MATRIX 2:\n");
    matrix_print(new_matrix, MATRIX_HEIGHT_1, MATRIX_WIDTH_2);

    matrix_free(matrix1, MATRIX_HEIGHT_1, MATRIX_WIDTH_1);
    matrix_free(matrix2, MATRIX_HEIGHT_2, MATRIX_WIDTH_2);
    matrix_free(new_matrix, MATRIX_HEIGHT_1, MATRIX_WIDTH_2);

    return 0;
}


int** multiplicate_matrix(int** matrix1, int** matrix2, int rows1, int cols1, int cols2) {
    int** new_matrix = (int**)malloc(rows1 * sizeof(int*));
    for (int i = 0; i < rows1; i++) {
        new_matrix[i] = (int*)malloc(cols2 * sizeof(int));
    }

    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            new_matrix[i][j] = 0;
            for (int k = 0; k < cols1; k++) {
                new_matrix[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    return new_matrix;
}
