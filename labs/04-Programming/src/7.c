#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../libc/binary_search_tree.h"
#include "../../../libc/cstring.h"
#include "../../../libc/input.h"
#include "../../../libc/int_vector.h"
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

err_t start_menu_word_count(word_count_bst **t);

err_t find_shortest_word(word_count_bst *t, String *word);
err_t find_longest_word(word_count_bst *t, String *word);

err_t find_depth_of_trees(word_count_bst *t, size_t *depth_1, size_t depth_2);

int program_04_7(int argc, char *argv[]) {
    err_t err = 0;
    word_count_bst *t = NULL;
    u_list *words = NULL;
    size_t i;
    char filename[BUFSIZ];

    if (argc < 3) {
        return INVALID_FLAG;
    }

    int_vector separators = int_vector_init();

    if (separators == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }
    strcpy(filename, argv[1]);

    for (i = 2; i < argc; ++i) {
        err = int_vector_push_back(&separators, argv[i][0]);
        if (err) {
            int_vector_free(separators);
            return err;
        }
    }

    err = u_list_init(&words, sizeof(String), free);
    if (err) {
        return err;
    }

    err = word_count_bst_init(&t);
    if (err) {
        word_count_bst_free(t);
        u_list_free(words);
        return err;
    }

    err = read_lexems_from_file(filename, t, separators);
    if (err) {
        return err;
    }

    err = start_menu_word_count(&t);
    if (err) {
        word_count_bst_free(t);
        u_list_free(words);
        return err;
    }
    // word_count_bst_find_n_frequent_words(t, 100, words);
    // u_list_const_traversion(words, print_word);
    //
    word_count_bst_free(t);
    u_list_free(words);
    int_vector_free(separators);

    t = NULL;
    words = NULL;
    separators = NULL;

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

    // qsort(separators, int_vector_size(separators), sizeof(int),
    //       int_comparer);  // for bsearch

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

    fclose(fin);

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

err_t start_menu_word_count(word_count_bst **tree) {
    word_count_bst *t = *tree;
    char filename[BUFSIZ];
    int c = -1;
    err_t err;
    size_t user_ans;
    String s_ans = NULL;
    u_list *words = NULL;

    while (c != 0) {
        clear_screen();
        printf(
            "1. Count by word.\n"
            "2. n frequent words. \n"
            "3. Find longest word. \n"
            "4. Find shortest word. \n"
            "5. Find depth.\n"
            "6. Serialize trees.\n"
            "7. Deserialize tree. \n"
            "0. Exit.\n"
            "Choose: ");
        scanf("%d", &c);

        switch (c) {
            case 0:
                break;
            case 1:

                printf("Enter word: ");
                while (c != EOF && (c = getchar()) != '\n');
                err = read_string_from_user(&s_ans);
                if (err) {
                    string_free(s_ans);
                    return err;
                }

                err = word_count_bst_check_word_count(t, s_ans, &user_ans);
                if (err != EXIT_SUCCESS && err != KEY_NOT_FOUND) {
                    string_free(s_ans);
                    return err;
                }
                string_free(s_ans);
                if (err == KEY_NOT_FOUND) {
                    printf("Word not found :(\n");
                } else {
                    printf("Count: %zu\n", user_ans);
                }
                printf("\nPress enter to continue");
                while (c != EOF && (c = getchar()) != '\n');
                getchar();
                break;

            case 2:
                printf("Enter n: ");
                scanf("%zu", &user_ans);
                err = u_list_init(&words, sizeof(String), free);
                if (err) {
                    return err;
                }
                err = word_count_bst_find_n_frequent_words(t, user_ans, words);
                if (err) {
                    u_list_free(words);
                    return err;
                }
                if (words->size == 0) {
                    printf("Empty tree.\n");
                } else {
                    err = u_list_const_traversion(words, print_word);
                    if (err) {
                        u_list_free(words);
                        return err;
                    }
                }
                u_list_free(words);

                printf("\nPress enter to continue");
                while (c != EOF && (c = getchar()) != '\n');
                getchar();
                break;

            case 3:
                err = find_longest_word(t, &s_ans);
                if (err) {
                    return err;
                }
                if (string_len(s_ans) == 0) {
                    printf("Tree is empty.\n");
                } else {
                    printf("Longest word: ");
                    string_print(s_ans);
                    printf("\n");
                }
                printf("\nPress enter to continue");
                while (c != EOF && (c = getchar()) != '\n');
                getchar();
                break;

            case 4:
                err = find_shortest_word(t, &s_ans);
                if (err) {
                    return err;
                }
                if (string_len(s_ans) == 0) {
                    printf("Tree is empty.\n");
                } else {
                    printf("Shortest word: ");
                    string_print(s_ans);
                    printf("\n");
                }
                printf("\nPress enter to continue");
                while (c != EOF && (c = getchar()) != '\n');
                getchar();
                break;

            case 5:
                err = bst_get_depth(t->with_word_comparer, &user_ans);
                if (err) {
                    return err;
                }
                printf("Word tree depth: %zu\n", user_ans);
                err = bst_get_depth(t->with_count_comparer, &user_ans);
                if (err) {
                    return err;
                }
                printf("Count tree depth: %zu\n", user_ans);
                printf("\nPress enter to continue");
                while (c != EOF && (c = getchar()) != '\n');
                getchar();
                break;

            case 6:
                printf("Enter file: ");
                scanf("%s", filename);
                err = word_count_bst_serialize(t, filename);
                if (err) {
                    return err;
                }
                printf("\nPress enter to continue");
                while (c != EOF && (c = getchar()) != '\n');
                getchar();
                break;

            case 7:
                printf("Enter file: ");
                scanf("%s", filename);
                word_count_bst_free(t);
                t = NULL;
                err = word_count_bst_init(tree);
                if (err) {
                    return err;
                }
                t = *tree;
                err = word_count_bst_deserialize(t, filename);
                if (err) {
                    return err;
                }

                printf("\nPress enter to continue");
                while (c != EOF && (c = getchar()) != '\n');
                getchar();
                break;

            default:
                printf("Undefined\n");
                printf("\nPress enter to continue");
                while (c != EOF && (c = getchar()) != '\n');
                getchar();
        }
    }

    return EXIT_SUCCESS;
}

err_t find_shortest_word(word_count_bst *t, String *word) {
    if (t == NULL || word == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    bst_node *current = t->with_word_comparer->root;
    if (current == NULL) {
        return EXIT_SUCCESS;
    }

    while (current->left_subtree != NULL) {
        current = current->left_subtree;
    }

    *word = (*(word_count_bst_item **)current->key)->word;

    return EXIT_SUCCESS;
}
err_t find_longest_word(word_count_bst *t, String *word) {
    if (t == NULL || word == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    bst_node *current = t->with_word_comparer->root;
    if (current == NULL) {
        return EXIT_SUCCESS;
    }

    while (current->right_subtree != NULL) {
        current = current->right_subtree;
    }

    *word = (*(word_count_bst_item **)current->key)->word;

    return EXIT_SUCCESS;
}

err_t find_depth_of_trees(word_count_bst *t, size_t *depth_1, size_t depth_2);
