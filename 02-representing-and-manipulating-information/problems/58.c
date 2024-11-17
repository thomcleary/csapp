/*
Write a procedure is_little_endian that will return 1 when compiled and run
on a little-endian machine, and will return 0 when compiled and run on a
big-endian machine. This program should run on any machine, regardless of its
word size.
*/

#include <stdio.h>

int is_little_endian(void) {
  unsigned short one = 1;
  unsigned char *bytes = (unsigned char *)&one;
  return bytes[0] & 1;
}

int main(void) {
  if (is_little_endian()) {
    printf("Little Endian\n");
  } else {
    printf("Big Endian\n");
  }

  return 0;
}
