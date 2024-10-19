#include <stdio.h>
#include <stdlib.h>

#include "../../../libs/input.h"
#include "../../../libs/custom_math.h"

int gorner_2p(int src, char **dest, int base);

int program_03_1(int argc, char *argv[]) {
    int num, base, r;
    char *ans;
    printf("Input number: ");
    read_number(&num);
    printf("Input r: ");
    read_number(&r);
    base = 1 << r;
    printf("Base: %d\n", base);
    
    switch (gorner_2p(num, &ans, r)) {
        case 0: printf("num %d in %d base is: %s\n", num, base, ans); 
        break;
        case 1: printf("Invalid base.\n");
        return 1;
        case 2: printf("Memory allocation error.\n");
        return 2;
        defauls: printf("Undefined behaviour 0_o");
        return -1;
    }
    free(ans);
    return 0;
}

int gorner_2p(int src, char** dest, int r) {
    int len = 0, is_negative = 0;
    char *ptr;
    int base = 1 << r;

    if (base < 2 || base > 32) {
        return 1; 
    }

    len = log_base(abs(src), base) + 1; 

    if (src == 0) {
        *dest = (char*)malloc(2 * sizeof(char));
        if (!*dest) {
            return 2;
        }
        (*dest)[0] = '0';
        (*dest)[1] = '\0';
        return 0;
    }

    if (src < 0) {
        is_negative = 1;
        src = abs(src);
        len++;
    }

    *dest = (char*)malloc((len + 1) * sizeof(char));
    if (!*dest) {
        return 2;
    }
    ptr = *dest;

    if (is_negative) {
        *ptr = '-';
        ptr++;
    }

    ptr+= len - is_negative;
    *ptr-- = 0;

    while (src) {
        int remainder = src & (base - 1);
        *ptr-- = (remainder < 10) ? (remainder + '0') : (remainder - 10 + 'A');
        src >>= r;
    }

    return 0;
}