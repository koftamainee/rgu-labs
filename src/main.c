#include <stdio.h>
#include <stdlib.h>

#include "labs.h"
#include "../libs/input.h"


void lab_00();
void lab_01();
void lab_03();

void lab_02();



int choose_lab();

int main() {
    clear_screen();
    void (*labs[])() = {
        lab_00, lab_01, lab_02, lab_03
    };

    int lab_number;

    while (1) {
        lab_number = choose_lab();
        if (lab_number == 0) {
            break;
        }
        labs[lab_number - 1]();
    }
    printf("Have a good day.\n");
    return 0;
}


int choose_lab() {
    int lab, c;

    #ifdef WIN32
    char labs[] = "\
1. 00 - Programming\n\
2. 01 - Bit arithmetic\n\
3. 02 - Bash lab -- unaviable on Windows\n\
4. 03 - Programming\n\
0. Exit\n\n\
Enter int number: ";

    printf("Hello. Choose lab:\n");
    while (1) {
        printf("%s", labs);
    if (scanf("%d", &lab) != 1) {
        while ((c = getchar()) != '\n' && c != EOF);
        clear_screen();
        printf("Input error. Try again.\n");
        continue;
    }
    if (lab > -1 && lab < 5 && lab != 3) {
        return lab;
    }
    clear_screen();
    printf("Invalid lab. Try again.\n");
    }

    #else

    char labs[] = "\
1. 00 - Programming\n\
2. 01 - Bit arithmetic\n\
3. 02 - Bash lab\n\
4. 03 - Programming\n\
0. Exit\n\n\
Enter int number: ";

    printf("Hello. Choose lab:\n");
    while (1) {
        printf("%s", labs);
    if (scanf("%d", &lab) != 1) {
        while ((c = getchar()) != '\n' && c != EOF);
        clear_screen();
        printf("Input error. Try again.\n");
        continue;
    }
    if (lab > -1 && lab < 5) {
        return lab;
    }
    clear_screen();
    printf("Invalid lab. Try again.\n");
    }
    #endif
}


void lab_00() {

    int (*programs[])() = {
        program_00_1, program_00_2, program_00_3, program_00_4,
        program_00_5, program_00_6, program_00_7, program_00_8,
        program_00_9,program_00_10, program_00_11, program_00_12,
        program_00_13, program_00_14, program_00_15, program_00_16,
        program_00_17, program_00_18, program_00_19, program_00_20,
        program_00_21
    };

    char tasks[] = "\
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
        printf("Choose task:\n");
        printf("%s", tasks);

        if (scanf("%d", &task) != 1) {
        while ((c = getchar()) != '\n' && c != EOF);
        clear_screen();
        printf("Input error. Try again.\n");
        continue;
    }
        
        if (task < 22 && task > 0) {
            clear_screen();
            programs[task - 1]();
            printf("\nPress enter to continue...\n");
            while ((c = getchar()) != '\n' && c != EOF);
            getchar();
            clear_screen();
        } else if (task == 0) {
            clear_screen();
            break;
        } else {
            clear_screen();
            printf("Invalid task number. Try again\n");
        }
    }
}


void lab_01() {
    int (*programs[])() = {
        program_01_1, program_01_2, program_01_3, program_01_4, program_01_5, 
        program_01_6, program_01_7, program_01_8, program_01_9,program_01_10, 
        program_01_11_a, program_01_11_b, program_01_12
    };

    char tasks[] = "\
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
        printf("Choose task:\n");
        printf("%s", tasks);

        if (scanf("%d", &task) != 1) {
        while ((c = getchar()) != '\n' && c != EOF);
        clear_screen();
        printf("Input error. Try again.\n");
        continue;
    }
        
        if (task < 14 && task > 0) {
            clear_screen();
            programs[task - 1]();
            printf("\nPress enter to continue...\n");
            while ((c = getchar()) != '\n' && c != EOF);
            c = getchar();
            clear_screen();
        } else if (task == 0) {
            clear_screen();
            break;
        } else {
            clear_screen();
            printf("Invalid task number. Try again\n");
        }
    }
}


void lab_02() {

    char tasks[] = "\
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
        printf("Choose task:\n");
        printf("%s", tasks);

        if (scanf("%d", &task) != 1) {
        while ((c = getchar()) != '\n' && c != EOF);
        clear_screen();
        printf("Input error. Try again.\n");
        continue;
    }
        
        if (task < 13 && task > 0) {
            clear_screen();

            snprintf(command, sizeof(command), "%s%d%s", "./scripts/02-Bash_lab/", task, ".sh");
            system(command);
            
            printf("\nPress enter to continue...\n");
            while ((c = getchar()) != '\n' && c != EOF);
            c = getchar();
            clear_screen();
        } else if (task == 0) {
            clear_screen();
            break;
        } else {
            clear_screen();
            printf("Invalid task number. Try again\n");
        }
    }
}


void lab_03() {

    int (*programs[])() = {
        program_03_1, program_03_2, program_03_3, program_03_4,
        program_03_5, program_03_6, program_03_7, program_03_8,
        program_03_9,program_03_10, program_03_11, program_03_12,
        program_03_13, program_03_14, program_03_15, program_03_16,
        program_03_17, program_03_18, program_03_19
    };

    char tasks[] = "TODO";

    int task;
    char c;
    clear_screen();
    while (1) {
        printf("Choose task:\n");
        printf("%s", tasks);

        if (scanf("%d", &task) != 1) {
        while ((c = getchar()) != '\n' && c != EOF);
        clear_screen();
        printf("Input error. Try again.\n");
        continue;
    }
        
        if (task < 20 && task > 0) {
            clear_screen();
            programs[task - 1]();
            printf("\nPress enter to continue...\n");
            while ((c = getchar()) != '\n' && c != EOF);
            c = getchar();
            clear_screen();
        } else if (task == 0) {
            clear_screen();
            break;
        } else {
            clear_screen();
            printf("Invalid task number. Try again\n");
        }
    }
}