#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../../libc/types.h"
#include "../../../libc/utils.h"
#include "../pathetic_hashtable.h"

int bitwise_or(int a, int b);
int bitwise_and(int a, int b);
int implication(int a, int b);
int reverse_implication(int a, int b);
int equivalence(int a, int b);
int bitwise_xor(int a, int b);
int coimplication(int a, int b);
int nand(int a, int b);
int nor(int a, int b);
int bitwise_not(int a);

err_t parse_file_bitwise_op(char *filename, char *trace_filename,
                            p_hash *variables);

void trace(FILE *trace, char *message, ...);

err_t parse_line_bitwise_op(char *line, FILE *fout, int *in_comment,
                            p_hash *variables);

err_t parse_instruction_bitwise_op(char *instruction, FILE *fout,
                                   int *in_comment, p_hash *variables);

err_t execute_instruction_bitwise_op(char *instruction, FILE *fout,
                                     p_hash *variables);
err_t parse_bitwise_operation(char *instruction, FILE *fout, p_hash *variables);

err_t read_bitwise_op(char *base, int *to_read, int variable_name, FILE *fout);
err_t write_bitwise_op(char *base, int to_write, int variable_name, FILE *fout);

err_t execute_bitwise_op(int first_var, int second_var, char op[3],
                         int *result);

int program_04_8(int argc, char *argv[]) {
    p_hash *variables = NULL;
    err_t err = 0;
    int i;
    char *filename = NULL;
    char *trace_filename = NULL;

    if (argc < 2) {
        return INVALID_FLAG;
    }
    for (i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "/trace") == 0) {
            if (i + 1 == argc) {  // out of bounds
                return INVALID_FLAG;
            }
            trace_filename = argv[++i];
            continue;
        }
        filename = argv[i];
    }

    if (filename == NULL) {
        return INVALID_FLAG;
    }

    err = p_hash_init(&variables);
    if (err) {
        return err;
    }

    err = parse_file_bitwise_op(filename, trace_filename, variables);
    if (err) {
        return err;
    }

    p_hash_free(variables);

    return EXIT_SUCCESS;
}

int bitwise_or(int a, int b) { return a | b; }

int bitwise_and(int a, int b) { return a & b; }

int implication(int a, int b) { return (~a | b); }

int reverse_implication(int a, int b) { return (~b | a); }

int equivalence(int a, int b) { return ~(a ^ b); }

int bitwise_xor(int a, int b) { return a ^ b; }

int coimplication(int a, int b) { return ~a & ~b; }

int nand(int a, int b) { return ~(a & b); }

int nor(int a, int b) { return ~(a | b); }

int bitwise_not(int a) { return ~a; }

err_t parse_file_bitwise_op(char *filename, char *trace_filename,
                            p_hash *variables) {
    if (filename == NULL || variables == NULL) {  // trace_filename MAY BE NULL
        return DEREFERENCING_NULL_PTR;
    }

    FILE *fin = NULL, *fout = NULL;
    char line[BUFSIZ];
    err_t err = 0;
    int in_comment = 0;

    fin = fopen(filename, "r");
    if (fin == NULL) {
        return OPENING_THE_FILE_ERROR;
    }

    if (trace_filename == NULL) {  // printing to stdout
        fout = stdout;
    } else {
        fout = fopen(trace_filename, "w");
        if (fout == NULL) {
            fclose(fin);
            return OPENING_THE_FILE_ERROR;
        }
    }

    while (fgets(line, BUFSIZ, fin)) {
        err = parse_line_bitwise_op(line, fout, &in_comment, variables);
        if (err) {
            fclose(fin);
            if (fout != stdout) {
                fclose(fout);
            }
            return err;
        }
    }
    fclose(fin);
    if (fout != stdout) {
        fclose(fout);
    }

    return EXIT_SUCCESS;
}

void trace(FILE *trace, char *message, ...) {
    char timestamp[20];
    va_list ap;
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);

    va_start(ap, message);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", local_time);

    fprintf(trace, "[%s] ", timestamp);
    vfprintf(trace, message, ap);

    fprintf(trace, "\n");

    fflush(trace);
    va_end(ap);
}

err_t parse_line_bitwise_op(char *line, FILE *fout, int *in_comment,
                            p_hash *variables) {
    if (line == NULL || fout == NULL || in_comment == NULL ||
        variables == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    size_t instruction_len = 0;
    err_t err = 0;
    char *instruction_ptr = NULL;

    instruction_len = 0;
    instruction_ptr = line;
    while (*instruction_ptr) {
        if (*instruction_ptr == '%') {
            *instruction_ptr = '\0';
            break;
        }
        instruction_ptr++;
        instruction_len += 1;
    }
    if (instruction_len > 0 && line[instruction_len - 1] == '\n') {
        line[instruction_len - 1] = '\0';
    }
    instruction_ptr = line;
    while (*instruction_ptr == ' ' || *instruction_ptr == '\t') {
        instruction_ptr++;
    }

    err = parse_instruction_bitwise_op(instruction_ptr, fout, in_comment,
                                       variables);
    if (err) {
        return err;
    }

    return EXIT_SUCCESS;
}

err_t parse_instruction_bitwise_op(char *instruction, FILE *fout,
                                   int *in_comment, p_hash *variables) {
    if (instruction == NULL || fout == NULL || in_comment == NULL ||
        variables == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    err_t err = 0;

    err = remove_comments_from_line(&instruction, in_comment, '{', '}');
    if (err) {
        return err;
    }
    err = remove_spaces_from_line(instruction);
    if (err) {
        return err;
    }

    if (*instruction == '\0') {
        return EXIT_SUCCESS;
    }
    err = execute_instruction_bitwise_op(instruction, fout, variables);
    if (err) {
        return err;
    }

    return EXIT_SUCCESS;
}

err_t execute_instruction_bitwise_op(char *instruction, FILE *fout,
                                     p_hash *variables) {
    if (instruction == NULL || fout == NULL || variables == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    err_t err = 0;
    int ans = 0;
    char variable_name = 0;
    char base[3];

    if (strncmp(instruction, "read(", 5) == 0) {
        instruction += 5;
        variable_name = *instruction;
        instruction++;
        if (variable_name == '\0') {
            return INVALID_INPUT_DATA;
        }
        if (*instruction != ',') {
            return INVALID_INPUT_DATA;
        }
        instruction++;
        base[0] = *instruction;
        if (!isdigit(base[0])) {
            return INVALID_INPUT_DATA;
        }
        instruction++;
        base[1] = *instruction;
        if (!isdigit(base[1]) && base[1] != ')') {
            return INVALID_INPUT_DATA;
        }
        instruction++;
        if (base[1] == ')') {
            base[1] = '\0';
            instruction--;
        } else {
            base[2] = '\0';
            if (*instruction != ')') {
                return INVALID_INPUT_DATA;
            }
        }
        instruction++;
        if (*instruction != ';' && *(instruction + 1) != '\0') {
            return INVALID_INPUT_DATA;
        }

        err = read_bitwise_op(base, &ans, variable_name, fout);
        if (err) {
            return err;
        }

        err = p_hash_set(variables, variable_name, ans);
        if (err) {
            return err;
        }
        return EXIT_SUCCESS;
    }

    if (strncmp(instruction, "write(", 6) == 0) {
        instruction += 6;
        variable_name = *instruction;
        instruction++;
        if (variable_name == '\0') {
            return INVALID_INPUT_DATA;
        }
        if (*instruction != ',') {
            return INVALID_INPUT_DATA;
        }
        instruction++;
        base[0] = *instruction;
        if (!isdigit(base[0])) {
            return INVALID_INPUT_DATA;
        }
        instruction++;
        base[1] = *instruction;
        if (!isdigit(base[1]) && base[1] != ')') {
            return INVALID_INPUT_DATA;
        }
        instruction++;
        if (base[1] == ')') {
            base[1] = '\0';
            instruction--;
        } else {
            base[2] = '\0';
            if (*instruction != ')') {
                return INVALID_INPUT_DATA;
            }
        }
        instruction++;
        if (*instruction != ';' && *(instruction + 1) != '\0') {
            return INVALID_INPUT_DATA;
        }
        err = p_hash_get(variables, variable_name, &ans);
        if (err) {
            return err;
        }
        err = write_bitwise_op(base, ans, variable_name, fout);
        if (err) {
            return err;
        }
        return EXIT_SUCCESS;
    }

    // now only operations
    err = parse_bitwise_operation(instruction, fout, variables);
    if (err) {
        return err;
    }

    return EXIT_SUCCESS;
}

err_t read_bitwise_op(char *base, int *to_read, int variable_name, FILE *fout) {
    if (base == NULL || to_read == NULL || fout == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    char user_ans[BUFSIZ];
    err_t err = 0;
    int base_num = 0;

    err = catoi(base, 10, &base_num);
    if (err) {
        return err;
    }
    if (base_num > 36 || base_num < 2) {
        return INVALID_INPUT_DATA;
    }
    printf("Input number in %d base to %c variable: ", base_num,
           toupper(variable_name));
    scanf("%s", user_ans);
    err = catoi(user_ans, base_num, to_read);
    if (err) {
        return err;
    }

    trace(fout, "Get from user value of %c variable in %d counting system: %s",
          toupper(variable_name), base_num, user_ans);

    return EXIT_SUCCESS;
}

err_t write_bitwise_op(char *base, int to_write, int variable_name,
                       FILE *fout) {
    if (base == NULL || fout == NULL) {
        return DEREFERENCING_NULL_PTR;
    }
    err_t err = 0;
    int base_num = 0;
    char *ans;

    err = catoi(base, 10, &base_num);
    if (err) {
        return err;
    }
    if (base_num > 36 || base_num < 2) {
        return INVALID_INPUT_DATA;
    }

    err = citoa(to_write, base_num, &ans);
    if (err) {
        return err;
    }

    printf("%c(%d) = %s\n", variable_name, base_num, ans);

    trace(fout, "Printed value of %c variable in %d counting system: %s",
          toupper(variable_name), base_num, ans);

    free(ans);

    return EXIT_SUCCESS;
}

err_t parse_bitwise_operation(char *instruction, FILE *fout,
                              p_hash *variables) {
    if (instruction == NULL || fout == NULL || variables == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    char first_variable_name = 0, second_variable_name = 0,
         result_variable_name = 0;
    char op[3];
    int first_variable_value = 0, second_variable_value = 0, res = 0;
    err_t err = 0;

    result_variable_name = *instruction;
    if (result_variable_name == '\0') {
        return EXIT_SUCCESS;  // empty string just in case
    }
    instruction++;
    if (strncmp(instruction, ":=", 2) != 0) {
        return INVALID_INPUT_DATA;
    }
    instruction += 2;
    if (*instruction == '\\') {
        instruction++;
        first_variable_name = *instruction;
        if (first_variable_name == '\0') {
            return INVALID_INPUT_DATA;
        }
        instruction++;
        if (*instruction != ';' || *(instruction + 1) != '\0') {
            return INVALID_INPUT_DATA;
        }
        err = p_hash_get(variables, first_variable_name, &first_variable_value);
        if (err) {
            return err;
        }
        res = bitwise_not(first_variable_value);
        err = p_hash_set(variables, result_variable_name, res);
        if (err) {
            return err;
        }
        trace(fout,
              "Executed \\ operation on %c variable (\\%d = %d). Result placed "
              "into %c: %d",
              toupper(first_variable_name), first_variable_value, res,
              toupper(result_variable_name), res);

        return EXIT_SUCCESS;
    }

    first_variable_name = *instruction;
    if (first_variable_name == '\0') {
        return INVALID_INPUT_DATA;
    }
    instruction++;

    op[0] = *instruction;
    if (op[0] == '\0') {
        return INVALID_INPUT_DATA;
    }
    if ((op[0] == '+' && *(instruction + 1) != '>') || op[0] == '&' ||
        op[0] == '~' || op[0] == '?' || op[0] == '!') {
        op[1] = '\0';
    } else {
        instruction++;
        op[1] = *instruction;
        op[2] = '\0';
        if (op[1] == '\0') {
            return INVALID_INPUT_DATA;
        } else if (strcmp(op, "->") && strcmp(op, "<-") && strcmp(op, "<>") &&
                   strcmp(op, "+>")) {
            return INVALID_INPUT_DATA;
        }
    }
    instruction++;
    second_variable_name = *instruction;
    if (second_variable_name == '\0') {
        return INVALID_INPUT_DATA;
    }
    instruction++;
    if (*instruction != ';' && *(instruction + 1) != '\0') {
        return INVALID_INPUT_DATA;
    }

    err = p_hash_get(variables, first_variable_name, &first_variable_value);
    if (err) {
        return err;
    }
    err = p_hash_get(variables, second_variable_name, &second_variable_value);
    if (err) {
        return err;
    }

    err = execute_bitwise_op(first_variable_value, second_variable_value, op,
                             &res);
    if (err) {
        return err;
    }
    err = p_hash_set(variables, result_variable_name, res);
    if (err) {
        return err;
    }
    trace(fout,
          "Executed %s operation to between %c and %c variables (%d %s %d = "
          "%d). Result placed into %c: %d",
          op, toupper(first_variable_name), toupper(second_variable_name),
          first_variable_value, op, second_variable_value, res,
          toupper(result_variable_name), res);

    return EXIT_SUCCESS;
}

err_t execute_bitwise_op(int first_var, int second_var, char op[3],
                         int *result) {
    if (op == NULL || result == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    if (strcmp(op, "+") == 0) {
        *result = bitwise_or(first_var, second_var);
    } else if (strcmp(op, "&") == 0) {
        *result = bitwise_and(first_var, second_var);
    } else if (strcmp(op, "->") == 0) {
        *result = implication(first_var, second_var);
    } else if (strcmp(op, "<-") == 0) {
        *result = reverse_implication(first_var, second_var);
    } else if (strcmp(op, "~") == 0) {
        *result = bitwise_not(first_var);
    } else if (strcmp(op, "<>") == 0) {
        *result = equivalence(first_var, second_var);
    } else if (strcmp(op, "+>") == 0) {
        *result = coimplication(first_var, second_var);
    } else if (strcmp(op, "?") == 0) {
        *result = bitwise_xor(first_var, second_var);
    } else if (strcmp(op, "!") == 0) {
        *result = nand(first_var, second_var);
    }

    return EXIT_SUCCESS;
}
