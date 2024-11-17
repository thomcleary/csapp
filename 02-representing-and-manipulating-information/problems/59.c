/*
Write a C expression that will yield a word consisting of the least significant
byte of x and the remaining bytes of y. For operands x=0x89ABCDEF and
y=0x76543210, this would give 0x765432EF
*/

#include <assert.h>

int merge_ints(int x, int y) { return (x & 0xFF) + (y & ~0xFF); }

int main(void) {
  assert(merge_ints(0x89ABCDEF, 0x76543210) == 0x765432EF);
  assert(merge_ints(0x76543210, 0x89ABCDEF) == 0x89ABCD10);

  return 0;
}
