#include <stdio.h>

#include "../../libs/input.h"


int null_bit(int n, int k);


int main() {
    int n, k;
    printf("Input a number: ");
    read_number(&n);
    printf("Input a bit number: ");
    read_number(&k);
    printf("New number: %d\n", null_bit(n, k));
    return 0;
}


int null_bit(int n, int k) {
    return n & ~(1 << k);
}