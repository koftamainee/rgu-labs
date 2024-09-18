#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ARRAY_SIZE 10

int print_array(int* array, int size);
int randomize_array(int* array, int size);
int input_number(int* number);

int comp(const void* a, const void* b);

int binary_search(int* array, int target);


int main() {
    int search_number;
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
    input_number(&search_number);
    int res = binary_search(arr, search_number);
    
    if (res == -1) {
        printf("%d not in the array\n", search_number);
    } else {
        printf("%d is on %d position in the array\n", search_number, res);
    }
    
    free(arr);
    return 0;
}


int print_array(int* array, int size) {
    for (int i = 0; i < size; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n");
    return 0;
}


int randomize_array(int* array, int size) {
    srand(time(NULL));
        for (int i = 0; i < size; ++i) {
        array[i] = rand() % 500 - 200;
    }
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

int input_number(int* number) {
    while (1) {
        printf("Input search value: ");
        if (scanf("%d", number) != 1) {
            printf("Input Error. Try again \n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        break;
    }
    return 0;
}