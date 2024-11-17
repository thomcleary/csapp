/*
Write code to implement the following function

// Return 1 when any odd bit of x equals 1; 0 otherwise
// Assume w=32
int any_odd_ones(unsigned x);

Your function should follow the bit-level integer coding rules (see rules.md),
except that you may assume that data type int has w = 32 bits;
*/

#include <assert.h>
#include <limits.h>

int any_odd_ones(unsigned x) {
  // 0101_0101_0101_0101_0101_0101_0101_0101
  unsigned mask = 0x55555555;
  return !!(x & mask);
}

int main(void) {
  unsigned all_even = 0x88888888;
  unsigned all_odd = 0x11111111;
  unsigned one_odd = 0x88818888;

  assert(!any_odd_ones(0));
  assert(!any_odd_ones(all_even));
  assert(any_odd_ones(all_odd));
  assert(any_odd_ones(one_odd));
  assert(any_odd_ones(UINT_MAX));

  return 0;
}
