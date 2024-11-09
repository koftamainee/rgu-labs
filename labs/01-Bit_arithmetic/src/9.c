#include <stdio.h>

#include "../../../libs/input.h"

int xor_all_digits(int n);

int program_01_9() {
  int n;
  read_binary_number(&n);

  int ans = xor_all_digits(n);
  printf("XOR all digits result: %d\n", ans);
  return 0;
}

int xor_all_digits(int n) {
  int ans = n % 2;
  n /= 2;
  while (n > 0) {
    ans = ans ^ (n % 2);
    n /= 2;
  }
  return ans;
}