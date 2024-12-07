#include <stdio.h>

#include "../../../libc/arrays.h"

#define ARRAY_SIZE 10
#define MIN -10000
#define MAX 10000

int program_00_4() {
    int arr[ARRAY_SIZE];
    int ans[4];
    randomize_array(arr, ARRAY_SIZE);
    print_array(arr, ARRAY_SIZE);
    find_max_and_min(arr, ARRAY_SIZE, ans);
    printf("Max element: %d, index: %d\nMin element: %d, index: %d\n", ans[0],
           ans[1], ans[2], ans[3]);
    return 0;
}
