#include <stdio.h>
#include <stdlib.h>

#include "linked_list.h"

List* list_create(void (*free_data)(void *)) {
    List *list = malloc(sizeof(List));
    if (!list) return NULL;
    list->head = NULL;
    list->count = 0;
    list->free_data = free_data;
    return list;
}

static ListNode* create_node(void *data) {
    ListNode *n = malloc(sizeof(ListNode));
    if (!n) return NULL;
    n->data = data;
    n->prev = NULL;
    n->next = NULL;
    return n;
}

int list_insert_end(List *list, void *data) {
    ListNode *n = create_node(data);
    if (!n) return -1;
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
    return 0;
}

int list_insert_front(List *list, void *data) {
    ListNode *n = create_node(data);
    if (!n) return -1;
    if (list->head == NULL) {
        list->head = n;
        n->prev = n;
        n->next = n;
    } else {
        ListNode *last = list->head->prev;
        n->next = list->head;
        n->prev = last;
        last->next = n;
        list->head->prev = n;
        list->head = n;
    }
    list->count++;
    return 0;
}

void* list_search(const List *list, const void *probe,
                  int (*cmp)(const void *, const void *))
{
    if (list->head == NULL) return NULL;
    const ListNode *cur = list->head;
    do {
        if (cmp(cur->data, probe) == 0)
            return cur->data;
        cur = cur->next;
    } while (cur != list->head);
    return NULL;
}

int list_delete(List *list, const void *probe,
                int (*cmp)(const void *, const void *))
{
    if (list->head == NULL) return -1;
    ListNode *cur = list->head;
    do {
        if (cmp(cur->data, probe) == 0) {
            if (list->count == 1) {
                /* last element */
                list->head = NULL;
            } else {
                cur->prev->next = cur->next;
                cur->next->prev = cur->prev;
                if (cur == list->head)
                    list->head = cur->next;
            }
            if (list->free_data) list->free_data(cur->data);
            free(cur);
            list->count--;
            return 0;
        }
        cur = cur->next;
    } while (cur != list->head);
    return -1;
}

void* list_get(const List *list, size_t index) {
    if (index >= list->count) return NULL;
    const ListNode *cur = list->head;
    for (size_t i = 0; i < index; i++)
        cur = cur->next;
    return cur->data;
}

size_t list_size(const List *list) {
    return list->count;
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
