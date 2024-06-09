#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

// Buffer size for reading from pipes
#define BUFFER_SIZE 1024

// Function to handle SIGINT signal in the child process
void handle_sigint(int sig) {
    printf("Child: Received SIGINT\n");
    fflush(stdout);

    // Open the file for reading
    int fd = open("output.txt", O_RDONLY);
    if (fd == -1) {
        perror("Child: Error opening file for reading");
        exit(EXIT_FAILURE);
    }

    // Read from the file and print to terminal
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
        write(STDOUT_FILENO, buffer, bytes_read);
    }

    if (bytes_read == -1) {
        perror("Child: Error reading from file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <pipe1> <pipe2>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *pipe1 = argv[1];
    char *pipe2 = argv[2];

    // Open the named pipes for reading
    int fd1 = open(pipe1, O_RDONLY | O_NONBLOCK);
    if (fd1 == -1) {
        perror("Error opening pipe1 for reading");
        exit(EXIT_FAILURE);
    }

    int fd2 = open(pipe2, O_RDONLY | O_NONBLOCK);
    if (fd2 == -1) {
        perror("Error opening pipe2 for reading");
        close(fd1);
        exit(EXIT_FAILURE);
    }

    // Open the file for writing (create if it doesn't exist)
    int file_fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file_fd == -1) {
        perror("Error opening file for writing");
        close(fd1);
        close(fd2);
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("Error forking");
        close(fd1);
        close(fd2);
        close(file_fd);
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process
        printf("Child: Waiting for SIGINT\n");
        fflush(stdout);
        signal(SIGINT, handle_sigint);

        // Wait for SIGINT signal
        pause();

        exit(EXIT_SUCCESS);
    } else {
        // Parent process
        char buffer[BUFFER_SIZE];
        ssize_t bytes_read1, bytes_read2;

        while (1) {
            bytes_read1 = read(fd1, buffer, BUFFER_SIZE);
            if (bytes_read1 > 0) {
                write(file_fd, buffer, bytes_read1);
                printf("Parent: Read %zd bytes from pipe1\n", bytes_read1);
                fflush(stdout);
            }

            bytes_read2 = read(fd2, buffer, BUFFER_SIZE);
            if (bytes_read2 > 0) {
                write(file_fd, buffer, bytes_read2);
                printf("Parent: Read %zd bytes from pipe2\n", bytes_read2);
                fflush(stdout);
            }

            // Check if both pipes are closed
            if ((bytes_read1 == -1 && errno == EAGAIN) && (bytes_read2 == -1 && errno == EAGAIN)) {
                break;
            }
        }

        // Close the pipes and file
        close(fd1);
        close(fd2);
        close(file_fd);

        // Send SIGINT signal to child process
        printf("Parent: Sending SIGINT to child\n");
        fflush(stdout);
        kill(pid, SIGINT);

        // Wait for child process to finish
        wait(NULL);
        printf("Parent: Child process finished\n");
        fflush(stdout);
    }

    return 0;
}
