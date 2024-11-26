#include <stdio.h>

typedef int tvalue;

int find_saddle_points(tvalue const *const *matrix, size_t matrix_rows_count,
                       size_t matrix_columns_count,
                       size_t ***found_saddle_points_storage,
                       size_t *found_saddle_points_count_storage,
                       int (*comparer)(tvalue const *, tvalue const *),
                       int is_comparison_is_strict);

int program_03_15(int argc, char *argv[]) { return 0; }