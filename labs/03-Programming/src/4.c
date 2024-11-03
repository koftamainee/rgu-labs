#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "../../../libs/cvector.h"
#include "../../../libs/errors.h"

int fstrstr(const char *str, ...);
int _strstr_inner(const char *substr, char const *str, int *index);
void remove_newline(char *buf);

int program_03_4() {
    int err, i;
    Vector vec;
    char substr[] = "a";
    err = fstrstr(substr, &vec, "files/1.txt", "files/2.txt", NULL);
    switch (err) {
        case DEREFERENCING_NULL_PTR: printf("Dereferencing NULL ptr\n"); break;
        case MEMORY_ALLOCATE_ERROR: printf("Memory allocate error\n"); break;
        case OPENING_THE_FILE_ERROR: printf("Opening file error\n"); break;
    }

    if (err) {
        cvector_free(&vec);
        return err;
    }

    if (vec.size == 0) {
        printf("There is no substr in files :-(\n");
    }

    for (i = 2; i < vec.size; i += 3) {
        printf("FILE %d contains \"%s\" substring at line %d, index %d\n", vec.data[i - 2], substr, vec.data[i - 1], vec.data[i]);
    }
    printf("\n");

    cvector_free(&vec);
    return 0;
}

int _strstr_inner(char const *substr, char const *str, int *index) {
    int i;
    if (substr == NULL || str == NULL || index == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    for (i = 0; str[i]; ++i) {
        const char *temp = str + i;
        const char *n = substr;

        while (*temp && *temp == *n) {
            temp++;
            n++;
        }

        if (!*n) {
            *index = i;
            return 0;
        }
    }
    *index = -1;
    return 0;
}

int fstrstr(char const *str, ...) {
    Vector *vec;
    int index, err, string_number, file_index;
    char buf[BUFSIZ];
    char *file_path = NULL;
    FILE *fptr = NULL;
    va_list valist;

    if (str == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    va_start(valist, str);
    vec = va_arg(valist, Vector*);
    
    if (vec == NULL) {
        va_end(valist);
        return DEREFERENCING_NULL_PTR;
    }

    cvector_init(vec);
    file_index = 0;

    file_path = va_arg(valist, char*);
    while (file_path != NULL) {
        file_index++;
        fptr = fopen(file_path, "r");
        if (fptr == NULL) {
            va_end(valist);
            return OPENING_THE_FILE_ERROR;
        }

        string_number = 1;
        while (fgets(buf, BUFSIZ, fptr)) {
            // remove_newline(buf);
            int i = 0;
            while (buf[i] != '\0') {
                err = _strstr_inner(str, buf + i, &index);
                if (err) {
                    fclose(fptr);
                    va_end(valist);
                    return err;
                }

                if (index == -1) break;

                i += index + 1;

                if ((err = cvector_push_back(vec, file_index)) ||
                    (err = cvector_push_back(vec, string_number)) ||
                    (err = cvector_push_back(vec, i - 1))) {
                    fclose(fptr);
                    va_end(valist);
                    return err;
                }
            }
            string_number++;
        }
        fclose(fptr);
        file_path = va_arg(valist, char*);
    }
    va_end(valist);

    return 0;
}
