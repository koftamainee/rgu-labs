#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>



int** random_matrix_generate(int height, int width) {
    srand(time(NULL));
    int** matrix = (int**)malloc(sizeof(int*) * height);
    for (int i = 0; i < height; ++i) {
        matrix[i] = (int*)malloc(sizeof(int) * width);
        for (int j = 0; j < width; ++j) {
            matrix[i][j] = (rand() % (10000 - (-10000) + 1)) + (-10000);
        }
    }

    return matrix;
}


int matrix_free(int** matrix, int height, int width) {
    for (int i = 0; i < height; ++i) {
        free(matrix[i]);
    }
    free(matrix);
    return 0;
}


int matrix_print(int** matrix, int height, int width) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
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