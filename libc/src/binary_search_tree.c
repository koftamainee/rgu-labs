#include "../binary_search_tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils.h"

/* local functions */
void __bst_free_inner(bst *t, bst_node *n);
void __bst_const_traversion_inner(const bst *t, bst_node *n,
                                  void (*callback)(const bst_node *));
void __bst_traversion_inner(bst *t, bst_node *n, void (*callback)(bst_node *));

err_t bst_init(bst **t, size_t key_size, size_t value_size,
               void (*key_destructor)(void *), void (*value_destructor)(void *),
               int (*keys_comparer)(const void *, const void *),
               bst_disposal_strategy disposal_strategy,
               bst_traversion_strategy traversion_strategy,
               bst_collision_strategy collision_strategy) {
    if (t == NULL ||
        keys_comparer ==
            NULL) {  // keys_destructor and value destructor may be NULL
        return DEREFERENCING_NULL_PTR;
    }
    // if (key_size == 0 || value_size == 0) {
    //     return ZERO_MEMORY_ALLOCATION;
    // }

    *t = (bst *)malloc(sizeof(bst));
    if (*t == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }

    (*t)->root = NULL;
    (*t)->value_destructor = value_destructor;
    (*t)->key_destructor = key_destructor;
    (*t)->traversion_strategy = traversion_strategy;
    (*t)->disposal_strategy = disposal_strategy;
    (*t)->collision_strategy = collision_strategy;
    (*t)->key_size = key_size;
    (*t)->value_size = value_size;
    (*t)->keys_comparer = keys_comparer;

    return EXIT_SUCCESS;
}

void bst_free(bst *t) {
    if (t == NULL) {
        return;
    }
    __bst_free_inner(t, t->root);
    free(t);
    return;
}

void __bst_free_inner(bst *t, bst_node *n) {
    if (n == NULL) {
        return;
    }
    __bst_free_inner(t, n->left_subtree);
    __bst_free_inner(t, n->right_subtree);
    if (t->value_destructor != NULL) {
        t->value_destructor(n->value);
    }

    if (t->key_destructor != NULL) {
        t->key_destructor(n->key);
    }
    free(n);
    return;
}

err_t bst_set_disposal_strategy(bst *t,
                                bst_disposal_strategy disposal_strategy) {
    if (t == NULL) {
        return DEREFERENCING_NULL_PTR;
    }
    t->disposal_strategy = disposal_strategy;
    return EXIT_SUCCESS;
}

err_t bst_set_traversion_strategy(bst *t,
                                  bst_traversion_strategy traversion_strategy) {
    if (t == NULL) {
        return DEREFERENCING_NULL_PTR;
    }
    t->traversion_strategy = traversion_strategy;
    return EXIT_SUCCESS;
}
err_t bst_set_collision_strategy(bst *t,
                                 bst_collision_strategy collision_strategy) {
    if (t == NULL) {
        return DEREFERENCING_NULL_PTR;
    }
    t->collision_strategy = collision_strategy;

    return EXIT_SUCCESS;
}

err_t bst_insert(bst *t, void *key, void *value) {
    int comparer_result;
    bst_node **iterator = NULL, *left_tree, *right_tree;
    if (t == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    iterator = &t->root;
    while (*iterator != NULL) {
        comparer_result = t->keys_comparer(key, (*iterator)->key);
        if (comparer_result == 0) {
            if (t->collision_strategy ==
                decline) {  // strategy to decline attempt to overwrite the key
                return REPEATING_KEY;
            } else {  // strategy == overwrite
                if (t->value_destructor != NULL) {
                    t->value_destructor((*iterator)->value);
                }
                if (t->key_destructor != NULL) {
                    t->key_destructor((*iterator)->key);
                }
                left_tree = (*iterator)->left_subtree;
                right_tree = (*iterator)->right_subtree;
                free(*iterator);

                *iterator = (bst_node *)malloc(sizeof(bst_node));
                if (*iterator == NULL) {
                    return MEMORY_ALLOCATION_ERROR;
                }
                (*iterator)->left_subtree = left_tree;
                (*iterator)->right_subtree = right_tree;
                if (t->value_size != 0) {
                    (*iterator)->value = malloc(t->value_size);
                    if ((*iterator)->value == NULL) {
                        free(*iterator);
                        *iterator = NULL;
                        return MEMORY_ALLOCATION_ERROR;
                    }
                    memcpy((*iterator)->value, value, t->value_size);
                }

                if (t->key_size != 0) {
                    (*iterator)->key = malloc(t->key_size);
                    if ((*iterator)->key == NULL) {
                        t->value_destructor((*iterator)->value);
                        (*iterator)->value = NULL;
                        free(*iterator);
                        *iterator = NULL;
                        return MEMORY_ALLOCATION_ERROR;
                    }
                    memcpy((*iterator)->key, key, t->key_size);
                }

                return EXIT_SUCCESS;
            }
        }

        iterator = comparer_result > 0 ? &(*iterator)->right_subtree
                                       : &(*iterator)->left_subtree;
    }

    // memory allocation time !!!!
    *iterator = (bst_node *)malloc(sizeof(bst_node));
    if (*iterator == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }
    (*iterator)->left_subtree = NULL;
    (*iterator)->right_subtree = NULL;
    if (t->value_size != 0) {
        (*iterator)->value = malloc(t->value_size);
        if ((*iterator)->value == NULL) {
            free(*iterator);
            *iterator = NULL;
            return MEMORY_ALLOCATION_ERROR;
        }
        memcpy((*iterator)->value, value, t->value_size);
    }

    if (t->key_size != 0) {
        (*iterator)->key = malloc(t->key_size);
        if ((*iterator)->key == NULL) {
            t->value_destructor((*iterator)->value);
            (*iterator)->value = NULL;
            free(*iterator);
            *iterator = NULL;
            return MEMORY_ALLOCATION_ERROR;
        }
        memcpy((*iterator)->key, key, t->key_size);
    }

    return EXIT_SUCCESS;
}

err_t bst_obtain(const bst *t, void *key, bst_node **target) {
    int comparer_result = 0;
    bst_node *iterator = NULL;  // THIS IS RIGHT
    if (t == NULL || key == NULL || target == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    iterator = t->root;
    while (iterator != NULL) {
        comparer_result = t->keys_comparer(key, iterator->key);
        if (comparer_result == 0) {
            *target = iterator;
            return EXIT_SUCCESS;
        }
        iterator = comparer_result > 0 ? iterator->right_subtree
                                       : iterator->left_subtree;
    }

    return KEY_NOT_FOUND;
}

err_t bst_dispose(bst *t, void *key) {
    int comparer_result = 0;
    bst_node **iterator = NULL, **for_swap, *temp_node;
    if (t == NULL || key == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    iterator = &t->root;
    while (*iterator != NULL) {
        comparer_result = t->keys_comparer(key, (*iterator)->key);
        if (comparer_result == 0) {  // element found, 3 cases

            if ((*iterator)->left_subtree != NULL &&
                (*iterator)->right_subtree != NULL) {
                for_swap = t->disposal_strategy == swap_with_max_of_left_subtree
                               ? &(*iterator)->left_subtree
                               : &(*iterator)->right_subtree;
                while ((t->disposal_strategy == swap_with_max_of_left_subtree
                            ? (*for_swap)->right_subtree
                            : (*for_swap)->left_subtree) !=
                       NULL) {  // go to left subtree and then go maximum to
                                // right (or vica versa)

                    for_swap =
                        t->disposal_strategy == swap_with_max_of_left_subtree
                            ? &(*for_swap)->right_subtree
                            : &(*for_swap)->left_subtree;
                }
                if (t->value_size > 0) {
                    swap((*iterator)->value, (*for_swap)->value,
                         sizeof(void *));
                }

                if (t->key_size > 0) {
                    swap((*iterator)->key, (*for_swap)->key, sizeof(void *));
                }

                if (t->value_destructor != NULL) {
                    t->value_destructor((*for_swap)->value);
                }
                if (t->key_destructor != NULL) {
                    t->key_destructor((*for_swap)->key);
                }
                free(*for_swap);
                *for_swap = NULL;
                return EXIT_SUCCESS;
            }
            if ((*iterator)->left_subtree == NULL &&
                (*iterator)->right_subtree == NULL) {
                if (t->value_destructor != NULL) {
                    t->value_destructor((*iterator)->value);
                }
                if (t->key_destructor != NULL) {
                    t->key_destructor((*iterator)->key);
                }
                free(*iterator);
                *iterator = NULL;
                return EXIT_SUCCESS;
            }
            // one of the subtrees is not NULL
            temp_node = (*iterator)->right_subtree == NULL
                            ? (*iterator)->left_subtree
                            : (*iterator)->right_subtree;
            if (t->value_destructor != NULL) {
                t->value_destructor((*iterator)->value);
            }
            if (t->key_destructor != NULL) {
                t->key_destructor((*iterator)->key);
            }
            free(*iterator);
            *iterator = temp_node;

            return EXIT_SUCCESS;
        }

        iterator = comparer_result > 0 ? &(*iterator)->right_subtree
                                       : &(*iterator)->left_subtree;
    }

    return KEY_NOT_FOUND;
}

err_t bst_const_traversion(const bst *t, void (*callback)(const bst_node *)) {
    if (t == NULL || callback == NULL) {
        return DEREFERENCING_NULL_PTR;
    }
    __bst_const_traversion_inner(t, t->root, callback);

    return EXIT_SUCCESS;
}

void __bst_const_traversion_inner(const bst *t, bst_node *n,
                                  void (*callback)(const bst_node *)) {
    if (n == NULL) {
        return;
    }

    switch (t->traversion_strategy) {
        case in_order:
            __bst_const_traversion_inner(t, n->left_subtree, callback);
            callback(n);
            __bst_const_traversion_inner(t, n->right_subtree, callback);
            break;
        case pre_order:
            callback(n);
            __bst_const_traversion_inner(t, n->left_subtree, callback);
            __bst_const_traversion_inner(t, n->right_subtree, callback);
            break;
        case post_order:
            __bst_const_traversion_inner(t, n->left_subtree, callback);
            __bst_const_traversion_inner(t, n->right_subtree, callback);
            callback(n);
            break;
    }

    return;
}

err_t bst_traversion(bst *t, void (*callback)(bst_node *)) {
    if (t == NULL || callback == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    __bst_traversion_inner(t, t->root, callback);

    return EXIT_SUCCESS;
}

void __bst_traversion_inner(bst *t, bst_node *n, void (*callback)(bst_node *)) {
    if (n == NULL) {
        return;
    }

    switch (t->traversion_strategy) {
        case in_order:
            __bst_traversion_inner(t, n->left_subtree, callback);
            callback(n);
            __bst_traversion_inner(t, n->right_subtree, callback);
            break;
        case pre_order:
            callback(n);
            __bst_traversion_inner(t, n->left_subtree, callback);
            __bst_traversion_inner(t, n->right_subtree, callback);
            break;
        case post_order:
            __bst_traversion_inner(t, n->left_subtree, callback);
            __bst_traversion_inner(t, n->right_subtree, callback);
            callback(n);
            break;
    }

    return;
}

void __bst_get_depth_inner(bst_node *t, size_t *depth, size_t current_depth) {
    if (t == NULL) {
        if (current_depth > *depth) {
            *depth = current_depth;
        }
        return;
    }
    __bst_get_depth_inner(t->left_subtree, depth, current_depth + 1);
    __bst_get_depth_inner(t->right_subtree, depth, current_depth + 1);
}

err_t bst_get_depth(bst *t, size_t *depth) {
    if (t == NULL || depth == NULL) {
        return DEREFERENCING_NULL_PTR;
    }
    *depth = 0;
    __bst_get_depth_inner(t->root, depth, 0);

    return EXIT_SUCCESS;
}
