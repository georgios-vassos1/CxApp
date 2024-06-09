#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    int fd, i;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <fifo> <message1> [<message2> ...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    fd = open(argv[1], O_WRONLY); /* open pipe write end */
    if (fd < 0) {
        perror("client: open");
        exit(EXIT_FAILURE);
    }

    for (i = 2; i < argc; i++) {
        write(fd, argv[i], strlen(argv[i]));
    }

    close(fd);
    return 0;
}

