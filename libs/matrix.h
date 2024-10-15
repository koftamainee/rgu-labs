#ifndef MATRIX_H
#define MATRIX_H

int** random_matrix_generate(int height, int width, int max, int min);
int** generate_zero_matrix(int size);
int** generate_identity_matrix(int size);
int matrix_free(int** matrix, int height, int width);
int matrix_print(int** matrix, int height, int width);

int LUdecompose(int** matrix,int** lower, int** upper, int size);
int determinant(int** matrix, int size);

void matrixcpy(int** dest, int** src, int rows, int cols);

#endif