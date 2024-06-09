#include <stdio.h>
#include <unistd.h>

void printNode(char node, int level) {
    for (int i = 0; i < level; i++) {
        putchar('-');
    }
    putchar(node);
    putchar('\n');
}

int main(int argc, char* argv[]) {
    // Print as tree
    fork();
    printNode('1', 0);
    fork();
    printNode('2', 1);
    fork();
    printNode('3', 2);

    return 0;
}
