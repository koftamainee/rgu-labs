#include "../../../libs/matrix.h"

#define MATRIX_HEIGHT 5
#define MATRIX_WIDTH 5

int** multiplicate_matrix(int** matrix1, int** matrix2);

int program_00_10() {
    int** matrix1 = random_matrix_generate(MATRIX_HEIGHT, MATRIX_WIDTH);
    int** matrix2 = random_matrix_generate(MATRIX_HEIGHT, MATRIX_WIDTH);

    matrix_free(matrix1, MATRIX_HEIGHT, MATRIX_WIDTH);
    matrix_free(matrix2, MATRIX_HEIGHT, MATRIX_WIDTH);
    return 0;
}


int** multiplicate_matrix(int** matrix1, int** matrix2) {
    int** new_matrix;
    return new_matrix;
}
