#include <stdio.h>

#define MAX_FILENAME_LENGTH (255)


int choose_mode();
int get_files(FILE** fptr, FILE** target_fptr);
int encrypt_file(FILE* fptr, FILE* target_fptr);


int program_01_12() {
    int err;
    FILE* fptr;
    FILE* target_fptr;
    printf("XOR encryptor\n");
    if ((err = get_files(&fptr, &target_fptr)) != 0) {
        return err;
    }
    encrypt_file(fptr, target_fptr);

    printf("Done. Have a good day\n");

    fclose(fptr);
    fclose(target_fptr);
    return 0;
}


int get_files(FILE** fptr, FILE** target_fptr) {
    char filename[MAX_FILENAME_LENGTH];
    char target_filename[MAX_FILENAME_LENGTH];
        printf("Greetings. Enter source file name[255]: ");
        scanf("%s", filename);
        *fptr = fopen(filename, "r");
        if (*fptr == NULL) {
            printf("Fatal error. \"%s\" not found in directory. Exit code: -1\n", filename);
            return -1;
        }
        printf("Enter target file name[255]: ");
        scanf("%s", target_filename);
        *target_fptr = fopen(target_filename, "w");

    return 0;
}


int encrypt_file(FILE* fptr, FILE* target_fptr) {
    char key, c, nc;
    printf("Enter encryption key[1]: ");
    scanf(" %c", &key);
    while ((c = fgetc(fptr)) != EOF) {
        nc = c ^ key;
        fputc(nc, target_fptr);
    }
    return 0;
}
