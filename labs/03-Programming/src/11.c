#include <stdio.h>

int tokenize(char *inital, int (*detector)(int), int accept_empty_lexems,
             char ***lexems, size_t *lexems_count);

int program_03_11(int argc, char *argv[]) { return 0; }