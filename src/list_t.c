#include "list_t.h"
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

typedef struct xor_node {
    void *data;
    struct xor_node *np;
} xor_node;

typedef struct {
    xor_node *head;
    xor_node *tail;
    size_t length;
} xor_list;

#define XOR(a, b) ((xor_node *)((uintptr_t)(a) ^ (uintptr_t)(b)))

list_t list_new() {
    xor_list *lst = malloc(sizeof(xor_list));
    if (!lst) return NULL;
    lst->head = lst->tail = NULL;
    lst->length = 0;
    return (list_t)lst;
}

void list_add(list_t *listp, void *x) {
    xor_list *lst = (xor_list *)(*listp);
    xor_node *new_node = malloc(sizeof(xor_node));
    xor_node *prev;

    new_node->data = x;
    new_node->np = XOR(lst->tail, NULL);

    if (!lst->head) {
        lst->head = lst->tail = new_node;
    } else {
        prev = XOR(lst->tail->np, NULL);
        lst->tail->np = XOR(prev, new_node);
        lst->tail = new_node;
    }
    lst->length++;
}

void *list_get(list_t list, size_t index) {
    xor_list *lst = (xor_list *)list;
    xor_node *prev = NULL;
    xor_node *curr = lst->head;
    xor_node *next;
    size_t i;

    for (i = 0; i < index; i++) {
        if (!curr) return NULL;
        next = XOR(prev, curr->np);
        prev = curr;
        curr = next;
    }
    return curr ? curr->data : NULL;
}

size_t list_len(list_t list) {
    xor_list *lst = (xor_list *)list;
    return lst->length;
}

void list_free(list_t list) {
    xor_list *lst = (xor_list *)list;
    xor_node *prev = NULL;
    xor_node *curr = lst->head;
    xor_node *next;

    while (curr) {
        next = XOR(prev, curr->np);
        prev = curr;
        free(curr);
        curr = next;
    }
    
    free(lst);
}
