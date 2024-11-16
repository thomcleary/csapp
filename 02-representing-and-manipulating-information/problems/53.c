/*
Fill in the following macro definitions to generate the double precision values
+infinity, -infinity and -0.

#define POS_INFINITY
#define NEG_INFINITY
#define NEG_ZERO

You cannot use any include files (such as math.h) but you can make use of the
fact that the largest finite number that can be represented with double
precision is around 1.8 x 10^308
*/

#include <stdio.h>

// [Sign +] [Exponent All Ones (11 bits)] [Mantissa All Zeros (52 bits)]
// 0 11111111111 0000000000000000000000000000000000000000000000000000
// 0111_1111_1111_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000
// C hex constants for doubles
// 0x[Mantissa]p[Decimal Exponent]
//
#define POS_INFINITY 0x1p1024
#define NEG_INFINITY -POS_INFINITY
#define NEG_ZERO -(1 / POS_INFINITY)

int main(void) {
  printf("POS_INFINITY: %lf\n", POS_INFINITY);
  printf("NEG_INFINITY:  %lf\n", NEG_INFINITY);
  printf("NEG_ZERO: %lf\n", NEG_ZERO);

  return 0;
}
