#include <stdarg.h>
#include <stdio.h>
#include <string.h>

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
                 va_list valist);
int __scan_from_stream(int stream, void *stream_ptr, char dest[BUFSIZ]);

int overscanf(char const *restrict _format, ...);
int overfscanf(FILE *restrict stream, char const *restrict _format, ...);
int oversscanf(char const *restrict s, char const *restrict _format, ...);

int program_03_10() {
  int err, int_num = 0;
  unsigned int uint_num;
  char str[BUFSIZ], c;

  FILE *fin;
  fin = fopen("files/3.txt", "r");
  if (fin == NULL) {
    printf("Cringe\n");
  }
  err = overfscanf(fin, "%d", int_num);
  if (err) {
    printf("%d", err);
    return err;
  }

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
  va_list valist;
  if (_format == NULL || stream == NULL) {
    printf("1");
    return DEREFERENCING_NULL_PTR;
  }
  va_start(valist, _format);
  err = __overvscanf(FILE_STREAM, (void *)stream, _format, valist);
  va_end(valist);
  return err;
}

int oversscanf(char const *restrict s, char const *restrict _format, ...) {
  int err;
  va_list valist;
  char const *p_s;
  if (_format == NULL || s == NULL) {
    return DEREFERENCING_NULL_PTR;
  }
  va_start(valist, _format);
  p_s = s;
  err = __overvscanf(FILE_STREAM, (void *)&p_s, _format, valist);
  va_end(valist);
  return err;
}

int __overvscanf(int stream, void *stream_ptr, char const *restrict _format,
                 va_list valist) {
  size_t current_format_index = 0;
  size_t format_len = strlen(_format);
  int err, int_ans;
  char ans[BUFSIZ];

  if (stream_ptr == NULL || _format == NULL || valist == NULL) {
    return DEREFERENCING_NULL_PTR;
  }

  while (current_format_index < format_len) {
    if (*_format == '%') {
      switch (*(_format + 1)) {
        case 'd':
          err = __scan_from_stream(stream, stream_ptr, ans);
          if (err) {
            return err;
          }
          err = catoi(ans, 10, va_arg(valist, int *));
          if (err) {
            return err;
          }
          _format += 2;
          format_len += 2;
          break;

        case 'u':
          err = __scan_from_stream(stream, stream_ptr, ans);
          if (err) {
            return err;
          }
          err = catoi(ans, 10, va_arg(valist, unsigned int *));
          if (err) {
            return err;
          }
          _format += 2;
          format_len += 2;
          break;

        case 'f':  // TODO
          break;

        case 'l':  // TODO
          break;

        case 's':
          err = __scan_from_stream(stream, stream_ptr, ans);
          if (err) {
            return err;
          }
          strcpy(va_arg(valist, char *), ans);
          _format += 2;
          format_len += 2;
          break;

        case 'c':
          switch (stream) {
            case STDIN_STREAM:
              fgetc(stdin);
              *va_arg(valist, char *) = fgetc(stdin);
              break;
            case FILE_STREAM:
              *va_arg(valist, char *) = fgetc((FILE *)stream_ptr);

              break;
            case STR_STREAM:
              *va_arg(valist, char *) = *((char *)stream_ptr);
              stream_ptr = (char *)stream_ptr + 1;
              break;
          }
          _format += 2;
          format_len += 2;
          break;
        case 'o':
          err = __scan_from_stream(stream, stream_ptr, ans);
          if (err) {
            return err;
          }
          err = catoi(ans, 8, va_arg(valist, int *));
          if (err) {
            return err;
          }
          _format += 2;
          format_len += 2;
          break;

        case 'x':
          err = __scan_from_stream(stream, stream_ptr, ans);
          if (err) {
            return err;
          }
          err = catoi(ans, 16, va_arg(valist, int *));
          if (err) {
            return err;
          }
          _format += 2;
          format_len += 2;
          break;

        case 'X':
          err = __scan_from_stream(stream, stream_ptr, ans);
          if (err) {
            return err;
          }
          err = catoi(ans, 16, va_arg(valist, int *));
          if (err) {
            return err;
          }
          _format += 2;
          format_len += 2;
          break;

        default:
          return INVALID_FLAG;
      }
    } else {
      _format++;
      current_format_index++;
    }
  }
  return OK;
}

int __scan_from_stream(int stream, void *stream_ptr, char dest[BUFSIZ]) {
  FILE *file_stream = NULL;
  char **p_str_stream = NULL;
  char *dest_cpy = dest;
  char c;

  if (stream_ptr == NULL || dest == NULL) {
    return DEREFERENCING_NULL_PTR;
  }
  switch (stream) {
    case STDIN_STREAM:
      c = fgetc(stdin);
      while ((c == ' ' || c == '\t' || c == '\n') && c != EOF) {
        c = fgetc(stdin);
      }
      while (c != '\0' && c != '\n' && c != ' ' && c != '\t') {
        *dest_cpy++ = c;
        c = fgetc(stdin);
      }
      dest_cpy = 0;
      fflush(stdin);
      break;

    case FILE_STREAM:
      file_stream = (FILE *)stream_ptr;
      c = fgetc(file_stream);
      while ((c == ' ' || c == '\t' || c == '\n') && c != EOF) {
        c = fgetc(file_stream);
      }
      while (c != '\0' && c != '\n' && c != ' ' && c != '\t') {
        *dest_cpy++ = c;
        c = fgetc(file_stream);
      }
      dest_cpy = 0;
      fflush(file_stream);
      break;

    case STR_STREAM:
      p_str_stream = (char **)stream_ptr;
      c = **p_str_stream;
      while ((c == ' ' || c == '\t' || c == '\n') && c != EOF) {
        c = **p_str_stream;
        (*p_str_stream)++;
      }
      (*p_str_stream)++;
      while (c != '\0' && c != '\n' && c != ' ' && c != '\t') {
        *dest_cpy++ = c;
        c = **p_str_stream;
        (*p_str_stream)++;
      }
      dest_cpy = 0;
      break;

    default:
      return INVALID_STREAM_PTR;
  }
  return OK;
}