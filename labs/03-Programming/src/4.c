#include <stdlib.h>
#include <stdio.h>

#include "../../../libs/memory.h"

int fstrstr(char const *str, ...);

int program_03_4() {
    int err;
    int* arr = (int*)malloc(15);
    err = rerealloc((void*)&arr, 123875715376125);
    if (err != 0) {
        free(arr);
        printf("Memory allocation error\n");
        return err;
    }

    printf("%p\n", arr);
    return 0;
}