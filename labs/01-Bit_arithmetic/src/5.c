#include <stdio.h>

#include "../../../libs/input.h"


int swap_bits(int n, int i, int j);


int program_01_5() {
    int n, i, j;
    printf("Input a number: ");
    read_number(&n);
    printf("Input first bit number: ");
    read_number(&i);
    printf("Input second bit number: ");
    read_number(&j);

    printf("New number: %d\n", swap_bits(n, i, j));

    return 0;
}


int swap_bits(int n, int i, int j) {
    int i_bit = n & (1 << i);
    int j_bit = n & (1 << j);
    if (i_bit != j_bit) {
        n = n ^ (1 << i);
        n = n ^ (1 << j);
    }

    return n;
}