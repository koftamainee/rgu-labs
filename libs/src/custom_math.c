#include <math.h>

#include "../custom_math.h"
#include "../errors.h"
#include "../memory.h"

double log_base(double x, double base) { return log(x) / log(base); }

int generate_fibonacci_row(unsigned int until, unsigned int **ans,
                           size_t *numbers_in_ans_count) {
  int err, size, capacity;
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
  for (int i = 0; i < size; ++i) {
  }
  *numbers_in_ans_count = size;
  return OK;
}