#include "../calculator.h"

#include <ctype.h>
#include <stdlib.h>

#include "../stack.h"

err_t infix_to_postfix(const String infix_exp, int (*lexem_selector)(int c),
                       int (*priority_mapper)(int c), String *postfix_exp) {
    if (infix_exp == NULL || lexem_selector == NULL ||
        priority_mapper == NULL || postfix_exp == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    String result = NULL;
    stack *operators = NULL;
    char for_stack = 0, prev = ' ';
    err_t err = 0;
    size_t i;
    stack_item *p_for_stack = NULL;

    result = string_init();
    if (result == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }
    err = stack_init(&operators, sizeof(char), free);
    if (err) {
        string_free(result);
        return err;
    }
    for_stack = '(';
    err = stack_push(operators, &for_stack);
    if (err) {
        string_free(result);
        stack_free(operators);
    }

    stack_free(operators);

    return EXIT_SUCCESS;
}
