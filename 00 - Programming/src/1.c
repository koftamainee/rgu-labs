#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "../../libs/arrays.h"

#define ARRAY_SIZE 20


int bubble_sort(int* array, int size);


int main() {
    int* arr = (int*)malloc(sizeof(int) * ARRAY_SIZE);

    if (arr == NULL) {
        printf("Memory alocation error. Exit code 1.");
        return 1;
    }

    randomize_array(arr, ARRAY_SIZE);
    printf("Normal: ");
    print_array(arr, ARRAY_SIZE);
    bubble_sort(arr, ARRAY_SIZE);
    printf("Sorted: ");
    print_array(arr, ARRAY_SIZE);
    free(arr);
    return 0;
}


int bubble_sort(int* array, int size) {
    int buf;
    for (int i = 0; i < size - 1; ++i) {
        for (int j = i + 1; j < size; j++) {
            if (array[i] > array[j]) {
                buf = array[i];
                array[i] = array[j];
                array[j] = buf;
            }
        }
    }
    return 0;
}
