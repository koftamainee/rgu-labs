#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../../../libc/arrays.h"

#define ARRAY_SIZE 20

int gen_new_array_8(int *new_array, int *old_array, int old_array_size);

int program_00_8() {
    int *old_array;
    int *new_array;
    int new_size;

    old_array = (int *)malloc(sizeof(int) * ARRAY_SIZE);
    if (old_array == NULL) {
        printf("Memory alocation error. Exit code 1.");
        return 1;
    }

    randomize_array(old_array, ARRAY_SIZE);

    new_array = (int *)malloc(sizeof(int) * ARRAY_SIZE);
    if (new_array == NULL) {
        printf("Memory alocation error. Exit code 1.");
        return 1;
    }

    new_size = gen_new_array_8(new_array, old_array, ARRAY_SIZE);
    new_array = realloc(new_array, sizeof(int) * new_size);
    if (new_array == NULL) {
        printf("Memory alocation error. Exit code 1.");
        return 1;
    }

    printf("Old array: ");
    print_array(old_array, ARRAY_SIZE);
    printf("New array: ");
    print_array(new_array, new_size);

    free(old_array);
    free(new_array);
    return 0;
}

int gen_new_array_8(int *new_array, int *old_array, int old_array_size) {
    int cnt = 0;
    int i;
    for (i = 0; i < old_array_size; ++i) {
        if (old_array[i] % 2 == 0) {
            new_array[cnt] = old_array[i];
            ++cnt;
        }
    }

    return cnt;
}
