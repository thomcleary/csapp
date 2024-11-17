/*
Suppose we number the bytes in a w-bit word from 0 (least significant) to w/8 -
1 (most significant). Write code for the following C function which will return
an unsigned value in which byte i of argument x has been replaced by byte b:

unsigned replace_byte(unsigned x, int i, unsigned char b);

Here are some examples showing how the function should work:
replace_byte(0x12345678, 2, 0xAB) --> 0x12AB5678
replace_byte(0x12345678, 0, 0xAB) --> 0xAB345678
*/

#include <assert.h>
#include <stdio.h>

unsigned replace_byte(unsigned x, int i, unsigned char b) {
  int bits = i << 3; // i * 8
  unsigned mask = (0xFF) << bits;
  return (x & ~mask) + (((unsigned)b) << bits);
  // return (x & ~mask) | (b << bits); // (solution)
}

int main(void) {
  unsigned x = 0x12345678;

  assert(replace_byte(x, 0, 0xAB) == 0x123456AB);
  assert(replace_byte(x, 1, 0xAB) == 0x1234AB78);
  assert(replace_byte(x, 2, 0xAB) == 0x12AB5678);
  assert(replace_byte(x, 3, 0xAB) == 0xAB345678);

  return 0;
}
