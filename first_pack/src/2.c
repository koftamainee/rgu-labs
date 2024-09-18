#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_LENGTH 10

int reverse(char* str);


int main() {
    char* str = (char*)malloc(sizeof(char) * STR_LENGTH);

    if (str == NULL) {
        printf("Memory alocation error. Exit code 1.");
        return 1;
    }
    strcpy(str, "123456789");

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