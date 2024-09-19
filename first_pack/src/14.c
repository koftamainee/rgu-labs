#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 10000


int read_array_from_file(FILE* fptr, int* array);
int print_array(int* array, int size);
int find_max_and_min(int* array, int size, int* ans);


int main() {

    FILE* fptr = fopen("14.txt", "r");
    if (fptr == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    int* array = (int*)malloc(sizeof(int) * ARRAY_SIZE); 
    read_array_from_file(fptr, array);
    print_array(array, ARRAY_SIZE);
    int ans[4];
    find_max_and_min(array, ARRAY_SIZE, ans);
    printf("Max element: %d, index: %d\nMin element: %d, index: %d\n", ans[0], ans[1], ans[2], ans[3]);

    free(array);
    fclose(fptr);
    return 0;
}


int print_array(int* array, int size) {
    for (int i = 0; i < size; ++i) {
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


int read_array_from_file(FILE* fptr, int* array) {
    int i = 0;
    while (fscanf(fptr, "%d", &array[i]) != EOF) {
        ++i;
    }
    return 0;
}