#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
    int pid;
    int status;

    printf("%d: creating new process\n", getpid());
    pid = fork();
    if (pid == 0) {
        sleep(5);
        exit(42); // return(42);
    }
    printf("%d: child pid = %d\n", getpid(), pid);
    waitpid(pid, &status, 0);
    printf("%d: status = %d\n", getpid(), status);
    if (WIFEXITED(status)) {
        printf("child returned %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("child terminated with %d\n", WTERMSIG(status));
    }

    return 0;
}
