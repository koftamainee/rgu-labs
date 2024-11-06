#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "../../../libs/errors.h"
#include "../../../libs/types.h"
#include "../../../libs/custom_math.h"
#include "../../../libs/memory.h"

int __format_string(char **formatted_string, char *restrict _format, va_list valist);

int overprintf(char *restrict _format, ...);
int overfprintf(FILE *restrict stream, char *restrict _format, ...);
int oversprintf(char *restrict s, char *restrict _format, ...);

int dec_float_to_str(float num, char **ans);
int dec_double_to_str(double num, char **ans);
int find_zykendorffs_representation(unsigned int num, unsigned int const *row, size_t numbers_in_row_count, unsigned int **ans, size_t *numbers_in_ans_count);
int int_to_roman(int num, char **ans);

int program_03_9() {
    FILE *fptr = NULL;
    char buffer[BUFSIZ];

    
    printf("Testing overfprintf: ");
    overprintf("standart flags: hi %d %u %f %lf %c %s %o %x %X %b | custom flags: %Ro || %Zr || %Cv %CV || %to %TO || %mi | %mu | %mf | %md\n",
    123, -52, 12.23, 52.52, 'a', "abcdefg", 123, 88, 14, 65, 144, 6566, 2731, 16, 2731, 16, "-aab", 16, "-AAB", 16, 55, -3, 82.23, 987.15);

    // Testing overfprintf
    printf("\nTesting overfprintf\n");
    fptr = fopen("1.txt", "w");
    if (fptr == NULL) {
        printf("Opening file error\n");
        return OPENING_THE_FILE_ERROR;
    }
    overfprintf(fptr, "standart flags: hi %d %u %f %lf %c %s %o %x %X %b | custom flags: %Ro || %Zr || %Cv %CV || %to %TO || %mi | %mu | %mf | %md\n",
    123, -52, 12.23, 52.52, 'a', "abcdefg", 123, 88, 14, 65, 144, 6566, 2731, 16, 2731, 16, "-aab", 16, "-AAB", 16, 55, -3, 82.23, 987.15);
    fclose(fptr);
    printf("Data have been written in to 1.txt file\n");

    //Testing oversprintf
    oversprintf(buffer, "standart flags: hi %d %u %f %lf %c %s %o %x %X %b | custom flags: %Ro || %Zr || %Cv %CV || %to %TO || %mi | %mu | %mf | %md\n",
    123, -52, 12.23, 52.52, 'a', "abcdefg", 123, 88, 14, 65, 144, 6566, 2731, 16, 2731, 16, "-aab", 16, "-AAB", 16, 55, -3, 82.23, 987.15);
    printf("\nPrinting oversprintf: %s", buffer);

    return OK;
}

int __format_string(char **formatted_string, char *restrict _format, va_list valist) {
    size_t nums_in_row, nums_in_ans, format_len, current_format_index = 0, capacity = BUFSIZ, current_ans_index = 0;
    float float_num;
    double double_num;
    unsigned int unum;
    int err, base, num;
    int i_ans, i;
    unsigned int *u_ptr = NULL, *u_ans_ptr = NULL;
    char *s_ans = NULL, *s_ans_cpy = NULL, *str_num = NULL, *temp_ans = NULL;
    unsigned char *uc_ptr;
    format_len = strlen(_format) - 1;

    temp_ans = (char*)malloc(capacity * sizeof(char));
    if (temp_ans == NULL) {
        return MEMORY_ALLOCATE_ERROR;
    }
    
    while (current_format_index <= format_len) {
        if (capacity - BUFSIZ < current_ans_index) {
            err = rerealloc((void**)&temp_ans, capacity * 2);
            if (err) {
                return err;
            }
            capacity *= 2;
        }
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
                    memcpy(temp_ans + current_ans_index, s_ans, strlen(s_ans));
                    current_ans_index += strlen(s_ans);
                    free(s_ans);
                        s_ans = NULL;
                        s_ans_cpy = NULL;
                    current_format_index += 2;
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
                    memcpy(temp_ans + current_ans_index, s_ans, strlen(s_ans));
                    current_ans_index += strlen(s_ans);
                    free(s_ans);
                        s_ans = NULL;
                        s_ans_cpy = NULL;
                    current_format_index += 2;
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
                    memcpy(temp_ans + current_ans_index, s_ans, strlen(s_ans));
                    current_ans_index += strlen(s_ans);
                    free(s_ans);
                        s_ans = NULL;
                        s_ans_cpy = NULL;
                    
                    current_format_index += 2;
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
                        memcpy(temp_ans + current_ans_index, s_ans, strlen(s_ans));
                    current_ans_index += strlen(s_ans);
                    free(s_ans);
                        s_ans = NULL;
                        s_ans_cpy = NULL;
                    
                        current_format_index += 3;
                        _format += 3;

                    } else {
                        *(temp_ans + current_ans_index) = '%';
                        current_ans_index += 1;
                        current_format_index += 1;
                        _format++;
                    }
                    break;

                case 'c':
                    temp_ans[current_ans_index] = va_arg(valist, int);
                    current_ans_index += 1;
                    current_format_index += 2;
                    _format += 2;
                    break;

                case 's':
                        s_ans = va_arg(valist, char*);
                        memcpy(temp_ans + current_ans_index, s_ans, strlen(s_ans));
                    current_ans_index += strlen(s_ans);
                        s_ans = NULL;
                        s_ans_cpy = NULL;
                    
                    current_format_index += 2;
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
                    memcpy(temp_ans + current_ans_index, s_ans, strlen(s_ans));
                    current_ans_index += strlen(s_ans);
                    free(s_ans);
                        s_ans = NULL;
                        s_ans_cpy = NULL;
                    
                    current_format_index += 2;
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
                        *s_ans_cpy = tolower(*s_ans_cpy);
                        s_ans_cpy++;
                    }
                    memcpy(temp_ans + current_ans_index, s_ans, strlen(s_ans));
                    current_ans_index += strlen(s_ans);
                    free(s_ans);
                        s_ans = NULL;
                        s_ans_cpy = NULL;
                    
                    current_format_index += 2;
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
                    memcpy(temp_ans + current_ans_index, s_ans, strlen(s_ans));
                     current_ans_index += strlen(s_ans);
                    free(s_ans);
                        s_ans = NULL;
                        s_ans_cpy = NULL;
                   
                    current_format_index += 2;
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
                    memcpy(temp_ans + current_ans_index, s_ans, strlen(s_ans));
                    current_ans_index += strlen(s_ans);
                    free(s_ans);
                        s_ans = NULL;
                        s_ans_cpy = NULL;
                    
                    current_format_index += 2;
                    _format += 2;
                    break;

                // custom flags
                case 'R':
                    if (*(_format + 2) == 'o') {
                        num = va_arg(valist, int);
                        err = int_to_roman(num, &s_ans);
                        if (err) {
                            free(s_ans);
                            return err;
                        }
                        memcpy(temp_ans + current_ans_index, s_ans, strlen(s_ans));
                    current_ans_index += strlen(s_ans);
                    free(s_ans);
                        s_ans = NULL;
                        s_ans_cpy = NULL;
                    
                        current_format_index += 3;
                        _format += 3;
                    } else {
                        temp_ans[current_ans_index] = '%';
                        current_ans_index += 1;
                        current_format_index += 1;
                        _format++;
                    }
                    break;
                
                case 'Z':
                    if (*(_format + 2) == 'r') {
                        unum = va_arg(valist, unsigned int);
                        err = generate_fibonacci_row(unum, &u_ptr, &nums_in_row);
                        if (err) {
                            free(u_ptr);
                            return err;
                        }
                        for (i = 0; i < nums_in_row; ++i) {
                        }
                        err = find_zykendorffs_representation(unum, u_ptr, nums_in_row, &u_ans_ptr, &nums_in_ans);
                        if (err) {
                            vilka("ff", u_ptr, u_ans_ptr);
                            return err;
                        }
                        for(i = 0; i < nums_in_ans; ++i) {
                            
                            err = cutoa(u_ans_ptr[i], 10, &s_ans);
                            if (err) {
                                free(s_ans);
                                s_ans = NULL;
                                s_ans_cpy = NULL;
                                return err;
                            }
                            memcpy(temp_ans + current_ans_index, s_ans, strlen(s_ans));
                    current_ans_index += strlen(s_ans);
                    temp_ans[current_ans_index] = ' ';
                    current_ans_index += 1;
                    free(s_ans);
                        s_ans = NULL;
                        s_ans_cpy = NULL;
                    

                        }
                        current_format_index += 3;
                        _format += 3;
                    } else {
                        temp_ans[current_ans_index] = '%';
                        current_ans_index += 1;
                        current_format_index += 1;
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
                        memcpy(temp_ans + current_ans_index, s_ans, strlen(s_ans));
                    current_ans_index += strlen(s_ans);
                    free(s_ans);
                        s_ans = NULL;
                        s_ans_cpy = NULL;
                    
                            current_format_index += 3;
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
                        memcpy(temp_ans + current_ans_index, s_ans, strlen(s_ans));
                    current_ans_index += strlen(s_ans);
                    free(s_ans);
                        s_ans = NULL;
                        s_ans_cpy = NULL;
                    
                            current_format_index += 3;
                            _format += 3;
                    } else {
                        temp_ans[current_ans_index] = '%';
                        current_ans_index += 1;
                        current_format_index += 1;
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
                        memcpy(temp_ans + current_ans_index, s_ans, strlen(s_ans));
                    current_ans_index += strlen(s_ans);
                    free(s_ans);
                        s_ans = NULL;
                        s_ans_cpy = NULL;
                    
                        current_format_index += 3;
                        _format += 3;
                    } else {
                        temp_ans[current_format_index] = '%';
                        current_ans_index += 1;
                        current_format_index += 1;
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
                        memcpy(temp_ans + current_ans_index, s_ans, strlen(s_ans));
                    current_ans_index += strlen(s_ans);
                    free(s_ans);
                        s_ans = NULL;
                        s_ans_cpy = NULL;
                    
                        current_format_index += 3;
                        _format += 3;
                    } else {
                        temp_ans[current_ans_index] = '%';
                        current_ans_index += 1;
                        current_format_index += 1;
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
                                memcpy(temp_ans + current_ans_index, s_ans, strlen(s_ans));
                    current_ans_index += strlen(s_ans);
                    free(s_ans);
                        s_ans = NULL;
                        s_ans_cpy = NULL;
                    
                    temp_ans[current_ans_index] = ' ';
                    current_ans_index += 1;
                            }
                                    uc_ptr = NULL;
                                    current_format_index += 3;
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
                                memcpy(temp_ans + current_ans_index, s_ans, strlen(s_ans));
                    current_ans_index += strlen(s_ans);
                    free(s_ans);
                        s_ans = NULL;
                        s_ans_cpy = NULL;
                    temp_ans[current_ans_index] = ' ';
                    current_ans_index += 1;
                        }
                        uc_ptr = NULL;
                        current_format_index += 3;
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
                                memcpy(temp_ans + current_ans_index, s_ans, strlen(s_ans));
                    current_ans_index += strlen(s_ans);
                    free(s_ans);
                        s_ans = NULL;
                        s_ans_cpy = NULL;
                    temp_ans[current_ans_index] = ' ';
                    current_ans_index += 1;
                        }
                        uc_ptr = NULL;
                        current_format_index += 3;
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
                                memcpy(temp_ans + current_ans_index, s_ans, strlen(s_ans));
                    current_ans_index += strlen(s_ans);
                    free(s_ans);
                        s_ans = NULL;
                        s_ans_cpy = NULL;
                    
                    temp_ans[current_ans_index] = ' ';
                    current_ans_index += 1;
                            }
                        uc_ptr = NULL;
                        current_format_index += 3;
                        _format += 3;
                        break;

                            
                        default:
                            temp_ans[current_ans_index] = '%';
                            current_ans_index += 1;
                            current_format_index += 1;
                            _format++;
                    }
                    break;

            }
        }

        if (*_format != '%') {
            temp_ans[current_ans_index] = *_format;
            current_ans_index += 1;
            current_format_index += 1;
            _format++;
        }
    }
    *formatted_string = temp_ans;
    return OK;
}

int overprintf(char *restrict _format, ...) {
    int err;
    char *s_ans, *s_ans_cpy;
    va_list valist;
    va_start(valist, _format);
    err = __format_string(&s_ans, _format, valist);
    if (err) {
        return err;
    }

    s_ans_cpy = s_ans;
    while (*s_ans_cpy) {
        fputc(*s_ans_cpy++, stdout);
    }

    free(s_ans);
    va_end(valist);
    return err;
}

int overfprintf(FILE *restrict stream, char *restrict _format, ...) {
    int err;
    char *s_ans, *s_ans_cpy;
    va_list valist;
    va_start(valist, _format);
    err = __format_string(&s_ans, _format, valist);
    if (err) {
        return err;
    }
    
    s_ans_cpy = s_ans;
    while (*s_ans_cpy) {
        fputc(*s_ans_cpy++, stream);
    }

    free(s_ans);
    va_end(valist);
    return err;
}

int oversprintf(char *restrict s, char *restrict _format, ...) {
    int err;
    char *s_ans;
    va_list valist;
    va_start(valist, _format);
    err = __format_string(&s_ans, _format, valist);
    if (err) {
        return err;
    }

    strcpy(s, s_ans);
    
    free(s_ans);
    va_end(valist);
    return err;
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

    return OK;
}

int find_zykendorffs_representation(unsigned int num, unsigned int const *row, size_t numbers_in_row_count, unsigned int **ans, size_t *numbers_in_ans_count) {
    int i, err;

    if (row == NULL || ans == NULL || numbers_in_ans_count == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    *ans = (unsigned int*)malloc(numbers_in_row_count * sizeof(unsigned int));
    if (*ans == NULL) {
        return MEMORY_ALLOCATE_ERROR;
    }

    *numbers_in_ans_count = 0;
    for (i = numbers_in_row_count - 1; i >= 0 && num > 0; --i) {
        if (row[i] <= num) {
            num -= row[i];
            (*ans)[(*numbers_in_ans_count)++] = row[i]; 
            i--;
        }
    }
    err = rerealloc((void**)ans, *numbers_in_ans_count);
    if (err) {
        return err;
    }

    return OK;
}

int int_to_roman(int num, char **ans) {
    int err;
    size_t len = 0;
    char *temp = NULL;
    int i = 0;
    struct {
        int value;
        const char *symbol;
    } roman_numerals[] = {
        {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"},
        {100, "C"}, {90, "XC"}, {50, "L"}, {40, "XL"},
        {10, "X"}, {9, "IX"}, {5, "V"}, {4, "IV"},
        {1, "I"}
    };

    if (ans == NULL) {
        return DEREFERENCING_NULL_PTR;
    }
    
    while (num > 0) {
        while (num >= roman_numerals[i].value) {
            err = rerealloc((void**)&temp, (len + strlen(roman_numerals[i].symbol)) * sizeof(char));
            if (err) {
                return err;
            }
            memcpy(temp + len, roman_numerals[i].symbol, strlen(roman_numerals[i].symbol));
            len += strlen(roman_numerals[i].symbol);
            num -= roman_numerals[i].value;
        }
        err = rerealloc((void**)&temp, (len + 1) * sizeof(char));
            if (err) {
                return err;
            }
        temp[len] = 0;
        i++;

    }
    *ans = temp;
    return OK;
}