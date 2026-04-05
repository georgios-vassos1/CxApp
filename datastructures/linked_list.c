#include <stdio.h>
#include <stdlib.h>

#include "linked_list.h"

List* list_create(void (*free_data)(void *)) {
    List *list = malloc(sizeof(List));
    if (!list) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    list->head = NULL;
    list->count = 0;
    list->free_data = free_data;
    return list;
}

static ListNode* create_node(void *data) {
    ListNode *n = malloc(sizeof(ListNode));
    if (!n) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    n->data = data;
    n->prev = NULL;
    n->next = NULL;
    return n;
}

void list_insert_end(List *list, void *data) {
    ListNode *n = create_node(data);
    if (list->head == NULL) {
        list->head = n;
        n->prev = n;
        n->next = n;
    } else {
        ListNode *last = list->head->prev;
        last->next = n;
        n->prev = last;
        n->next = list->head;
        list->head->prev = n;
    }
    list->count++;
}

void list_display(const List *list, void (*print_fn)(const void *)) {
    if (list->head == NULL) {
        printf("List is empty\n");
        return;
    }
    const ListNode *cur = list->head;
    do {
        print_fn(cur->data);
        cur = cur->next;
    } while (cur != list->head);
}

void list_free(List *list) {
    if (list->head) {
        ListNode *cur = list->head->next;
        while (cur != list->head) {
            ListNode *next = cur->next;
            if (list->free_data) list->free_data(cur->data);
            free(cur);
            cur = next;
        }
        if (list->free_data) list->free_data(list->head->data);
        free(list->head);
    }
    free(list);
}
