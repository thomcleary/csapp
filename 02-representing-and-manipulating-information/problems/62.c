/*
Write a function int_shifts_are_arithmetic() that yields 1 when run on a
machine that uses arithmetic right shifts for data type int and yields 0
otherwise. Your code should work on a machine with any word size. Test your code
on several machines
*/

#include <limits.h>
#include <stdio.h>

int int_shifts_are_arithmetic(void) {
  int mask = 0xFF << ((sizeof(int) - 1) << 3);
  return !!((INT_MIN >> 8) & mask);
}

// You didn't have to follow rules.md for this one lol
int solution() {
  int x = -1;          // All 1's
  return (x >> 1) > 0; // If logical shift, sign bit will become positive
}

int main(void) {
  if (int_shifts_are_arithmetic()) {
    printf("int shifts are arithmetic\n");
  } else {
    printf("int shifts are logical\n");
  }

  return 0;
}
