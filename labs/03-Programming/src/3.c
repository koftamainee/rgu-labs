#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int free_resources(const char* flags, ...);

int program_03_3() {
    char* str = (char*)malloc(23 * sizeof(char));
    int* arr = (int*)malloc(12 * sizeof(int));
    FILE* fptr = fopen("1.txt", "w");
    free_resources("fcf", str, fptr, arr);
    return 0;
}

int free_resources(const char* flags, ...) {
    const char* ptr = flags;
    va_list valist;
    va_start(valist, flags);
    while (*ptr) {
        printf("%c", *ptr);
        if (*ptr == 'f') {
            free(va_arg(valist, void*));
        } else if (*ptr == 'c') {
            fclose(va_arg(valist, FILE*));
        } else {
            return 1;
        }
        ptr++;
    }
    va_end(valist);
    return 0;
}