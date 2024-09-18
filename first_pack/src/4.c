#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define ARRAY_SIZE 10
#define MIN -10000
#define MAX 10000


int generate_array(int arr[ARRAY_SIZE]);
int print_array(int array[ARRAY_SIZE]);


int main() {
    int arr[ARRAY_SIZE];
    generate_array(arr);
    print_array(arr);
    
    int max = -10000;
    int min = 10000;
    int max_index = -1;
    int min_index = -1;
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        if (arr[i] > max) {
            max = arr[i];
            max_index = i;
        }
        if (arr[i] < min) {
            min = arr[i];
            min_index = i;
        }
    }

        printf("Max element: %d, index: %d\nMin element: %d, index: %d\n", max, max_index, min, min_index);
        return 0;
    }


int generate_array(int arr[ARRAY_SIZE]) {
    srand(time(NULL));
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        arr[i] = (rand() % (MAX - MIN + 1)) + MIN;
    }
    return 0;
}


int print_array(int array[ARRAY_SIZE]) {
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n");
    return 0;
}
