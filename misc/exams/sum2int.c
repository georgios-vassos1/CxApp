#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

// Structure for message
struct msg_buffer {
    long msg_type;
    int data[2];
};

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <integer1> <integer2>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Convert arguments to integers
    int num1 = atoi(argv[1]);
    int num2 = atoi(argv[2]);

    // Key for message queue
    key_t key = ftok("msgq", 65);

    // Create a message queue
    int msgid = msgget(key, 0666 | IPC_CREAT);

    // Fork
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process
        struct msg_buffer message;

        // Receive data from message queue
        msgrcv(msgid, &message, sizeof(message), 1, 0);

        // Sum the integers
        int sum = message.data[0] + message.data[1];

        // Prepare message to send back to parent
        message.msg_type = 2;
        message.data[0] = sum;

        // Send sum to message queue
        msgsnd(msgid, &message, sizeof(message), 0);
    } else { // Parent process
        struct msg_buffer message;
        message.msg_type = 1;
        message.data[0] = num1;
        message.data[1] = num2;

        // Send data to message queue
        msgsnd(msgid, &message, sizeof(message), 0);

        // Wait for child to finish
        wait(NULL);

        // Receive sum from message queue
        msgrcv(msgid, &message, sizeof(message), 2, 0);

        // Print sum
        printf("Sum received from child: %d\n", message.data[0]);

        // Remove the message queue
        msgctl(msgid, IPC_RMID, NULL);
    }

    return 0;
}
