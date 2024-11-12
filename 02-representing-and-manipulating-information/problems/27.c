/*
Write a function with the following prototype

// Determine whether arguments can be added without overflow
int uadd_ok(unsigned x, unsigned y);

This function should return 1 if arguments x and y can be added without causing
overflow
*/

#include <assert.h>
#include <limits.h>
#include <stdio.h>

// Determine whether arguments can be added without overflow
int uadd_ok(unsigned x, unsigned y) { return x + y >= x; }

int main(void) {
  assert(uadd_ok(0, 0) == 1);
  assert(uadd_ok(0, 1) == 1);
  assert(uadd_ok(37, 42) == 1);
  assert(uadd_ok(UINT_MAX / 2 + 1, UINT_MAX / 2) == 1);
  assert(uadd_ok(UINT_MAX, 1) == 0);
  assert(uadd_ok(-1, 1) == 0); // (-1) bit pattern will be UINT_MAX
  assert(uadd_ok(UINT_MAX / 2 + 1, UINT_MAX / 2 + 1) == 0);

  return 0;
}
