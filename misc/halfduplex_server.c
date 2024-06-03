#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main() {
    int sockfd, newsockfd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    char buffer[BUFFER_SIZE];

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Error opening socket");

    // Initialize server address struct
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    // Bind socket
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Error on binding");

    // Listen for incoming connections
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    // Accept connection from client
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0)
        error("Error on accept");

    // Half-duplex communication
    int n;
    while (1) {
        // Receive data from client
        n = read(newsockfd, buffer, BUFFER_SIZE);
        if (n < 0)
            error("Error reading from socket");
        printf("Client: %s", buffer);
        memset(buffer, 0, BUFFER_SIZE); // Clear buffer

        // Send data from server to client
        printf("Server: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        write(newsockfd, buffer, strlen(buffer));
    }

    // Close sockets
    close(newsockfd);
    close(sockfd);

    return 0;
}

