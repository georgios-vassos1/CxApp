#include "linked_list.h"

int main() {
    Node *head = NULL; // Initialize an empty list

    // Create the sentinel node
    head = createNode("", 0, 0);
    head->next = head; // Make it circular
    head->prev = head; // Make it circular

    // Example usage:
    insertEnd(&head, "example", 5, 20);
    insertEnd(&head, "test", 3, 15);
    insertEnd(&head, "list", 8, 30);

    // Display the list
    displayList(head);

    // Free memory allocated for the list
    freeList(&head);

    return 0;
}
