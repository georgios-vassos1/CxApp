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

    // Attach the shared memory segment
    char *str = (char*) shmat(shmid, (void*)0,0);
    printf("Memory attached at %p\n", str);

    // Write data to the shared memory
    sprintf(str, "Hello from the first process");

    // Detach the shared memory segment
    shmdt(str);

    return 0;
}

