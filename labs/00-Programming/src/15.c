#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "../../../libs/input.h"


int reorder_string(char **str) {
    char* ptr = *str;
    char* digits;
    char* letters;
    char* others;
    int digits_cnt = 0, letters_cnt = 0, others_cnt = 0;
    int len = strlen(*str);

    // Allocate memory for digits, letters, and others.
    if (!(digits = (char*)malloc((len + 1) * sizeof(char)))) {
        return 1; // Failed to allocate memory
    }
    if (!(letters = (char*)malloc((len + 1) * sizeof(char)))) {
        free(digits);  // Free previously allocated memory
        return 1;
    }
    if (!(others = (char*)malloc((len + 1) * sizeof(char)))) {
        free(digits);  // Free previously allocated memory
        free(letters);
        return 1;
    }

    // Categorize characters.
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

    // Null-terminate the strings.
    digits[digits_cnt] = 0;
    letters[letters_cnt] = 0;
    others[others_cnt] = 0;

    // Reorder the original string.
    strcpy(*str, digits);
    strcat(*str, letters);
    strcat(*str, others);

    // Free allocated memory.
    free(digits);
    free(letters);
    free(others);
    
    return 0;
}

int program_00_15() {
    char* str = NULL;

    if (read_str(&str) != 0) {  // Check if read_str succeeds.
        printf("Error reading string.\n");
        return 1;
    }
    
    if (reorder_string(&str) != 0) {
        printf("Error reordering string.\n");
        free(str);  // Free str even in error case
        return 1;
    }

    printf("ans: %s\n", str);
    free(str);  // Free the string after use
    return 0;
}
