#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "../../../libc/errors.h"
#include "../../../libc/int_vector.h"
#include "../../../libc/memory.h"

#define INITIAL_CAPACITY (16)
#define GROWTH_FACTOR (2)

int sums_decomposition(int value, int_vector **result_decompositions,
                       size_t *result_decompositions_count,
                       int allowed_equal_sum_components);

int __sums_decomposition_inner(int_vector **result_decompositions,
                               int remainder,
                               size_t *current_decompositions_count,
                               size_t *current_decompositions_capacity,
                               int_vector *temp_vector,
                               int allowed_equal_sum_components);

int program_03_13(int argc, char *argv[]) {
    int_vector *res = NULL;
    size_t len = 0, i = 0, j = 0;
    int err = 0;

    err = sums_decomposition(5, &res, &len, 1);
    for (i = 0; i < len; ++i) {
        for (j = 0; j < int_vector_size(res[i]); ++j) {
            printf("%d ", res[i][j]);
        }

        int_vector_free(res[i]);
        printf("\n");
    }
    free(res);
    return err;
}

int sums_decomposition(int value, int_vector **result_decompositions,
                       size_t *result_decompositions_count,
                       int allowed_equal_sum_components) {
    int err, i, j;
    int_vector temp = NULL;
    size_t capacity = INITIAL_CAPACITY;

    if (result_decompositions == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    if (value < 0) {
        return INVALID_NUMBER;
    }

    *result_decompositions =
        (int_vector *)malloc(sizeof(int_vector) * capacity);
    if ((*result_decompositions) == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }

    temp = int_vector_init();
    if (temp == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }
    err = __sums_decomposition_inner(result_decompositions, value,
                                     result_decompositions_count, &capacity,
                                     &temp, allowed_equal_sum_components);
    int_vector_free(temp);
    if (err) {
        return err;
    }
    return EXIT_SUCCESS;
}

int __sums_decomposition_inner(int_vector **result_decompositions,
                               int remainder,
                               size_t *current_decompositions_count,
                               size_t *current_decompositions_capacity,
                               int_vector *temp_vector,
                               int allowed_equal_sum_components) {
    int err, i, sumand;
    int_vector temp;
    if (result_decompositions == NULL || current_decompositions_count == NULL ||
        temp_vector == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    if (remainder == 0) {
        // doing some growth
        if (*current_decompositions_capacity < *current_decompositions_count) {
            err = rerealloc((void **)result_decompositions,
                            sizeof(int_vector) *
                                (*current_decompositions_capacity) *
                                GROWTH_FACTOR);
            if (err) {
                return err;
            }
            *current_decompositions_capacity *= GROWTH_FACTOR;
        }
        // base case recursion
        // moving decomposition from temp vector to result
        temp = int_vector_init();
        if (temp == NULL) {
            return MEMORY_ALLOCATION_ERROR;
        }
        for (i = 0; i < int_vector_size(*temp_vector); ++i) {
            int_vector_push_back(&temp, (*temp_vector)[i]);
        }
        (*result_decompositions)[(*current_decompositions_count)++] = temp;
        return EXIT_SUCCESS;
    }

    // reqursive case
    // check all possible
    for (i = remainder; i > 0; i--) {
        // if vector is empty => summand = 0 (not natural number); else =>
        // summand = last vector element
        sumand = int_vector_size(*temp_vector)
                     ? (*temp_vector)[int_vector_size(*temp_vector) - 1]
                     : 0;

        if (sumand == 0 ||
            (allowed_equal_sum_components != 0 ? i <= sumand : i < sumand)) {
            // inserting new decomposition element
            int_vector_push_back(temp_vector, i);
            err = __sums_decomposition_inner(
                result_decompositions, remainder - i,
                current_decompositions_count, current_decompositions_capacity,
                temp_vector, allowed_equal_sum_components);
            if (err) {
                return err;
            }
            // pop to err since wi didnt need ret val
            int_vector_pop_back(temp_vector, &err);
            err = 0;
        }
    }
    return EXIT_SUCCESS;
}
