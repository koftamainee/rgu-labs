#include <stdio.h>
#include <stdlib.h>

#include "../../../libc/errors.h"

#define SEQUENCE_LENGTH (18)

typedef int tvalue;

int find_the_longest_sawtooth_subsequence(
    tvalue const *sequence, size_t sequence_length,
    size_t *subsequence_start_index_storage, size_t *subsequence_length_storage,
    int (*comparer)(tvalue const *, tvalue const *),
    int is_comparison_is_strict);

int comparer_for_subtooth_sequence(tvalue const *first, tvalue const *second);

int program_03_14(int argc, char *argv[]) {
    int err, i;
    size_t len, start_index;
    tvalue seq[SEQUENCE_LENGTH] = {1, 3, 2, 15, 5, 52, 8, 8, 8,
                                   1, 3, 2, 15, 5, 52, 3, 4, 2};

    err = find_the_longest_sawtooth_subsequence(
        seq, SEQUENCE_LENGTH, &start_index, &len,
        comparer_for_subtooth_sequence, 1);
    if (err) {
        return err;
    }
    printf("Finded longest sawtooth: ");
    for (i = start_index; i < start_index + len; ++i) {
        printf("%d ", seq[i]);
    }
    printf("\n");

    return 0;
}

int find_the_longest_sawtooth_subsequence(
    tvalue const *sequence, size_t sequence_length,
    size_t *subsequence_start_index_storage, size_t *subsequence_length_storage,
    int (*comparer)(tvalue const *, tvalue const *),
    int is_comparison_is_strict) {
    int is_current_element_sawtooth = 0, left = 0, right = 0,
        is_sawtooth_element_condition = 0;
    size_t i, current_sabtooth_length = 0, current_sabtooth_start_index = -1;

    if (sequence == NULL || subsequence_start_index_storage == NULL ||
        subsequence_length_storage == NULL || comparer == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    *subsequence_length_storage = 0;
    *subsequence_start_index_storage = -1;

    for (i = 0; i < sequence_length; ++i) {
        if (is_current_element_sawtooth == 0) {
            current_sabtooth_start_index = i;
        }

        if (i == 0) {
            right = comparer(sequence + i, sequence + i + 1);
            left = right;
        } else if (i == sequence_length - 1) {
            left = comparer(sequence + i, sequence + i - 1);
            right = left;
        } else {
            left = comparer(sequence + i, sequence + i - 1);
            right = comparer(sequence + i, sequence + i + 1);
        }

        if (is_comparison_is_strict) {
            is_sawtooth_element_condition =
                ((right < 0 && left < 0) || (right > 0 && left > 0));
        } else {
            is_sawtooth_element_condition =
                ((right <= 0 && left <= 0) || (right >= 0 && left >= 0));
        }

        if (is_sawtooth_element_condition) {
            is_current_element_sawtooth = 1;
            current_sabtooth_length++;
        } else {
            is_current_element_sawtooth = 0;
            if (current_sabtooth_length > *subsequence_length_storage) {
                *subsequence_length_storage = current_sabtooth_length;
                *subsequence_start_index_storage = current_sabtooth_start_index;
                current_sabtooth_length = 0;
            }
        }
    }
    if (current_sabtooth_length > *subsequence_length_storage) {
        *subsequence_length_storage = current_sabtooth_length;
        *subsequence_start_index_storage = current_sabtooth_start_index;
    }

    return EXIT_SUCCESS;
}

int comparer_for_subtooth_sequence(tvalue const *first, tvalue const *second) {
    return *first - *second;
}
