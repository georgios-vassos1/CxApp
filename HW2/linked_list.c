#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linked_list.h"

// Function to create a new node
Node* createNode(char *word, int num_reviews, int sum_ratings) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newNode->word = strdup(word); // Dynamically allocate memory for word
    newNode->num_reviews = num_reviews;
    newNode->sum_ratings = sum_ratings;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

// Function to insert a node at the end of the list
void insertEnd(Node **head, char *word, int num_reviews, int sum_ratings) {
    Node *newNode = createNode(word, num_reviews, sum_ratings);
    if (*head == NULL) { // If list is empty
        *head = newNode;
        (*head)->prev = newNode;
        (*head)->next = newNode;
    } else {
        Node *last = (*head)->prev; // Last node
        last->next = newNode;
        newNode->prev = last;
        newNode->next = *head;
        (*head)->prev = newNode;
    }
}

// Function to display the list
void displayList(Node *head) {
    if (head == NULL) {
        printf("List is empty\n");
        return;
    }
    Node *temp = head;
    do {
        printf("Word: %s, Num_reviews: %d, Sum_ratings: %d\n", temp->word, temp->num_reviews, temp->sum_ratings);
        temp = temp->next;
    } while(temp != head);
}

// Function to free memory allocated for the list
void freeList(Node **head) {
    if (*head == NULL)
        return;
    Node *current = (*head)->next; // Start from the first node
    Node *next;
    while (current != *head) { // Iterate until reaching the sentinel node
        next = current->next;
        free(current->word); // Free dynamically allocated word
        free(current);
        current = next;
    }
    free(*head); // Free the sentinel node
    *head = NULL;
}

