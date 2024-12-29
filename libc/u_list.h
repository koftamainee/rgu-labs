#ifndef ULIST_H_
#define ULIST_H_

#include <stdlib.h>

#include "errors.h"

typedef struct u_list_node {
    struct u_list_node *next;
    void *data;
} u_list_node;

typedef struct u_list {
    u_list_node *first;
    u_list_node *last;  // for queue functional
    size_t size;
    size_t elem_size;
    void (*elem_destructor)(void *);
} u_list;

err_t u_list_init(u_list **l, size_t elem_size,
                  void (*elem_destructor)(void *));
void u_list_free(u_list *l);

err_t u_list_insert(u_list *l, size_t index, const void *data);
err_t u_list_push_back(u_list *l, const void *data);
err_t u_list_insert_sorted(u_list *l, const void *data,
                           int (*comp)(const void *, const void *));

err_t u_list_get_node_by_index(u_list *l, size_t index, u_list_node **ret_node);
err_t u_list_get_node_by_value(u_list *l, const void *target,
                               int (*comp)(const void *, const void *),
                               u_list_node **ret_node);

err_t u_list_delete_by_index(u_list *l, size_t index);
err_t u_list_delete_by_value(u_list *l, const void *target,
                             int (*comp)(const void *, const void *));

err_t u_list_const_traversion(const u_list *l,
                              void (*callback)(const u_list_node *));
err_t u_list_traversion(u_list *l, void (*callback)(u_list_node *));

err_t u_list_sort(u_list *l, int (*comp)(const void *, const void *));
#endif
