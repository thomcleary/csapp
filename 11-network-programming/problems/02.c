/*
Problem 2
Write a program that converts its hex argument to a dotted-decimal
string and prints the result. For example:

linux> ./dd2hex 0x8002c2f2
128.2.194.242
*/

#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "usage: ./02 <hex network address>\n");
    exit(EXIT_FAILURE);
  }

  uint32_t hex_addr;

  int result = sscanf(argv[1], "%x", &hex_addr);

  if (result != 1) {
    fprintf(stderr, "Invalid address\n");
    exit(EXIT_FAILURE);
  }

  struct in_addr addr = {.s_addr = htonl(hex_addr)};

  size_t pbuf_size = (3 * 4) + 3 + 1; // 3 chars per byte + 3 dots + \0
  char pbuf[pbuf_size];

  const char *pstr = inet_ntop(AF_INET, &addr, pbuf, sizeof(pbuf));

  if (pstr == NULL) {
    fprintf(stderr, "inet_ntop failed\n");
    exit(EXIT_FAILURE);
  }

  printf("%s\n", pstr);

  return 0;
}
