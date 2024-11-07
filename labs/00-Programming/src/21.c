#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "../../../libs/input.h"
#include "../../../libs/types.h"


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
    catoi(str_num, base, &num);
    printf("Your number in %d numeral system is %d\n", base, num);

    free(str_num);
    return 0;   
}