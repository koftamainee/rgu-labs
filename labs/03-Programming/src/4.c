/*-------------------- This is EXTREMLY outdated --------------------*/
/*--------- PLS implement it with new cvector or int_vector ---------*/
/*
//
//
*/

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../../libc/errors.h"

/*------------------------------- old cvector ------------------------------- */
#define ____INITIAL_CAPACITY 16
#define ____GROWTH_FACTOR 2

typedef struct {
    size_t size;
    size_t capacity;
    int *data;
} ____Vector;

int ____memory_allocate(____Vector *vec);

int ____cvector_init(____Vector *vec);
int ____cvector_push_back(____Vector *vec, int elem);
void ____cvector_free(____Vector *vec);
int ____cvector_pop(____Vector *vec, int index);
int ____cvector_pop_front(____Vector *vec);
int ____cvector_pop_back(____Vector *vec);
void ____cvector_print(____Vector *vec);
int ____cvector_insert(____Vector *vec, size_t index, int value);
/*----------------------------------------------------------------------------*/
int ____cvector_init(____Vector *vec) {
    vec->size = 0;
    vec->capacity = ____INITIAL_CAPACITY;
    vec->data = (int *)malloc(vec->capacity * sizeof(int));
    if (vec->data == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }
    return 0;
}

int ____memory_allocate(____Vector *vec) {
    int *for_realloc;
    for_realloc = (int *)realloc(vec->data, vec->capacity * sizeof(int));
    if (for_realloc == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }
    vec->data = for_realloc;
    return 0;
}

int ____cvector_push_back(____Vector *vec, int elem) {
    if (vec->size >= vec->capacity) {
        vec->capacity *= ____GROWTH_FACTOR;
        if (____memory_allocate(vec) == 1) {
            return MEMORY_ALLOCATION_ERROR;
        }
    }
    vec->data[vec->size] = elem;
    vec->size++;
    return 0;
}

int ____cvector_pop(____Vector *vec, int index) {
    if (index < 0 || index >= vec->size) {
        return INDEX_OUT_OF_BOUNDS;
    }

    for (int i = index; i < vec->size - 1; i++) {
        vec->data[i] = vec->data[i + 1];
    }

    vec->size--;

    if (vec->size > 0 && vec->size <= vec->capacity / (____GROWTH_FACTOR * 2)) {
        vec->capacity /= ____GROWTH_FACTOR;
        ____memory_allocate(vec);
    }

    return 0;
}

void ____cvector_free(____Vector *vec) {
    free(vec->data);
    vec->data = NULL;
}

void ____cvector_print(____Vector *vec) {
    int i;
    for (i = 0; i < vec->size; ++i) {
        printf("%d ", vec->data[i]);
    }
    printf("\n");
}

int ____cvector_pop_front(____Vector *vec) { return ____cvector_pop(vec, 0); }

int ____cvector_pop_back(____Vector *vec) {
    return ____cvector_pop(vec, vec->size - 1);
}

int ____cvector_insert(____Vector *vec, size_t index, int value) {
    if (index > vec->size) {
        return INDEX_OUT_OF_BOUNDS;
    }

    if (vec->size >= vec->capacity) {
        vec->capacity *= 2;
        int *new_data = realloc(vec->data, vec->capacity * sizeof(int));
        if (new_data == NULL) {
            return MEMORY_ALLOCATION_ERROR;
        }
        vec->data = new_data;
    }

    for (size_t i = vec->size; i > index; i--) {
        vec->data[i] = vec->data[i - 1];
    }

    vec->data[index] = value;
    vec->size++;

    return 0;
}
/*----------------------------------------------------------------------------*/
/*
//
//
//
//
//
//
//
*/

int fstrstr(const char *str, ...);
int _strstr_inner(const char *substr, char const *str, int *index);
void remove_newline(char *buf);

int program_03_4(int argc, char *argv[]) {
    int err, i;
    ____Vector vec;
    char substr[] = "a";
    err = fstrstr(substr, &vec, "files/1.txt", "files/2.txt", NULL);
    switch (err) {
        case DEREFERENCING_NULL_PTR:
            printf("Dereferencing NULL ptr\n");
            break;
        case MEMORY_ALLOCATION_ERROR:
            printf("Memory allocate error\n");
            break;
        case OPENING_THE_FILE_ERROR:
            printf("Opening file error\n");
            break;
    }

    if (err) {
        ____cvector_free(&vec);
        return err;
    }

    if (vec.size == 0) {
        printf("There is no substr in files :-(\n");
    }

    for (i = 2; i < vec.size; i += 3) {
        printf("FILE %d contains \"%s\" substring at line %d, index %d\n",
               vec.data[i - 2], substr, vec.data[i - 1], vec.data[i]);
    }
    printf("\n");

    ____cvector_free(&vec);
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
    ____Vector *vec;
    int index, err, string_number, file_index;
    char buf[BUFSIZ];
    char *file_path = NULL;
    FILE *fptr = NULL;
    va_list valist;

    if (str == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    va_start(valist, str);
    vec = va_arg(valist, ____Vector *);

    if (vec == NULL) {
        va_end(valist);
        return DEREFERENCING_NULL_PTR;
    }

    ____cvector_init(vec);
    file_index = 0;

    file_path = va_arg(valist, char *);
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

                if ((err = ____cvector_push_back(vec, file_index)) ||
                    (err = ____cvector_push_back(vec, string_number)) ||
                    (err = ____cvector_push_back(vec, i - 1))) {
                    fclose(fptr);
                    va_end(valist);
                    return err;
                }
            }
            string_number++;
        }
        fclose(fptr);
        file_path = va_arg(valist, char *);
    }
    va_end(valist);

    return 0;
}
