#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stddef.h>

typedef struct ListNode {
    void *data;
    struct ListNode *prev;
    struct ListNode *next;
} ListNode;

typedef struct List {
    ListNode *head;
    size_t count;
    void (*free_data)(void *);
} List;

List* list_create(void (*free_data)(void *));
void  list_insert_end(List *list, void *data);
void  list_display(const List *list, void (*print_fn)(const void *));
void  list_free(List *list);

#endif /* LINKED_LIST_H */
