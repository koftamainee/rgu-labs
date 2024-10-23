#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int cstrlen(const char *str, int *ans);
int creverse(const char *str, char **ans);
int ctoupper(const char *str, char **ans);
int creorder_string(const char *str, char **ans);
int cstrcat(const char **strings, char **ans, int seed);

int program_03_2(int argc, char *argv[]) {
    int int_ans;
    char* str_ans;
    char flag = argv[1][1];
    if (argc < 3) {
        printf("Not enough arguments\n");
        return 1;
    } else if (argv[1][0] == '-') {
        switch (flag) {
            case 'l': cstrlen(argv[2], &int_ans);
                printf("len of the string %d", int_ans);
                break;
            // case 'r':
            // case 'u':
            // case 'n':
            // case 'c':
            default: printf("Invalid flag\n");
            return 2;
        }
    }


    return 0;
}

int cstrlen(const char *str, int *ans) {
    if (str == NULL || ans == NULL) {
        return 1;
    }
    
    int len = 0;
    const char* ptr = str;

    while (*ptr++) {
        len++;
    }

    *ans = len;
    return 0;
}

int creverse(const char *str, char **ans) {
    if (str == NULL || ans == NULL) {
        return 1;
    }
    int len, i;
    cstrlen(str, &len);
    *ans = (char*)malloc((len + 1) * sizeof(char));
    if (!*ans) {
        return 2;
    }
    (*ans)[len] = 0;
    for (i = 0; i < len; ++i) {
        (*ans)[len - i - 1] = str[i];
    }
    return 0;
}

int ctoupper(const char *str, char **ans) {
    if (str == NULL || ans == NULL) {
        return 1;
    }
    int len, i;
    cstrlen(str, &len);
    *ans = (char*)malloc((len + 1) * sizeof(char));
    if (!*ans) {
        return 2;
    }
    (*ans)[len] = 0;

    for (i = 0; i < len; ++i) {
        if (i % 2 != 0) {
            (*ans)[i] = toupper(str[i]);
        } else {
            (*ans)[i] = str[i];
        }
    }
    return 0;
}

int creorder_string(const char *str, char **ans) {
    const char* ptr = str;
    char* digits;
    char* letters;
    char* others;
    int digits_cnt = 0, letters_cnt = 0, others_cnt = 0;
    int len;
    cstrlen(str, &len);

    if (!(digits = (char*)malloc((len + 1) * sizeof(char)))) {
        return 1;
    }
    if (!(letters = (char*)malloc((len + 1) * sizeof(char)))) {
        free(digits);
        return 1;
    }
    if (!(others = (char*)malloc((len + 1) * sizeof(char)))) {
        free(digits);
        free(letters);
        return 1;
    }

    while (*ptr) {
        if (isdigit(*ptr)) {
            digits[digits_cnt++] = *ptr;
        } else if (isalpha(*ptr)) {
            letters[letters_cnt++] = *ptr;
        } else {
            others[others_cnt++] = *ptr;
        }
        ptr++;
    }

    digits[digits_cnt] = 0;
    letters[letters_cnt] = 0;
    others[others_cnt] = 0;

    strcpy(*ans, digits);
    strcat(*ans, letters);
    strcat(*ans, others);

    free(digits);
    free(letters);
    free(others);
    
    return 0;
}

int cstrcat(const char **strings, char **ans, int seed) {
    return 0;
}