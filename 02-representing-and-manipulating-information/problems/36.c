/*
For the case where data type int has 32 bits, devise a version of tmult_ok
(Problem 2.35) that uses the 64 bit precision of data type int64_t without using
division
*/

#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>

int tmult_ok(int x, int y) {
  if (x == 0 || y == 0) {
    return 1;
  }

  int64_t result = (int64_t)x * y;

  int positive_result = (x > 0 && y > 0) || (x < 0 && y < 0);
  int negative_overflow = !positive_result && result < INT_MIN;
  int positive_overflow = positive_result && result > INT_MAX;

  return !negative_overflow && !positive_overflow;

  // Solution
  // Can also just cast result back down to an int and see if the result is
  // still the same:
  // return result == (int)result;
}

int main(void) {
  assert(tmult_ok(0, 0) == 1);
  assert(tmult_ok(0, 1) == 1);
  assert(tmult_ok(1, 1) == 1);
  assert(tmult_ok(37, 42) == 1);
  assert(tmult_ok(37, -42) == 1);
  assert(tmult_ok(-37, 42) == 1);
  assert(tmult_ok(-37, -42) == 1);
  assert(tmult_ok(INT_MAX, 0) == 1);
  assert(tmult_ok(INT_MAX, 1) == 1);
  assert(tmult_ok(INT_MAX / 2, 2) == 1);
  assert(tmult_ok(INT_MIN, 0) == 1);
  assert(tmult_ok(INT_MIN, 1) == 1);
  assert(tmult_ok(INT_MIN / 2, 2) == 1);

  assert(tmult_ok(INT_MAX, 2) == 0);
  assert(tmult_ok(INT_MAX / 2 + 1, 2) == 0);
  assert(tmult_ok(INT_MAX, INT_MAX) == 0);
  assert(tmult_ok(INT_MAX, INT_MIN) == 0);
  assert(tmult_ok(INT_MIN, 2) == 0);
  assert(tmult_ok(INT_MIN / 2 - 1, 2) == 0);
  assert(tmult_ok(INT_MIN, INT_MIN) == 0);

  return 0;
}
