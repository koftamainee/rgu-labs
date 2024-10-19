#include "../matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../errors.h"

int __matrix_memory_alocate(double ***matrix, int height, int width) {
    int i, j;
    *matrix = (double**)malloc(sizeof(double*) * height);
    if (*matrix == NULL) {
        return MEMORY_ALLOCATE_ERROR;
    }
    for (i = 0; i < height; ++i) {
        (*matrix)[i] = (double*)malloc(sizeof(double) * width);
        if ((*matrix)[i] == NULL) {
            for (j = i - 1; j >= 0; --j) {
                free((*matrix)[j]);
            }
            free(*matrix);
            return MEMORY_ALLOCATE_ERROR;
        }
    }
    return 0;
}

int random_matrix_generate(double ***matrix, int height, int width, int min, int max) {
    int i, j;
    if (__matrix_memory_alocate(matrix, height, width) != 0) {
        return MEMORY_ALLOCATE_ERROR;
    }
    for (i = 0; i < height; ++i) {
        for (j = 0; j < width; ++j) {
            (*matrix)[i][j] = (rand() % (max - min + 1)) + min;
        }
    }
    return 0;
}

int matrix_free(double ***matrix, int height, int width) {
    int i;
    for (i = 0; i < height; ++i) {
        free((*matrix)[i]);
    }
    free(*matrix);
    return 0;
}

int matrix_print(double ***matrix, int height, int width) {
    int i, j;
    if (*matrix == NULL) {
        return DEREFERENCING_NULL_PTR;
    }
    for (i = 0; i < height; ++i) {
        if ((*matrix)[i] == NULL) {
            return DEREFERENCING_NULL_PTR;
        }
        for (j = 0; j < width; ++j) {
            printf("%lf ", (*matrix)[i][j]);
        }
        printf("\n");
    }
    return 0;
}

int gaussian_elimination(double ***matrix, int size) {
    int i, j, pivot;
    double factor;

    if (*matrix == NULL) {
        return DEREFERENCING_NULL_PTR;
    }
    for (i = 0; i < size; ++i) {
        if ((*matrix)[i] == NULL) {
            return DEREFERENCING_NULL_PTR;
        }
    }

    for (pivot = 0; pivot < size; pivot++) {
        for (i = pivot + 1; i < size; i++) {
            if ((*matrix)[pivot][pivot] != 0) {
                factor = (*matrix)[i][pivot] / (*matrix)[pivot][pivot];
                for (j = 0; j < size; j++) {
                    (*matrix)[i][j] -= factor * (*matrix)[pivot][j];
                }
            }
        }
    }
    return 0;
}

int determinant(double ***matrix, int size, double* det) {
    int i;
    *det = 1;

    if (*matrix == NULL) {
        return DEREFERENCING_NULL_PTR;
    }
    for (i = 0; i < size; ++i) {
        if ((*matrix)[i] == NULL) {
            return DEREFERENCING_NULL_PTR;
        }
    }

    if (size == 1) {
        *det = (*matrix)[0][0];
        return 0;
    }
    if (size == 2) {
        *det = ((*matrix)[0][0] * (*matrix)[1][1]) - ((*matrix)[0][1] * (*matrix)[1][0]);
        return 0;
    }
    
    double **new_matrix;
    if (__matrix_memory_alocate(&new_matrix, size, size) == MEMORY_ALLOCATE_ERROR) {
        return MEMORY_ALLOCATE_ERROR;
    }
    matrixcpy(&new_matrix, matrix, size, size);

    gaussian_elimination(&new_matrix, size);
    for (i = 0; i < size; ++i) {
        *det *= new_matrix[i][i];
    }

    matrix_free(&new_matrix, size, size);
    return 0;
}

int matrixcpy(double*** dest, double*** src, int rows, int cols) {
    if (*dest == NULL || *src == NULL) {
        return DEREFERENCING_NULL_PTR;
    }
    for (int i = 0; i < rows; i++) {
        if ((*dest)[i] == NULL || (*src)[i] == NULL) {
            return DEREFERENCING_NULL_PTR;
        }
        for (int j = 0; j < cols; j++) {
            (*dest)[i][j] = (*src)[i][j];
        }
    }
    return 0;
}

int generate_zero_matrix(double*** matrix, int size) {
    if (*matrix == NULL) {
        return DEREFERENCING_NULL_PTR;
    }
    int i, j;
    for (i = 0; i < size; ++i) {
        if ((*matrix)[i] == NULL) {
            return DEREFERENCING_NULL_PTR;
        }
        for (j = 0; j < size; ++j) {
            (*matrix)[i][j] = 0.0;
        }
    }
    return 0;
}

int generate_identity_matrix(double ***matrix, int size) {
    int i, j;

    if (__matrix_memory_alocate(matrix, size, size) == MEMORY_ALLOCATE_ERROR) {
        return MEMORY_ALLOCATE_ERROR;
    }

    for (i = 0; i < size; ++i) {
        for (j = 0; j < size; ++j) {
            (*matrix)[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
    return 0;
}

int inverse_matrix(double ***matrix, double ***inverse, int size) {
    int i, j, pivot;
    double factor;
    double** temp_matrix;

    if (__matrix_memory_alocate(&temp_matrix, size, size) == MEMORY_ALLOCATE_ERROR) {
        return MEMORY_ALLOCATE_ERROR;
    }

    matrixcpy(&temp_matrix, matrix, size, size);

    for (i = 0; i < size; ++i) {
        for (j = 0; j < size; ++j) {
            (*inverse)[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }

    for (pivot = 0; pivot < size; ++pivot) {
        if (temp_matrix[pivot][pivot] == 0) {
            return 1; // Handle zero pivot
        }

        factor = temp_matrix[pivot][pivot];
        for (j = 0; j < size; ++j) {
            temp_matrix[pivot][j] /= factor;
            (*inverse)[pivot][j] /= factor;
        }

        for (i = pivot + 1; i < size; ++i) {
            factor = temp_matrix[i][pivot];
            for (j = 0; j < size; ++j) {
                temp_matrix[i][j] -= factor * temp_matrix[pivot][j];
                (*inverse)[i][j] -= factor * (*inverse)[pivot][j];
            }
        }
    }

    for (pivot = size - 1; pivot >= 0; --pivot) {
        for (i = pivot - 1; i >= 0; --i) {
            factor = temp_matrix[i][pivot];
            for (j = 0; j < size; ++j) {
                temp_matrix[i][j] -= factor * temp_matrix[pivot][j];
                (*inverse)[i][j] -= factor * (*inverse)[pivot][j];
            }
        }
    }

    matrix_free(&temp_matrix, size, size);
    return 0;
}
