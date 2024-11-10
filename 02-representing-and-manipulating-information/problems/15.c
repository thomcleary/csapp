/*
Using only bit-level and logical operations, write a C expression that is
equivalent to x==y.

In other words, it will return 1 when x and y are equal and 0 otherwise
*/

#include <assert.h>

int equal(int x, int y) { return !(x ^ y); }

int main(void) {
  assert(equal(0, 0) == 1);
  assert(equal(0, 1) == 0);
  assert(equal(1, 1) == 1);
  assert(equal(1, 37) == 0);
  assert(equal(456, 456) == 1);
  assert(equal(456, 457) == 0);
  assert(equal(-456, -456) == 1);
  assert(equal(-456, 456) == 0);

  return 0;
}
