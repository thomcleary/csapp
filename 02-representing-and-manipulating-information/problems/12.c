/*
Write C expression, in terms of variable x, for the following values.
Your code should work for any word size w >= 8.
For reference we show the result of evaluating the expression for
x = 0x87654321 with w = 32

(a) The least significant byte of x, with all other bits set to 0. [0x00000021]

(b) All but the least significant byte of x complemented, with the least
significant byte left unchanged [0x789ABC21]

(c) The least significant byte set to all ones, and all other bytes of x left
unchanged [0x876543FF]
*/

#include <stdio.h>

int main(void) {
  unsigned int x = 0x87654321;

  printf("x = %#x\n", x);

  printf("(a): %#x\n", x & 0xFF);

  printf("(b): %#x\n", (~x & (~0 ^ 0xFF)) | (x & 0xFF));
  // Solution: x ^ ~0xFF

  printf("(c): %#x\n", (x & (~0 ^ 0xFF)) | 0xFF);
  // Solution: x | 0xFF

  return 0;
}
