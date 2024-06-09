#include <stdio.h>
#include <unistd.h>


int main(int argc, char* argv[]) {
  int counter = 0;
  for (int i = 0; i < 2; i++) {
    fork();
    counter++;
    printf("Inner: counter = %d\n", counter);
  }
  printf("Outer: counter = %d\n", counter);

  return 0;
}
