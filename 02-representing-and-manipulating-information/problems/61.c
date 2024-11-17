/*
Write C expressions that evaluate to 1 when the following conditions are true
and to 0 when they are false. Assume x is of type int.

A. Any bit of x equals 1
B. Any bit of x equals 0
C. Any bit in the least significant byte of x equals 1
D. Any bit in the most significant byte of x equals 0

Your code should follow the bit-level integer coding rules (see rules.md) with
the additional restriction that you may not use equality or inequality tests.
*/

#include <assert.h>
#include <stdio.h>

/* Any bit of x equals 1 */
int a(unsigned x) { return !!x; }

/* Any bit of x equals 0 */
int b(unsigned x) { return !!(~x); }

/* Any bit in the least significant byte of x equals 1 */
int c(unsigned x) { return !!(x & 0xFF); }

/* Any bit in the most significant byte of x equals 0 */
int d(unsigned x) { return !!(~x & (0xFF << ((sizeof(unsigned) - 1) << 3))); }

int main(void) {
  unsigned ones_and_zeros = 0x88888888;
  unsigned zero = 0x0;
  unsigned all_ones = 0xFFFFFFFF;
  unsigned lsb_zero = ~0xFF;
  unsigned msb_zero = ~(0xFF << ((sizeof(unsigned) - 1) << 3));

  assert(a(ones_and_zeros));
  assert(!a(zero));
  assert(a(all_ones));
  assert(a(lsb_zero));
  assert(a(msb_zero));

  assert(b(ones_and_zeros));
  assert(b(zero));
  assert(!b(all_ones));
  assert(b(lsb_zero));
  assert(b(msb_zero));

  assert(c(ones_and_zeros));
  assert(!c(zero));
  assert(c(all_ones));
  assert(!c(lsb_zero));
  assert(c(msb_zero));

  assert(d(ones_and_zeros));
  assert(d(zero));
  assert(!d(all_ones));
  assert(!d(lsb_zero));
  assert(d(msb_zero));

  return 0;
}
