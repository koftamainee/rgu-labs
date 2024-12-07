#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "../custom_math.h"
#include "../errors.h"

int citoa(int num, int base, char **ans) {
  int digit, len, is_negative = 0;

  if (ans == NULL) {
    return DEREFERENCING_NULL_PTR;
  }

  if (base < 2 || base > 36) {
    return INVALID_NUMERIC_BASE;
  }

  if (num == 0) {
    *ans = malloc(2 * sizeof(char));
    if (*ans == NULL) {
      return MEMORY_ALLOCATION_ERROR;
    }
    (*ans)[0] = '0';
    (*ans)[1] = '\0';
    return OK;
  }

  if (num < 0) {
    is_negative = 1;
    num = abs(num);
  }

  len = log_base(num, base) + 1 - is_negative;
  *ans = malloc((len + 1) * sizeof(char));
  if (*ans == NULL) {
    return MEMORY_ALLOCATION_ERROR;
  }
  char *ptr = *ans;

  if (is_negative) {
    *ptr++ = '-';
  }

  ptr += len + is_negative;
  *ptr-- = '\0';

  while (num) {
    digit = num % base;
    num /= base;
    if (digit > 9) {
      *ptr = digit + 'A' - 10;
    } else {
      *ptr = digit + '0';
    }
    ptr--;
  }
  return OK;
}

int cutoa(unsigned int num, int base, char **ans) {
  unsigned int digit, len;

  if (ans == NULL) {
    return DEREFERENCING_NULL_PTR;
  }

  if (base < 2 || base > 36) {
    return INVALID_NUMERIC_BASE;
  }

  if (num == 0) {
    *ans = malloc(2 * sizeof(char));
    if (*ans == NULL) {
      return MEMORY_ALLOCATION_ERROR;
    }
    (*ans)[0] = '0';
    (*ans)[1] = '\0';
    return OK;
  }

  len = log_base(num, base) + 1;
  *ans = malloc((len + 1) * sizeof(char));
  if (*ans == NULL) {
    return MEMORY_ALLOCATION_ERROR;
  }

  char *ptr = *ans + len;
  *ptr-- = '\0';

  while (num) {
    digit = num % base;
    num /= base;
    if (digit > 9) {
      *ptr = digit + 'A' - 10;
    } else {
      *ptr = digit + '0';
    }
    ptr--;
  }

  return OK;
}

int catoi(char const *str, int base, int *ans) {
  int num = 0;
  int digit = 0;
  int minus = 0;

  if (str == NULL || ans == NULL) {
    return DEREFERENCING_NULL_PTR;
  }

  if (*str == '-') {
    minus = 1;
    str++;
  }

  while (*str) {
    if (isalpha(*str)) {
      digit = tolower(*str) - 'a' + 10;
    } else if (isdigit(*str)) {
      digit = *str - '0';
    }
    num = num * base + digit;
    str++;
  }

  *ans = minus ? -num : num;

  return OK;
}

int char_to_int(int *ans, char c) {
  if (!isalnum(c)) {
    return INVALID_CHAR_TO_NUM_CONVERSION;
  }

  if (isdigit(c)) {
    *ans = c - '0';
  } else {
    *ans = toupper(c) - 'A' + 10;
  }

  return OK;
}

int int_to_char(char *ans, int num) {
  if (num < 0 || num > 35) {
    printf("%d", num);
    return INVALID_NUM_TO_CHAR_CONVERSION;
  }

  if (num < 10) {
    *ans = '0' + num;
  } else {
    *ans = 'A' + num - 10;
  }

  return OK;
}
