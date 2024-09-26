#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "../../../libs/arrays.h"
#include "../../../libs/input.h"

#define ARRAY_SIZE 10


int comp(const void* a, const void* b);
int binary_search(int* array, int target);


int program_00_5() {
    int search_number, res;
    int* arr = (int*)malloc(sizeof(int) * ARRAY_SIZE);

    if (arr == NULL) {
        printf("Memory alocation error. Exit code 1.");
        return 1;
    }

    randomize_array(arr, ARRAY_SIZE);
    printf("Unsorted array: ");
    print_array(arr, ARRAY_SIZE);
    qsort(arr, ARRAY_SIZE, sizeof(int), comp);
    printf("Sorted array: ");
    print_array(arr, ARRAY_SIZE);
    read_number(&search_number);
    res = binary_search(arr, search_number);
    
    if (res == -1) {
        printf("%d not in the array\n", search_number);
    } else {
        printf("%d is on %d position in the array\n", search_number, res);
    }
    
    free(arr);
    return 0;
}


int binary_search(int* array, int target) {
    int mid;
    int left = 0;
    int right = ARRAY_SIZE - 1;
    
    while (left <= right) {
        mid = (left + right) / 2;
        if (array[mid] == target) {
            return mid;
        } else if (array[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}


int comp(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

