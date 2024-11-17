/*
Fill in code for the following C functions. Function srl performs a logical
right shift using an arithmetic right shift (given by value xrsa), followed by
other operations not including right shifts or division. Function sra performs
an arithmetic right shift using a logical right shift (given by value xsrl),
followed by other operations not including right shifts or division. You may use
the computation 8*sizeof(int) to determine w, the number of bits in data type
int. The shift amount k can range from 0->w-1.
*/

#include <assert.h>
#include <limits.h>
#include <stdio.h>

// Total # of bits
#define W (8 * sizeof(int))

unsigned srl(unsigned x, int k) {
  /* Perform shift arithmetically */
  unsigned xsra = (int)x >> k;
  // Fill in...

  // First k bits all set to 1
  // Remaining bits set to 0
  unsigned bits_to_mask = (0x1 << (k)) - 1;

  // Shift bits_to_mask to be most signficant bits, and negate
  // Most significant k bits will all be set to 0
  // Remaining bits set to 1
  unsigned mask = ~(bits_to_mask << (W - k));

  // Set arithmetically shifted bits to 0 instead of 1
  return xsra & mask;
}

int sra(int x, int k) {
  /* Perform shift logically */
  int xsrl = (unsigned)x >> k;
  // Fill in...

  // First k bits all set to 1
  // Remaining bits set to 0
  unsigned bits_to_mask = (0x1 << (k)) - 1;

  // Shift bits_to_mask to be most signficant bits
  // Most significant k bits will all be set to 1
  // Remaining bits set to 0
  unsigned mask = bits_to_mask << (W - k);

  // Set logically shifted bits to 1 instead of 0
  // only if sign bit of x is 1 (negative)
  return x < 0 ? xsrl | mask : xsrl;
}

int main(void) {
  assert(srl(INT_MIN, 0) == INT_MIN);
  assert(srl(INT_MAX, 0) == INT_MAX);
  assert(srl(UINT_MAX, 0) == UINT_MAX);
  assert(srl(INT_MIN, 3) == 0x10000000);
  assert(srl(INT_MAX, 3) == 0x0FFFFFFF);
  assert(srl(UINT_MAX, 3) == 0x1FFFFFFF);

  assert(sra(INT_MIN, 0) == INT_MIN);
  assert(sra(INT_MAX, 0) == INT_MAX);
  assert(sra(UINT_MAX, 0) == UINT_MAX);
  assert(sra(INT_MIN, 3) == 0xF0000000);
  assert(sra(INT_MAX, 3) == 0x0FFFFFFF); // >0 so should have 0's shifted in
  assert(sra(UINT_MAX, 3) == UINT_MAX);  // sign bit = 1, 1's shifted in

  return 0;
}
