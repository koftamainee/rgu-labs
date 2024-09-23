#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../libs/input.h"


int reverse(char* str);


int main() {
    char* str;
    if (read_str(&str) == 1) {
        printf("Memory alocation error. Exit code 1.");
        return 1;
    }
    printf("normal: %s\n", str);
    reverse(str);
    printf("reverse: %s\n", str);
    free(str);

    return 0;
}


int reverse(char* str) {
    int left = 0;
    int right = strlen(str) - 1;
    char buffer;

    while (left < right) {
        buffer = str[left];
        str[left] = str[right];
        str[right] = buffer;
        ++left;
        --right;
    }

    return 0;
}


