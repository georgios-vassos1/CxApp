#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int file_descriptor;
    char data[] = "This is some data to write to the file.\n";

    // Include additional header files: unistd.h for the write() function and fcntl.h for file control options

    // Open a file in write mode
    file_descriptor = open("example.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file_descriptor < 0) {
        perror("Error opening file");
        exit(1);
    }

    // Use the write() function to directly write the data to the file descriptor returned by open()
    if (write(file_descriptor, data, sizeof(data)) < 0) {
        perror("Error writing to file");
        close(file_descriptor);
        exit(1);
    }

    // Close the file descriptor using the close() function to ensure that any buffered data is flushed to the disk
    // and that the file descriptor is released
    close(file_descriptor);

    return 0;
}

