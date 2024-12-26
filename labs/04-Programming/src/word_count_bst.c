#include "../word_count_bst.h"

#include <stdlib.h>

#include "../../../libc/errors.h"

void print_bnode(const bst_node *t) {
    string_print(((*(word_count_bst_item **)t->key))->word);
    printf(": %zu\n", ((*(word_count_bst_item **)t->key))->count);
}

int by_word_comparer(const void *a, const void *b) {
    return string_cmp((*((word_count_bst_item **)a))->word,
                      ((*(word_count_bst_item **)b))->word);
}

int by_count_comparer(const void *a, const void *b) {
    int ret = ((*(word_count_bst_item **)a))->count -
              ((*(word_count_bst_item **)b))->count;
    if (ret) {
        return ret;  // if different count
    }
    return by_word_comparer(a, b);  // if same count
}

void free_word_count_bst_item(void *item) {
    string_free((*((word_count_bst_item **)item))
                    ->word);  // destruct string in the object
    free(item);               // destruct copy of the pointer to object
}

err_t word_count_bst_init(word_count_bst **t) {
    // First tree using free_word_count_bst_item for free, and second just free.
    // bc objects in struct are the same, we wont double free
    if (t == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    err_t err = 0;

    *t = (word_count_bst *)malloc(sizeof(word_count_bst));
    if (*t == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }

    err = bst_init(&(*t)->with_word_comparer, sizeof(word_count_bst_item *), 0,
                   free, NULL, by_word_comparer, swap_with_max_of_left_subtree,
                   in_order, overwrite);
    if (err) {
        return err;
    }
    err = bst_init(&(*t)->with_count_comparer, sizeof(word_count_bst_item *), 0,
                   free_word_count_bst_item, NULL, by_count_comparer,
                   swap_with_max_of_left_subtree, in_order, overwrite);
    if (err) {
        bst_free((*t)->with_word_comparer);
        return err;
    }

    return EXIT_SUCCESS;
}

err_t word_count_bst_insert_new_word_or_increment_word_count_if_it_exists(
    word_count_bst *t, const String to_insert) {
    if (t == NULL || to_insert == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    err_t err = 0;
    word_count_bst_item *search = NULL;
    bst_node *ret = NULL;

    search = (word_count_bst_item *)malloc(sizeof(word_count_bst_item));
    if (search == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }
    search->word = to_insert;

    err = bst_obtain(t->with_word_comparer, &search, &ret);
    if (err != EXIT_SUCCESS && err != KEY_NOT_FOUND) {
        free(search);
        return err;
    }

    if (err ==
        EXIT_SUCCESS) {  // ret is needed entry, disposing previous entrance
        search->count = (*((word_count_bst_item **)ret->key))->count;
        err = bst_dispose(t->with_word_comparer, &search);
        if (err != EXIT_SUCCESS && err != KEY_NOT_FOUND) {
            free(search);
            return err;
        }

        err = bst_dispose(t->with_count_comparer, &search);
        if (err != EXIT_SUCCESS && err != KEY_NOT_FOUND) {
            free(search);
            return err;
        }

        search->count++;
    } else {  // creating new (err == NO_SUCH_ENTRY_IN_COLLECTION)
        search->count = 1;
    }
    search->word = string_init();
    if (search->word == NULL) {
        free(search);
        return MEMORY_ALLOCATION_ERROR;
    }
    err = string_cpy(&search->word, &to_insert);
    if (err) {
        return err;
    }
    err = bst_insert(t->with_word_comparer, &search, NULL);
    if (err) {
        free(search);
        return err;
    }
    err = bst_insert(t->with_count_comparer, &search, NULL);
    if (err) {
        free(search);
        return err;
    }

    return EXIT_SUCCESS;
}

err_t word_count_bst_check_word_count(word_count_bst *t, const String needle,
                                      size_t *count) {
    if (t == NULL || needle == NULL || count == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    err_t err = 0;
    word_count_bst_item *search = NULL;
    bst_node *ret = NULL;

    search = (word_count_bst_item *)malloc(sizeof(word_count_bst_item));
    if (search == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }

    search->word = needle;

    err = bst_obtain(t->with_word_comparer, &search, &ret);
    if (err) {
        free(search);
        return err;
    }

    *count = (*((word_count_bst_item **)ret->key))->count;

    free(search);
    return EXIT_SUCCESS;
}

err_t __find_n_frequent_words_inner(bst_node *t, size_t n,
                                    size_t *current_count,
                                    u_list *words_nodes) {
    err_t err;

    if (t == NULL || *current_count >= n) {
        return EXIT_SUCCESS;
    }

    // Traverse the right subtree first (reverse in-order traversal)
    err = __find_n_frequent_words_inner(t->right_subtree, n, current_count,
                                        words_nodes);
    if (err) {
        return err;
    }

    if (*current_count < n) {
        err = u_list_insert(words_nodes, 0, t->key);  // TODO: redo
        if (err) {
            return err;
        }
        (*current_count)++;
    }

    if (*current_count < n) {
        err = __find_n_frequent_words_inner(t->left_subtree, n, current_count,
                                            words_nodes);
        if (err) {
            return err;
        }
    }

    return EXIT_SUCCESS;
}

err_t word_count_bst_find_n_frequent_words(word_count_bst *t, size_t n,
                                           u_list *words_nodes) {
    if (t == NULL || words_nodes == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    // i will do reverse in order traversion from binary search tree to find max
    // elems

    bst_node *current = NULL;
    size_t current_count = 0;

    current = t->with_count_comparer->root;

    return __find_n_frequent_words_inner(current, n, &current_count,
                                         words_nodes);
}

void word_count_bst_free(word_count_bst *t) {
    if (t == NULL) {
        return;
    }
    bst_free(t->with_word_comparer);
    bst_free(t->with_count_comparer);
    return;
}
