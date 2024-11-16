#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../libs/custom_math.h"
#include "../../../libs/errors.h"
#include "../../../libs/types.h"

#define STDIN_STREAM (1)
#define FILE_STREAM (2)
#define STR_STREAM (3)

#define P_INT (1)
#define P_DOUBLE (2)
#define P_CHAR (3)
#define P_FLOAT (4)
#define P_STRING (5)

int __overvscanf(int stream, void *stream_ptr, char const *restrict _format,
                 va_list args);

int __scan_from_stream(int stream, void **stream_ptr, char dest[BUFSIZ]);

int overscanf(char const *restrict _format, ...);
int overfscanf(FILE *restrict stream, char const *restrict _format, ...);
int oversscanf(char *s, char const *restrict _format, ...);

int rometoi(char *str, int *ans);

/*      tests      */

int program_03_10(int argc, char *argv[]) {
  FILE *file = fopen("files/1.txt", "r");
  int d, u, roman, fib, cv_lower, cv_upper;
  float f;
  double lf;
  char c;
  char s[100];

  // Example for %d
  overfscanf(file, "%d", &d);
  printf("Read int: %d\n", d);

  // Example for %u
  overfscanf(file, "%u", &u);
  printf("Read unsigned int: %u\n", u);

  // Example for %f
  overfscanf(file, "%f", &f);
  printf("Read float: %f\n", f);

  // Example for %lf
  overfscanf(file, "%lf", &lf);
  printf("Read double: %lf\n", lf);

  // Example for %c
  overfscanf(file, "%c", &c);
  printf("Read character: %c\n", c);

  // Example for %s
  overfscanf(file, "%s", s);
  printf("Read string: %s\n", s);

  // Example for %o (octal)
  overfscanf(file, "%o", &u);
  printf("Read octal: %o\n", u);

  // Example for %x (hexadecimal lowercase)
  overfscanf(file, "%x", &u);
  printf("Read hexadecimal (lowercase): %x\n", u);

  // Example for %X (hexadecimal uppercase)
  overfscanf(file, "%X", &u);
  printf("Read hexadecimal (uppercase): %X\n", u);

  // Example for %Ro (Roman numeral)
  overfscanf(file, "%Ro", &roman);
  printf("Read Roman numeral: %d\n", roman);

  // Example for %Zr (Fibonacci code)
  overfscanf(file, "%Zr", &fib);
  printf("Read Fibonacci code: %d\n", fib);

  // Example for %Cv (Custom base, lowercase)
  overfscanf(file, "%Cv", &cv_lower, 16); // Base 16
  printf("Read custom base (lowercase): %d\n", cv_lower);

  // Example for %CV (Custom base, uppercase)
  overfscanf(file, "%CV", &cv_upper, 16); // Base 16
  printf("Read custom base (uppercase): %d\n", cv_upper);

  fclose(file);
  return OK;
}

int overscanf(char const *restrict _format, ...) {
  int err;
  va_list valist;

  if (_format == NULL) {
    return DEREFERENCING_NULL_PTR;
  }

  va_start(valist, _format);
  err = __overvscanf(STDIN_STREAM, (void *)stdin, _format, valist);
  va_end(valist);
  return err;
}

int overfscanf(FILE *restrict stream, char const *restrict _format, ...) {
  int err;
  if (stream == NULL || _format == NULL) {
    return DEREFERENCING_NULL_PTR;
  }
  va_list args;
  va_start(args, _format);
  err = __overvscanf(FILE_STREAM, (void *)stream, _format, args);
  va_end(args);
  return err;
}

int oversscanf(char *s, char const *restrict _format, ...) {
  int err;
  char *str_ptr;
  if (s == NULL || _format == NULL) {
    return DEREFERENCING_NULL_PTR;
  }
  va_list args;
  va_start(args, _format);
  str_ptr = s;
  err = __overvscanf(STR_STREAM, (void *)str_ptr, _format, args);

  va_end(args);
  return err;
}

int __overvscanf(int stream, void *stream_ptr, char const *restrict _format,
                 va_list args) {
  int err;
  int current_format_index = 0, f_str_len;
  char dest[BUFSIZ];
  char dest_2[BUFSIZ];
  void *variable_ptr;
  int temp_int = 0;
  unsigned int *temp_uint_ptr;
  int i;
  int fib_arr[BUFSIZ];
  int count;

  if (stream_ptr == NULL || _format == NULL || args == NULL) {
    return DEREFERENCING_NULL_PTR;
  }

  f_str_len = strlen(_format);

  while (current_format_index < f_str_len) {

    if (*_format == '%') {
      switch (*(_format + 1)) {
      case 'd': {
        err = __scan_from_stream(stream, &stream_ptr, dest);
        if (err) {
          return err;
        }
        variable_ptr = va_arg(args, int *);
        err = catoi(dest, 10, variable_ptr);
        if (err) {
          return err;
        }

        current_format_index += 2;
        _format += 2;
        break;
      }
      case 'u': {
        err = __scan_from_stream(stream, &stream_ptr, dest);
        if (err) {
          return err;
        }
        err = catoi(dest, 10, va_arg(args, unsigned int *));
        if (err) {
          return err;
        }

        current_format_index += 2;
        _format += 2;
        break;
      }
      case 'f': {
        err = __scan_from_stream(stream, &stream_ptr, dest);
        if (err) {
          return err;
        }
        *va_arg(args, double *) = atof(dest);

        current_format_index += 2;
        _format += 2;
        break;
      }
      case 'l': {
        if (*(_format + 2) == 'f') {
          err = __scan_from_stream(stream, &stream_ptr, dest);
          if (err) {
            return err;
          }
          *va_arg(args, double *) = err = atof(dest);
          current_format_index += 3;
          _format += 3;
        } else {
          return INVALID_FLAG;
        }
        break;
      }
      case 'c': {
        switch (stream) {
        case STDIN_STREAM: {
          *va_arg(args, char *) = fgetc(stdin);
          break;
        }
        case FILE_STREAM: {
          *va_arg(args, char *) = fgetc((FILE *)stream_ptr);
          break;
        }
        case STR_STREAM: {
          *va_arg(args, char *) = *((char *)stream_ptr);
          stream_ptr = (char *)stream_ptr + 1;
          break;
        }
        default:
          break;
        }

        current_format_index += 2;
        _format += 2;
        break;
      }
      case 's': {
        err = __scan_from_stream(stream, &stream_ptr, dest);
        if (err) {
          return err;
        }
        strcpy(va_arg(args, char *), dest);

        current_format_index += 2;
        _format += 2;
        break;
      }
      case 'o': {
        err = __scan_from_stream(stream, &stream_ptr, dest);
        if (err) {
          return err;
        }
        variable_ptr = va_arg(args, int *);
        err = catoi(dest, 8, variable_ptr);
        if (err) {
          return err;
        }

        current_format_index += 2;
        _format += 2;
        break;
      }
      case 'x': {
        err = __scan_from_stream(stream, &stream_ptr, dest);
        if (err) {
          return err;
        }
        variable_ptr = va_arg(args, int *);
        err = catoi(dest, 16, variable_ptr);
        if (err) {
          return err;
        }

        current_format_index += 2;
        _format += 2;
        break;
      }
      case 'X': {
        err = __scan_from_stream(stream, &stream_ptr, dest);
        if (err) {
          return err;
        }
        variable_ptr = va_arg(args, int *);
        err = catoi(dest, 16, variable_ptr);
        if (err) {
          return err;
        }

        current_format_index += 2;
        _format += 2;
        break;
      }
      case 'R': {
        if (*(_format + 2) == 'o') {
          err = __scan_from_stream(stream, &stream_ptr, dest);
          if (err) {
            return err;
          }

          err = rometoi(dest, va_arg(args, int *));
          if (err) {
            return err;
          }

          current_format_index += 3;
          _format += 3;
        } else {
          return INVALID_FLAG;
        }
        break;
      }
      case 'Z': {
        if (*(_format + 2) == 'r') {
          err = __scan_from_stream(stream, &stream_ptr, dest);
          if (err) {
            return err;
          }
          err = generate_fibonacci_row_upto_n(strlen(dest), fib_arr, &count);

          for (i = 0; i < strlen(dest) - 1; i++) {
            if (dest[i] == '1') {
              temp_int += fib_arr[i];
            }
          }
          *va_arg(args, int *) = temp_int;
          temp_int = 0;
          current_format_index += 3;
          _format += 3;
        } else {
          return INVALID_FLAG;
        }
        break;
      }
      case 'C': {
        if (*(_format + 2) == 'v') {
          err = __scan_from_stream(stream, &stream_ptr, dest);
          if (err) {
            return err;
          }
          err = __scan_from_stream(stream, &stream_ptr, dest_2);
          if (err) {
            return err;
          }
          err = catoi(dest_2, 10, &temp_int);
          if (err) {
            return err;
          }

          if (temp_int < 2 || temp_int > 26) {
            temp_int = 10;
          }

          variable_ptr = va_arg(args, int *);
          err = catoi(dest, temp_int, variable_ptr);
          if (err) {
            return err;
          }

          temp_int = 0;
          current_format_index += 3;
          _format += 3;
        } else if (*(_format + 2) == 'V') {
          err = __scan_from_stream(stream, &stream_ptr, dest);
          if (err) {
            return err;
          }
          err = __scan_from_stream(stream, &stream_ptr, dest_2);
          if (err) {
            return err;
          }
          err = catoi(dest_2, 10, &temp_int);
          if (err) {
            return err;
          }

          if (temp_int < 2 || temp_int > 26) {
            temp_int = 10;
          }

          variable_ptr = va_arg(args, int *);
          err = catoi(dest, temp_int, variable_ptr);
          if (err) {
            return err;
          }

          current_format_index += 3;
          _format += 3;
        } else {
          return INVALID_FLAG;
        }
        break;
      }
      default:
        return INVALID_FLAG;
        break;
      }
    } else {
      current_format_index += 1;
      _format++;
    }
  }

  return OK;
}

int __scan_from_stream(int stream, void **stream_ptr, char dest[BUFSIZ]) {
  FILE **file_stream;
  char *str_stream;
  char *p_str_stream;
  char *dest_cpy = dest;
  char c = 65;

  if (stream_ptr == NULL || dest == NULL) {
    return DEREFERENCING_NULL_PTR;
  }

  switch (stream) {
  case STDIN_STREAM: {
    c = fgetc(stdin);
    while ((c == ' ' || c == '\t' || c == '\n') && c != EOF) {
      c = fgetc(stdin);
    }
    while (c != '\0' && c != '\n' && c != ' ' && c != '\t') {
      *dest_cpy++ = c;
      c = fgetc(stdin);
    }
    *dest_cpy = 0;
    break;
  }
  case FILE_STREAM: {
    file_stream = (FILE **)stream_ptr;
    c = fgetc(*file_stream);
    while ((c == ' ' || c == '\t' || c == '\n') && c != EOF) {
      c = fgetc(*file_stream);
    }
    while (c != '\0' && c != '\n' && c != ' ' && c != '\t' && c != EOF) {
      *dest_cpy++ = c;
      c = fgetc(*file_stream);
    }
    *dest_cpy = 0;
    break;
  }
  case STR_STREAM: {
    p_str_stream = *((char **)stream_ptr);
    c = *p_str_stream;
    p_str_stream++;
    while ((c == ' ' || c == '\t' || c == '\n') && c != '\0') {
      c = *p_str_stream;
      p_str_stream++;
    }
    while (c != '\0' && c != '\n' && c != ' ' && c != '\t') {
      *dest_cpy++ = c;
      c = *p_str_stream;
      p_str_stream++;
    }
    *dest_cpy = 0;
    *stream_ptr = p_str_stream;
    break;
  }
  default:
    return INVALID_STREAM_PTR;
    break;
  }

  return OK;
}

int rometoi(char *str, int *ans) {
  int len = strlen(str);
  int i = 0, j = 0;
  char letter_1, letter_2;
  int temp_int = 0;

  struct roman_item {
    int val;
    int len;
    char *letter;
  };

  struct roman_item roman_data[] = {
      {1000, 1, "M"}, {900, 2, "CM"}, {500, 1, "D"}, {400, 2, "CD"},
      {100, 1, "C"},  {90, 2, "XC"},  {50, 1, "L"},  {40, 2, "XL"},
      {10, 1, "X"},   {9, 2, "IX"},   {5, 1, "V"},   {4, 2, "IV"},
      {1, 1, "I"}};

  while (i < len) {
    letter_1 = str[i];
    letter_2 = str[i + 1];
    for (j = 0; j < 13; j++) {
      if (roman_data[j].len == 1 && roman_data[j].letter[0] == letter_1) {
        temp_int = roman_data[j].val;
      }
      if (roman_data[j].len == 2 && roman_data[j].letter[0] == letter_1 &&
          roman_data[j].letter[1] == letter_2) {
        temp_int = roman_data[j].val;
        i++;
        break;
      }
    }
    (*ans) += temp_int;
    i++;
  }
  return OK;
}