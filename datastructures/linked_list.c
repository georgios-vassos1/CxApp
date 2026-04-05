#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linked_list.h"

ListNode* list_create_node(const char *word, int num_reviews, int sum_ratings) {
    ListNode *newNode = malloc(sizeof(ListNode));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->word = strdup(word);
    if (newNode->word == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(newNode);
        exit(EXIT_FAILURE);
    }
    newNode->num_reviews = num_reviews;
    newNode->sum_ratings = sum_ratings;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

void list_insert_end(ListNode **head, const char *word, int num_reviews, int sum_ratings) {
    ListNode *newNode = list_create_node(word, num_reviews, sum_ratings);
    if (*head == NULL) {
        *head = newNode;
        (*head)->prev = newNode;
        (*head)->next = newNode;
    } else {
        ListNode *last = (*head)->prev;
        last->next = newNode;
        newNode->prev = last;
        newNode->next = *head;
        (*head)->prev = newNode;
    }
}

void list_display(const ListNode *head) {
    if (head == NULL) {
        printf("List is empty\n");
        return;
    }
    const ListNode *temp = head;
    do {
        printf("Word: %s, Num_reviews: %d, Sum_ratings: %d\n",
               temp->word, temp->num_reviews, temp->sum_ratings);
        temp = temp->next;
    } while (temp != head);
}

void list_free(ListNode **head) {
    if (*head == NULL)
        return;
    ListNode *current = (*head)->next;
    ListNode *next;
    while (current != *head) {
        next = current->next;
        free(current->word);
        free(current);
        current = next;
    }
    free((*head)->word);
    free(*head);
    *head = NULL;
}
