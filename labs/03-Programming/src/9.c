#include <stdarg.h>
#include <stdio.h>

#include "../../../libs/errors.h"
#include "../../../libs/types.h"

int overprintf(char *restrict _format, ...);
int overfprintf(FILE *restrict stream, char *restrict _format, ...);
int oversprintf(char *restrict s, char *restrict _format, ...);

int num_to_str(int num, int base, char** ans);

int program_03_9() {
    overprintf("hi %d %u\n", 123, -52);
    overprintf("hi %d %u\n", 123, -52);
    return 0;
}

int overprintf(char *restrict _format, ...) {
    int err;
    int i_ans;
    char *s_ans;
    va_list valist;
    va_start(valist, _format);

    while (*_format) {
        if (*_format == '%') {
            switch (*(_format + 1)) {
                case 'd': 
                    err = citoa(va_arg(valist, int), 10, &s_ans);
                    if (err) {
                        return err;
                    }
                    while (*s_ans) {
                        putc(*s_ans++, stdout);
                    }
                    _format += 2;
                    break;
                case 'u': 
                    err = citoa((unsigned int)va_arg(valist, unsigned int), 10, &s_ans); // FIXME
                    if (err) {
                        return err;
                    }
                    while (*s_ans) {
                        putc(*s_ans++, stdout);
                    }
                    _format += 2;
                    break;
            }
        }

        putc(*_format, stdout);
        _format++;
    }
    return OK;
}

