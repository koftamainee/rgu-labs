#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../libc/errors.h"
#include "../../../libc/types.h"

int sum_in_base(char **ans, int base, size_t numbers_count, ...);
int add_two_numbers_in_base(char **ans, int base, char const *num_1,
                            char const *num_2);

int program_03_8(int argc, char *argv[]) {
    int err;
    char *sum = NULL;

    int const base = 16;

    err = sum_in_base(&sum, base, 5, "5", "ABCDEF", "52", "AF5E", "FE52");
    switch (err) {
        case OK:
            printf("sum in base %d: %s\n", base, sum);
            break;
        case DEREFERENCING_NULL_PTR:
            printf("Dereferencing NULL ptr\n");
            break;
        case MEMORY_ALLOCATION_ERROR:
            printf("Memory allocation error\n");
            break;
        case INVALID_NUMERIC_BASE:
            printf("Invalid numeric base\n");
            break;
        case INVALID_CHAR_TO_NUM_CONVERSION:
            printf("Invalid char to int conversion\n");
            break;
        case INVALID_NUM_TO_CHAR_CONVERSION:
            printf("Invalid int to char conversion\n");
            break;
        default:
            printf("Undefined behavior 0_o. Exit code %d", err);
            break;
    }

    free(sum);
    return err;
}

int sum_in_base(char **ans, int base, size_t numbers_count, ...) {
    int i, err;
    char *num = NULL, *temp = NULL;
    va_list valist;

    if (ans == NULL) {
        return DEREFERENCING_NULL_PTR;
    }
    if (base < 2 || base > 36) {
        return INVALID_NUMERIC_BASE;
    }

    va_start(valist, numbers_count);

    if (numbers_count == 1) {
        num = va_arg(valist, char *);
        *ans = (char *)malloc((strlen(num) + 1) * sizeof(char));
        if (*ans == NULL) {
            va_end(valist);
            return MEMORY_ALLOCATION_ERROR;
        }
        strcpy(*ans, num);
        va_end(valist);
        return OK;
    }

    num = va_arg(valist, char *);
    temp = va_arg(valist, char *);
    printf("Summing %s and %s\n", num, temp);
    err = add_two_numbers_in_base(ans, base, num, temp);
    if (err) {
        va_end(valist);
        return err;
    }

    for (i = 2; i < numbers_count; ++i) {
        num = va_arg(valist, char *);
        printf("Summing %s and %s\n", *ans, num);
        err = add_two_numbers_in_base(ans, base, *ans, num);
        if (err) {
            free(*ans);
            *ans = NULL;
            va_end(valist);
            return err;
        }
    }
    va_end(valist);

    return OK;
}

int add_two_numbers_in_base(char **ans, int base, char const *num_1,
                            char const *num_2) {
    int i, j, k, carry = 0, digit_1, digit_2, err, sum;
    size_t len_1, len_2, max_len;
    char *temp_num, c;

    len_1 = strlen(num_1);
    len_2 = strlen(num_2);
    max_len = (len_1 > len_2) ? len_1 : len_2;

    temp_num = (char *)calloc((max_len + 2), sizeof(char));
    if (temp_num == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }
    temp_num[max_len + 1] = '\0';

    i = len_1 - 1;
    j = len_2 - 1;
    k = max_len;

    while (i >= 0 || j >= 0 || carry) {
        if (i >= 0) {
            err = char_to_int(&digit_1, num_1[i--]);
            if (err) {
                free(temp_num);
                return err;
            }
        } else {
            digit_1 = 0;
        }
        if (j >= 0) {
            err = char_to_int(&digit_2, num_2[j--]);
            if (err) {
                free(temp_num);
                return err;
            }
        } else {
            digit_2 = 0;
        }
        sum = digit_1 + digit_2 + carry;
        carry = sum / base;

        err = int_to_char(&c, sum % base);
        temp_num[k--] = c;
        if (err) {
            free(temp_num);
            return err;
        }
    }

    if (carry) {
        temp_num[0] = '1';
    } else {
        memmove(temp_num, temp_num + 1, max_len + 1);
    }

    free(*ans);
    *ans = temp_num;
    return OK;
}
