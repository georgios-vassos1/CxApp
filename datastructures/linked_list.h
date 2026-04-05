#ifndef LINKED_LIST_H
#define LINKED_LIST_H

// Structure for list node
typedef struct Node {
    char *word;
    int num_reviews;
    int sum_ratings;
    struct Node *prev;
    struct Node *next;
} Node;

Node* createNode(char *word, int num_reviews, int sum_ratings);
void insertEnd(Node **head, char *word, int num_reviews, int sum_ratings);
void displayList(Node *head);
void freeList(Node **head);

#endif // LINKED_LIST_H
