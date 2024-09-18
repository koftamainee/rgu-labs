#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ARRAY_SIZE 20


int bubble_sort(int* array);
int print_array(int* array);
int randomize_array(int* array);


int main() {
    int* arr = (int*)malloc(sizeof(int) * ARRAY_SIZE);

    if (arr == NULL) {
        printf("Memory alocation error. Exit code 1.");
        return 1;
    }

    randomize_array(arr);
    printf("Normal: ");
    print_array(arr);
    bubble_sort(arr);
    printf("Sorted: ");
    print_array(arr);
    free(arr);
    return 0;
}


int print_array(int* array) {
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n");
    return 0;
}


int bubble_sort(int* array) {
    int buf;
    for (int i = 0; i < ARRAY_SIZE - 1; ++i) {
        for (int j = i + 1; j < ARRAY_SIZE; j++) {
            if (array[i] > array[j]) {
                buf = array[i];
                array[i] = array[j];
                array[j] = buf;
            }
        }
    }
    return 0;
}


int randomize_array(int* array) {
    srand(time(NULL));
        for (int i = 0; i < ARRAY_SIZE; ++i) {
        array[i] = rand() % 500 - 200;
    }
    return 0;
}