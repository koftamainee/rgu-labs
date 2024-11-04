#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>

#include "../../../libs/errors.h"
#include "../../../libs/types.h"
#include "../../../libs/custom_math.h"
#include "../../../libs/memory.h"

int overprintf(char *restrict _format, ...);
int overfprintf(FILE *restrict stream, char *restrict _format, ...);
int oversprintf(char *restrict s, char *restrict _format, ...);

int dec_float_to_str(float num, char **ans);
int dec_double_to_str(double num, char **ans);

int program_03_9() {
    printf("Standart flags:\n");
    printf("printf:     ");
    printf("standart flags: hi %d %u %f %lf %c %s %o %x %X %b\n", 123, -52, 12.23, 52.52, 'a', "abcdefg", 123, 88, 14, 65);
    printf("overprintf: ");
    overprintf("standart flags: hi %d %u %f %lf %c %s %o %x %X %b | custom flags: %Ro %Zr || %Cv %CV %to %TO || %mi | %mu | %mf | %md\n",
    123, -52, 12.23, 52.52, 'a', "abcdefg", 123, 88, 14, 65,144, 98, 2731, 16, 2731, 16, "-aab", 16, "-AAB", 16, 55, -3, 82.23, 987.15);
    return 0;
}

int overprintf(char *restrict _format, ...) {
    float float_num;
    double double_num;
    unsigned int unum;
    int err, base, num;
    int i_ans, i;
    char *s_ans = NULL, *s_ans_cpy = NULL, *str_num = NULL;
    unsigned char *uc_ptr;
    va_list valist;
    va_start(valist, _format);

    while (*_format) {
        if (*_format == '%') {
            switch (*(_format + 1)) {

                //default flags
                case 'd': 
                    err = citoa(va_arg(valist, int), 10, &s_ans);
                    if (err) {
                        free(s_ans);
                        s_ans = NULL;
                        s_ans_cpy = NULL;
                        return err;
                    }
                    s_ans_cpy = s_ans;
                    while (*s_ans_cpy) {
                        putc(*s_ans_cpy++, stdout);
                    }
                    free(s_ans);
                        s_ans = NULL;
                        s_ans_cpy = NULL;
                    _format += 2;
                    break;

                case 'u': 
                    err = cutoa(va_arg(valist, unsigned int), 10, &s_ans);
                    if (err) {
                        free(s_ans);
                        s_ans = NULL;
                        s_ans_cpy = NULL;
                        return err;
                    }
                    s_ans_cpy = s_ans;
                    while (*s_ans_cpy) {
                        putc(*s_ans_cpy++, stdout);
                    }
                    free(s_ans);
                        s_ans = NULL;
                        s_ans_cpy = NULL;
                    _format += 2;
                    break;

                case 'f':
                    err = dec_float_to_str(va_arg(valist, double), &s_ans);
                    if (err) {
                        free(s_ans);
                        s_ans = NULL;
                        s_ans_cpy = NULL;
                        return err;
                    }
                    s_ans_cpy = s_ans;
                    while (*s_ans_cpy) {
                        putc(*s_ans_cpy++, stdout);
                    }
                    free(s_ans);
                        s_ans = NULL;
                        s_ans_cpy = NULL;
                    _format += 2;
                    break;

                case 'l':
                    if (*(_format + 2) == 'f') {
                        err = dec_float_to_str(va_arg(valist, double), &s_ans);
                        if (err) {
                            free(s_ans);
                            s_ans = NULL;
                            s_ans_cpy = NULL;
                            return err;
                        }
                        s_ans_cpy = s_ans;
                        while (*s_ans_cpy) {
                            putc(*s_ans_cpy++, stdout);
                        }
                        free(s_ans);
                        s_ans = NULL;
                        s_ans_cpy = NULL;
                        _format += 3;

                    } else {
                        putc('%', stdout);
                        _format++;
                    }
                    break;

                case 'c':
                putc(va_arg(valist, int), stdout);
                _format += 2;
                break;
                case 's':
                s_ans = va_arg(valist, char*);
                while (*s_ans) {
                    putc(*s_ans++, stdout);
                }
                _format += 2;
                break;

                case 'o': 
                    err = citoa(va_arg(valist, int), 8, &s_ans);
                    if (err) {
                        free(s_ans);
                        s_ans = NULL;
                        s_ans_cpy = NULL;
                        return err;
                    }
                    s_ans_cpy = s_ans;
                    while (*s_ans_cpy) {
                        putc(*s_ans_cpy++, stdout);
                    }
                    free(s_ans);
                        s_ans = NULL;
                        s_ans_cpy = NULL;
                    _format += 2;
                    break;

                case 'x': 
                    err = citoa(va_arg(valist, int), 16, &s_ans);
                    if (err) {
                        free(s_ans);
                        s_ans = NULL;
                        s_ans_cpy = NULL;
                        return err;
                    }
                    s_ans_cpy = s_ans;
                    while (*s_ans_cpy) {
                        putc(tolower(*s_ans_cpy++), stdout);
                    }
                    free(s_ans);
                        s_ans = NULL;
                        s_ans_cpy = NULL;
                    _format += 2;
                    break;
                
                case 'X': 
                    err = citoa(va_arg(valist, int), 16, &s_ans);
                    if (err) {
                        free(s_ans);
                        s_ans = NULL;
                        s_ans_cpy = NULL;
                        return err;
                    }
                    s_ans_cpy = s_ans;
                    while (*s_ans_cpy) {
                        putc(*s_ans_cpy++, stdout);
                    }
                    free(s_ans);
                        s_ans = NULL;
                        s_ans_cpy = NULL;
                    _format += 2;
                    break;

                case 'b': 
                    err = citoa(va_arg(valist, int), 2, &s_ans);
                    if (err) {
                        free(s_ans);
                        s_ans = NULL;
                        s_ans_cpy = NULL;
                        return err;
                    }
                    s_ans_cpy = s_ans;
                    while (*s_ans_cpy) {
                        putc(*s_ans_cpy++, stdout);
                    }
                    free(s_ans);
                        s_ans = NULL;
                        s_ans_cpy = NULL;
                    _format += 2;
                    break;

                // custom flags
                case 'R':
                    if (*(_format + 2) == 'o') {
                        va_arg(valist, int); //TODO
                        _format += 3;
                    } else {
                        putc('%', stdout);
                        _format++;
                    }
                    break;
                
                case 'Z':
                    if (*(_format + 2) == 'r') {
                        va_arg(valist, int); //TODO
                        _format += 3;
                    } else {
                        putc('%', stdout);
                        _format++;
                    }
                    break;
                
                case 'C':
                    if (*(_format + 2) == 'v') {
                        num = va_arg(valist, int);
                        base = va_arg(valist, int);
                        if (base < 2 || base > 36) {
                            base = 10;
                        }
                        err = citoa(num, base, &s_ans);
                        if (err) {
                            free(s_ans);
                            s_ans = NULL;
                            s_ans_cpy = NULL;
                            return err;
                        }
                        s_ans_cpy = s_ans;
                        while (*s_ans_cpy) {
                            putc(tolower(*s_ans_cpy++), stdout);
                        }
                        free(s_ans);
                            s_ans = NULL;
                            s_ans_cpy = NULL;
                            _format += 3;
                    } else if (*(_format + 2) == 'V') {
                         num = va_arg(valist, int);
                        base = va_arg(valist, int);
                        if (base < 2 || base > 36) {
                            base = 10;
                        }
                        err = citoa(num, base, &s_ans);
                        if (err) {
                            free(s_ans);
                            s_ans = NULL;
                            s_ans_cpy = NULL;
                            return err;
                        }
                        s_ans_cpy = s_ans;
                        while (*s_ans_cpy) {
                            putc(*s_ans_cpy++, stdout);
                        }
                        free(s_ans);
                            s_ans = NULL;
                            s_ans_cpy = NULL;
                            _format += 3;
                    } else {
                        putc('%', stdout);
                        _format++;
                    }
                    break;

                case 't':
                    if (*(_format + 2) == 'o') {
                        str_num = va_arg(valist, char*);
                        base = va_arg(valist, int);
                        err = catoi(str_num, base, &num);
                        if (err) {
                            return err;
                        }
                        err = citoa(num, 10, &s_ans);
                        if (err) {
                            free(s_ans);
                            s_ans = NULL;
                            s_ans_cpy = NULL;
                            return err;
                        }
                        s_ans_cpy = s_ans;
                        while (*s_ans_cpy) {
                            putc(*s_ans_cpy++, stdout);
                        }
                        free(s_ans);
                            s_ans = NULL;
                            s_ans_cpy = NULL;

                        _format += 3;
                    } else {
                        putc('%', stdout);
                        _format++;
                    }
                    break;
                
                case 'T':
                    if (*(_format + 2) == 'O') {
                        str_num = va_arg(valist, char*);
                        base = va_arg(valist, int);
                        err = catoi(str_num, base, &num);
                        if (err) {
                            return err;
                        }
                        err = citoa(num, 10, &s_ans);
                        if (err) {
                            free(s_ans);
                            s_ans = NULL;
                            s_ans_cpy = NULL;
                            return err;
                        }
                        s_ans_cpy = s_ans;
                        while (*s_ans_cpy) {
                            putc(*s_ans_cpy++, stdout);
                        }
                        free(s_ans);
                            s_ans = NULL;
                            s_ans_cpy = NULL;

                        _format += 3;
                    } else {
                        putc('%', stdout);
                        _format++;
                    }
                    break;

                case 'm':
                    switch (*(_format + 2)) {
                        case 'i':
                            num = va_arg(valist, int);
                            uc_ptr = (unsigned char*)&num;
                            for (i = 0; i < sizeof(int); ++i) {
                                err = citoa((*uc_ptr)++, 2, &s_ans);
                                if (err) {
                                    free(s_ans);
                                    s_ans = NULL;
                                    s_ans_cpy = NULL;
                                    return err;
                                }
                                s_ans_cpy = s_ans;
                                while (*s_ans_cpy) {
                                    putc(*s_ans_cpy++, stdout);
                                }
                                free(s_ans);
                                    s_ans = NULL;
                                    s_ans_cpy = NULL;
                                putc(' ', stdout);
                                    }
                                    uc_ptr = NULL;
                                    _format += 3;
                            break;
                    
                    case 'u':
                            unum = va_arg(valist, unsigned int);
                            uc_ptr = (unsigned char*)&unum;
                            for (i = 0; i < sizeof(unsigned int); ++i) {
                                err = citoa((*uc_ptr)++, 2, &s_ans);
                                if (err) {
                                    free(s_ans);
                                    s_ans = NULL;
                                    s_ans_cpy = NULL;
                                    return err;
                                }
                                s_ans_cpy = s_ans;
                                while (*s_ans_cpy) {
                                    putc(*s_ans_cpy++, stdout);
                                }
                                free(s_ans);
                                    s_ans = NULL;
                                    s_ans_cpy = NULL;
                                putc(' ', stdout);
                        }
                        uc_ptr = NULL;
                        _format += 3;
                        break;
                    
                    case 'f':
                            float_num = va_arg(valist, double);
                            uc_ptr = (unsigned char*)&float_num;
                            for (i = 0; i < sizeof(float); ++i) {
                                err = citoa((*uc_ptr)++, 2, &s_ans);
                                if (err) {
                                    free(s_ans);
                                    s_ans = NULL;
                                    s_ans_cpy = NULL;
                                    return err;
                                }
                                s_ans_cpy = s_ans;
                                while (*s_ans_cpy) {
                                    putc(*s_ans_cpy++, stdout);
                                }
                                free(s_ans);
                                    s_ans = NULL;
                                    s_ans_cpy = NULL;
                                putc(' ', stdout);
                        }
                        uc_ptr = NULL;
                        _format += 3;
                        break;
                    
                    case 'd':
                            double_num = va_arg(valist, double);
                            uc_ptr = (unsigned char*)&double_num;
                            for (i = 0; i < sizeof(double); ++i){
                                err = citoa((*uc_ptr)++, 2, &s_ans);
                                if (err) {
                                    free(s_ans);
                                    s_ans = NULL;
                                    s_ans_cpy = NULL;
                                    return err;
                                }
                                s_ans_cpy = s_ans;
                                while (*s_ans_cpy) {
                                    putc(*s_ans_cpy++, stdout);
                                }
                                free(s_ans);
                                    s_ans = NULL;
                                    s_ans_cpy = NULL;
                                putc(' ', stdout);
                            }
                        uc_ptr = NULL;
                        _format += 3;
                        break;

                            
                        default:
                            putc('%', stdout);
                            _format++;
                    }
                    break;

            }
        }

        putc(*_format, stdout);
        _format++;
    }
    return OK;
}

int dec_float_to_str(float num, char **ans) {
    char *str_num = NULL;
    int integer_part, err, len, i, int_end;
    float float_part;

    if (ans == NULL) {
        return DEREFERENCING_NULL_PTR;
    }
    integer_part = (int)num;
    float_part = num - integer_part;

    err = citoa(integer_part, 10, &str_num);
    if (err) {
        return err;
    }
    int_end = log_base(num, 10) + 1;
    len = int_end + 1 + 6; // 1 for '.', 6 for frac. part
    err = rerealloc((void**)&str_num, len + 1);
    if (err) {
        return err;
    }
    str_num[len] = 0;
    str_num[int_end] = '.';
    
    for (i = int_end + 1; i < len; ++i) {
        float_part *= 10;
        str_num[i] = ((int)float_part) + '0';
        float_part = float_part - (int)float_part;
    }
    *ans = str_num;

    return OK;
}

int dec_double_to_str(double num, char **ans) {
    char *str_num = NULL;
    int integer_part, err, len, i, int_end;
    double float_part;

    if (ans == NULL) {
        return DEREFERENCING_NULL_PTR;
    }
    integer_part = (int)num;
    float_part = num - integer_part;

    err = citoa(integer_part, 10, &str_num);
    if (err) {
        return err;
    }
    int_end = log_base(num, 10) + 1;
    len = int_end + 1 + 6; // 1 for '.', 6 for frac. part
    err = rerealloc((void**)&str_num, len + 1);
    if (err) {
        return err;
    }
    str_num[len] = 0;
    str_num[int_end] = '.';
    
    for (i = int_end + 1; i < len; ++i) {
        float_part *= 10;
        str_num[i] = ((int)float_part) + '0';
        float_part = float_part - (int)float_part;
    }
    *ans = str_num;

    return OK;
}

