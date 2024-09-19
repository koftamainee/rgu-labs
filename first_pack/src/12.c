#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define STR_LENGTH 10
#define STRSTR_length 4


char* custom_strstr(char* main_str, char* str);


int main() {
    char* str1 = (char*)malloc(sizeof(char) * STR_LENGTH);
    char* str2 = (char*)malloc(sizeof(char) * STRSTR_length);
    strcpy(str1, "012345678");
    strcpy(str2, "567");
    
    

    free(str1);
    free(str2);
    return 0;
}


// Sorry 
char* custom_strstr(char* main_str, char* str) {
    char* c_ptr = main_str;
    char* s_ptr = str;
    in

    int cnt = 0;

    while (1) {
        while (c_ptr != s_ptr) {
            ++c_ptr;
        if (strcmp(main_str, str))
    }
}