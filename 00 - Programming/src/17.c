#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../../libs/input.h"


char* custom_strcat(char* str1, char* str2);


int main() {
    printf("First string: ");
    char* str1;
    read_str(&str1);
    printf("Second String: ");
    char* str2;
    read_str(&str2);

    printf("str1: %s, str2: %s", str1, str2);
    char* newstr = custom_strcat(str1, str2);
    printf("newstr: %s\n", newstr);

    free(str1);
    free(str2);
    return 0;
}


char* custom_strcat(char* str1, char* str2) {
    char* new_str = (char*)malloc(sizeof(char) * (strlen(str1) + strlen(str2) + 1));
    int cnt = 0;
    char* ptr = str1;
    while (*ptr != '\0') {
        new_str[cnt] = *ptr;
        ++cnt;
        ++ptr;
    }
    ptr = str2;
    while (*ptr != '\0') {
        new_str[cnt] = *ptr;
        ++cnt;
        ++ptr;
    }
    ++cnt;
    new_str[cnt] = '\0';

    return new_str;
}