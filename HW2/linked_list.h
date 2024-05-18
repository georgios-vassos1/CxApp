#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
