#include <stdio.h>

#include "labs.h"


void lab_00();
void lab_01();

int choose_lab();
void clear_screen();

int main() {
    clear_screen();
    void (*labs[])() = {
        lab_00, lab_01
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
    char labs[] = "1. 00 - Programming\n2. 01 - Bit arithmetic\n0. Exit\n\nEnter int number: ";
    printf("Hello. Choose lab:\n");
    while (1) {
        printf("%s", labs);
    if (scanf("%d", &lab) != 1) {
        while ((c = getchar()) != '\n' && c != EOF);
        clear_screen();
        printf("Input error. Try again.\n");
        continue;
    }
    if (lab > -1 && lab < 3) {
        return lab;
    }
    clear_screen();
    printf("Invalid lab. Try again.\n");
    }
}


void clear_screen() {
    printf("\e[1;1H\e[2J");
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
    char tasks[] = "1. Bubble sort array.\n2. Reverse a string.\n3. Swap two integers.\n4. Find max/min in array.\n5. Binary search array.\n6. Find matrix determinant.\n7. Copy odd-indexed elements.\n8. Copy even numbers.\n9. Remove duplicates from array.\n10. Multiply matrices.\n11. Count string length.\n12. Find substring in string.\n13. Uppercase odd-indexed chars.\n14. Read array from file, find max/min.\n15. Reorder string: digits, letters, symbols.\n16. Remove spaces from string.\n17. Concatenate strings.\n18. Transpose matrix.\n19. Find inverse matrix.\n20. Convert int to string (base 2-36).\n21. Convert string (base 2-36) to int.\n0. Back\n\nInput int number: ";


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


void lab_01() {
    int (*programs[])() = {
        program_01_1, program_01_2, program_01_3, program_01_4, program_01_5, 
        program_01_6, program_01_7, program_01_8, program_01_9,program_01_10, 
        program_01_11_a, program_01_11_b, program_01_12
    };
    char tasks[] = "1. Output least significant bit.\n2. Output k-th bit of a.\n3. Set k-th bit of a.\n4. Clear k-th bit of a.\n5. Swap i-th and j-th bits of a.\n6. Clear least k bits.\n7. Swap bytes in 32-bit integer.\n8. Find max power of 2 divisor.\n9. XOR all bits together.\n10. Circular shift macros left/right.\n11. Concatenate first and last i bits.\n12. Extract bits between first and last i bits.\n13. XOR-based file encryption/decryption.\n0. Back\n\nInput int number: ";


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
