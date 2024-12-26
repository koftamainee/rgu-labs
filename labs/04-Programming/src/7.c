#include <stdlib.h>

#include "../../../libc/binary_search_tree.h"
#include "../../../libc/cstring.h"
#include "../../../libc/int_vector.h"
#include "../../../libc/utils.h"
#include "../word_count_bst.h"

err_t read_lexems_from_file(const char *filename, word_count_bst *tree,
                            const int_vector separators);

int is_symbol_good_for_string(const char c, const int_vector separators);
int int_comparer(const void *a, const void *b);

void print_word(const u_list_node *word_node) {
    word_count_bst_item *item = (*(word_count_bst_item **)word_node->data);
    string_print(item->word);
    printf(": %zu\n", item->count);
}

int program_04_7(int argc, char *argv[]) {
    err_t err = 0;
    word_count_bst *t;
    u_list *words = NULL;
    int_vector separators = int_vector_init();
    if (separators == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }
    int_vector_push_back(&separators, ',');

    err = u_list_init(&words, sizeof(String), free);
    if (err) {
        return err;
    }

    err = word_count_bst_init(&t);
    if (err) {
        return err;
    }

    err = read_lexems_from_file("files/word_count.txt", t, separators);
    if (err) {
        return err;
    }
    word_count_bst_find_n_frequent_words(t, 10, words);
    u_list_const_traversion(words, print_word);

    word_count_bst_free(t);
    u_list_free(words);

    return EXIT_SUCCESS;
}

err_t read_lexems_from_file(const char *filename, word_count_bst *tree,
                            const int_vector separators) {
    if (filename == NULL || tree == NULL || separators == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    FILE *fin = NULL;
    String temp_str = NULL;
    bst_node *ret_bst_node = NULL;

    char c;
    err_t err;
    int is_valid;

    fin = fopen(filename, "r");
    if (fin == NULL) {
        return OPENING_THE_FILE_ERROR;
    }

    qsort(separators, int_vector_size(separators), sizeof(int),
          int_comparer);  // for bsearch

    temp_str = string_init();
    if (temp_str == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }

    while ((c = fgetc(fin)) != EOF) {
        err = is_symbol_good_for_string(c, separators);
        if (err != EXIT_SUCCESS && err != NO_SUCH_ENTRY_IN_COLLECTION) {
            string_free(temp_str);
            return err;
        }
        is_valid = (err == EXIT_SUCCESS) ? 0 : 1;

        if (is_valid) {
            err = string_add(&temp_str, c);
            if (err) {
                string_free(temp_str);
                return err;
            }
        } else {
            err =
                word_count_bst_insert_new_word_or_increment_word_count_if_it_exists(
                    tree, temp_str);
            if (err) {
                string_free(temp_str);
                return err;
            }
            string_free(temp_str);
            temp_str = string_init();
            if (temp_str == NULL) {
                return MEMORY_ALLOCATION_ERROR;
            }
        }
    }

    if (string_len(temp_str) > 0) {
        err =
            word_count_bst_insert_new_word_or_increment_word_count_if_it_exists(
                tree, temp_str);
        if (err) {
            string_free(temp_str);
            return err;
        }
    }
    string_free(temp_str);

    return EXIT_SUCCESS;
}

int int_comparer(const void *a, const void *b) { return *(int *)a - *(int *)b; }

int is_symbol_good_for_string(const char c, const int_vector separators) {
    int *res = NULL;
    size_t i;
    // return binary_search(separators, &c, int_vector_size(separators),
    //                      sizeof(int), int_comparer, (void **)&res);
    //
    for (i = 0; i < int_vector_size(separators); ++i) {
        if (separators[i] == c) {
            return EXIT_SUCCESS;
        }
    }
    return NO_SUCH_ENTRY_IN_COLLECTION;
}
