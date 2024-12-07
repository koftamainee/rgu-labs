#include <stdio.h>

#include "../../../libc/errors.h"
#include "../../../libc/int_vector.h"
#include "../../../libc/memory.h"

#define INITIAL_CAPACITY (16)
#define GROWTH_FACTOR (2)

int towers_construction(int blocks_count, int_vector **result_towers,
                        size_t *result_towers_count,
                        int allowed_partial_blocks_usage,
                        int allowed_adjacent_layers_blocks_equal_count);

int __tower_construction_inner(int_vector **result_towers, int remainder,
                               size_t *current_towers_count,
                               size_t *current_towers_capacity,
                               int_vector *temp_vector,
                               int allowed_adjacent_layers_block_equal_count);

int program_03_19(int argc, char *argv[]) {
    int_vector *res = NULL;
    size_t len = 0, i = 0, j = 0;
    int err = 0;

    err = towers_construction(5, &res, &len, 1, 1);
    for (i = 0; i < len; ++i) {
        for (j = 0; j < int_vector_size(res[i]); ++j) {
            printf("%d ", res[i][j]);
        }

        int_vector_free(res[i]);
        printf("\n");
    }
    free(res);
    return err;
    return 0;
}

int towers_construction(int blocks_count, int_vector **result_towers,
                        size_t *result_towers_count,
                        int allowed_partial_blocks_usage,
                        int allowed_adjacent_layers_blocks_equal_count) {
    int err, i, j;
    int_vector temp = NULL;
    size_t capacity = INITIAL_CAPACITY, loop_iteration_upto = 0;

    if (result_towers == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    if (blocks_count < 0) {
        return INVALID_NUMBER;
    }

    *result_towers = (int_vector *)malloc(sizeof(int_vector) * capacity);
    if ((*result_towers) == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }
    loop_iteration_upto = allowed_partial_blocks_usage ? 0 : blocks_count - 1;
    for (i = blocks_count; i > loop_iteration_upto; --i) {
        temp = int_vector_init();
        if (temp == NULL) {
            return MEMORY_ALLOCATION_ERROR;
        }
        err = __tower_construction_inner(
            result_towers, i, result_towers_count, &capacity, &temp,
            allowed_adjacent_layers_blocks_equal_count);
        int_vector_free(temp);
        if (err) {
            return err;
        }
    }
    return EXIT_SUCCESS;
}

int __tower_construction_inner(int_vector **result_towers, int remainder,
                               size_t *current_towers_count,
                               size_t *current_towers_capacity,
                               int_vector *temp_vector,
                               int allowed_adjacent_layers_block_equal_count) {
    int err, i, sumand;
    int_vector temp;
    if (result_towers == NULL || current_towers_count == NULL ||
        temp_vector == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    if (remainder == 0) {
        // doing some growth
        if (*current_towers_capacity < *current_towers_count) {
            err = rerealloc((void **)result_towers,
                            sizeof(int_vector) * (*current_towers_capacity) *
                                GROWTH_FACTOR);
            if (err) {
                return err;
            }
            *current_towers_capacity *= GROWTH_FACTOR;
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
        (*result_towers)[(*current_towers_count)++] = temp;
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
            (allowed_adjacent_layers_block_equal_count != 0 ? i <= sumand
                                                            : i < sumand)) {
            // inserting new decomposition element
            int_vector_push_back(temp_vector, i);
            err = __tower_construction_inner(
                result_towers, remainder - i, current_towers_count,
                current_towers_capacity, temp_vector,
                allowed_adjacent_layers_block_equal_count);
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
