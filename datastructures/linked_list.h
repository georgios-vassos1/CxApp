#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct ListNode {
    char *word;
    int num_reviews;
    int sum_ratings;
    struct ListNode *prev;
    struct ListNode *next;
} ListNode;

ListNode* list_create_node(const char *word, int num_reviews, int sum_ratings);
void list_insert_end(ListNode **head, const char *word, int num_reviews, int sum_ratings);
void list_display(const ListNode *head);
void list_free(ListNode **head);

#endif // LINKED_LIST_H
