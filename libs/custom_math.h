#ifndef MATH_H
#define MATH_H

#include <stdlib.h>

double log_base(double x, double base);
double factorial(double x);
int generate_fibonacci_row(unsigned int until, unsigned int **ans,
                           size_t *numbers_in_ans_count);

int generate_fibonacci_row_upto_n(int num_elements, int *fib_sequence,
                                  int *element_count);

#endif
