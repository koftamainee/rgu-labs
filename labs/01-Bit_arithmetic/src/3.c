#include <stdio.h>

#include "../../../libs/input.h"

int set_bit(int n, int k);

int program_01_3() {
  int n, k;
  printf("Input a number: ");
  read_number(&n);
  printf("Input a bit number: ");
  read_number(&k);
  printf("New number: %d\n", set_bit(n, k));
  return 0;
}

int set_bit(int n, int k) { return n | (1 << k); }