#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

#define N 256

int main(int argc, char *argv[]) {
    int fd, n;
    char str[N];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <fifo>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    mkfifo(argv[1], S_IRWXU); /* create named pipe */

    while (1) {
        fd = open(argv[1], O_RDONLY); /* open pipe read end */
        if (fd < 0) {
            perror("server: open");
            exit(EXIT_FAILURE);
        }

        while ((n = read(fd, str, N-1)) > 0) {
            str[n] = '\0';
            printf("server read: %s\n", str);
        }

        if (n < 0) {
            perror("server: read");
            exit(EXIT_FAILURE);
        }
        else {
            printf("server: end of input\n");
            close(fd);
        }
    }

    return 0;
}

