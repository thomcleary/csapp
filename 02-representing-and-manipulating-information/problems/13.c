/*
The Digital Equipment VAX computer was a very popular machine from the late
1970s until the late 1980s. Rather than instructions for Boolean operations AND
and OR, it had instructions bis (bit set) and bic (bit clear). Both instructions
take a data word x and a mask word m. They generate a result z consisting of the
bits of x modified according to the bits of m. With bis, the modification
involves setting z to 1 at each bit position where m is 1. With bic the
modification involves setting z to 0 at each bit position where m is 1.

To see how these operations relate to the C bit-level operations, assume we
have functions bis and bic implementing the bit set and bit clear operations,
and that we want to use these to implement functions computing bitwise
operations | and ^ without using any other C operations. Fill in the missing
code below.

Hint: Write C expression for the operations bis and bic.

// Declarations of functions implementing operations bis and bic
int bis(int x, int m);
int bic(int x, int m);

// Compute x|y using only calls to functions bis and bic
int bool_or(int x, int y) {
  int result = ???;
  return result;
}

// Compute x^y using only calls to functions bis and bic
int bool_xor(int x, int y) {
  int result = ???;
  return result;
}
*/

#include <assert.h>
#include <stdio.h>

// Declarations of functions implementing operations bis and bic
int bis(int x, int m) { return x | m; }

int bic(int x, int m) { return x & ~m; }

// Compute x|y using only calls to functions bis and bic
int bool_or(int x, int y) {
  int result = bis(x, y);
  return result;
}

// Compute x^y using only calls to functions bis and bic
int bool_xor(int x, int y) {
  int result = bis(bic(x, y), bic(y, x));
  return result;
}

int main(void) {
  assert(bool_or(0, 0) == 0x0);
  assert(bool_or(0x7, 0x0) == 0x7);
  assert(bool_or(0xF, 0xF) == 0xF);
  assert(bool_or(0x8, 0x1) == 0x9); // 1000 | 0001 = 1001
  assert(bool_or(0xA, 0x5) == 0xF); // 1010 | 0101 = 1111

  assert(bool_xor(0, 0) == 0x0);
  assert(bool_xor(0x7, 0x0) == 0x7);
  assert(bool_xor(0xF, 0xF) == 0x0);
  assert(bool_xor(0x8, 0x1) == 0x9); // 1000 | 0001 = 1001
  assert(bool_xor(0xA, 0x5) == 0xF); // 1010 | 0101 = 1111
  assert(bool_xor(0xA, 0x7) == 0xD); // 1010 | 0111 = 1101

  return 0;
}
