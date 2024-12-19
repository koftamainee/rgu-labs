#include <stdlib.h>

#include "../../../libc/binary_search_tree.h"
#include "../../../libc/cstring.h"
#include "../../../libc/u_list.h"

typedef enum { Male, Female } Gender;

typedef struct Citizen {
    String last_name;
    String name;
    String lastname;
    Gender gender;
    double salary;
} Citizen;

int keys_comparer(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}

void print_node(const bst_node *t) {
    printf("key: %d, value: %d\n", *(int *)t->key, *(int *)t->value);
}

int program_04_5(int argc, char *argv[]) {
    int i;
    bst *t;
    err_t err;
    int key;
    int value;
    err = bst_init(&t, sizeof(int), sizeof(int), free, free, keys_comparer,
                   swap_with_max_of_left_subtree, in_order, overwrite);
    if (err) {
        return err;
    }

    for (i = 0; i < 52; ++i) {
        value = 1000 - i;
        err = bst_insert(t, &i, &value);
        if (err) {
            bst_free(t);
            return err;
        }
    }
    value = 1488;
    key = 51;
    bst_insert(t, &key, &value);
    bst_set_traversion_strategy(t, post_order);
    bst_const_traversion(t, print_node);
    bst_dispose(t, &key);
    bst_const_traversion(t, print_node);

    for (i = 0; i < 25; ++i) {
        err = bst_dispose(t, &i);
        if (err) {
            bst_free(t);
            return err;
        }
    }
    printf("\n\n\n");
    bst_const_traversion(t, print_node);

    bst_free(t);

    return EXIT_SUCCESS;
}
