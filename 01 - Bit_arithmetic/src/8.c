#include <stdio.h>
#include <math.h>

#include "../../libs/input.h"


int find_max_div(int n, int div);


int main() {
    int n, i, j;
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