#include "../u_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

err_t u_list_init(u_list **l, size_t elem_size,
                  void (*elem_destructor)(void *)) {
    if (l == NULL || elem_destructor == NULL) {
        return DEREFERENCING_NULL_PTR;
    }
    *l = (u_list *)malloc(sizeof(u_list));
    if (*l == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }

    (*l)->first = NULL;
    (*l)->last = NULL;
    (*l)->size = 0;
    (*l)->elem_destructor = elem_destructor;
    (*l)->elem_size = elem_size;

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
    free(l);
    return;
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
        if (l->size == 0) {  // first element also last element
            l->last = l->first;
        }
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
    new->next = NULL;
    l->size++;
    l->last = father->next;

    return EXIT_SUCCESS;
}

err_t u_list_push_back(u_list *l, const void *data) {
    if (l == NULL || data == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    u_list_node *new_node = (u_list_node *)malloc(sizeof(u_list_node));
    if (new_node == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }

    new_node->data = malloc(l->elem_size);
    if (new_node->data == NULL) {
        free(new_node);
        return MEMORY_ALLOCATION_ERROR;
    }

    memcpy(new_node->data, data, l->elem_size);
    new_node->next = NULL;

    if (l->last == NULL) {
        l->first = new_node;
        l->last = new_node;
    } else {
        l->last->next = new_node;
        l->last = new_node;
    }

    l->size++;

    return EXIT_SUCCESS;
}

err_t u_list_insert_sorted(u_list *l, const void *data,
                           int (*comp)(const void *, const void *)) {
    if (l == NULL || data == NULL || comp == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    u_list_node *new = (u_list_node *)malloc(sizeof(u_list_node));
    if (new == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }

    new->data = malloc(l->elem_size);
    if (new->data == NULL) {
        free(new);
        return MEMORY_ALLOCATION_ERROR;
    }

    memcpy(new->data, data, l->elem_size);  // deep copy

    // Insert at the beginning if the list is empty or the new data is less
    if (l->first == NULL || comp(new->data, l->first->data) <= 0) {
        new->next = l->first;
        l->first = new;
        l->size++;
        if (l->size == 1) {  // first element is also last element
            l->last = new;
        }
        return EXIT_SUCCESS;
    }

    // Find the correct position to insert the new node
    u_list_node *item = l->first;
    u_list_node *father = NULL;

    while (item != NULL && comp(new->data, item->data) > 0) {
        father = item;
        item = item->next;
    }

    // Insert the new node
    father->next = new;
    new->next = item;
    l->size++;

    // Update last node if inserted at the end
    if (new->next == NULL) {
        l->last = new;
    }

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
        l->first = item->next;
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

    // Check the first element
    if (l->first != NULL && comp(l->first->data, target) == 0) {
        item = l->first;
        l->first = l->first->next;
        l->elem_destructor(item->data);
        free(item);
        l->size--;
        return EXIT_SUCCESS;
    }

    // Check the rest of the list
    item = l->first;  // Start at the head
    while (item != NULL && item->next != NULL) {
        if (comp(item->next->data, target) == 0) {
            u_list_node *temp = item->next;
            item->next = item->next->next;
            l->elem_destructor(temp->data);
            free(temp);
            l->size--;
            return EXIT_SUCCESS;
        }
        item = item->next;
    }

    return INDEX_OUT_OF_BOUNDS;  // Target not found
}

err_t u_list_const_traversion(const u_list *l,
                              void (*callback)(const u_list_node *)) {
    void print_node_ulist(const u_list_node *n);
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

void swap_nodes_data(u_list_node *a, u_list_node *b) {
    void *temp = a->data;
    a->data = b->data;
    b->data = temp;
}

static u_list_node *u_list_split(u_list_node *head) {
    if (!head || !head->next) return head;

    u_list_node *slow = head;
    u_list_node *fast = head->next;

    // Move `slow` one step and `fast` two steps at a time
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    u_list_node *mid = slow->next;
    slow->next = NULL;  // Split the list into two halves
    return mid;
}

static u_list_node *u_list_merge(u_list_node *left, u_list_node *right,
                                 int (*comp)(const void *, const void *)) {
    if (!left) return right;
    if (!right) return left;

    if (comp(left->data, right->data) <= 0) {
        left->next = u_list_merge(left->next, right, comp);
        return left;
    } else {
        right->next = u_list_merge(left, right->next, comp);
        return right;
    }
}

u_list_node *u_list_merge_sort(u_list_node *head,
                               int (*comp)(const void *, const void *)) {
    if (!head || !head->next) return head;
    u_list_node *mid = u_list_split(head);

    u_list_node *left = u_list_merge_sort(head, comp);
    u_list_node *right = u_list_merge_sort(mid, comp);

    return u_list_merge(left, right, comp);
}

err_t u_list_sort(u_list *l, int (*comp)(const void *, const void *)) {
    if (l == NULL) {
        return DEREFERENCING_NULL_PTR;
    }

    l->first = u_list_merge_sort(l->first, comp);

    u_list_node *temp = l->first;
    l->size = 0;
    while (temp) {
        l->size++;
        temp = temp->next;
    }

    return EXIT_SUCCESS;
}
