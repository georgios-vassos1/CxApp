#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    int fd;
    pid_t pid;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    fd = open(argv[1], O_RDWR|O_CREAT|O_TRUNC, S_IRWXU);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    if ((pid = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process
        if (write(fd, "hello from child ", 17) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
        close(fd);
        exit(EXIT_SUCCESS);
    } else { // Parent process
        waitpid(pid, NULL, 0); /* wait for child termination */
        if (write(fd, "and hello from parent", 21) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
        close(fd);
        exit(EXIT_SUCCESS);
    }

    return 0;
}

