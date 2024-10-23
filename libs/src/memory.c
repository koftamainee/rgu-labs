#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void vilka(char const *restrict_format, ...) {
    char flag;
    void *arg;
    va_list valist;
    va_start(valist, restrict_format);
    while (*restrict_format) {
        flag = *restrict_format;
        arg = va_arg(valist, void*);
        if (flag == 'f') {
            free(arg);
        } else if (flag == 'c') {
            fclose((FILE*)arg);
        }
    }
    va_end(valist);
    return;
}

int rerealloc(void *src, void **dest, size_t size) {
    void *for_realloc = NULL;
    int err;

    for_realloc = realloc(src, size);
    if (for_realloc == NULL) {
        return 1;
    }
    *dest = for_realloc;
    return 0;
}