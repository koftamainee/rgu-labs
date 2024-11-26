#include <stdio.h>

typedef int tvalue;

int find_the_longest_sawtooth_subsequence(
    tvalue const *sequence, size_t *subsequence_start_index_storage,
    size_t *subsequence_length_storage,
    int (*comparer)(tvalue const *, tvalue const *),
    int is_comparison_is_strict);

int program_03_14(int argc, char *argv[]) { return 0; }