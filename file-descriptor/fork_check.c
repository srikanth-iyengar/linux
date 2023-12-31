#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  int fd = open("../file.db", O_RDONLY | O_CLOEXEC);
  int pid = fork();
  if (pid == -1) {
    printf("Error forking the process");
    exit(1);
  }
  if (pid == 0) {
    lseek(fd, 33, SEEK_SET);
  }
  return 0;
}
