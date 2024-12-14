#include "../custom_math.h"

#include <math.h>

#include "../errors.h"
#include "../memory.h"

double log_base(double x, double base) { return log(x) / log(base); }

int generate_fibonacci_row(unsigned int until, unsigned int **ans,
                           size_t *numbers_in_ans_count) {
    int err, size, capacity;
    size_t i;
    unsigned int fib_1 = 1, fib_2 = 1, fib = 0;
    if (ans == NULL || numbers_in_ans_count == NULL) {
        return DEREFERENCING_NULL_PTR;
    }
    capacity = 16;
    size = 0;
    *ans = (unsigned int *)malloc(capacity * sizeof(unsigned int));
    (*ans)[0] = fib_1;
    (*ans)[1] = fib_2;
    size += 2;
    fib = fib_1 + fib_2;
    while (fib <= until) {
        if (size >= capacity) {
            err = rerealloc((void **)ans, capacity * 2 * sizeof(unsigned int));
            if (err) {
                return err;
            }
            capacity *= 2;
        }

        fib_1 = fib_2;
        fib_2 = fib;
        fib = fib_1 + fib_2;
        (*ans)[size] = fib;
        size++;
    }
    *numbers_in_ans_count = size;
    return OK;
}

int generate_fibonacci_row_upto_n(int num_elements, int *fib_sequence,
                                  int *element_count) {
    int first = 0, second = 1;
    *element_count = 0;

    if (num_elements >= 1) {
        fib_sequence[(*element_count)++] = first;
    }
    if (num_elements >= 2) {
        fib_sequence[(*element_count)++] = second;
    }

    for (int index = 2; index < num_elements; index++) {
        int next_value = fib_sequence[index - 1] + fib_sequence[index - 2];
        fib_sequence[(*element_count)++] = next_value;
    }

    return OK;
}

double factorial(double x) {
    double fact = 1;
    if (x < 0) {
        return -1;
    }
    if (x == 0 || x == 1) {
        return 1;
    }
    for (int i = 1; i <= x; ++i) {
        fact *= i;
    }
    return fact;
}
