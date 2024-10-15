#ifndef MATRIX_H
#define MATRIX_H

double** random_matrix_generate(int height, int width, int max, int min);
double** generate_zero_matrix(int size);
double** generate_identity_matrix(int size);
int matrix_free(double** matrix, int height, int width);
int matrix_print(double** matrix, int height, int width);

int gaussian_elimination(double** matrix, int size);
double determinant(double** matrix, int size);

void matrixcpy(double** dest, double** src, int rows, int cols);

int inverse_matrix(double** matrix, double** inverse, int size);

#endif