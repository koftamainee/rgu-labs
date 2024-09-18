#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ARRAY_SIZE 10


int print_array(double* array, int size);
int randomize_array(double* array, int size);
int check_for_memory_alocation(double* array);
int gen_new_array(double* new_array, double* array, int size);
int search(double* array, int size, double target);


int main() {
    double* array = (double*)malloc(sizeof(double) * ARRAY_SIZE);
    if (array == NULL) {
        printf("Memory alocation error. Exit code 1.");
        return 1;
    }
    randomize_array(array, ARRAY_SIZE);

    double* new_array = (double*)malloc(sizeof(double) * ARRAY_SIZE);
    if (new_array == NULL) {
        printf("Memory alocation error. Exit code 1.");
        return 1;
    }

    int new_size = gen_new_array(new_array, array, ARRAY_SIZE);
    if (new_size == -1) {
        return 1;
    }

    printf("Old array: ");
    print_array(array, ARRAY_SIZE);
    printf("New array: ");
    print_array(new_array, new_size);
    
    free(array);
    free(new_array);
    printf("Elements deleted: %d\n", ARRAY_SIZE - new_size);
    return 0;
}


int print_array(double* array, int size) {
    for (int i = 0; i < size; ++i) {
        printf("%f ", array[i]);
    }
    printf("\n");
    return 0;
}


int randomize_array(double* array, int size) {
    srand(time(NULL));
        for (int i = 0; i < size; ++i) {
        array[i] = rand() % 500 - 200;
    }
    return 0;
}


int gen_new_array(double* new_array, double* array, int size) {
    int cnt = 0;

    for (int i = 0; i < size; ++i) {
        if (search(new_array, cnt + 1, array[i]) == 0) {
            new_array[cnt] = array[i];
            ++cnt;
        }
    }
    new_array = realloc(new_array, sizeof(double) * cnt);
    if (new_array == NULL) {
        printf("Memory alocation error. Exit code 1.");
        return -1;
    }

    return cnt;
}


int search(double* array, int size,  double target) {
    for (int i = 0; i < size; ++i) {
        if (target == array[i]) {
            return 1;
        }
    }
    return 0;
}