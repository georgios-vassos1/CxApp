#include <stdio.h>
#include <stdlib.h>

#include "linked_list.h"
#include "cxds_internal.h"

/* ── struct definitions (private) ────────────────────────────────── */

typedef struct ListNode {
    void *data;
    struct ListNode *prev;
    struct ListNode *next;
} ListNode;

struct List {
    ListNode *head;
    size_t count;
    void (*free_data)(void *);
};

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
    if (!list) return -1;
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
    if (!list) return -1;
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
    if (!list || !list->head) return NULL;
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
    if (!list || !list->head) return -1;
    ListNode *cur = list->head;
    do {
        if (cmp(cur->data, probe) == 0) {
            if (list->count == 1) {
                list->head = NULL;
            } else {
                cur->prev->next = cur->next;
                cur->next->prev = cur->prev;
                if (cur == list->head)
                    list->head = cur->next;
            }
            free_data_if(list->free_data, cur->data);
            free(cur);
            list->count--;
            return 0;
        }
        cur = cur->next;
    } while (cur != list->head);
    return -1;
}

void* list_get(const List *list, size_t index) {
    if (!list || index >= list->count) return NULL;
    const ListNode *cur;
    if (index <= list->count / 2) {
        /* traverse forward from head */
        cur = list->head;
        for (size_t i = 0; i < index; i++)
            cur = cur->next;
    } else {
        /* traverse backward from tail */
        cur = list->head->prev;
        for (size_t i = list->count - 1; i > index; i--)
            cur = cur->prev;
    }
    return cur->data;
}

size_t list_size(const List *list) {
    return list ? list->count : 0;
}

void list_display(const List *list, void (*print_fn)(const void *)) {
    if (!list || !list->head) return;
    const ListNode *cur = list->head;
    do {
        print_fn(cur->data);
        cur = cur->next;
    } while (cur != list->head);
}

void list_free(List *list) {
    if (!list) return;
    if (list->head) {
        ListNode *cur = list->head->next;
        while (cur != list->head) {
            ListNode *next = cur->next;
            free_data_if(list->free_data, cur->data);
            free(cur);
            cur = next;
        }
        free_data_if(list->free_data, list->head->data);
        free(list->head);
    }
    free(list);
}
