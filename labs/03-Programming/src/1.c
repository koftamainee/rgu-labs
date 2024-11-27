#include <stdio.h>
#include <stdlib.h>

#include "../../../libs/custom_math.h"
#include "../../../libs/errors.h"
#include "../../../libs/input.h"

// custom errors
#define INVALID_BASE -1

int gorner_2p(int src, char **dest, int base);

int program_03_1(int argc, char *argv[]) {
  int num, base, r, err;
  char *ans;
  printf("Input number: ");
  read_number(&num);
  printf("Input r: ");
  read_number(&r);
  base = 1 << r;
  printf("Base: %d\n", base);
  err = gorner_2p(num, &ans, r);

  switch (err) {
  case 0:
    printf("num %d in %d base is: %s\n", num, base, ans);
    break;
  case INVALID_BASE:
    printf("Invalid base.\n");
    return err;
  case MEMORY_ALLOCATION_ERROR:
    printf("Memory allocation error.\n");
    return err;
  }
  free(ans);
  return 0;
}

int gorner_2p(int src, char **dest, int r) {
  int len = 0, is_negative = 0;
  char *ptr;
  int base = 1 << r;

  if (base < 2 || base > 32) {
    return INVALID_BASE;
  }

  len = log_base(abs(src), base) + 1;

  if (src == 0) {
    *dest = (char *)malloc(2 * sizeof(char));
    if (!*dest) {
      return MEMORY_ALLOCATION_ERROR;
    }
    (*dest)[0] = '0';
    (*dest)[1] = '\0';
    return 0;
  }

  if (src < 0) {
    is_negative = 1;
    src = abs(src);
    len++;
  }

  *dest = (char *)malloc((len + 1) * sizeof(char));
  if (!*dest) {
    return MEMORY_ALLOCATION_ERROR;
  }
  ptr = *dest;

  if (is_negative) {
    *ptr = '-';
    ptr++;
  }

  ptr += len - is_negative;
  *ptr-- = 0;

  while (src) {
    int remainder = src & (base - 1);
    *ptr-- = (remainder < 10) ? (remainder + '0') : (remainder - 10 + 'A');
    src >>= r;
  }

  return 0;
}
