#include <stdio.h>
#include <stdlib.h>

#include "../../../libc/binary_search_tree.h"

int int_comparator(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}

int program_04_8(int argc, char *argv[]) {
    bst *t = NULL;

    bst_init(&t, sizeof(int), sizeof(int), free, free, int_comparator,
             swap_with_max_of_left_subtree, in_order, overwrite);
    for (int i = 0; i < 10; ++i) {
        int key = i;
        int value = rand();
        printf("%d\n", key);
        bst_insert(t, &key, &value);
        key = 10 - i;
        printf("%d\n", key);
        bst_insert(t, &key, &value);
    }
    int key = 2;
    bst_dispose(t, &key);

    return EXIT_SUCCESS;
}
