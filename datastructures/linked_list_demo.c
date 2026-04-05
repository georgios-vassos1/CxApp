#include <stdio.h>
#include "linked_list.h"

int main(void) {
    ListNode *head = NULL;

    list_insert_end(&head, "example", 5, 20);
    list_insert_end(&head, "test", 3, 15);
    list_insert_end(&head, "list", 8, 30);

    list_display(head);

    list_free(&head);

    return 0;
}
