#include <stdio.h>
#include <unistd.h>


int main(int argc, char* argv[]) {
    printf("Hello ");
    execlp("./myecho", "myecho", "World!\n", NULL);
    printf("Bye World!\n");
    return 0;
}
