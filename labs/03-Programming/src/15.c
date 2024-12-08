#include <stdio.h>
#include <stdlib.h>

#define MATRIX_SIZE (5)
#define INITIAL_CAPACITY (16)
#define GROWTH_FACTOR (2)

#include "../../../libc/errors.h"
#include "../../../libc/memory.h"

typedef int tvalue;

int find_saddle_points(tvalue const *const *matrix, size_t matrix_rows_count,
                       size_t matrix_columns_count,
                       tvalue ***found_saddle_points_storage,
                       size_t *found_saddle_points_count_storage,
                       int (*comparer)(tvalue const *, tvalue const *),
                       int is_comparison_is_strict);

int comparer_for_saddle_points(tvalue const *first, tvalue const *second);

int program_03_15(int argc, char *argv[]) {
    int err = 0;
    size_t len = 0, i = 0, j = 0;
    tvalue **ans = NULL;
    tvalue matrix_static[MATRIX_SIZE][MATRIX_SIZE] = {{12, 11, 19, 25, 14},
                                                      {21, 10, 16, 9, 22},
                                                      {15, 5, 8, 6, 11},
                                                      {17, 1, 12, 7, 20},
                                                      {18, 2, 21, 19, 9}};

    tvalue **matrix = (tvalue **)malloc(sizeof(tvalue *) * MATRIX_SIZE);
    if (matrix == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }
    for (i = 0; i < MATRIX_SIZE; ++i) {
        matrix[i] = (tvalue *)malloc(sizeof(tvalue) * MATRIX_SIZE);
        if (matrix[i] == NULL) {
            for (j = 0; j < i; ++j) {
                free(matrix[j]);
            }
            free(matrix);
            return MEMORY_ALLOCATION_ERROR;
        }
        for (j = 0; j < MATRIX_SIZE; ++j) {
            matrix[i][j] = matrix_static[i][j];
        }
    }

    err = find_saddle_points((tvalue const *const *)matrix, MATRIX_SIZE,
                             MATRIX_SIZE, &ans, &len,
                             comparer_for_saddle_points, 0);
    if (err) {
        for (i = 0; i < len; ++i) {
            free(ans[i]);
        }
        free(ans);
        return err;
    }
    printf("Coords\n");
    for (i = 0; i < len; ++i) {
        printf("%d %d\n", ans[i][0], ans[i][1]);
        free(ans[i]);
    }
    free(ans);

    for (i = 0; i < MATRIX_SIZE; ++i) {
        free(matrix[i]);
    }
    free(matrix);

    return EXIT_SUCCESS;
}

int find_saddle_points(tvalue const *const *matrix, size_t matrix_rows_count,
                       size_t matrix_columns_count,
                       tvalue ***found_saddle_points_storage,
                       size_t *found_saddle_points_count_storage,
                       int (*comparer)(tvalue const *, tvalue const *),
                       int is_comparison_is_strict) {
    size_t i, j, k, current_capacity;
    int comparer_result = 0, is_saddle = 1, err = 0;

    if (matrix == NULL || found_saddle_points_count_storage == NULL ||
        found_saddle_points_storage == NULL || comparer == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    current_capacity = INITIAL_CAPACITY;
    *found_saddle_points_storage =
        (tvalue **)malloc(INITIAL_CAPACITY * sizeof(tvalue *));
    if (*found_saddle_points_storage == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }

    for (i = 0; i < matrix_rows_count; ++i) {
        for (j = 0; j < matrix_columns_count; j++) {
            is_saddle = 1;
            // Check if max in column
            for (k = 0; k < matrix_rows_count; ++k) {
                if (k != i &&
                    !(is_comparison_is_strict
                          ? comparer(&matrix[i][j], &matrix[k][j]) > 0
                          : comparer(&matrix[i][j], &matrix[k][j]) >= 0)) {
                    is_saddle = 0;
                    break;
                }
            }

            if (is_saddle) {
                // Check if min in row
                for (k = 0; k < matrix_columns_count; ++k) {
                    if (k != j &&
                        !(is_comparison_is_strict
                              ? comparer(&matrix[i][j], &matrix[i][k]) < 0
                              : comparer(&matrix[i][j], &matrix[i][k]) <= 0)) {
                        is_saddle = 0;
                        break;
                    }
                }

                if (is_saddle) {
                    if (*found_saddle_points_count_storage + 1 >=
                        current_capacity) {
                        err = rerealloc((void **)found_saddle_points_storage,
                                        current_capacity * GROWTH_FACTOR *
                                            sizeof(tvalue *));
                        if (err) {
                            return err;
                        }
                        current_capacity *= GROWTH_FACTOR;
                    }

                    (*found_saddle_points_storage)[(
                        *found_saddle_points_count_storage)] =
                        (tvalue *)malloc(2 * sizeof(tvalue));
                    if ((*found_saddle_points_storage)[(
                            *found_saddle_points_count_storage)] == NULL) {
                        return MEMORY_ALLOCATION_ERROR;
                    }
                    (*found_saddle_points_storage)[(
                        *found_saddle_points_count_storage)][0] = i;
                    (*found_saddle_points_storage)[(
                        *found_saddle_points_count_storage)][1] = j;
                    (*found_saddle_points_count_storage)++;
                }
            }
        }
    }
    return EXIT_SUCCESS;
}

int comparer_for_saddle_points(tvalue const *first, tvalue const *second) {
    return *first - *second;
}
