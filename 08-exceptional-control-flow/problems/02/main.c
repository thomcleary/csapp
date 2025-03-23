/*
🐳 > gcc -o main main.c
*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void) {
  int x = 1;

  pid_t pid = fork();

  if (pid < 0) {
    fprintf(stderr, "fork failed: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {
    printf("p1: x=%d\n", ++x);
  }
  printf("p2: x=%d\n", --x);
  exit(0);
}
