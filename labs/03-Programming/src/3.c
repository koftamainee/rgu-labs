#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int vilka(int flag, ...);

int program_03_3(int argc, char *argv[]) {
    char* str = (char*)malloc(23 * sizeof(char));
    int* arr = (int*)malloc(12 * sizeof(int));
    FILE* fptr = fopen("1.txt", "w");

    printf("resources for free: %p %p %p\n\n", str, fptr, arr);

    vilka('f', str, (char*)'c', fptr, (char)'f', arr);
    return 0;
}

int vilka(int flag, ...) {
    int c;
    void *arg;
    va_list valist;
    c = flag;
    va_start(valist, flag);
    while (1) {
        arg = va_arg(valist, void*);
        if (arg == NULL) {
            return 0;
        }
        if (c == 'f') {
            printf("freeing memory at %p\n", arg);
            free(arg);
        } else if (c == 'c') {
            printf("closing file var at %p\n", arg);
            fclose((FILE*)arg);
        } else {
            return 1;
        }

        c = va_arg(valist, int);
    }
    va_end(valist);
    return 0;
}