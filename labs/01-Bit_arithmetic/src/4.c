#include <stdio.h>

#include "../../../libc/bits.h"
#include "../../../libc/input.h"

int program_01_4() {
    int n, k;
    printf("Input a number: ");
    read_number(&n);
    printf("Input a bit number: ");
    read_number(&k);
    printf("New number: %d\n", null_bit(n, k));
    return 0;
}
