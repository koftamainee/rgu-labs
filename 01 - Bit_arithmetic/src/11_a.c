#include <stdio.h>

#include "../../libs/input.h"


int main() {
    int i, len, num;
    printf("Enter number. ");
    read_number(&num);
    printf("%d == %b\n", num, num);

    printf("Enter i. ");
    read_number(&i);
    printf("Enter len. ");
    read_number(&len);


    return 0;
}