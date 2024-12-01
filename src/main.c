#include <stdio.h>
#include <stdlib.h>

#include "../libs/errors.h"
#include "../libs/input.h"
#include "labs.h"

int lab_00(int argc, char *argv[]);
int lab_01(int argc, char *argv[]);
int lab_03(int argc, char *argv[]);
int lab_04(int argc, char *argv[]);

int lab_02();

int choose_lab();

int main(int argc, char *argv[]) {
    int err, c;
    clear_screen();
    int (*labs[])(int argc, char *argv[]) = {lab_00, lab_01, lab_02, lab_03,
                                             lab_04};

    int lab_number;

    while (1) {
        lab_number = choose_lab();
        if (lab_number == 0) {
            break;
        }
        err = labs[lab_number - 1](argc, argv);
        if (err) {
            clear_screen();
            printf("Process killed ┌( ͝° ͜ʖ͡°)=ε/̵͇̿̿/’̿’̿ ̿  (╥﹏╥)\n");
        }
        switch (err) {
            case OK:
                break;
            case MEMORY_ALLOCATION_ERROR:
                printf("Memory allocation error :(\n");
                break;
            case DEREFERENCING_NULL_PTR:
                printf("Passed ptr is NULL ._.\n");
                break;
            case INDEX_OUT_OF_BOUNDS:
                printf("Index out of bounds :0\n");
                break;
            case OPENING_THE_FILE_ERROR:
                printf("Error while openning a file -_-\n");
                break;
            case INVALID_NUMERIC_BASE:
                printf("Invalid numeric base :P\n");
                break;
            case INVALID_NUM_TO_CHAR_CONVERSION:
                printf("Invalid num to char conversion °_o");
                break;
            case INVALID_CHAR_TO_NUM_CONVERSION:
                printf("Invalid char to num conversion ・‿・\n");
                break;
            case INVALID_FLAG:
                printf("Invalid flag Ö\n");
                break;
            case INVALID_STREAM_PTR:
                printf("Invalid stream ptr 	☠	☠	☠");
                break;
            case INVALID_NUMBER:
                printf("INVALID NUMBER///\n");
                break;
            case INVALID_COMBINATION_SIZE:
                printf("INVALID COMIBNATION SIZE !!!1!11!11!01)01\n");
                break;
            case EQUAL_ELEMENTS_FOUND:
                printf("There is equal elements in collectiosn :(\n");
                break;
            default:
                printf("Undefined behavior ಠ_ಠ\n");
                break;
        }
        if (err) {
            printf("\n→ Press enter to continue...\n");
            getchar();
            while ((c = getchar()) != '\n' && c != EOF);
            clear_screen();
        }
    }
    printf("\n☆  Have a good day ☆\n\n");
    return OK;
}

int choose_lab() {
    int lab, c;

#ifdef WIN32
    char labs[] =
        "\
1. 00 - Programming\n\
2. 01 - Bit arithmetic\n\
3. 02 - Bash lab -- unaviable on Windows\n\
4. 03 - Programming\n\
5. 04 - Programming\n\
0. Exit\n\n\
Enter int number: ";

    printf("→ Hello. Choose lab:\n");
    while (1) {
        printf("%s", labs);
        if (scanf("%d", &lab) != 1) {
            while ((c = getchar()) != '\n' && c != EOF);
            clear_screen();
            printf("→ Input error. Try again.\n");
            continue;
        }
        if (lab > -1 && lab < 6 && lab != 3) {
            return lab;
        }
        clear_screen();
        printf("→ Invalid lab. Try again.\n");
    }

#else

    char labs[] =
        "\
1. 00 - Programming\n\
2. 01 - Bit arithmetic\n\
3. 02 - Bash lab\n\
4. 03 - Programming\n\
5. 04 - Programming\n\
0. Exit\n\n\
Enter int number: ";

    printf("→ Hello. Choose lab:\n");
    while (1) {
        printf("%s", labs);
        if (scanf("%d", &lab) != 1) {
            while ((c = getchar()) != '\n' && c != EOF);
            clear_screen();
            printf("→ Input error. Try again.\n");
            continue;
        }
        if (lab > -1 && lab < 6) {
            return lab;
        }
        clear_screen();
        printf("→ Invalid lab. Try again.\n");
    }
#endif
}

int lab_00(int argc, char *argv[]) {
    int (*programs[])() = {program_00_1,  program_00_2,  program_00_3,
                           program_00_4,  program_00_5,  program_00_6,
                           program_00_7,  program_00_8,  program_00_9,
                           program_00_10, program_00_11, program_00_12,
                           program_00_13, program_00_14, program_00_15,
                           program_00_16, program_00_17, program_00_18,
                           program_00_19, program_00_20, program_00_21};

    char tasks[] =
        "\
1. Bubble sort array.\n\
2. Reverse a string.\n\
3. Swap two integers.\n\
4. Find max/min in array.\n\
5. Binary search array.\n\
6. Find matrix determinant.\n\
7. Copy odd-indexed elements.\n\
8. Copy even numbers.\n\
9. Remove duplicates from array.\n\
10. Multiply matrices.\n\
11. Count string length.\n\
12. Find substring in string.\n\
13. Uppercase odd-indexed chars.\n\
14. Read array from file, find max/min.\n\
15. Reorder string: digits, letters, symbols.\n\
16. Remove spaces from string.\n\
17. Concatenate strings.\n\
18. Transpose matrix.\n\
19. Find inverse matrix.\n\
20. Convert int to string (base 2-36).\n\
21. Convert string (base 2-36) to int.\n\
0. Back\n\n\
Input int number: ";

    int task;
    char c;
    clear_screen();
    while (1) {
        printf("→ Choose task:\n");
        printf("%s", tasks);

        if (scanf("%d", &task) != 1) {
            while ((c = getchar()) != '\n' && c != EOF);
            clear_screen();
            printf("→ Input error. Try again.\n");
            continue;
        }

        if (task < 22 && task > 0) {
            clear_screen();
            programs[task - 1]();
            printf("\n→ Press enter to continue...\n");
            while ((c = getchar()) != '\n' && c != EOF);
            getchar();
            clear_screen();
        } else if (task == 0) {
            clear_screen();
            break;
        } else {
            clear_screen();
            printf("→ Invalid task number. Try again\n");
        }
    }
    return OK;
}

int lab_01(int argc, char *argv[]) {
    int (*programs[])() = {program_01_1,  program_01_2,    program_01_3,
                           program_01_4,  program_01_5,    program_01_6,
                           program_01_7,  program_01_8,    program_01_9,
                           program_01_10, program_01_11_a, program_01_11_b,
                           program_01_12};

    char tasks[] =
        "\
1. Output least significant bit.\n\
2. Output k-th bit of a.\n\
3. Set k-th bit of a.\n\
4. Clear k-th bit of a.\n\
5. Swap i-th and j-th bits of a.\n\
6. Clear least k bits.\n\
7. Swap bytes in 32-bit integer.\n\
8. Find max power of 2 divisor.\n\
9. XOR all bits together.\n\
10. Circular shift macros left/right.\n\
11. Concatenate first and last i bits.\n\
12. Extract bits between first and last i bits.\n\
13. XOR-based file encryption/decryption.\n\
0. Back\n\n\
Input int number: ";

    int task;
    char c;
    clear_screen();
    while (1) {
        printf("→ Choose task:\n");
        printf("%s", tasks);

        if (scanf("%d", &task) != 1) {
            while ((c = getchar()) != '\n' && c != EOF);
            clear_screen();
            printf("→ Input error. Try again.\n");
            continue;
        }

        if (task < 14 && task > 0) {
            clear_screen();
            programs[task - 1]();
            printf("\n→ Press enter to continue...\n");
            while ((c = getchar()) != '\n' && c != EOF);
            c = getchar();
            clear_screen();
        } else if (task == 0) {
            clear_screen();
            break;
        } else {
            clear_screen();
            printf("→ Invalid task number. Try again\n");
        }
    }
    return OK;
}

int lab_02() {
    char tasks[] =
        "\
1. Create a table of powers (0-6) for numbers 1 to 9.\n\
2. Count lines in files passed as script parameters.\n\
3. Count files in a directory, including subdirectories; default is current directory.\n\
4. Find directories containing a file, starting from the specified directory.\n\
5. List files in directories with more words than a given count.\n\
6. Create hard links with extensions (.1-.9), remove links with \"-r\" flag.\n\
7. Recursively create hard/symbolic links for files containing a string.\n\
8. List subdirectories sorted by file sizes.\n\
9. Delete files from a list in a specified file.\n\
10. Collect interactive user input to update a settings file for each file in a directory.\n\
11. Modify file permissions and ownership based on a settings file or default values.\n\
12. Implement a killall command alternative to terminate processes by name with signal options.\n\
0. Back\n\n\
Input int number: ";

    int task;
    char c;
    char command[50];
    clear_screen();
    while (1) {
        printf("→ Choose task:\n");
        printf("%s", tasks);

        if (scanf("%d", &task) != 1) {
            while ((c = getchar()) != '\n' && c != EOF);
            clear_screen();
            printf("→ Input error. Try again.\n");
            continue;
        }

        if (task < 13 && task > 0) {
            clear_screen();

            snprintf(command, sizeof(command), "%s%d%s",
                     "bash ./scripts/02-Bash_lab/", task, ".sh");
            system(command);

            printf("\n→ Press enter to continue...\n");
            while ((c = getchar()) != '\n' && c != EOF);
            c = getchar();
            clear_screen();
        } else if (task == 0) {
            clear_screen();
            break;
        } else {
            clear_screen();
            printf("→ Invalid task number. Try again\n");
        }
    }
    return OK;
}

int lab_03(int argc, char *argv[]) {
    int err;
    int (*programs[])(int argc, char *argv[]) = {
        program_03_1,  program_03_2,  program_03_3,  program_03_4,
        program_03_5,  program_03_6,  program_03_7,  program_03_8,
        program_03_9,  program_03_10, program_03_11, program_03_12,
        program_03_13, program_03_14, program_03_15, program_03_16,
        program_03_17, program_03_18, program_03_19};

    char tasks[] =
        "1. Gorner scheme with 2^r base.\
1. Gorner scheme with 2^r base.\n\
2. Transform string with argc and argv.\n\
3. Vilka prototype.\n\
4. Strstr in files.\n\
5. Is polygon convex.\n\
6. Value of polynomal in x.\n\
7. Find Kaprekar numbers.\n\
8. Sum numbers in base (v stolbik ._.).\n\
9. Custom printf-like functions implementation.\n\
10. Custom scanf-like functions implementation.\n\
11. Select lexemes in a string by predicate.\n\
12. Find strstr in several strings.\n\
13. Find all possible unique decompositions of natural number.\n\
14. Find longest sawtooth subsequence.\n\
15. Find all saddle points in matrix.\n\
16. Find all possible permutations of array.\n\
17. Find the set of all unique subsets for elements of array.\n\
18. Find all possible unique combinations of k elements for elements of array.\n\
19. Find all possible configurations of cube towers.\n\
0. Back\n\n\
Input int number: ";

    int task;
    char c;
    clear_screen();
    while (1) {
        printf("→ Choose task:\n");
        printf("%s", tasks);

        if (scanf("%d", &task) != 1) {
            while ((c = getchar()) != '\n' && c != EOF);
            clear_screen();
            printf("→ Input error. Try again.\n");
            continue;
        }

        if (task < 20 && task > 0) {
            clear_screen();
            err = programs[task - 1](argc, argv);
            if (err) {
                return err;
            }
            printf("\n→ Press enter to continue...\n");
            while ((c = getchar()) != '\n' && c != EOF);
            c = getchar();
            clear_screen();
        } else if (task == 0) {
            clear_screen();
            break;
        } else {
            clear_screen();
            printf("→ Invalid task number. Try again\n");
        }
    }
    return OK;
}

int lab_04(int argc, char *argv[]) {
    int err;
    int (*programs[])(int argc, char *argv[]) = {
        program_04_1, program_04_2, program_04_3, program_04_4, program_04_5,
        program_04_6, program_04_7, program_04_8, program_04_9, program_04_10,
    };

    char tasks[] = "TODO\n\n";

    int task;
    char c;
    clear_screen();
    while (1) {
        printf("→ Choose task:\n");
        printf("%s", tasks);

        if (scanf("%d", &task) != 1) {
            while ((c = getchar()) != '\n' && c != EOF);
            clear_screen();
            printf("→ Input error. Try again.\n");
            continue;
        }

        if (task < 11 && task > 0) {
            clear_screen();
            err = programs[task - 1](argc, argv);
            if (err) {
                return err;
            }
            printf("\n→ Press enter to continue...\n");
            while ((c = getchar()) != '\n' && c != EOF);
            c = getchar();
            clear_screen();
        } else if (task == 0) {
            clear_screen();
            break;
        } else {
            clear_screen();
            printf("→ Invalid task number. Try again\n");
        }
    }
    return OK;
}
