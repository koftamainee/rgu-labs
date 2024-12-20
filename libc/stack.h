#ifndef STACK_H_
#define STACK_H_
#include "u_list.h"

typedef u_list stack;
typedef u_list_node stack_item;

err_t stack_init(stack **s, size_t elem_size, void (*elem_destructor)(void *));
void stack_free(stack *s);

err_t stack_push(stack *s, const void *data);
err_t stack_pop(stack *s);
err_t stack_top(stack *s, stack_item **ret_item);

int stack_is_empty(stack *s);
#endif
