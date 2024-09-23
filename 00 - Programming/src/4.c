#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "../../libs/arrays.h"

#define ARRAY_SIZE 10
#define MIN -10000
#define MAX 10000





int main() {
    int arr[ARRAY_SIZE];
    randomize_array(arr, ARRAY_SIZE);
    print_array(arr, ARRAY_SIZE);
    int ans[4];
    find_max_and_min(arr, ARRAY_SIZE, ans);
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
