#include <stdio.h>

int main(int argc, char *argv[]) {

    // Skip the program name (first argument)
    for (int i = 1; i < argc; i++) {
        // Print each argument followed by a space
        printf("%s", argv[i]);
    }

    return 0;
}

