#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include <stdio.h>
#include <stdlib.h>

int read_str(char** str) {
    int length, c;
    while (1) {
        printf("Input str len: ");
        if (scanf("%d", &length) != 1) {
            printf("Input Error. Try again \n");
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        break;
    }
    
    *str = (char*)malloc(sizeof(char) * (length + 1));
    if (*str == NULL) {
        return 1;
    }
    
    printf("Input %d symbol str: ", length);
    scanf("%s", *str);
    return 0;
}


int read_number(int* number) {
    int c;
    while (1) {
        printf("Input int number: ");
        if (scanf("%d", number) != 1) {
            printf("Input Error. Try again \n");
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        break;
    }
    return 0;
}

int read_binary_number(int* number) {
    int c;
    while (1) {
        printf("Input binary number: ");
        if (scanf("%b", number) != 1) {
            printf("Input Error. Try again \n");
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        break;
    }
    return 0;
}


int read_array_from_file(FILE* fptr, int* array) {
    int i = 0;
    while (fscanf(fptr, "%d", &array[i]) != EOF) {
        ++i;
    }
    return 0;
}