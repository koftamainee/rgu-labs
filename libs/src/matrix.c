#include "../matrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>



int** random_matrix_generate(int height, int width, int max, int min) {
    int i, j;
    int** matrix = (int**)malloc(sizeof(int*) * height);
    srand(time(NULL));
    for (i = 0; i < height; ++i) {
        matrix[i] = (int*)malloc(sizeof(int) * width);
        for (j = 0; j < width; ++j) {
            matrix[i][j] = (rand() % (max - min + 1)) + min;
        }
    }
    return matrix;
}


int matrix_free(int** matrix, int height, int width) {
    int i;
    for (i = 0; i < height; ++i) {
        free(matrix[i]);
    }
    free(matrix);
    return 0;
}


int matrix_print(int** matrix, int height, int width) {
    int i, j;
    for (i = 0; i < height; ++i) {
        for (j = 0; j < width; ++j) {
            printf("%5d ", matrix[i][j]);
        }
        printf("\n");
    }
    return 0;
}

/* I'M ABSOLUTLY DOESN'T KNOW HOW DOES THIS WORKS */
/* UPD. this doesn't work properly, pls FIXME */
int LUdecompose(int** matrix, int** lower, int** upper, int size) {
    for (int i = 0; i < size; i++) 
    {
        // Upper Triangular
        for (int k = i; k < size; k++)
        {
            // Summation of L(i, j) * U(j, k)
            int sum = 0;
            for (int j = 0; j < i; j++)
                sum += (lower[i][j] * upper[j][k]);

            // Evaluating U(i, k)
            upper[i][k] = matrix[i][k] - sum;
        }

        // Lower Triangular
        for (int k = i; k < size; k++) 
        {
            if (i == k)
                lower[i][i] = 1; // Diagonal as 1
            else 
            {
                // Summation of L(k, j) * U(j, i)
                int sum = 0;
                for (int j = 0; j < i; j++)
                    sum += (lower[k][j] * upper[j][i]);

                // Evaluating L(k, i)
                lower[k][i]
                    = (matrix[k][i] - sum) / upper[i][i];
            }
        }
    }

    return 0; // Success
}


int determinant(int** matrix, int size) {
    int det = 1, i;
    int** L;
    int** U;

    L = generate_identity_matrix(size);
    U = generate_zero_matrix(size);

    LUdecompose(matrix, L, U, size);
    printf("\n\n\n");
    matrix_print(U, size, size);
    for (i = 0; i < size; ++i) {
        det *= U[i][i];
    }

    matrix_free(L, size, size);
    matrix_free(U, size, size);
    return det;
}


int** generate_zero_matrix(int size) {
    int i, j;
    int** matrix;
    if (!(matrix = (int**)malloc(size * sizeof(int*)))) {
        return NULL;
    }
    for (int i = 0; i < size; ++i) {
        if (!(matrix[i] = (int*)malloc(size * sizeof(int)))) {
            return NULL;
        }
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = 0;
        }
    }
    return matrix;
}


int** generate_identity_matrix(int size) {
    int i, j;
    int** matrix;
    if (!(matrix = (int**)malloc(size * sizeof(int*)))) {
        return NULL;
    }
    for (int i = 0; i < size; ++i) {
        if (!(matrix[i] = (int*)malloc(size * sizeof(int)))) {
            return NULL;
        }
        for (int j = 0; j < size; ++j) {
            if (i == j) {
                matrix[i][j] = 1;
            } else {
                matrix[i][j] = 0;
            }       
        }
    }
    return matrix;
}


void matrixcpy(int** dest, int** src, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            dest[i][j] = src[i][j];
        }
    }
}