#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "../../../libs/input.h"


int str_to_num(char* str, int base);


int program_00_21() {
    int num, base;
    char* str_num;
    printf("Input str number: ");
    if (read_str(&str_num) == 1) {
        return 1;
    }
    
    printf("Input base: ");
    read_number(&base);
    num = str_to_num(str_num, base);
    printf("Your number in %d numeral system is %d\n", base, num);

    free(str_num);
    return 0;   
}

int str_to_num(char* str, int base) {
    int num = 0;
    int digit = 0;
    int minus = 0;

    if (*str == '-') {
        minus = 1;
        str++;
    }

    while (*str) {
        if (isalpha(*str)) {
            digit = tolower(*str++) - 'a' + 10;
        } else {
            digit = *str++ - '0';
        }
        num = num * base + digit;
    }

    if (minus) {
        num = -num;
    }

    return num;
}