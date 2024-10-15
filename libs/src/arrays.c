#include "../arrays.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>


int randomize_array(int *array, int size) {
    int i;
    srand(time(NULL));
        for (i = 0; i < size; ++i) {
        array[i] = rand() % 500 - 200;
    }
    return 0;
}


int print_array(int* array, int size) {
    int i;
    for (i = 0; i < size; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n");
    return 0;
}


int find_max_and_min(int* array, int size, int* ans) {
    int max = -10000;
    int min = 10000;
    int max_index = -1;
    int min_index = -1;
    for (int i = 0; i < size; ++i) {
        if (array[i] > max) {
            max = array[i];
            max_index = i;
        }
        if (array[i] < min) {
            min = array[i];
            min_index = i;
        }
    }

    ans[0] = max;
    ans[1] = max_index;
    ans[2] = min;
    ans[3] = min_index;
    return 0;  
}