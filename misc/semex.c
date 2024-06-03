#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>

// Semaphore keys
#define SEM_KEY_A 123
#define SEM_KEY_B 456

// Function to perform semaphore operations
void semOperation(int semid, struct sembuf* operations, int num_ops) {
    if (semop(semid, operations, num_ops) == -1) {
        perror("semop");
        exit(EXIT_FAILURE);
    }
}

int main() {
    int semid;
    struct sembuf operations[2]; // Array to hold semaphore operations

    // Create or get the semaphore set
    semid = semget(IPC_PRIVATE, 2, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    // Initialize Semaphore A with an initial value of 2
    semctl(semid, 0, SETVAL, 2);

    // Perform semaphore operations
    operations[0].sem_num = 0; // Semaphore A
    operations[0].sem_op = -2; // down -2
    operations[0].sem_flg = 0;

    operations[1].sem_num = 1; // Semaphore B
    operations[1].sem_op = 1;  // up +1
    operations[1].sem_flg = 0;

    // Perform semaphore operations atomically
    semOperation(semid, operations, 2);

    printf("Semaphore operations performed successfully.\n");

    // Remove the semaphore set
    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("semctl");
        exit(EXIT_FAILURE);
    }

    return 0;
}

