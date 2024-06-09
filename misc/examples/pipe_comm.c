#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int fd[2], i;
    char str[64];

    pipe(fd);
    if (!fork()) {
        dup2(fd[1], STDOUT_FILENO); /* redirect stdout */
        close(fd[0]); close(fd[1]);
        for (i = 0; i < argc; i++) {
            printf("%s\n", argv[i]);
        }
        return 0;
    }

    dup2(fd[0], STDIN_FILENO); /* redirect stdin */
    close(fd[0]); close(fd[1]);
    while (scanf("%63s", str) != EOF) {
        printf("%s\n", str);
    }
    return 0;
}
