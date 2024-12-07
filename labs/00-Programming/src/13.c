#include <stdio.h>
#include <stdlib.h>

#include "../../../libc/input.h"

#define STR_LENGTH 10
#define ASCII_MOVE 32

int change_odd_elements(char *str);

int program_00_13() {
    char *str;
    read_str(&str);
    printf("new str: %s\n", str);
    change_odd_elements(str);
    printf("new str: %s\n", str);
    free(str);
    return 0;
}

int change_odd_elements(char *str) {
    char *ptr = str;
    while (*ptr != '\0') {
        if ((ptr - str) % 2 != 0) {
            if (96 < *ptr && *ptr < 123) {
                *ptr = *ptr - ASCII_MOVE;
            }
        }
        ++ptr;
    }
    return 0;
}
