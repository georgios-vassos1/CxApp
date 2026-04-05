#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "bsTree.h"

BSTNode* bst_create_node(int key) {
    BSTNode *newNode = malloc(sizeof(BSTNode));
    if (!newNode) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    newNode->key = key;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Recursive insert (ignores duplicates)
BSTNode* bst_insert(BSTNode *node, int key) {
    if (node == NULL) {
        return bst_create_node(key);
    }
    if (key < node->key) {
        node->left = bst_insert(node->left, key);
    } else if (key > node->key) {
        node->right = bst_insert(node->right, key);
    }
    return node;
}

// Iterative insert (ignores duplicates)
BSTNode* bst_insert_iter(BSTNode *root, int key) {
    BSTNode *newNode = bst_create_node(key);
    if (root == NULL)
        return newNode;

    BSTNode *current = root;
    while (current != NULL) {
        if (key < current->key) {
            if (current->left == NULL) {
                current->left = newNode;
                break;
            }
            current = current->left;
        } else if (key > current->key) {
            if (current->right == NULL) {
                current->right = newNode;
                break;
            }
            current = current->right;
        } else {
            free(newNode);
            break;
        }
    }

    return root;
}

// Recursive search
BSTNode* bst_search(BSTNode *root, int key) {
    if (root == NULL || root->key == key) {
        return root;
    }
    if (key < root->key) {
        return bst_search(root->left, key);
    }
    return bst_search(root->right, key);
}

// Iterative search
BSTNode* bst_search_iter(BSTNode *root, int key) {
    while (root != NULL && root->key != key) {
        if (key < root->key)
            root = root->left;
        else
            root = root->right;
    }
    return root;
}

static BSTNode* min_value_node(BSTNode *node) {
    BSTNode *current = node;
    while (current && current->left != NULL) {
        current = current->left;
    }
    return current;
}

// Recursive delete
BSTNode* bst_delete(BSTNode *root, int key) {
    if (root == NULL) {
        return root;
    }
    if (key < root->key) {
        root->left = bst_delete(root->left, key);
    } else if (key > root->key) {
        root->right = bst_delete(root->right, key);
    } else {
        if (root->left == NULL) {
            BSTNode *temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            BSTNode *temp = root->left;
            free(root);
            return temp;
        }
        BSTNode *temp = min_value_node(root->right);
        root->key = temp->key;
        root->right = bst_delete(root->right, temp->key);
    }
    return root;
}

// Iterative delete
BSTNode* bst_delete_iter(BSTNode *root, int key) {
    BSTNode *parent = NULL;
    BSTNode *current = root;

    while (current != NULL && current->key != key) {
        parent = current;
        if (key < current->key)
            current = current->left;
        else
            current = current->right;
    }

    if (current == NULL)
        return root;

    // Two children: replace with inorder successor
    if (current->left != NULL && current->right != NULL) {
        BSTNode *successorParent = current;
        BSTNode *successor = current->right;
        while (successor->left != NULL) {
            successorParent = successor;
            successor = successor->left;
        }
        current->key = successor->key;
        current = successor;
        parent = successorParent;
    }

    // At most one child
    BSTNode *child = (current->left != NULL) ? current->left : current->right;

    if (parent == NULL) {
        free(current);
        return child;
    }

    if (parent->left == current)
        parent->left = child;
    else
        parent->right = child;

    free(current);
    return root;
}

// Recursive traversals
void bst_inorder(const BSTNode *root) {
    if (root != NULL) {
        bst_inorder(root->left);
        printf("%d ", root->key);
        bst_inorder(root->right);
    }
}

void bst_preorder(const BSTNode *root) {
    if (root != NULL) {
        printf("%d ", root->key);
        bst_preorder(root->left);
        bst_preorder(root->right);
    }
}

void bst_postorder(const BSTNode *root) {
    if (root != NULL) {
        bst_postorder(root->left);
        bst_postorder(root->right);
        printf("%d ", root->key);
    }
}

// Iterative inorder traversal
void bst_inorder_iter(const BSTNode *root) {
    if (root == NULL)
        return;

    Stack stack;
    stack_init(&stack, sizeof(const BSTNode *));

    const BSTNode *current = root;

    while (current != NULL || !stack_is_empty(&stack)) {
        while (current != NULL) {
            stack_push(&stack, &current);
            current = current->left;
        }

        const BSTNode **ptr = stack_pop(&stack);
        current = *ptr;
        free(ptr);
        printf("%d ", current->key);

        current = current->right;
    }

    stack_free(&stack);
}

// Iterative preorder traversal
void bst_preorder_iter(const BSTNode *root) {
    if (root == NULL)
        return;

    Stack stack;
    stack_init(&stack, sizeof(const BSTNode *));
    stack_push(&stack, &root);

    while (!stack_is_empty(&stack)) {
        const BSTNode **ptr = stack_pop(&stack);
        const BSTNode *current = *ptr;
        free(ptr);
        printf("%d ", current->key);
        if (current->right != NULL)
            stack_push(&stack, &current->right);
        if (current->left != NULL)
            stack_push(&stack, &current->left);
    }

    stack_free(&stack);
}

// Iterative postorder traversal (two-stack approach)
void bst_postorder_iter(const BSTNode *root) {
    if (root == NULL)
        return;

    Stack s1, s2;
    stack_init(&s1, sizeof(const BSTNode *));
    stack_init(&s2, sizeof(const BSTNode *));

    stack_push(&s1, &root);
    while (!stack_is_empty(&s1)) {
        const BSTNode **ptr = stack_pop(&s1);
        const BSTNode *current = *ptr;
        free(ptr);
        stack_push(&s2, &current);
        if (current->left != NULL)
            stack_push(&s1, &current->left);
        if (current->right != NULL)
            stack_push(&s1, &current->right);
    }

    while (!stack_is_empty(&s2)) {
        const BSTNode **ptr = stack_pop(&s2);
        printf("%d ", (*ptr)->key);
        free(ptr);
    }

    stack_free(&s1);
    stack_free(&s2);
}

void bst_free(BSTNode *root) {
    if (root != NULL) {
        bst_free(root->left);
        bst_free(root->right);
        free(root);
    }
}

void bst_print(const BSTNode *root, int level, const char *prefix) {
    if (root != NULL) {
        printf("%*s%s%d\n", level * 4, "", prefix, root->key);
        bst_print(root->left, level + 1, "L-- ");
        bst_print(root->right, level + 1, "R-- ");
    }
}
