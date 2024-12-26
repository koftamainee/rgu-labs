#ifndef WORD_COUNT_BST_H_
#define WORD_COUNT_BST_H_

#include "../../libc/binary_search_tree.h"
#include "../../libc/cstring.h"
#include "../../libc/u_list.h"

typedef struct {
    String word;
    size_t count;
} word_count_bst_item;

typedef struct {
    bst *with_word_comparer;
    bst *with_count_comparer;
} word_count_bst;

int by_word_comparer(const void *a, const void *b);
int by_count_comparer(const void *a, const void *b);

err_t word_count_bst_init(word_count_bst **t);

err_t word_count_bst_insert_new_word_or_increment_word_count_if_it_exists(
    word_count_bst *t, const String to_insert);

err_t word_count_bst_check_word_count(word_count_bst *t, const String needle,
                                      size_t *count);

err_t word_count_bst_find_n_frequent_words(word_count_bst *t, size_t n,
                                           u_list *words_nodes);

void word_count_bst_free(word_count_bst *t);

#endif
