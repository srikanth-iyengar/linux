#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int pid = getpid();
  int fd = open("../file.db", O_RDONLY | O_CLOEXEC | O_RSYNC);
  printf("My pid is %d\nfd of file is %d", pid, fd);
  int offset = atoi(argv[1]);
  lseek(fd, offset, SEEK_SET);
  sleep(999999);
  return 0;
}
