#include "../matrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>



double** random_matrix_generate(int height, int width, int max, int min) {
    int i, j;
    double** matrix = (double**)malloc(sizeof(double*) * height);
    srand(time(NULL));
    for (i = 0; i < height; ++i) {
        matrix[i] = (double*)malloc(sizeof(double) * width);
        for (j = 0; j < width; ++j) {
            matrix[i][j] = (rand() % (max - min + 1)) + min;
        }
    }
    return matrix;
}


int matrix_free(double** matrix, int height, int width) {
    int i;
    for (i = 0; i < height; ++i) {
        free(matrix[i]);
    }
    free(matrix);
    return 0;
}


int matrix_print(double** matrix, int height, int width) {
    int i, j;
    for (i = 0; i < height; ++i) {
        for (j = 0; j < width; ++j) {
            printf("%5lf ", matrix[i][j]);
        }
        printf("\n");
    }
    return 0;
}

/* NOW THIS SH!T FINALLY WORKS */
int gaussian_elimination(double** matrix, int size) {
    int i, j, pivot;
    float factor;

    // Iterate over each row for pivot element
    for (pivot = 0; pivot < size; pivot++) {
        // Iterate over each row below pivot to make element umder diag = 0
        for (i = pivot + 1; i < size; i++) {
            // Check for 0-division
            if (matrix[pivot][pivot] != 0) {
                factor = (float)matrix[i][pivot] / (float)matrix[pivot][pivot];  // Calculate factor to scale row k
                // Perform row reduction
                for (j = 0; j < size; j++) {
                    matrix[i][j] = matrix[i][j] - (factor * matrix[pivot][j]);  // Subtract scaled row k from row i
                }
            }
        }
    }
    return 0;
}


double determinant(double** matrix, int size) {
    if (size == 2) {
        return ((matrix[0][0] * matrix[1][1]) - (matrix[0][1] * matrix[1][0]));
    }
    
    int det = 1, i;
    double** new_matrix;
    new_matrix = generate_zero_matrix(size);
    matrixcpy(new_matrix, matrix, size, size);


    gaussian_elimination(new_matrix, size);
    printf("\n\n\n");
    for (i = 0; i < size; ++i) {
        det *= new_matrix[i][i];
    }

    matrix_free(new_matrix, size, size);
    return det;
}


void matrixcpy(double** dest, double** src, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            dest[i][j] = src[i][j];
        }
    }
}


double** generate_zero_matrix(int size) {
    int i, j;
    double** matrix;
    if (!(matrix = (double**)malloc(size * sizeof(double*)))) {
        return NULL;
    }
    for (int i = 0; i < size; ++i) {
        if (!(matrix[i] = (double*)malloc(size * sizeof(double)))) {
            return NULL;
        }
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = 0.0;
        }
    }
    return matrix;
}


double** generate_identity_matrix(int size) {
    int i, j;
    double** matrix;
    if (!(matrix = (double**)malloc(size * sizeof(double*)))) {
        return NULL;
    }
    for (int i = 0; i < size; ++i) {
        if (!(matrix[i] = (double*)malloc(size * sizeof(double)))) {
            return NULL;
        }
        for (int j = 0; j < size; ++j) {
            if (i == j) {
                matrix[i][j] = 1.0;
            } else {
                matrix[i][j] = 0.0;
            }       
        }
    }
    return matrix;
}
