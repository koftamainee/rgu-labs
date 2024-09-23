#include <stdio.h>
#include <math.h>

#include "../../libs/input.h"

int glue(int num, int i, int len);
int num_to_ones(int num);

int main() {
    int i, len, num;
    printf("Enter number. ");
    read_number(&num);
    printf("%d == %b\n", num, num);
    printf("Enter i. ");
    read_number(&i);
    printf("Enter len. ");
    read_number(&len);
    printf("ans: %b\n", glue(num, i, len));

    return 0;
}


int glue(int num, int i, int len) {
    int left, right;
    int ones = num_to_ones(i);
    right = ones & num;
    left = (num & (ones << (len - i))) >> (len - 2 * i);
    return right | left;
}


int num_to_ones(int num) {
    int i, res = 0;
    for (i = 0; i < num; ++i) {
        res += pow(2, i);
    }
    return res;
}