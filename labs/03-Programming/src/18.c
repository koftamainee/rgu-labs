#include <stdio.h>

int combinations(int *items, size_t items_count, int **result_combinations,
                 size_t *result_combinations_count,
                 int (*equality_comparer)(int const *, int const *), size_t k);

int program_03_18(int argc, char *argv[]) { return 0; }