/*
Write a function div16 that returns the value x/16 for integer argument x. Your
function should not use division, modulus, multiplication, any condtionals (if
or
?:), any comparison operators (e.g., <, >, or ==), or any loops. You may assume
that data type int is 32 bits long and uses a two's complement representation,
and that right shifts are performed arithmetically.
*/

#include <assert.h>
#include <limits.h>
#include <stdio.h>

int div16(int x) {
  // If x is negative, right shift will produce all 1's otherwise all 0's
  // & 15 will result in either 15, or 0
  int bias = x >> 31 & 15;
  return (x + bias) >> 4;
}

int main(void) {
  assert(div16(0) == 0);
  assert(div16(1) == 0);
  assert(div16(15) == 0);
  assert(div16(16) == 1);
  assert(div16(25) == 1);
  assert(div16(31) == 1);
  assert(div16(32) == 2);
  assert(div16(33) == 2);
  assert(div16(48) == 3);

  assert(div16(-0) == 0);
  assert(div16(-1) == 0);
  assert(div16(-15) == 0);
  assert(div16(-16) == -1);
  assert(div16(-25) == -1);
  assert(div16(-31) == -1);
  assert(div16(-32) == -2);
  assert(div16(-33) == -2);
  assert(div16(-48) == -3);

  return 0;
}
