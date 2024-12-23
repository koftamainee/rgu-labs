#include "../stack.h"
/* NULL pointers are not handling bc u_list functional handle them already */

err_t stack_init(stack **s, size_t elem_size, void (*elem_destructor)(void *)) {
    return u_list_init(s, elem_size, elem_destructor);
}

void stack_free(stack *s) { u_list_free(s); }

err_t stack_push(stack *s, const void *data) {
    return u_list_insert(s, 0, data);
}
err_t stack_pop(stack *s) { return u_list_delete_by_index(s, 0); }
err_t stack_top(stack *s, stack_item **ret_item) {
    if (stack_is_empty(s)) {
        return STACK_IS_EMPTY;
    }
    *ret_item = s->first;
    return EXIT_SUCCESS;
}

int stack_is_empty(stack *s) { return s == NULL || s->first == NULL; }
