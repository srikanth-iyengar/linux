#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int pid = getpid();
  int fd = open("../file.db", O_RDONLY | O_CLOEXEC | O_APPEND);
  int fd1 = dup(fd);
  int fd2 = dup2(fd, 999);

  lseek(fd, 420, SEEK_SET);

  char pid_str[100], fd_str[100];

  sprintf(pid_str, "%d", pid);
  sprintf(fd_str, "%d", fd);

  char *command[] = {"build/fd_decoder", pid_str, fd_str, NULL};
  char *env[] = {NULL};

  printf("pid: %d\n", pid);
  printf("fd: %d\n", fd);
  printf("fd1: %d\n", fd1);
  printf("fd2: %d\n", fd2);
  execve("build/fd_decoder", command, env);
  sleep(999);
  return 0;
}
