#include <stdio.h>

#include "../../../libs/bits.h"
#include "../../../libs/input.h"

int get_between(int num, int i, int len);

int program_01_11_b() {
  int i, len, num;
  printf("Enter number. ");
  read_number(&num);
  printf("%d == %b\n", num, num);
  printf("Enter i. ");
  read_number(&i);
  printf("Enter len. ");
  read_number(&len);
  printf("%b\n", get_between(num, i, len));

  return 0;
}

int get_between(int num, int i, int len) {
  int ones = num_to_ones(i);
  ones = ~(ones | (ones << (len - i)));
  return (ones & num) >> i;
}