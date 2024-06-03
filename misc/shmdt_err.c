#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main() {
    // Key to identify the shared memory segment
    key_t key = ftok("shmfile", 65);

    // Create the shared memory segment
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);

    // Attempt to detach the shared memory segment
    char *str = "Trying to detach memory";
    if (shmdt(str) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    return 0;
}

