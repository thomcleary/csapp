/*
Write a function with the following prototype:

// Determine whether arguments can be added without overflow
int tadd_ok(int x, int y);

This function should return 1 if arguments x and y can be added without causing
overflow
*/

#include <assert.h>
#include <limits.h>

int tadd_ok(int x, int y) {
  int result = x + y;

  int negative_overflow = x < 0 && y < 0 && result >= 0;
  int positive_overflow = x > 0 && y > 0 && result < 0;

  return !negative_overflow && !positive_overflow;
}

int main(void) {
  assert(tadd_ok(0, 0) == 1);
  assert(tadd_ok(0, 1) == 1);
  assert(tadd_ok(0, -1) == 1);
  assert(tadd_ok(1, 1) == 1);
  assert(tadd_ok(-1, -1) == 1);
  assert(tadd_ok(37, 42) == 1);
  assert(tadd_ok(-37, -42) == 1);
  assert(tadd_ok(INT_MAX, 0) == 1);
  assert(tadd_ok(INT_MAX / 2, INT_MAX / 2 + 1) == 1);
  assert(tadd_ok(INT_MAX, INT_MIN) == 1);
  assert(tadd_ok(INT_MIN / 2, INT_MIN / 2) == 1);

  assert(tadd_ok(INT_MAX, 1) == 0);
  assert(tadd_ok(INT_MAX, INT_MAX) == 0);
  assert(tadd_ok(INT_MAX / 2 + 1, INT_MAX / 2 + 1) == 0);
  assert(tadd_ok(INT_MIN, -1) == 0);
  assert(tadd_ok(INT_MIN, INT_MIN) == 0);
  assert(tadd_ok(INT_MIN / 2 - 1, INT_MIN / 2 - 1) == 0);

  return 0;
}
