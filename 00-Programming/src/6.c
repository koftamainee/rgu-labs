#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "../../libs/matrix.h"

#define MATRIX_SIZE 5
#define MIN -10000
#define MAX 10000


int program_00_6() {
    int i, det;
    int** matrix = random_matrix_generate(MATRIX_SIZE, MATRIX_SIZE);

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

    matrix_print(matrix, MATRIX_SIZE, MATRIX_SIZE);

    det = find_det(matrix, MATRIX_SIZE);

    printf("Determinant for matrix is: %d\n", det);

    matrix_free(matrix, MATRIX_SIZE, MATRIX_SIZE);
    return 0;
}



