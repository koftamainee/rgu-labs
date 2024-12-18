#include <stdio.h>

#include "../../../libc/input.h"

int get_bit(int n, int k);

int program_01_2() {
    int n, k;
    printf("Input a number: ");
    read_number(&n);
    printf("Input a bit number: ");
    read_number(&k);
    printf("%d bit of a number %d is %b\n", k, n, get_bit(n, k));
    return 0;
}

int get_bit(int n, int k) { return (n & (1 << k)) >> k; }
