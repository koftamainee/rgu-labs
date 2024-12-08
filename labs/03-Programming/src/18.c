#include <stdio.h>
#include <stdlib.h>

#include "../../../libc/custom_math.h"
#include "../../../libc/errors.h"
#include "../../../libc/int_vector.h"

int combinations(const int_vector items, int_vector *result_combinations,
                 int (*equality_comparer)(int const *, int const *), size_t k);
int __combinations_inner(const int_vector items, int_vector *result_combination,
                         int_vector *temp_vector, size_t start, size_t current,
                         size_t k);
int comparer_for_combinations(int const *a, int const *b);

int program_03_18(int argc, char *argv[]) {
    size_t i, j = 0;
    int err, k = 3;
    int_vector items = int_vector_init();
    int_vector ans = NULL;
    if (items == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }

    for (i = 0; i < 10; ++i) {
        err = int_vector_push_back(&items, i);
        if (err) {
            int_vector_free(items);
            return MEMORY_ALLOCATION_ERROR;
        }
    }

    err = combinations(items, &ans, comparer_for_combinations, k);
    if (err) {
        int_vector_free(items);
        int_vector_free(ans);
        return err;
    }

    for (i = 0; i < int_vector_cap(ans); ++i) {
        if (i % k == 0) {
            printf("\nCombination %zu: ", ++j);
        }
        printf("%d ", ans[i]);
    }
    int_vector_free(items);
    int_vector_free(ans);

    return EXIT_SUCCESS;
}

int combinations(const int_vector items, int_vector *result_combinations,
                 int (*equality_comparer)(int const *, int const *), size_t k) {
    int n;
    size_t i, j;
    int_vector temp_vec = NULL;
    if (items == NULL || result_combinations == NULL ||
        equality_comparer == NULL) {
        return DEREFERENCING_NULL_PTR;
    }
    n = int_vector_size(items);  // n is n
    if (n == 0) {
        return EXIT_SUCCESS;
    }
    if (k > n) {
        return INVALID_COMBINATION_SIZE;
    }

    for (i = 0; i < n - 1; ++i) {
        for (j = i + 1; j < n; ++j) {
            if (!equality_comparer(items + i, items + j)) {
                return EQUAL_ELEMENTS_FOUND;
            }
        }
    }

    n = (factorial(n)) /
        (factorial(n - k) * factorial(k));  // now n is result vector size

    *result_combinations =
        int_vector_init_with_cap(n * k);  // n combinations of k elements
    if (*result_combinations == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }

    temp_vec = int_vector_init_with_cap(k);
    if (temp_vec == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }

    __combinations_inner(items, result_combinations, &temp_vec, 0, 0, k);

    return EXIT_SUCCESS;
}

int __combinations_inner(const int_vector items, int_vector *result_combination,
                         int_vector *temp_vector, size_t start, size_t current,
                         size_t k) {
    size_t i, end = int_vector_size(items);
    int temp_elem;
    // base case
    if (current == k) {
        for (i = 0; i < k; ++i) {
            int_vector_push_back(result_combination, (*temp_vector)[i]);
        }
        return EXIT_SUCCESS;
    }

    // reqursive case
    for (i = start; i < end; ++i) {
        int_vector_push_back(temp_vector, items[i]);
        __combinations_inner(items, result_combination, temp_vector, i + 1,
                             current + 1, k);
        int_vector_pop_back(temp_vector, &temp_elem);
    }
    return EXIT_SUCCESS;
}

int comparer_for_combinations(int const *a, int const *b) { return *a - *b; }
