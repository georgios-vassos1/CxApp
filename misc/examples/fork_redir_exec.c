#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    int fd, pid;

    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) { // Child process
        printf("Child: Execute ./add 123 456 with redirection\n");
        fd = open("test.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
        if (fd < 0) {
            perror("open");
            exit(1);
        }
        dup2(fd, STDOUT_FILENO); // Redirect stdout to the file
        close(fd);
        execl("./add", "add", "123", "456", NULL);
        perror("execl");
        exit(1);
    } else { // Parent process
        waitpid(pid, NULL, 0);
        printf("Parent: Child terminated\n");
        exit(0);
    }

    return 0; // This line is unnecessary since we exit earlier
}

