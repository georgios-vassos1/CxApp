#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    pid_t pid;

    pid = fork();
    if (pid < 0) {
        // Fork failed
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        // This is the child process
        printf("child: execute ./add 123 456\n");
        execl("./add", "add", "123", "456", NULL);
        // If execl returns, there was an error
        perror("execl");
        return 1;
    } else {
        // This is the parent process
        if (waitpid(pid, NULL, 0) < 0) {
            perror("waitpid");
            return 1;
        }
        printf("parent: child terminated\n");
    }

    return 0;
}

