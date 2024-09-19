#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int reverse(char* str);
char* read_str();


int main() {
    char* str = read_str();
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