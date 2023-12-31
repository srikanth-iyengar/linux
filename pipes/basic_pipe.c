#include <fcntl.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int fd[2] = {0, 0};

  printf("%d, %d\n", fd[0], fd[1]);
  pipe(fd);
  printf("%d, %d\n", fd[0], fd[1]);

  int cpid = fork();

  int pid = getpid();
  printf("pid: %d\n", pid);

  if (cpid > 0) {
    // Parent
    close(fd[0]);
    char message[200];
    write(fd[1], "Hey Child, How are you?", 23);
    close(fd[1]);
  } else {
    // Child
    close(fd[1]);
    char message[200];
    read(fd[0], message, 23);
    printf("This is the message from my parent: %s\n", message);
    close(fd[0]);
  }
  printf("[%d], I am going to sleep\n", pid);
  sleep(1);
  return 0;
}
