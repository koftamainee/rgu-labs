#include "../floating.h"

#include <math.h>

int is_float_equal(double a, double b, double EPS) {
  if (fabs(a - b) < EPS) {
    return 1;
  } else {
    return 0;
  }
}
