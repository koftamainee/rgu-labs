#include <stdio.h>

#include "../../../libc/input.h"

int program_01_1() {
    int n;
    read_number(&n);
    printf("ans: %d\n", n % 2);
    return 0;
}
