#include <stdio.h>
#include <stdint.h>

#include "../../../libs/input.h"


int custom_strlen(char* str, size_t *len);


int program_00_11() {
    size_t len = 0;
    char* str;
    read_str(&str);
    custom_strlen(str, &len);
    printf("%zu\n", len);
    return 0;
}

int custom_strlen(char* str, size_t *len) {
    char* c = str;
    int cnt = 0;
    while (*c) {
        ++c;
        ++cnt;
    }
    *len = cnt - 1;
    return 0;
}
