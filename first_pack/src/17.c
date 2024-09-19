#include <stdlib.h>
#include <stdio.h>
#include <string.h>


char* read_str();
char* custom_strcat(char* str1, char* str2);


int main() {
    printf("First string: ");
    char* str1 = read_str();
    printf("Second String: ");
    char* str2 = read_str();

    printf("str1: %s, str2: %s", str1, str2);
    char* newstr = custom_strcat(str1, str2);
    printf("newstr: %s\n", newstr);

    free(str1);
    free(str2);
    return 0;
}


char* read_str() {
    int length;
    while (1) {
        printf("Input str len: ");
        if (scanf("%d", &length) != 1) {
            printf("Input Error. Try again \n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        break;
    }
        char* str = (char*)malloc(sizeof(char) * (length + 1));
        while (1) {
        printf("Input %d symbol str: ", length);
        if (scanf("%s", str) != 1) {
            printf("Input Error. Try again \n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        break;
    }
    return str;
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