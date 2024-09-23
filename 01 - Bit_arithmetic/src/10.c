#include <stdio.h>

#include "../../libs/input.h"

#define slide_left(num, k) (num << k)
#define slide_right(num, k) (num >> k)


int main() {
    int num, r_slide, l_slide;
    read_binary_number(&num);
    printf("Enter left slide length. ");
    read_number(&l_slide);
    printf("Enter right slide length. ");
    read_number(&r_slide);
    printf("left slide: %b\nright slide: %b\n", slide_left(num, l_slide), slide_right(num, r_slide));
    return 0;
}