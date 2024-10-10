#include <stdio.h>

#include "../../../libs/input.h"


char* num_to_str(int num, int base);

int program_00_20() {
    int num, base;
    char* str_num;
    printf("Input a number:\n");
    read_number(&num);
    printf("Input a counting system base: ");
    read_number(&base);
    num_to_str(num, base);

    return 0;    
}



char* num_to_str(int num, int base) {

    

    return "";
}
