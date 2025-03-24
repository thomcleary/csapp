/*
🐳 > gcc -o myecho myecho.c
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
  printf("Command-line arguments:\n");
  for (int i = 0; i < argc; i++) {
    printf("    argv[%d]: %s\n", i, argv[i]);
  }

  printf("\nEnvironment variables:\n");
  int i = 0;
  while (environ[i]) {
    printf("    envp[%d]: %s\n", i, environ[i]);
    i++;
  }

  return 0;
}
