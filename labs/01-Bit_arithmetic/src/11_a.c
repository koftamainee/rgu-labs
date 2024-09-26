#include <stdio.h>
#include "../../../libs/input.h"
#include "../../../libs/bits.h"

int glue(int num, int i, int len);

int program_01_11_a() {
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
