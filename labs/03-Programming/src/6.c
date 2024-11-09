#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../../../libs/errors.h"

#define POLYNOMIAL_POWER 3

int calculate_polynomial(double *ans, double x, int power, ...);

int program_03_6() {
  double ans;
  int err, i;
  double x;
  double data[POLYNOMIAL_POWER + 1];

  srand((unsigned int)time(NULL));

  printf("Example polynomial: ");
  for (i = 0; i < POLYNOMIAL_POWER + 1; ++i) {
    data[i] = (rand() % 201 - 100) + (((double)(rand() % 1001)) / 1000);
    if (i == POLYNOMIAL_POWER) {
      printf("%.3lf\n", data[i]);
    } else if (i == POLYNOMIAL_POWER - 1) {
      printf("%.3lfx + ", data[i]);
    } else {
      printf("%.3lfx^%d + ", data[i], POLYNOMIAL_POWER - i);
    }
  }

  x = (rand() % 201 - 100) + (((double)(rand() % 1001)) / 1000);
  printf("x = %.3lf\n", x);

  err = calculate_polynomial(&ans, x, POLYNOMIAL_POWER, data[0], data[1],
                             data[2], data[3]);
  switch (err) {
  case OK:
    printf("Value of polynomial at %.3lf is %.3lf\n", x, ans);
    break;
  case DEREFERENCING_NULL_PTR:
    printf("Dereferencing NULL pointer\n");
    break;
  }

  return err;
}

int calculate_polynomial(double *ans, double x, int power, ...) {
  int i;
  double temp = 0;
  va_list valist;

  if (ans == NULL) {
    return DEREFERENCING_NULL_PTR;
  }

  va_start(valist, power);

  for (i = power; i >= 0; --i) {
    temp = temp * x + va_arg(valist, double);
  }
  *ans = temp;

  va_end(valist);
  return OK;
}
