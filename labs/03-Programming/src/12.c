/* PLS REDO THIS TO CSTRING */

#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "../../../libc/errors.h"
#include "../../../libc/memory.h"

int substr(char *to_find, int case_sensitive, char ***results, int ***positions,
           size_t *results_count, ...);

int program_03_12() {
    int err, i, j;
    int **pos;
    size_t results_count;
    char *inital = "el";
    char *str1 = "Helloworld";
    char *str2 = "HELLOHEllohelLO";
    char **results = NULL;

    err = substr(inital, 0, &results, &pos, &results_count, str1, str2, NULL);
    if (err) {
        return err;
    }
    for (i = 0; i < results_count - 1; ++i) {
        printf("String: %s", results[i]);
        printf("\n");
        for (j = 1; j < pos[i][0] + 1; ++j) {
            printf("%d ", pos[i][j]);
        }
        printf("\n");
    }

    return EXIT_SUCCESS;
}

int substr(char *to_find, int case_sensitive, char ***results, int ***positions,
           size_t *results_count, ...) {
    va_list ap;
    int i, err;
    char *str_ptr;
    char *find_ptr;
    int k = 0, pos = -1;
    int size = 10;
    int **temp_int_ptr_ptr;
    char *temp_str_ptr;
    int current_ans_index = 0, current_ans_size = 0;
    int current_position_str_index = 1, current_position_str_size = 0;

    if (to_find == NULL || results == NULL ||
        positions == NULL && results_count == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    (*results) = (char **)malloc(size * sizeof(char *));
    if ((*results) == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }

    (*positions) = (int **)malloc(size * sizeof(int *));
    if ((*positions) == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }
    current_ans_size = size;

    for (i = 0; i < current_ans_size; i++) {
        (*positions)[i] = (int *)malloc(size * sizeof(int));
        if ((*positions)[i] == NULL) {
            return MEMORY_ALLOCATION_ERROR;
        }
        (*positions)[i][0] = 0;
    }

    va_start(ap, results_count);
    find_ptr = to_find;

    while ((str_ptr = va_arg(ap, char *)) != NULL) {
        temp_str_ptr = str_ptr;
        k = 0;
        current_position_str_index = 1;
        current_position_str_size = size;
        if (current_ans_index >= current_ans_size) {
            err = rerealloc((void **)positions,
                            current_ans_size * 2 * sizeof(int *));
            if (err) {
                return MEMORY_ALLOCATION_ERROR;
            }
            (*positions) = temp_int_ptr_ptr;

            err = rerealloc((void **)results,
                            current_ans_size * 2 * sizeof(char *));
            if (err) {
                return MEMORY_ALLOCATION_ERROR;
            }

            for (i = current_ans_size; i < current_ans_size * 2; i++) {
                (*positions)[i] = (int *)malloc(size * sizeof(int));
                if ((*positions)[i] == NULL) {
                    return MEMORY_ALLOCATION_ERROR;
                }
                (*positions)[i][0] = 0;
            }
            current_ans_size *= 2;
        }

        while (*str_ptr != '\0') {
            if (case_sensitive == 0 &&
                toupper(*str_ptr) == toupper(*find_ptr)) {
                if (pos == -1) {
                    pos = k;
                }
                find_ptr++;
            } else if (*str_ptr == *find_ptr) {
                if (pos == -1) {
                    pos = k;
                }
                find_ptr++;
            } else if (pos != -1 && strlen(find_ptr) == 0) {
                if (current_position_str_index >= current_position_str_size) {
                    err =
                        rerealloc((void **)(*positions)[current_ans_index],
                                  current_position_str_size * 2 * sizeof(int));
                    if (err) {
                        return MEMORY_ALLOCATION_ERROR;
                        free(*positions[current_ans_index]);
                    }
                    current_position_str_size *= 2;
                }

                (*positions)[current_ans_index][current_position_str_index] =
                    pos;
                (*positions)[current_ans_index][0]++;
                int cc = (*positions)[current_ans_index][0];
                current_position_str_index++;
                pos = -1;
                find_ptr = to_find;
            } else {
                pos = -1;
                find_ptr = to_find;
            }
            str_ptr++;
            k++;
        }

        if ((*positions)[current_ans_index][0] != 0) {
            (*results)[current_ans_index] = temp_str_ptr;
            current_ans_index++;
            (*results_count)++;
        }
    }

    va_end(ap);
    return 0;
}
