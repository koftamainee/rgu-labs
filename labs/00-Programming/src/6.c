#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "../../../libs/matrix.h"

#define MATRIX_SIZE 3


int program_00_6() {
    int i;
    double det, err;
    double** matrix;
    random_matrix_generate(&matrix, MATRIX_SIZE, MATRIX_SIZE, 100, -100);

    if (matrix == NULL) {
        printf("Memory alocation error. Exit code 1.");
        return 1;
    }
    for (i = 0; i < MATRIX_SIZE; ++i) {
        if (matrix[i] == NULL) {
        printf("Memory alocation error. Exit code 1.");
        return 1;
    }
    
    }

    matrix_print(&matrix, MATRIX_SIZE, MATRIX_SIZE);

    err = determinant(&matrix, MATRIX_SIZE, &det);

    printf("Determinant for matrix is: %lf\n", det);

    matrix_free(&matrix, MATRIX_SIZE, MATRIX_SIZE);
    return 0;
}
