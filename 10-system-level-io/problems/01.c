/*
What is the output of the following program?

Answer
fd2 = 3
*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// 🐳> gcc 01.c -o 01
int main(void) {
  int fd1, fd2;

  if ((fd1 = open("../../README.md", O_RDONLY, 0)) == -1) {
    perror("open failed");
    exit(EXIT_FAILURE);
  }

  if (close(fd1) == -1) {
    perror("close failed");
    exit(EXIT_FAILURE);
  }

  if ((fd2 = open("01.c", O_RDONLY, 0)) == -1) {
    perror("open failed");
    exit(EXIT_FAILURE);
  };

  printf("fd2 = %d\n", fd2);
  exit(EXIT_SUCCESS);
}
