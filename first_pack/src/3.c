#include <stdio.h>

int swap(int* a, int* b);


int main() {
    int a = 5;
    int b = 10;
    printf("a: %d, b: %d\n", a, b);
    swap(&a, &b);
    printf("a: %d, b: %d\n", a, b);

    return 0;
}


int swap(int* a, int* b) {
    int buffer;
    buffer = *a;
    *a = *b;
    *b = buffer;

    return 0;
}