#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int fd[2];
    char str[64];

    pipe(fd);
    if (!fork()) {
        // Child process
        dup2(fd[1], STDOUT_FILENO); /* redirect stdout */
        close(fd[0]);
        close(fd[1]);
        execl("./add", "add", "123", "456", NULL);
        return 1; // execl will only return if there's an error
    }

    // Parent process
    dup2(fd[0], STDIN_FILENO); /* redirect stdin */
    close(fd[0]);
    close(fd[1]);

    while (scanf("%63s", str) != EOF) {
        printf("%s ", str); // Print without new line
    }
    printf("\n"); // New line after all inputs are printed
    return 0;
}
