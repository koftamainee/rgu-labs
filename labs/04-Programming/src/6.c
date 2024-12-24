#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../../../libc/cstring.h"
#include "../../../libc/u_list.h"

typedef struct {
    int coef;
    int exp;
} poly_term;

typedef u_list poly;

typedef enum { Add, Sub, Mult, Div, Mod, Eval, Diff, Cmps } operation;

/* ------------------------------------------------------ */

err_t poly_init(poly **p);
void poly_free(poly *p);
err_t poly_from_string(poly **p, const String src);

err_t poly_add(const poly *a, const poly *b, poly *result);
err_t poly_sub(const poly *a, const poly *b, poly *result);
err_t poly_mult(const poly *a, const poly *b, poly *result);
err_t poly_div(const poly *a, const poly *b, poly *result);
err_t poly_mod(const poly *a, const poly *b, poly *result);
err_t poly_diff(poly *p);
err_t poly_comp(const poly *a, const poly *b, poly *result);

err_t poly_eval(const poly *p, int value, int *result);

void print_poly(const poly *p);
void print_poly_node(const u_list_node *node);

int is_valid_char_in_poly(char c);

int compare_terms(const void *a, const void *b);
int compare_terms_rev(const void *a, const void *b);

err_t poly_add_term(poly *poly, int coef, int exp);
err_t poly_sub_term(poly *poly, int coef, int exp);

/* ------------------------------------------------------ */

err_t start_execution(const char *filename);
err_t parse_instruction(char *instruction, poly *current_sum, int *in_comment);
err_t remove_comments_from_line(char **line, int *in_comment);
err_t do_stuff_with_polys(poly *poly1, poly *poly2, poly *current_sum,
                          operation op, int for_eval);

int program_04_6(int argc, char *argv[]) {
    err_t err;
    if (argc < 2) {
        return INVALID_CLI_ARGUMENT;
    }
    err = start_execution(argv[1]);
    if (err) {
        return err;
    }

    return EXIT_SUCCESS;
}

err_t poly_init(poly **p) { return u_list_init(p, sizeof(poly_term), free); }

void poly_free(poly *p) { u_list_free(p); }

err_t poly_from_string(poly **p, const String src) {
    err_t err;
    int coef = 0, exp = 0, sign = 1;  // + by default
    size_t i = 0;
    poly_term new_term;

    if (p == NULL || src == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    err = poly_init(p);
    if (err) {
        return err;
    }

    for (i = 0; i < string_len(src);) {
        if (!is_valid_char_in_poly(src[i])) {
            poly_free(*p);
            return INVALID_INPUT_DATA;
        }

        if (src[i] == '+') {
            sign = 1;
            i++;
        } else if (src[i] == '-') {
            sign = -1;
            i++;
        }

        coef = 0;
        if (isdigit(src[i])) {
            while (i < string_len(src) && isdigit(src[i])) {
                coef = coef * 10 + (src[i] - '0');
                i++;
            }
            coef *= sign;
        } else if (src[i] == 'x') {
            coef = sign;
        } else {
            poly_free(*p);
            return INVALID_INPUT_DATA;
        }

        exp = 0;
        if (i < string_len(src) && src[i] == 'x') {
            i++;
            if (i < string_len(src) && src[i] == '^') {
                i++;
                if (i >= string_len(src) || !isdigit(src[i])) {
                    poly_free(*p);
                    return INVALID_INPUT_DATA;
                }
                while (i < string_len(src) && isdigit(src[i])) {
                    exp = exp * 10 + (src[i] - '0');
                    i++;
                }
            } else {
                exp = 1;
            }
        } else {
            exp = 0;
        }

        new_term.coef = coef;
        new_term.exp = exp;
        err = u_list_insert(*p, 0, &new_term);
        if (err) {
            poly_free(*p);
            return err;
        }
    }

    err = u_list_sort(*p, compare_terms);
    if (err) {
        poly_free(*p);
        return err;
    }

    return EXIT_SUCCESS;
}

void __poly_add_inner(const u_list_node *node, poly *result) {
    if (!node) return;
    poly_term *term = node->data;
    poly_add_term(result, term->coef, term->exp);
    __poly_add_inner(node->next, result);
}

err_t poly_add(const poly *a, const poly *b, poly *result) {
    if (result == NULL) {
        return DEREFERENCING_NULL_PTR;
    }
    if (a != NULL) {
        __poly_add_inner(a->first, result);
    }
    if (b != NULL) {
        __poly_add_inner(b->first, result);
    }

    return EXIT_SUCCESS;
}

void __poly_sub_inner(const u_list_node *node, poly *result) {
    if (!node) return;
    poly_term *term = node->data;
    poly_sub_term(result, term->coef, term->exp);
    __poly_sub_inner(node->next, result);
}

err_t poly_sub(const poly *a, const poly *b, poly *result) {
    if (result == NULL) {
        return DEREFERENCING_NULL_PTR;
    }
    if (a != NULL) {
        __poly_sub_inner(a->first, result);
    }
    if (b != NULL) {
        __poly_sub_inner(b->first, result);
    }

    return EXIT_SUCCESS;
}

void __poly_mult_inner(const u_list_node *node, const poly *a, poly *result) {
    if (!node) return;
    poly_term *term = node->data;

    const u_list_node *current = a->first;
    while (current != NULL) {
        poly_term *a_term = current->data;
        poly_add_term(result, term->coef * a_term->coef,
                      term->exp + a_term->exp);
        current = current->next;
    }

    __poly_mult_inner(node->next, a, result);
}

err_t poly_mult(const poly *a, const poly *b, poly *result) {
    if (result == NULL || a == NULL || b == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    __poly_mult_inner(b->first, a, result);

    return EXIT_SUCCESS;
}

err_t poly_div(const poly *a, const poly *b, poly *result) {
    err_t err;
    u_list_node *current_1, *current_2, *result_node;
    poly_term *term_1, *term_2, *result_term;

    if (a == NULL || b == NULL || result == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    current_1 = a->first;
    current_2 = b->first;

    if (current_2 != NULL) {
        term_2 = (poly_term *)current_2->data;
        if (term_2->coef == 0) {
            return ZERO_DIVISION;
        }
    }

    while (current_1 != NULL) {
        term_1 = (poly_term *)current_1->data;

        if (term_1->coef == 0 && term_1->exp == 0) {
            break;
        }

        if (current_2 != NULL) {
            term_2 = (poly_term *)current_2->data;
            if (term_1->exp < term_2->exp) {
                return INVALID_INPUT_DATA;
            }
        }
        current_1 = current_1->next;
    }

    current_1 = a->first;
    while (current_1 != NULL) {
        term_1 = (poly_term *)current_1->data;

        if (current_2 != NULL) {
            term_2 = (poly_term *)current_2->data;

            if (term_2->coef == 0) {
                return ZERO_DIVISION;
            }

            if (term_1->exp >= term_2->exp) {
                result_term = (poly_term *)malloc(sizeof(poly_term));
                if (result_term == NULL) {
                    return MEMORY_ALLOCATION_ERROR;
                }

                result_term->coef = term_1->coef / term_2->coef;
                result_term->exp = term_1->exp - term_2->exp;

                err = u_list_insert(result, result->size, result_term);
                if (err) {
                    free(result_term);
                    return err;
                }
            }
        }

        current_1 = current_1->next;
    }

    return EXIT_SUCCESS;
}

err_t poly_mod(const poly *a, const poly *b, poly *result) {
    err_t err;
    u_list_node *current_1, *current_2;
    poly_term *term_1, *term_2, *result_term;

    if (a == NULL || b == NULL || result == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    current_1 = a->first;
    current_2 = b->first;

    if (current_2 != NULL) {
        term_2 = (poly_term *)current_2->data;
        if (term_2->coef == 0) {
            return ZERO_DIVISION;
        }
    }

    while (current_1 != NULL) {
        term_1 = (poly_term *)current_1->data;

        if (current_2 != NULL) {
            term_2 = (poly_term *)current_2->data;
            if (term_1->exp < term_2->exp) {
                return INVALID_INPUT_DATA;
            }
        }
        current_1 = current_1->next;
    }

    result->first = NULL;
    result->size = 0;

    current_1 = a->first;
    while (current_1 != NULL) {
        term_1 = (poly_term *)current_1->data;

        while (current_2 != NULL &&
               term_1->exp >= ((poly_term *)current_2->data)->exp) {
            term_2 = (poly_term *)current_2->data;

            if (term_1->exp >= term_2->exp) {
                result_term = (poly_term *)malloc(sizeof(poly_term));
                if (result_term == NULL) {
                    return MEMORY_ALLOCATION_ERROR;
                }

                result_term->coef = term_1->coef - term_2->coef;
                result_term->exp = term_1->exp - term_2->exp;

                err = u_list_insert(result, result->size, result_term);
                if (err) {
                    free(result_term);
                    return err;
                }
            }

            current_2 = current_2->next;
        }

        current_1 = current_1->next;
    }

    return EXIT_SUCCESS;
}

err_t poly_diff(poly *p) {
    int deleted = 0;
    err_t err = 0;
    if (p == NULL) {
        return DEREFERENCING_NULL_PTR;
    }
    u_list_node *current = NULL;
    poly_term *current_c = NULL;

    current = p->first;
    while (current != NULL) {
        current_c = (poly_term *)current->data;
        current_c->coef *= current_c->exp;
        current_c->exp--;
        if (current_c->exp < 0) {
            deleted = 1;
            current = current->next;
            err = u_list_delete_by_value(p, current_c, compare_terms);
            if (err) {
                return err;
            }
        }
        if (!deleted) {
            current = current->next;
        } else {
            deleted = 0;
        }
    }

    return EXIT_SUCCESS;
}
err_t poly_comp(const poly *a, const poly *b, poly *result) {
    u_list_node *current = NULL;
    err_t err = 0;
    poly_term *current_c = NULL;
    int term_result = 0;
    if (a == NULL || b == NULL || result == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    result->first = NULL;  // WARNING: if result it not free'd this is
                           // potential memory leak

    current = a->first;
    while (current != NULL) {
        current_c = (poly_term *)current->data;
        err = poly_eval(b, current_c->coef, &term_result);
        if (err) {
            return err;
        }
        poly_add_term(result, term_result, current_c->exp);
        current = current->next;
    }

    return EXIT_SUCCESS;
}

err_t poly_eval(const poly *p, int value, int *result) {
    u_list_node *current = NULL;
    poly_term *term = NULL;
    int term_value;
    if (p == NULL || result == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    *result = 0;

    current = p->first;
    while (current != NULL) {
        term = (poly_term *)current->data;

        term_value = (int)pow(value, term->exp);
        *result += term->coef * term_value;

        current = current->next;
    }

    return EXIT_SUCCESS;
}
int is_valid_char_in_poly(char c) {
    return isdigit(c) || c == '+' || c == '-' || c == 'x' || c == '^';
}

void print_poly(const poly *p) {
    if (p == NULL) {
        return;
    }
    u_list_const_traversion(p, print_poly_node);
    printf("\n");
    return;
}

void print_poly_node(const u_list_node *node) {
    if (node == NULL) return;
    const poly_term *term = node->data;

    if (term->coef > 0) printf("+");

    if (term->exp == 0) {
        printf("%d", term->coef);
    } else if (term->exp == 1) {
        if (term->coef == 1) {
            printf("x");
        } else if (term->coef == -1) {
            printf("-x");
        } else {
            printf("%dx", term->coef);
        }
    } else {
        if (term->coef == 1) {
            printf("x^%d", term->exp);
        } else if (term->coef == -1) {
            printf("-x^%d", term->exp);
        } else {
            printf("%dx^%d", term->coef, term->exp);
        }
    }
}

int compare_terms(const void *a, const void *b) {
    const poly_term *term_a = a;
    const poly_term *term_b = b;
    return term_b->exp - term_a->exp;
}

err_t poly_add_term(poly *poly, int coef, int exp) {
    err_t err;
    u_list_node *existing_node = NULL;
    poly_term *existing_term = NULL;
    if (poly == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    if (coef == 0) {
        return EXIT_SUCCESS;
    }

    poly_term term = {coef, exp};

    existing_node = NULL;
    err = u_list_get_node_by_value(poly, &term, compare_terms, &existing_node);
    if (err != NO_SUCH_ENTRY_IN_COLLECTION && err != EXIT_SUCCESS) {
        return err;
    }

    if (existing_node) {
        existing_term = (poly_term *)existing_node->data;
        existing_term->coef += coef;

        if (existing_term->coef == 0) {
            return u_list_delete_by_value(poly, existing_term, compare_terms);
        }

        return EXIT_SUCCESS;
    }

    return u_list_insert_sorted(poly, &term, compare_terms);
}

err_t poly_sub_term(poly *poly, int coef, int exp) {
    err_t err;
    u_list_node *existing_node = NULL;
    poly_term *existing_term = NULL;
    if (poly == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    if (coef == 0) {
        return EXIT_SUCCESS;
    }

    poly_term term = {coef, exp};

    existing_node = NULL;
    err = u_list_get_node_by_value(poly, &term, compare_terms, &existing_node);
    if (err != NO_SUCH_ENTRY_IN_COLLECTION && err != EXIT_SUCCESS) {
        return err;
    }

    if (existing_node) {
        existing_term = (poly_term *)existing_node->data;
        existing_term->coef -= coef;

        if (existing_term->coef == 0) {
            return u_list_delete_by_value(poly, existing_term, compare_terms);
        }

        return EXIT_SUCCESS;
    }

    term.coef = -coef;
    return u_list_insert_sorted(poly, &term, compare_terms);
}

int compare_terms_rev(const void *a, const void *b) {
    const poly_term *term_a = a;
    const poly_term *term_b = b;
    return term_a->exp - term_b->exp;
}

err_t start_execution(const char *filename) {
    int in_comment = 0;
    err_t err;
    char instruction[BUFSIZ];
    size_t instruction_len = 0;
    char *instruction_ptr;
    poly *current_sum = NULL;
    FILE *fin = NULL;
    if (filename == NULL) {
        return DEREFERENCING_NULL_PTR;
    }
    fin = fopen(filename, "r");
    if (fin == NULL) {
        return OPENING_THE_FILE_ERROR;
    }
    err = poly_init(&current_sum);
    if (err) {
        fclose(fin);
        return err;
    }

    while (fgets(instruction, sizeof(instruction), fin)) {
        instruction_len = 0;
        instruction_ptr = instruction;
        while (*instruction_ptr) {
            if (*instruction_ptr == '%') {
                *instruction_ptr = '\0';
                break;
            }
            instruction_ptr++;
            instruction_len += 1;
        }
        if (instruction_len > 0 && instruction[instruction_len - 1] == '\n') {
            instruction[instruction_len - 1] = '\0';
        }
        instruction_ptr = instruction;
        while (*instruction_ptr == ' ' || *instruction_ptr == '\t') {
            instruction_ptr++;
        }
        err = parse_instruction(instruction_ptr, current_sum, &in_comment);
        if (err) {
            fclose(fin);
            poly_free(current_sum);
            return err;
        }
    }

    fclose(fin);
    poly_free(current_sum);
    return EXIT_SUCCESS;
}

err_t parse_instruction(char *instruction, poly *current_sum, int *in_comment) {
    err_t err;
    int for_eval = 0;
    operation o;
    String poly_str = NULL;
    poly *poly1 = NULL, *poly2 = NULL;

    if (instruction == NULL || current_sum == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    err = remove_comments_from_line(&instruction, in_comment);

    while (*instruction == ' ' || *instruction == '\t') {
        instruction++;
    }

    if (*instruction == '\0') {
        return EXIT_SUCCESS;
    }

    if (strncmp(instruction, "Add(", 4) == 0) {
        o = Add;
        instruction += 4;
    } else if (strncmp(instruction, "Sub(", 4) == 0) {
        o = Sub;
        instruction += 4;
    } else if (strncmp(instruction, "Mult(", 5) == 0) {
        o = Mult;
        instruction += 5;
    } else if (strncmp(instruction, "Div(", 4) == 0) {
        o = Div;
        instruction += 4;
    } else if (strncmp(instruction, "Mod(", 4) == 0) {
        o = Mod;
        instruction += 4;
    } else if (strncmp(instruction, "Eval(", 5) == 0) {
        o = Eval;
        instruction += 5;
    } else if (strncmp(instruction, "Diff(", 5) == 0) {
        o = Diff;
        instruction += 5;
    } else if (strncmp(instruction, "Cmps(", 5) == 0) {
        o = Cmps;
        instruction += 5;
    } else {
        return INVALID_INPUT_DATA;
    }

    // Parse Eval operation
    if (o == Eval) {
        poly_str = string_init();
        if (poly_str == NULL) {
            return MEMORY_ALLOCATION_ERROR;
        }

        while (is_valid_char_in_poly(*instruction)) {
            err = string_add(&poly_str, *instruction);
            if (err) {
                string_free(poly_str);
                return err;
            }
            instruction++;
        }

        err = string_add(&poly_str, '\0');
        if (err) {
            string_free(poly_str);
            return err;
        }

        for_eval = atoi(poly_str);
        string_free(poly_str);

        if (*instruction == ')') {
            instruction++;
            while (*instruction == ' ' || *instruction == '\t') {
                instruction++;
            }
            if (*instruction == ';') {
                instruction++;
                while (*instruction != '\0') {
                    if (*instruction != ' ' && *instruction != '\t') {
                        return INVALID_INPUT_DATA;
                    }
                    instruction++;
                }
            } else {
                return INVALID_INPUT_DATA;
            }
            err = do_stuff_with_polys(NULL, NULL, current_sum, o, for_eval);
            if (err) {
                return err;
            }
            return EXIT_SUCCESS;
        }
    }

    // Parse polynomial arguments
    poly_str = string_init();
    if (poly_str == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }

    while (is_valid_char_in_poly(*instruction)) {
        err = string_add(&poly_str, *instruction);
        if (err) {
            string_free(poly_str);
            return err;
        }
        instruction++;
    }

    err = poly_from_string(&poly1, poly_str);
    string_free(poly_str);
    if (err) {
        return err;
    }

    if (*instruction == ')') {
        instruction++;
        while (*instruction == ' ' || *instruction == '\t') {
            instruction++;
        }

        if (*instruction == ';') {
            instruction++;
            while (*instruction != '\0') {
                if (*instruction != ' ' && *instruction != '\t') {
                    poly_free(poly1);
                    return INVALID_INPUT_DATA;
                }
                instruction++;
            }
        }

    } else if (*instruction == ',' || o != Diff) {
        poly_str = string_init();
        if (poly_str == NULL) {
            poly_free(poly1);
            return MEMORY_ALLOCATION_ERROR;
        }

        instruction++;
        while (is_valid_char_in_poly(*instruction)) {
            err = string_add(&poly_str, *instruction);
            if (err) {
                string_free(poly_str);
                poly_free(poly1);
                return err;
            }
            instruction++;
        }

        err = poly_from_string(&poly2, poly_str);
        string_free(poly_str);
        if (err) {
            poly_free(poly1);
            return err;
        }
    } else {
        poly_free(poly1);
        return INVALID_INPUT_DATA;
    }
    err = do_stuff_with_polys(poly1, poly2, current_sum, o, 0);
    if (err) {
        poly_free(poly1);
        poly_free(poly2);
        return err;
    }

    poly_free(poly1);
    poly_free(poly2);

    return EXIT_SUCCESS;
}

err_t do_stuff_with_polys(poly *poly1, poly *poly2, poly *current_sum,
                          operation op, int for_eval) {
    err_t err;
    int eval_result;
    if (current_sum == NULL || (op == Eval ? (0) : (poly1 == NULL))) {
        return DEREFERENCING_NULL_PTR;
    }
    if (op == Eval) {
        printf("Evaluating polinom: ");
    } else if (poly2 == NULL) {  // take from summator
        poly2 = poly1;
        poly1 = current_sum;
        current_sum = NULL;
    }
    if (op != Eval) {
        poly_free(current_sum);
        err = poly_init(&current_sum);
        if (err) {
            return err;
        }
    }
    switch (op) {
        case Add:
            err = poly_add(poly1, poly2, current_sum);
            if (err) {
                return err;
            }
            print_poly(current_sum);
            break;
        case Sub:
            err = poly_sub(poly1, poly2, current_sum);
            if (err) {
                return err;
            }
            print_poly(current_sum);
            break;
        case Mult:
            err = poly_mult(poly1, poly2, current_sum);
            if (err) {
                return err;
            }
            print_poly(current_sum);
            break;
        case Div:
            err = poly_div(poly1, poly2, current_sum);
            if (err) {
                return err;
            }
            print_poly(current_sum);
            break;
        case Mod:
            err = poly_mod(poly1, poly2, current_sum);
            if (err) {
                return err;
            }
            print_poly(current_sum);
            break;
        case Eval:
            err = poly_eval(current_sum, for_eval, &eval_result);
            if (err) {
                return err;
            }
            printf("%d\n", eval_result);
            break;
        case Diff:
            err = poly_diff(poly1);
            if (err) {
                return err;
            }
            current_sum = poly1;
            poly1 = NULL;
            print_poly(current_sum);
            break;
        case Cmps:
            err = poly_comp(poly1, poly2, current_sum);
            if (err) {
                return err;
            }
            print_poly(current_sum);
            break;
        default:
            break;
    }

    return EXIT_SUCCESS;
}

err_t remove_comments_from_line(char **line, int *in_comment) {
    if (line == NULL || *line == NULL || in_comment == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    char *read_ptr = *line;
    char *write_ptr = *line;

    while (*read_ptr) {
        if (*read_ptr == '[' && !*in_comment) {
            *in_comment = 1;  // Entering comment
        }
        if (*read_ptr == ']' && *in_comment) {
            *in_comment = 0;  // Exiting comment
            read_ptr++;       // Skip closing bracket
            continue;
        }

        if (!*in_comment) {
            *write_ptr = *read_ptr;  // Copy non-comment characters
            write_ptr++;
        }
        read_ptr++;
    }

    *write_ptr = '\0';  // Null-terminate the string
    return EXIT_SUCCESS;
}
