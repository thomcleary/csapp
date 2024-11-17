/*
Try running the code for show_bytes for different sample values
*/

#include <limits.h>
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

  printf("Value: %d\n", val);
  show_int(ival);
  show_float(fval);
  show_pointer(pval);
  printf("\n");
}

int main(void) {
  test_show_bytes(INT_MIN);
  test_show_bytes(-12345);
  test_show_bytes(-1);
  test_show_bytes(0);
  test_show_bytes(1);
  test_show_bytes(12345);
  test_show_bytes(INT_MAX);

  return 0;
}
