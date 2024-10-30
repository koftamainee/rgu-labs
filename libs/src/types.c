#include <stdlib.h>
#include <ctype.h>

#include "../errors.h"
#include "../custom_math.h"

int citoa(int num, int base, char **ans) {
    int digit, len, is_negative = 0;
    if (base < 2 || base > 36) {
        return INVALID_NUMERIC_BASE;
    }

    if (num == 0) {
        *ans = malloc(2 * sizeof(char));
        if (*ans == NULL) {
            return MEMORY_ALLOCATE_ERROR;
        }
        (*ans)[0] = '0';
        (*ans)[1] = '\0';
        return OK;
    }

    if (num < 0) {
        is_negative = 1;
        num = abs(num);
    }

    len = log_base(num, base) + 1 - is_negative;
    *ans = malloc((len + 1) * sizeof(char));
    if (*ans == NULL) {
        return MEMORY_ALLOCATE_ERROR;
    }
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
    return OK;
}

int catoi(char* str, int base, int *ans) {
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

    *ans = num;

    return OK;
}