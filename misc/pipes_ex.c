#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    int pipefd[2];
    pid_t cpid;
    char buf;

    // Create the pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    cpid = fork();

    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0) { // Child process
        close(pipefd[1]); // Close the write end of the pipe

        // Attempt to read from the pipe (this will block since the pipe is empty)
        // Explanation of Behavior:
        // - The child process blocks here because it tries to read from an empty pipe
        // - The read operation will only proceed once data is written to the pipe
        printf("Child: Attempting to read from the pipe...\n");
        ssize_t numRead = read(pipefd[0], &buf, 1);
        
        if (numRead == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        } else if (numRead == 0) {
            printf("Child: End of file (EOF) reached. No data to read.\n");
        } else {
            printf("Child: Read '%c' from the pipe\n", buf);
        }

        close(pipefd[0]); // Close the read end
        exit(EXIT_SUCCESS);

    } else { // Parent process
        close(pipefd[0]); // Close the read end of the pipe

        // Sleep for a few seconds to demonstrate the blocking behavior
        // Explanation of Behavior:
        // - This delay demonstrates the blocking behavior of the read operation in the child process
        sleep(5);

        // Write data to the pipe
        // Explanation of Behavior:
        // - The parent process writes data to the pipe after a delay, unblocking the read operation in the child
        printf("Parent: Writing 'x' to the pipe...\n");
        write(pipefd[1], "x", 1);

        close(pipefd[1]); // Close the write end
        wait(NULL); // Wait for child to exit
        exit(EXIT_SUCCESS);
    }
}

