#include <assert.h>
#include <stdio.h>

int isAsciiDigit(int x) {
  int high_nibble_mask = 0x30;
  int x_without_low_nibble = x & (~0xF);
  int matches_high_nibble = !(high_nibble_mask ^ x_without_low_nibble);

  // printf("matches_high_nibble: %d\n", matches_high_nibble);

  // The low nibble is invalid if most signficant bit is 1
  // and either of the 2nd and/or 3rd bit is 1
  // 1010
  // 1011
  // 1100
  // 1101
  // 1110
  // 1111
  int x_low_nibble = x & (0xF);
  int high_bit = !!(0x8 & x_low_nibble);
  int middle_bits = !!(0x6 & x_low_nibble);

  // printf("low_nibble: %#x\n", x_low_nibble);
  // printf("high_bit: %d\n", high_bit);
  // printf("middle_bits: %d\n", middle_bits);

  return matches_high_nibble & !(high_bit & middle_bits);
}

int main(void) {
  // int a = 0x30;
  // printf("a: %#x\n", a);
  // printf("isAsciiDigit: %d\n", isAsciiDigit(a));

  assert(isAsciiDigit(0x30));
  assert(isAsciiDigit(0x31));
  assert(isAsciiDigit(0x32));
  assert(isAsciiDigit(0x33));
  assert(isAsciiDigit(0x34));
  assert(isAsciiDigit(0x35));
  assert(isAsciiDigit(0x36));
  assert(isAsciiDigit(0x37));
  assert(isAsciiDigit(0x38));
  assert(isAsciiDigit(0x39));

  assert(!isAsciiDigit(-1));
  assert(!isAsciiDigit(0x0));
  assert(!isAsciiDigit(0x20));
  assert(!isAsciiDigit(0x2F));
  assert(!isAsciiDigit(0x3A));
  assert(!isAsciiDigit(0x40));
  assert(!isAsciiDigit(0x50));
  assert(!isAsciiDigit(0x70));

  return 0;
}
