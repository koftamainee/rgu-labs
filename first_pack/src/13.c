#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define STR_LENGTH 10
#define ASCII_MOVE 32

int change_odd_elements(char* str);

int main() {
    char* str = (char*)malloc(sizeof(char) * STR_LENGTH);
    strcpy(str, "qw2er5t67");
    printf("new str: %s\n", str);
    change_odd_elements(str);
    printf("new str: %s\n", str);
    free(str);
    return 0;
}


int change_odd_elements(char* str) {
    char* ptr = str;
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