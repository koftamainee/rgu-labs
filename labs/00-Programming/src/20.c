#include <stdio.h>
#include <stdlib.h>

#include "../../../libs/input.h"
#include "../../../libs/custom_math.h"


int num_to_str(int num, int base, char** ans);

int program_00_20() {
    int num, base;
    char* str_num;
    printf("Input a number:\n");
    read_number(&num);
    printf("Input a counting system base: ");
    read_number(&base);
    if (!num_to_str(num, base, &str_num)) {
        printf("%s", str_num);
    }
    free(str_num);
    return 0;    
}


int num_to_str(int num, int base, char** ans) {
    int digit, len, is_negative = 0;
    if (base < 2 || base > 36) {
        return 1;
    }

    if (num == 0) {
        *ans = malloc(2 * sizeof(char));
        (*ans)[0] = '0';
        (*ans)[1] = '\0';
        return 0;
    }

    if (num < 0) {
        is_negative = 1;
        num = abs(num);
    }

    len = log_base(num, base) + 1 - is_negative;
    *ans = malloc((len + 1) * sizeof(char));
    char* ptr = *ans;

    if (is_negative) {
        *ptr++ = '-'; 
    }


    ptr += len + is_negative;
    *ptr-- = '\0'; 

    while (num) {
        digit = num % base;
        num /= base;
        if (digit > 9) {
            *ptr = digit + 'A' - 10;
        } else {
            *ptr = digit + '0';
        }
        ptr--;
    }
    return 0;
}
