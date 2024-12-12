#include <stdlib.h>

#include "../../../libc/cstring.h"

typedef enum { Male, Female } Gender;

typedef struct Citizen {
    String last_name;
    String name;
    String lastname;
    Gender gender;
    double salary;
} Citizen;

int program_04_5(int argc, char *argv[]) { return EXIT_SUCCESS; }
