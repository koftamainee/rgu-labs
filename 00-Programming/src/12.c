#include <stdlib.h>
#include <stdio.h>

#include "../../libs/input.h"

#define STR_LENGTH 10
#define STRSTR_length 4


char* custom_strstr(char* main_str, char* str);


int program_00_12() {
    char* str1;
    char* str2;
    printf("First string...\n");
    read_str(&str1);
    printf("Second string...\n");
    read_str(&str2);

    char* p = custom_strstr(str1, str2);
    printf("%s", p);

    free(str1);
    free(str2);
    return 0;
}


 
char* custom_strstr(char* main_str, char* str) {
    char* ptr;
    /*
    //
    // TODO: implement strstr
    //
    */    
    return ptr;
}