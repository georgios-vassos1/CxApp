#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

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

List*  list_create(void (*free_data)(void *));
int    list_insert_end(List *list, void *data);
int    list_insert_front(List *list, void *data);
void*  list_search(const List *list, const void *probe,
                   int (*cmp)(const void *, const void *));
int    list_delete(List *list, const void *probe,
                   int (*cmp)(const void *, const void *));
void*  list_get(const List *list, size_t index);
size_t list_size(const List *list);
void   list_display(const List *list, void (*print_fn)(const void *));
void   list_free(List *list);

#ifdef __cplusplus
}
#endif

#endif /* LINKED_LIST_H */
