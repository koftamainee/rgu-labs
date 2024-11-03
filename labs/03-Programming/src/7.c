#include <stdio.h>
#include <stdlib.h>

#include "../../../libs/types.h"
#include "../../../libs/errors.h"
#include "../../../libs/cvector.h"

int find_kaprekar_numbers(Vector *ans, int *answers_count, int numbers_count, int base, ...);
int is_kaprekar(const int *ans, const char *number, int base);

int program_03_7() {
    int err, answers_count;
    Vector vec;

    find_kaprekar_numbers(&vec, &answers_count, 4, 16, "AA43687F", "FFABC3", "52", "9");
    return 0;
}

int find_kaprekar_numbers(Vector *ans, int *answers_count, int numbers_count, int base, ...) {
    return 0;
}