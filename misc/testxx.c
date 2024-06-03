#include <stdio.h>
#include <unistd.h>


int main(int argc, char *argv[]) {
  fork();
  putchar('1');
  fork();
  putchar('2');
  fork();
  putchar('3');
  return 0;
}
