#ifndef MATRIX_H
#define MATRIX_H

int __matrix_memory_alocate(double ***matrix, int height, int width);
int random_matrix_generate(double ***matrix, int height, int width, int max,
                           int min);
int generate_zero_matrix(double ***matrix, int size);
int generate_identity_matrix(double ***matrix, int size);
int matrix_free(double ***matrix, int height, int width);
int matrix_print(double ***matrix, int height, int width);

int gaussian_elimination(double ***matrix, int size);
int determinant(double ***matrix, int size, double *det);

int matrixcpy(double ***dest, double ***src, int rows, int cols);

int inverse_matrix(double ***matrix, double ***inverse, int size);

#endif