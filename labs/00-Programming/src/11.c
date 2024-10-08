#include <stdio.h>
#include <stdint.h>

#include "../../../libs/input.h"


size_t custom_strlen(char* str);


int program_00_11() {
    char* str;
    read_str(&str);
    printf("%zu\n", custom_strlen(str));
    return 0;
}

size_t custom_strlen(char* str) {
    char* c = str;
    int cnt = 0;
    while (*c) {
        ++c;
        ++cnt;
    }
    return cnt;
}
