#include <stdio.h>

#include "../../libs/input.h"


int main() {
    int n;
    read_number(&n);
    printf("least significant bit of a number %d is %d\n", n, n % 2);
    return 0;
}
