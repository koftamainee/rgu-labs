#include <math.h>
#include <stdio.h>

#include "../../../libc/input.h"

int find_max_div(int n, int div);

int program_01_8() {
    int n;
    printf("Input a number: ");
    read_number(&n);

    int ans = find_max_div(n, pow(2, sizeof(int) * 8 - 2));
    printf("max pow of 2 is : %d\n", ans);
    return 0;
}

int find_max_div(int n, int div) {
    if (n % div == 0) {
        return div;
    } else {
        return find_max_div(n, div / 2);
    }
}
