#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
  FILE *fptr;

  char path[100];
  char spid[10];
  int pid, fd;
  if (argc < 3) {
    pid = getpid();

    sprintf(spid, "%d", pid);

    fd = open("../file.db", O_RDONLY | O_CLOEXEC | O_RSYNC);
  } else {
    pid = atoi(argv[1]);
    sprintf(spid, "%d", pid);
    fd = atoi(argv[2]);
  }

  if (fd == -1) {
    printf("Error opening file: file.db\n");
    exit(1);
  }
  printf("My pid is %d\n", pid);
  printf("fd: %d\n", fd);
  sprintf(path, "/proc/%s/fdinfo/%d", spid, fd);
  fptr = fopen(path, "r");
  printf("path: %s", path);
  char flag_meta[100];
  fscanf(fptr, "%[^\n]", flag_meta);
  printf("%s\n", flag_meta);
  fgetc(fptr);
  fscanf(fptr, "%[^\n]", flag_meta);
  char *flags = NULL;
  flags = strtok(flag_meta, "\t");
  flags = strtok(NULL, "\t");
  int flag = strtol(flags, NULL, 8);
  if (flag & O_RDONLY) {
    printf("O_RDONLY flag is set \n");
  }
  if (flag & O_EXCL) {
    printf("O_EXCL flag is set \n");
  }
  if (flag & O_RDWR) {
    printf("O_RDWR flag is set \n");
  }
  if (flag & O_SYNC) {
    printf("O_SYNC flag is set \n");
  }
  if (flag & O_ASYNC) {
    printf("O_ASYNC flag is set \n");
  }
  if (flag & O_DSYNC) {
    printf("O_DSYNC flag is set \n");
  }
  if (flag & O_CREAT) {
    printf("O_CREAT flag is set \n");
  }
  if (flag & O_FSYNC) {
    printf("O_FSYNC flag is set \n");
  }
  if (flag & O_RSYNC) {
    printf("O_RSYNC flag is set \n");
  }
  if (flag & O_TRUNC) {
    printf("O_TRUNC flag is set \n");
  }
  if (flag & O_APPEND) {
    printf("O_APPEND flag is set \n");
  }
  if (flag & O_NDELAY) {
    printf("O_NDELAY flag is set \n");
  }
  if (flag & O_NOCTTY) {
    printf("O_NOCTTY flag is set \n");
  }
  if (flag & O_WRONLY) {
    printf("O_WRONLY flag is set \n");
  }
  if (flag & O_ACCMODE) {
    printf("O_ACCMODE flag is set \n");
  }
  if (flag & O_CLOEXEC) {
    printf("O_CLOEXEC flag is set \n");
  }
  if (flag & O_NOFOLLOW) {
    printf("O_NOFOLLOW flag is set \n");
  }
  if (flag & O_DIRECTORY) {
    printf("O_DIRECTORY flag is set \n");
  }

  return 0;
}
