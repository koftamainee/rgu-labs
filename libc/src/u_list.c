#include "../u_list.h"

#include <stdlib.h>
#include <string.h>

err_t u_list_init(u_list *l, size_t elem_size,
                  void (*elem_destructor)(void *)) {
    if (l == NULL || elem_destructor == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    l->first = NULL;
    l->size = 0;
    l->elem_destructor = elem_destructor;
    l->elem_size = elem_size;

    return EXIT_SUCCESS;
}

void u_list_free(u_list *l) {
    u_list_node *item = NULL, *next = NULL;
    if (l == NULL) {
        return;
    }
    item = l->first;
    while (item != NULL) {
        l->elem_destructor(item->data);
        next = item->next;
        free(item);
        item = next;
    }
}

err_t u_list_insert(u_list *l, size_t index, const void *data) {
    size_t i = 0;
    u_list_node *item, *father, *new;
    if (l == NULL || data == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    new = (u_list_node *)malloc(sizeof(u_list_node));
    if (new == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }
    new->data = malloc(l->elem_size);
    if (new->data == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }
    memcpy(new->data, data, l->elem_size);  // deep dark copy

    if (index == 0) {
        new->next = l->first;
        l->first = new;
        l->size++;
        return EXIT_SUCCESS;
    }
    item = l->first->next;
    father = l->first;
    while (item != NULL) {
        if (i == index) {
            father->next = new;
            new->next = item;
            l->size++;
            return EXIT_SUCCESS;
        }
        father = item;
        item = item->next;
    }
    // too big index
    father->next = new;
    new->next = item;
    l->size++;

    return EXIT_SUCCESS;
}

err_t u_list_get_node_by_index(u_list *l, size_t index,
                               u_list_node **ret_node) {
    size_t i = 0;
    u_list_node *item;
    if (l == NULL || ret_node == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    item = l->first;
    while (item != NULL) {
        if (i == index) {
            *ret_node = item;
            return EXIT_SUCCESS;
        }
        i++;
        item = item->next;
    }
    return INDEX_OUT_OF_BOUNDS;
}

err_t u_list_get_node_by_value(u_list *l, const void *target,
                               int (*comp)(const void *, const void *),
                               u_list_node **ret_node) {
    u_list_node *item;
    if (l == NULL || target == NULL || comp == NULL || ret_node == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    item = l->first;
    while (item != NULL) {
        if (comp(item->data, target) == 0) {
            *ret_node = item;
            return EXIT_SUCCESS;
        }
        item = item->next;
    }

    return NO_SUCH_ENTRY_IN_COLLECTION;
}

err_t u_list_delete_by_index(u_list *l, size_t index) {
    size_t i = 0;
    u_list_node *item, *father;
    if (l == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    if (index == 0) {
        item = l->first;
        l->first = NULL;
        l->elem_destructor(item->data);
        free(item);
        l->size--;
        return EXIT_SUCCESS;
    }
    item = l->first->next;
    father = l->first;
    while (item != NULL) {
        if (i == index) {
            father->next = item->next;
            l->elem_destructor(item->data);
            free(item);
            l->size--;
            return EXIT_SUCCESS;
        }
        father = item;
        item = item->next;
    }
    // too big index

    return INDEX_OUT_OF_BOUNDS;
}
err_t u_list_delete_by_value(u_list *l, const void *target,
                             int (*comp)(const void *, const void *)) {
    u_list_node *item, *father;
    if (l == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    if (comp(l->first, target) == 0) {
        item = l->first;
        l->first = NULL;
        l->elem_destructor(item->data);
        free(item);
        l->size--;
        return EXIT_SUCCESS;
    }
    item = l->first->next;
    father = l->first;
    while (item != NULL) {
        if (comp(item, target) == 0) {
            father->next = item->next;
            l->elem_destructor(item->data);
            free(item);
            l->size--;
            return EXIT_SUCCESS;
        }
        father = item;
        item = item->next;
    }
    // too big index

    return INDEX_OUT_OF_BOUNDS;
}

err_t u_list_const_traversion(u_list *l,
                              void *(callback)(const u_list_node *)) {
    u_list_node *item;
    if (l == NULL || callback == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    item = l->first;
    while (item != NULL) {
        callback(item);
        item = item->next;
    }

    return EXIT_SUCCESS;
}

err_t u_list_traversion(u_list *l, void (*callback)(u_list_node *)) {
    u_list_node *item;
    if (l == NULL || callback == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    item = l->first;
    while (item != NULL) {
        callback(item);
        item = item->next;
    }

    return EXIT_SUCCESS;
}
