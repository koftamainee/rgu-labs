#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "../../../libc/cstring.h"
#include "../../../libc/hash_table.h"

int string_keys_comparer(const void *a, const void *b);
int string_comparer_defines(const void *a, const void *b);

void strnig_string_bucket_destructor(void *b);

size_t base62_hash(const void *key, size_t key_size, size_t capacity);
size_t base62_to_decimal(const String str);

err_t parse_file_macro(char *input_filename, char *output_filename,
                       hash_table *defines);
err_t parse_lines_from_file_macro(FILE *fin, FILE *fout, hash_table *defines);
err_t add_new_define_from_line(char *define_line, hash_table *defines,
                               u_list *define_keys);

err_t replace_macroses_in_line(char *line, FILE *fout, hash_table *defines,
                               u_list *define_keys);

int program_04_9(int argc, char *argv[]) {
    err_t err = 0;
    hash_table *defines = NULL;
    size_t i = 0;
    char *input_filename = NULL, *output_filename = NULL;

    if (argc < 5) {
        return INVALID_FLAG;
    }
    for (i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-f") == 0) {
            if (i + 1 == argc) {  // out of bounds
                return INVALID_FLAG;
            }
            input_filename = argv[++i];
            continue;
        } else if (strcmp(argv[i], "-o") == 0) {
            if (i + 1 == argc) {  // out of bounds
                return INVALID_FLAG;
            }
            output_filename = argv[++i];
            continue;
        }
    }
    if (input_filename == NULL || output_filename == NULL) {
        return INVALID_FLAG;
    }

    err = hash_table_init(&defines, string_keys_comparer, base62_hash,
                          sizeof(String *), sizeof(String *),
                          strnig_string_bucket_destructor);

    err = parse_file_macro(input_filename, output_filename, defines);
    hash_table_free(defines);
    if (err) {
        return err;
    }

    printf("File \"%s\" succesfully processed. Result written in \"%s\" file\n",
           input_filename, output_filename);

    return err;
}

int string_keys_comparer(const void *a, const void *b) {
    const hash_table_bucket *b1 = a, *b2 = b;
    String s1 = *(String *)(b1->key);
    String s2 = *(String *)(b2->key);
    return string_cmp(s1, s2);
}

void strnig_string_bucket_destructor(void *b) {
    hash_table_bucket *bucket = b;

    string_free(*(String *)bucket->value);
    string_free(*(String *)bucket->key);

    free(bucket->value);
    free(bucket->key);

    free(bucket);
}

size_t base62_hash(const void *key, size_t key_size, size_t capacity) {
    const String key_s = *(String *)key;
    size_t index = base62_to_decimal(key_s);
    return index % capacity;
}

size_t base62_to_decimal(const String str) {
    size_t base = 62;
    size_t result = 0;
    size_t value = 0;
    size_t i = 0;
    char c = 0;

    for (i = 0; i < string_len(str); ++i) {
        c = str[i];
        value = 0;
        if (isdigit(c)) {
            value = c - '0';
        } else if (isupper(c)) {
            value = c - 'A' + 10;
        } else if (islower(c)) {
            value = c - 'a' + 36;
        } else {
            continue;
        }
        result = result * base + value;
    }
    return result;
}

err_t parse_file_macro(char *input_filename, char *output_filename,
                       hash_table *defines) {
    if (input_filename == NULL || output_filename == NULL || defines == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    FILE *fin = NULL, *fout = NULL;
    err_t err = 0;

    fin = fopen(input_filename, "r");
    if (fin == NULL) {
        return OPENING_THE_FILE_ERROR;
    }
    fout = fopen(output_filename, "w");
    if (fin == NULL) {
        fclose(fin);
        return OPENING_THE_FILE_ERROR;
    }

    err = parse_lines_from_file_macro(fin, fout, defines);

    fclose(fin);
    fclose(fout);

    return err;
}

err_t parse_lines_from_file_macro(FILE *fin, FILE *fout, hash_table *defines) {
    if (fin == NULL || fout == NULL || defines == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    err_t err = 0;
    char line[BUFSIZ], *define_ptr;
    size_t len = 0;
    u_list *define_lines = NULL;

    err = u_list_init(&define_lines, sizeof(String *), free);
    if (err) {
        return err;
    }
    while (fgets(line, BUFSIZ, fin)) {
        len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';  // delete '\n' symbol
        }

        define_ptr = strstr(line, "#define");
        if (define_ptr == NULL) {
            break;
        }
        err = add_new_define_from_line(define_ptr, defines, define_lines);
        if (err) {
            u_list_free(define_lines);
            return err;
        }
    }
    while (fgets(line, BUFSIZ, fin)) {
        err = replace_macroses_in_line(line, fout, defines, define_lines);
        if (err) {
            u_list_free(define_lines);
            return err;
        }
    }

    u_list_free(define_lines);

    return EXIT_SUCCESS;
}

err_t add_new_define_from_line(char *define_line, hash_table *defines,
                               u_list *define_keys) {
    if (define_line == NULL || defines == NULL || define_keys == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    String key = NULL, value = NULL;
    err_t err = 0;

    define_line += strlen("#define");  // prob counting during comptime
    if (*define_line != ' ') {
        return INVALID_INPUT_DATA;
    }
    while (*(define_line + 1) == ' ') {
        define_line++;
    }
    define_line++;
    if (*define_line == '\0') {
        return INVALID_INPUT_DATA;
    }
    key = string_init();
    if (key == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }
    while (*define_line != ' ' && *define_line != '\0') {
        err = string_add(&key, *define_line);
        if (err) {
            string_free(key);
            return err;
        }
        define_line++;
    }
    if (*define_line == '\0') {
        string_free(key);
        return INVALID_INPUT_DATA;
    }

    while (*(define_line + 1) == ' ') {
        define_line++;
    }
    define_line++;
    if (*define_line == '\0') {
        string_free(key);
        return INVALID_INPUT_DATA;
    }

    value = string_init();
    if (value == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }
    if (*define_line == '(') {
        define_line++;
        while (*define_line != ')' && *define_line != '\0') {
            err = string_add(&value, *define_line);
            if (err) {
                string_free(key);
                string_free(value);
                return err;
            }
            define_line++;
        }
        if (*define_line == '\0') {
            string_free(key);
            string_free(value);
            return INVALID_INPUT_DATA;
        }
        define_line++;
    } else {
        while (*define_line != ' ' && *define_line != '\0') {
            err = string_add(&value, *define_line);
            if (err) {
                string_free(key);
                string_free(value);
                return err;
            }
            define_line++;
        }
    }
    if (*define_line == ' ') {
        define_line++;
        while (*(define_line + 1) == ' ') {
            define_line++;
        }
    }
    if (*define_line != '\0') {
        string_free(key);
        string_free(value);
        return INVALID_INPUT_DATA;
    }

    err = hash_table_set(defines, &key, &value);
    if (err) {
        string_free(key);
        string_free(value);
        return err;
    }

    err = u_list_insert_sorted(define_keys, &key, string_comparer_defines);
    if (err) {
        return err;
    }

    key = NULL;
    value = NULL;

    return EXIT_SUCCESS;
}

err_t replace_macroses_in_line(char *line, FILE *fout, hash_table *defines,
                               u_list *define_keys) {
    if (line == NULL || fout == NULL || defines == NULL ||
        define_keys == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    u_list_node *current = NULL;
    int found;
    String needle = NULL, *replacement = NULL;
    size_t needle_len = 0, replacement_len = 0, line_len = 0;
    err_t err = 0;

    current = define_keys->first;
    while (current != NULL) {
        line_len = strlen(line);
        needle = *(String *)current->data;
        needle_len = string_len(needle);
        found = string_c_str(line, needle);
        if (found != -1) {
            err = hash_table_get(defines, &needle, (void **)&replacement);
            if (err) {
                return err;
            }
            replacement_len = string_len(*replacement);

            memmove(line + found + replacement_len, line + found + needle_len,
                    line_len - found - needle_len + 1);

            memcpy(line + found, *replacement, replacement_len);
        }
        current = current->next;
    }
    fprintf(fout, "%s", line);

    return EXIT_SUCCESS;
}

int string_comparer_defines(const void *a, const void *b) {
    String s1 = *(String *)a;
    String s2 = *(String *)b;
    return -string_cmp(s1, s2);
}
