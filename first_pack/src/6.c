#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MATRIX_SIZE 5
#define MIN -10000
#define MAX 10000


int** matrix_init(int size);
int matrix_free(int** matrix, int size);
int matrix_print(int** matrix, int size);
int find_det(int** matrix, int size);


int main() {
    int** matrix = matrix_init(MATRIX_SIZE);

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

    matrix_print(matrix, MATRIX_SIZE);

    int det = find_det(matrix, MATRIX_SIZE);

    printf("Determinant for matrix is: %d\n", det);

    matrix_free(matrix, MATRIX_SIZE);
    return 0;
}


int** matrix_init(int size) {
    srand(time(NULL));
    int** matrix = (int**)malloc(sizeof(int*) * size);
    for (int i = 0; i < size; ++i) {
        matrix[i] = (int*)malloc(sizeof(int) * size);
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = (rand() % (MAX - MIN + 1)) + MIN;
        }
    }

    return matrix;
}


int matrix_free(int** matrix, int size) {
    for (int i = 0; i < size; ++i) {
        free(matrix[i]);
    }
    free(matrix);
    return 0;
}


int matrix_print(int** matrix, int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            printf("%5d ", matrix[i][j]);
        }
        printf("\n");
    }
    return 0;
}



int find_det(int** matrix, int size) {
    int det;
    /*
    // 
    // TODO: implement Laplace expansion
    //
    */
    return det;
}
