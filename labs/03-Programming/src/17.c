#include <stdio.h>

int subsets(int *items, size_t items_count, int **result_subsets,
            size_t *result_subsets_count,
            int (*equality_comparer)(int const *, int const *));

int program_03_17(int argc, char *argv[]) { return 0; }