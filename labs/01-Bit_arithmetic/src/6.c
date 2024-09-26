#include <stdio.h>

#include "../libs/input.h"
#include "../libs/bits.h"


int program_01_6() {
    int n, k;
    printf("Input a number: ");
    read_number(&n);
    printf("Input a bit number: ");
    read_number(&k);
    for (int i = 0; i < k; ++i) {
        n = null_bit(n, i);
    }
    printf("New number: %d\n", n);
    return 0;
}
