#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void vilka(char const *restrict_format, ...) {
    void *arg;
    va_list valist;
    va_start(valist, restrict_format);
    while (*restrict_format) {
        arg = va_arg(valist, void*);
        if (*restrict_format == 'f') {
            free(arg);
        } else if (*restrict_format == 'c') {
            fclose((FILE*)arg);
        }
        restrict_format++;
    }
    va_end(valist);
    return;
}

int rerealloc(void **ptr, size_t size) {
    void *for_realloc = NULL;
    int err;

    for_realloc = realloc(*ptr, size);
    if (for_realloc == NULL) {
        return 1;
    }
    *ptr = for_realloc;
    return 0;
}