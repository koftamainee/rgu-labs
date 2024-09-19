#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define ARRAY_SIZE 10
#define MIN -10000
#define MAX 10000


int generate_array(int* arr);
int print_array(int* array);
int find_max_and_min(int* array, int* ans);


int main() {
    int arr[ARRAY_SIZE];
    generate_array(arr);
    print_array(arr);
    int ans[4];
    find_max_and_min(arr, ans);
    printf("Max element: %d, index: %d\nMin element: %d, index: %d\n", ans[0], ans[1], ans[2], ans[3]);
    return 0;
}

int generate_array(int* arr) {
    srand(time(NULL));
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        arr[i] = (rand() % (MAX - MIN + 1)) + MIN;
    }
    return 0;
}


int print_array(int* array) {
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n");
    return 0;
}


int find_max_and_min(int* array, int* ans) {
    int max = -10000;
    int min = 10000;
    int max_index = -1;
    int min_index = -1;
    for (int i = 0; i < ARRAY_SIZE; ++i) {
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
