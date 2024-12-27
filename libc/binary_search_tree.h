#ifndef BINARY_SEARCH_TREE_H_
#define BINARY_SEARCH_TREE_H_

#include <stdlib.h>

#include "errors.h"

typedef int (*binary_search_tree_keys_comparer)(const void *, const void *);

typedef struct binary_search_tree_node {
    void *key;
    void *value;
    struct binary_search_tree_node *left_subtree;
    struct binary_search_tree_node *right_subtree;

} binary_search_tree_node, bst_node;

typedef enum binary_search_tree_disposal_strategy {
    swap_with_max_of_left_subtree,
    swap_with_min_of_right_subtree
} binary_search_tree_disposal_strategy,
    bst_disposal_strategy;

typedef enum binary_search_tree_traversion_strategy {
    in_order,
    pre_order,
    post_order
} binary_search_tree_traversion_strategy,
    bst_traversion_strategy;

typedef enum binary_search_tree_collision_strategy {
    overwrite,
    decline
} binary_search_tree_collision_strategy,
    bst_collision_strategy;

typedef struct binary_search_tree {
    binary_search_tree_node *root;

    size_t key_size;
    size_t value_size;

    void (*key_destructor)(void *);
    void (*value_destructor)(void *);

    int (*keys_comparer)(const void *, const void *);

    binary_search_tree_disposal_strategy disposal_strategy;
    binary_search_tree_traversion_strategy traversion_strategy;
    binary_search_tree_collision_strategy collision_strategy;

} binary_search_tree, bst;

err_t bst_init(bst **t, size_t key_size, size_t value_size,
               void (*key_destructor)(void *), void (*value_destructor)(void *),
               int (*keys_comparer)(const void *, const void *),
               bst_disposal_strategy disposal_strategy,
               bst_traversion_strategy traversion_strategy,
               bst_collision_strategy collision_strategy);

void bst_free(bst *t);

err_t bst_set_disposal_strategy(bst *t,
                                bst_disposal_strategy disposal_strategy);
err_t bst_set_traversion_strategy(bst *t,
                                  bst_traversion_strategy traversion_strategy);
err_t bst_set_collision_strategy(bst *t,
                                 bst_collision_strategy collision_strategy);

err_t bst_insert(bst *t, void *key, void *value);
err_t bst_obtain(const bst *t, void *key, bst_node **target);
err_t bst_dispose(bst *t, void *key);

err_t bst_const_traversion(const bst *t, void (*callback)(const bst_node *));
err_t bst_traversion(bst *t, void (*callback)(bst_node *));

err_t bst_get_depth(bst *t, size_t *depth);

#endif
