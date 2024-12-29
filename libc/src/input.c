#include "../input.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../errors.h"

void clear_screen() {
#ifdef WIN32
    system("cls");
#else
    system("clear");
#endif
}

int read_str(char **str) {
    char buf[BUFSIZ];
    int len;
    int c;

    while ((c = getchar()) != '\n' && c != EOF);
    printf("Input string: ");
    // fgets(buf, BUFSIZ, stdin);
    scanf("%s", buf);
    len = strlen(buf);
    *str = (char *)malloc((len + 1) * sizeof(char));
    if (!*str) {
        return MEMORY_ALLOCATION_ERROR;
    }

    strcpy(*str, buf);
    return 0;
}

void read_number(int *number) {
    int c;
    while (1) {
        printf("Input int number: ");
        if (scanf("%d", number) != 1) {
            clear_screen();
            printf("Input Error. Try again \n");
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        break;
    }
}

int read_binary_number(int *number) {
    int c;
    while (1) {
        printf("Input binary number: ");
        if (scanf("%b", number) != 1) {
            clear_screen();
            printf("Input Error. Try again \n");
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        break;
    }
    return 0;
}

int read_array_from_file(FILE *fptr, int *array) {
    int i = 0;
    while (fscanf(fptr, "%d", &array[i]) != EOF) {
        ++i;
    }
    return 0;
}

err_t read_string_from_user(String *str) {
    size_t len = 0;
    char buf[BUFSIZ];

    if (str == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    if (!fgets(buf, BUFSIZ, stdin)) {
        return ERROR_READING_FROM_STDIN;
    }
    len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0';  // delete '\n' symbol
    }

    *str = string_from(buf);  // allocating memory for string
    if (*str == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }
    return EXIT_SUCCESS;
}
