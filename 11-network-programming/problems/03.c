/*
Problem 3
Write a program that converts its dotted-decimal argument to a hex
number and prints the result. For example:

linux> ./dd2hex 128.2.194.242
0x8002c2f2
*/

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "usage: ./02 <presentation network address>\n");
    exit(EXIT_FAILURE);
  }

  struct in_addr addr;
  int result = inet_pton(AF_INET, argv[1], &addr);

  if (result != 1) {
    fprintf(stderr, result == 0 ? "Invalid address\n" : "inet_pton failed\n");
    exit(EXIT_FAILURE);
  }

  printf("%#x\n", ntohl(addr.s_addr));

  return 0;
}
