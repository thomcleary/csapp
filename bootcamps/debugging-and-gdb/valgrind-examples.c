/*
🐳 ❯ gcc -o valgrind-examples valgrind-examples.c
🐳 ❯ valgrind --leak-resolution=high --leak-check=full --show-reachable=yes
--track-fds=yes ./valgrind-examples
*/

#include <stdio.h>
#include <stdlib.h>

int invalid_read(int y) {
  int *bar = malloc(sizeof(int));
  *bar = y;
  free(bar);
  printf("bar: %d\n", *bar);
  return y;
}

int memory_leak(int y) {
  int *bar = malloc(sizeof(int));
  *bar = y;
  printf("bar: %d \n", *bar);
  return y;
}

int main(void) {
  invalid_read(37);

  for (int i = 0; i < 3; i++) {
    memory_leak(i);
  }

  return 0;
}
