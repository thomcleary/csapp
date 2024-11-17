/*
Write procedures show_short, show_long and show_double that print the byte
representations of C objects of types short, long and double respectively. Try
these out on several machines
*/

#include <limits.h>
#include <stddef.h>
#include <stdio.h>

typedef unsigned char *byte_pointer;

void print_type(char *type) { printf("%-10s:", type); }

void show_bytes(byte_pointer start, size_t len) {
  size_t i;
  for (i = 0; i < len; i++) {
    printf(" %.2x", start[i]);
  }
}

void show_short(short x) { show_bytes((byte_pointer)&x, sizeof(short)); }

void show_int(int x) { show_bytes((byte_pointer)&x, sizeof(int)); }

void show_long(long x) { show_bytes((byte_pointer)&x, sizeof(long)); }

void show_float(float x) { show_bytes((byte_pointer)&x, sizeof(float)); }

void show_double(double x) { show_bytes((byte_pointer)&x, sizeof(double)); }

void show_pointer(void *x) { show_bytes((byte_pointer)&x, sizeof(void *)); }

void test_show_bytes(long val) {
  printf("-------------------------------------\n");
  printf("%-10s: %ld\n", "Value", val);
  printf("-------------------------------------\n");

  print_type("short");
  show_short(val);
  printf(" (%d)\n", (short)val);

  print_type("int");
  show_int(val);
  printf(" (%d)\n", (int)val);

  print_type("long");
  show_long(val);
  printf(" (%ld)\n", val);

  print_type("float");
  show_float(val);
  printf(" (%f)\n", (float)val);

  print_type("double");
  show_double(val);
  printf(" (%lf)\n", (double)val);

  print_type("pointer");
  show_pointer(&val);
  printf(" (%p)\n", &val);

  printf("\n");
}

int main(void) {
  test_show_bytes(LONG_MIN);
  test_show_bytes(INT_MIN);
  test_show_bytes(SHRT_MIN);
  test_show_bytes(-12345);
  test_show_bytes(-1);
  test_show_bytes(0);
  test_show_bytes(1);
  test_show_bytes(12345);
  test_show_bytes(SHRT_MAX);
  test_show_bytes(INT_MAX);
  test_show_bytes(LONG_MAX);

  return 0;
}
