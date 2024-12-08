#include <stdio.h>
#include <stdlib.h>

#include "../../../libc/errors.h"
#include "../../../libc/int_vector.h"

#define SET_LEN (10)

int subsets(const int_vector items, size_t *subsets_count,
            int_vector *result_subsets,
            int (*equality_comparer)(int const *, int const *));

int comparer_for_subsets(int const *a, int const *b);

int program_03_17(int argc, char *argv[]) {
    int err;
    size_t i, j, len, subset_size, current_index;
    int_vector ans = NULL;
    int_vector items = int_vector_init_with_cap(SET_LEN);
    if (items == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }

    for (i = 0; i < SET_LEN; ++i) {
        err = int_vector_push_back(&items, rand());
        if (err) {
            int_vector_free(items);
            return MEMORY_ALLOCATION_ERROR;
        }
    }

    err = subsets(items, &len, &ans, comparer_for_subsets);
    if (err) {
        int_vector_free(items);
        int_vector_free(ans);
        return err;
    }

    printf("Generated %zu subsets:\n", len);
    current_index = 0;
    for (i = 0; i < len; ++i) {
        subset_size = ans[current_index++];
        printf("{ ");
        for (j = 0; j < subset_size; ++j) {
            printf("%d ", ans[current_index++]);
        }
        printf("}\n");
    }

    int_vector_free(items);
    int_vector_free(ans);
    return EXIT_SUCCESS;
}

int subsets(const int_vector items, size_t *subsets_count,
            int_vector *result_subsets,
            int (*equality_comparer)(int const *, int const *)) {
    size_t n, total_subsets, i, j, subset_size;
    if (items == NULL || subsets_count == NULL || result_subsets == NULL ||
        equality_comparer == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    n = int_vector_size(items);
    total_subsets = (size_t)1 << n;

    *subsets_count = total_subsets;
    *result_subsets = int_vector_init_with_cap(total_subsets * n);
    if (*result_subsets == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }

    for (i = 0; i < total_subsets; ++i) {
        subset_size = 0;
        for (j = 0; j < n; ++j) {
            if (i & (1 << j)) {
                ++subset_size;
            }
        }

        int_vector_push_back(result_subsets, subset_size);
        for (j = 0; j < n; ++j) {
            if (i & (1 << j)) {
                int_vector_push_back(result_subsets, items[j]);
            }
        }
    }

    return EXIT_SUCCESS;
}

int comparer_for_subsets(int const *a, int const *b) { return *a - *b; }
