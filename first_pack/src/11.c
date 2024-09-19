#include <stdio.h>
#include <stdint.h>

size_t custom_strlen(char* str);


int main() {
    printf("%zu\n", custom_strlen("123456789"));
    return 0;
}

size_t custom_strlen(char* str) {
    char* c = str;
    int cnt = 0;
    while (*c != '\0') {
        ++c;
        ++cnt;
    }
    return cnt;
}
