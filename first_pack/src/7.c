#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ARRAY_SIZE 20


int print_array(int* array, int size);
int randomize_array(int* array, int size);
int gen_new_array(int* new_array, int* old_array, int old_array_size);

int main() {

    int* old_array = (int*)malloc(sizeof(int) * ARRAY_SIZE);
    if (old_array == NULL) {
        printf("Memory alocation error. Exit code 1.");
        return 1;
    }

    int* new_array = (int*)malloc(sizeof(int) * (ARRAY_SIZE / 2));
    if (new_array == NULL) {
        printf("Memory alocation error. Exit code 1.");
        return 1;
    }

    
    randomize_array(old_array, ARRAY_SIZE);
    
    gen_new_array(new_array, old_array, ARRAY_SIZE);


    printf("Old array: ");
    print_array(old_array, ARRAY_SIZE);
    printf("New array: ");
    print_array(new_array, ARRAY_SIZE / 2);

    free(old_array);
    free(new_array);
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


int gen_new_array(int* new_array, int* old_array, int old_array_size) {
    for (int i = 1; i < old_array_size; i += 2) {
        new_array[i / 2] = old_array[i];
    }

    return 0;
}