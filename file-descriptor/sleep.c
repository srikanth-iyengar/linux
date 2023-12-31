#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int pid = getpid();
  printf("%d\n", pid);
  fflush(stdout);
  sleep(atoi(argv[1]));
}
