/*
Compile and run the sample code that uses show_bytes (file show-bytes.c) on
different machines to which you have access. Determine the byte orderings used
by these machines

Answer:

Macbook (Intel x86-64):
❯ ./55
 39 30 00 00
 00 e4 40 46
 d8 61 ba b6 f7 7f 00 00

(little endian)
*/

#include <stddef.h>
#include <stdio.h>

typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start, size_t len) {
  size_t i;
  for (i = 0; i < len; i++) {
    printf(" %.2x", start[i]);
  }
  printf("\n");
}

void show_int(int x) { show_bytes((byte_pointer)&x, sizeof(int)); }

void show_float(float x) { show_bytes((byte_pointer)&x, sizeof(float)); }

void show_pointer(void *x) { show_bytes((byte_pointer)&x, sizeof(void *)); }

void test_show_bytes(int val) {
  int ival = val;
  float fval = (float)ival;
  int *pval = &ival;

  show_int(ival);
  show_float(fval);
  show_pointer(pval);
}

int main(void) { test_show_bytes(12345); }
