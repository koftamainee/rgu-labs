#include "../utils.h"

#include <stdlib.h>

int is_file_empty(FILE *fp) {
    long file_size, original_position;
    int current_position;

    original_position = ftell(fp);

    fseek(fp, 0, SEEK_END);

    file_size = ftell(fp);

    current_position = fseek(fp, original_position, SEEK_SET);
    if (file_size == 0) {
        return 1;
    }

    return 0;
}

err_t swap(void *first, void *second, size_t len) {
    if (first == NULL || second == NULL) {
        return DEREFERENCING_NULL_PTR;
    }
    int i;
    for (i = 0; i < len; ++i) {
        *((unsigned char *)first + i) ^= *((unsigned char *)second + i);
        *((unsigned char *)second + i) ^= *((unsigned char *)first + i);
        *((unsigned char *)first + i) ^= *((unsigned char *)second + i);
    }

    return EXIT_SUCCESS;
}

err_t binary_search_inner(void const *values, void const *value_to_find,
                          size_t value_size, size_t left_bound_inclusive,
                          size_t right_bound_exclusive,
                          int (*comparer)(void const *, void const *),
                          void **result_placement) {
    size_t mediant_index;
    int comparison_result;

    if (left_bound_inclusive >= right_bound_exclusive) {
        return NO_SUCH_ENTRY_IN_COLLECTION;
    }

    mediant_index = (left_bound_inclusive + right_bound_exclusive) >> 1;
    comparison_result = comparer(
        value_to_find, (unsigned char *)values + (mediant_index * value_size));

    if (comparison_result == 0) {
        *result_placement = (void *)values + mediant_index;
        return EXIT_SUCCESS;
    } else if (comparison_result < 0) {
        right_bound_exclusive = mediant_index;
    } else {
        left_bound_inclusive = mediant_index + 1;
    }

    return binary_search_inner(values, value_to_find, value_size,
                               left_bound_inclusive, right_bound_exclusive,
                               comparer, result_placement);
}

err_t binary_search(void const *values, void const *value_to_find, size_t count,
                    size_t value_size,
                    int (*comparer)(void const *, void const *),
                    void **result_placement) {
    if (values == NULL || value_to_find == NULL || comparer == NULL ||
        result_placement == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    return binary_search_inner(values, value_to_find, value_size, 0, count,
                               comparer, result_placement);
}

err_t remove_comments_from_line(char **line, int *in_comment,
                                int comment_enter_symbol,
                                int comment_exit_symbol) {
    if (line == NULL || *line == NULL || in_comment == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    char *read_ptr = *line;
    char *write_ptr = *line;

    while (*read_ptr) {
        if (*read_ptr == comment_enter_symbol && !*in_comment) {
            *in_comment = 1;  // Entering comment
        }
        if (*read_ptr == comment_exit_symbol && *in_comment) {
            *in_comment = 0;  // Exiting comment
            read_ptr++;       // Skip closing bracket
            continue;
        }

        if (!*in_comment) {
            *write_ptr = *read_ptr;  // Copy non-comment characters
            write_ptr++;
        }
        read_ptr++;
    }

    *write_ptr = '\0';  // Null-terminate the string
    return EXIT_SUCCESS;
}

err_t remove_spaces_from_line(char *line) {
    if (line == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    char *write_ptr = line;

    while (*line) {
        if (*line != ' ') {
            *write_ptr = *line;
            write_ptr++;
        }
        line++;
    }

    *write_ptr = '\0';

    return EXIT_SUCCESS;
}
