#include <stdio.h>

#include "../../libs/input.h"


int swap_bytes(int n, int i, int j);


int main() {
    int n, i, j;
    printf("Input a number: ");
    read_number(&n);
    printf("Input first byte number: ");
    read_number(&i);
    printf("Input second byte number: ");
    read_number(&j);
    n = swap_bytes(n, i, j);
    printf("New number: %d\n", n);
    return 0;
}


int swap_bytes(int n, int i, int j) {
    char* bytes_array = (char*)&n;
    char buf = bytes_array[i];
    bytes_array[i] = bytes_array[j];
    bytes_array[j] = buf;
    return n;
}