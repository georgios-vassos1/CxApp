#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// Define message structure
struct message {
    long mtype;
    int data;
};

int main() {
    // Create message queue
    key_t key = ftok("/tmp", 'A');
    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    // Fork a child process
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) { // Child process
        // Receive message from parent
        struct message msg;
        msgrcv(msgid, &msg, sizeof(struct message), 1, 0);

        // Compute sum
        int sum = msg.data + msg.data;

        // Send sum to parent
        msg.mtype = 2;
        msg.data = sum;
        msgsnd(msgid, &msg, sizeof(struct message), 0);
    } else { // Parent process
        // Get two integers from user
        int num1, num2;
        printf("Enter two integers: ");
        scanf("%d %d", &num1, &num2);

        // Send integers to child
        struct message msg;
        msg.mtype = 1;
        msg.data = num1;
        msgsnd(msgid, &msg, sizeof(struct message), 0);
        msg.data = num2;
        msgsnd(msgid, &msg, sizeof(struct message), 0);

        // Receive sum from child
        msgrcv(msgid, &msg, sizeof(struct message), 2, 0);

        // Print sum
        printf("Sum: %d\n", msg.data);
    }

    // Remove message queue
    if (pid != 0) {
        msgctl(msgid, IPC_RMID, NULL);
    }

    return 0;
}

