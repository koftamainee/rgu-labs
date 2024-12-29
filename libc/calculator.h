#ifndef CALCULATOR_H_

#include "cstring.h"
#include "errors.h"

err_t infix_to_postfix(const String infix_exp, int (*lexem_selector)(int c),
                       int (*priority_mapper)(int c), String *postfix_exp);

#endif  // CALCULATOR_H_
