/*
 * CS:APP Data Lab
 *
 * <Please put your name and userid here>
 *
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.
 */

#include <limits.h>
#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:

  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code
  must conform to the following style:

  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>

  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.


  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 *
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce
 *      the correct answers.
 */

#endif

// 1

/*
 * bitXor - x^y using only ~ and &
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  // Determine bits that are 1 in both x and y
  int both_one = x & y;

  // Determine bits that are 0 in both x and y
  int both_zero = ~x & ~y;

  // Return the bits that do not match in x and y
  return ~both_one & ~both_zero;
}

/*
 * tmin - return minimum two's complement integer
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  return 1 << (31);
}

/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
  // Will be 0x80000000 when x is Tmax
  int x_plus_one = x + 1;

  // x+1 == ~Tmax, when x=Tmax
  // x^y will evaluate to 0 when x==y
  int x_plus_one_is_x_negated = !(x_plus_one ^ ~x);

  // However, x+1 == 0, when x=(-1)
  // ~(-1) == 0, ~0xFFFFFFFF = 0x00000000
  // So x_plus_one_is_x_negated == 1
  // !(0 ^ 0) == 1

  // Need to also check that x != (-1)
  // If x_plus_one is 0, x == (-1)
  return x_plus_one_is_x_negated & !!x_plus_one;
}

// 2

/*
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
  int odd_bits = 0xAA;

  // Construct 0xAAAAAAAA (all odd bits)
  int mask = (odd_bits << 24) | (odd_bits << 16) | (odd_bits << 8) | odd_bits;

  // mask & x -> (get all odd bits of x)
  // (mask & x) ^ mask -> (if x ==  mask, ^ against mask will result in 0)
  // return ! of the result
  return !((mask & x) ^ mask);
}

/*
 * negate - return -x
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  return (~x) + 1;
}

// 3

/*
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0'
 * to '9') Example: isAsciiDigit(0x35) = 1. isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  int high_nibble_mask = 0x30;
  int x_without_low_nibble = x & (~0xF);
  int matches_high_nibble = !(high_nibble_mask ^ x_without_low_nibble);

  // The low nibble is invalid if the most signficant bit is 1
  // AND
  // either the 2nd and/or 3rd bit is 1
  // low_nibble >= 0xA, since largest ascii digit is 9, 0x9, 0b1001
  int x_low_nibble = x & (0xF);
  int high_bit = !!(0x8 & x_low_nibble);
  int middle_bits = !!(0x6 & x_low_nibble);

  return matches_high_nibble & !(high_bit & middle_bits);
}

/*
 * conditional - same as x ? y : z
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  int sign_extended = (!!x << 31) >> 31;
  int y_result = sign_extended & y;
  int z_result = ~sign_extended & z;

  return y_result | z_result;
}

/*
 * isLessOrEqual - if x <= y  then return 1, else return 0
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  int x_sign = (x >> 31) & 1;
  int y_sign = (y >> 31) & 1;

  // If x and y have the same sign then,
  // y-x will be >=0 when x<=y
  // y-x will be <0 when x>y
  int diff = (y + (~x + 1)); // y-x
  int diff_sign = diff >> 31;

  // If signs differ, return 1 when x is negative
  // Otherwise return 1 if the diff is positive (y-x >= 0)
  return ((x_sign ^ y_sign) & x_sign) | !diff_sign;
}

// 4

/*
 * logicalNeg - implement the ! operator, using all of
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4
 */
int logicalNeg(int x) {
  // When x != 0, either x or -x will have a sign bit of 1
  // When x == 0, x and -x will both have a sign bit of 0

  // Arithmetic shift will make all bits equal to the sign bit
  int x_sign = x >> 31;
  int x_negated_sign = (~x + 1) >> 31;

  // If x == 0, (x_sign | x_negated_sign) -> (all zeros) == 0
  // If x != 0, (x_sign | x_negated_sign) -> (all ones) == -1
  return (x_sign | x_negated_sign) + 1;
}

/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
  // 0 if x is positive, -1 (all 1s) if x is negative
  int bits = 0;
  int x_sign = (x >> 31);

  // If x is negative (sign is all 1s), then negate x's bits
  // Equivalent of absolute(x) - 1, since -x = ~x + 1
  // In some cases, the positive version of x requires 1 more bit than its
  // negative inverse
  // +2 requires 3 bits 010 (signbit + 10bits)
  // -2 requires 2 bits  10 (signbit + 1bit), since inverse is 01 = 2-1 = 1
  x = x ^ x_sign;

  // Binary search for the most signficant bit
  // 0000_0000_0100_0000_0000_0000_0000

  bits = !!(x >> 16) << 4;
  // 0000_0000_0000_0000_[0000_0000_0100_0000]..._0000_0000_0000_0000
  // There is a 1 in the high 16 bits, the low 16 bits are required
  // Reduce search to the top 16 bits

  bits += !!(x >> (bits + 8)) << 3;
  // x >> (16 + 8)
  // 0000_0000_0000_0000_0000_0000_[0000_0000]..._1000_0000_0000_0000_0000_0000
  // There is no 1 in the top 8 (32-24) bits
  // bits remains at 16

  bits += !!(x >> (bits + 4)) << 2;
  // x >> (16 + 4)
  // 0000_0000_0000_0000_0000_0000_0000_[0100]..._0000_0000_0000
  // There is a 1 in the top 12 (32-20) bits, at least 20 bits are required
  // Reduce search to the top 16 + 4 bits

  bits += !!(x >> (bits + 2)) << 1;
  // x >> (20 + 2)
  // 0000_0000_0000_0000_0000_0000_0000_[01]...00_0000_0000_0000
  // There is a 1 in the top 10 (32-22) bits, at least 22 bits are required
  // Reduce search to the top 20 + 2 bits

  bits += !!(x >> (bits + 1));
  // x >> (22 + 1)
  // 0000_0000_0000_0000_0000_0000_0000_[0]...100_0000_0000_0000
  // There is no 1 in the top 9 (32-23) bits
  // bits remain at 22

  bits += !!(x >> bits);
  // Add the position of the most significant bit
  // x >> 22
  // 0000_0000_0000_0000_0000_0000_0000_0[1]...00_0000_0000_0000
  // Bits finishes at 22+1 = 23 bits are required

  // +1 for the sign bit
  return bits + 1;
}

// float

/*
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf) {
  const unsigned sign_mask = 0x80000000;
  const unsigned sign = uf & sign_mask;

  const unsigned significand_size = 23;
  const unsigned significand_mask = 0x007FFFFF;
  unsigned significand = uf & significand_mask;

  const unsigned exponent_mask = 0x7F800000;
  unsigned exponent = uf & exponent_mask;
  const unsigned exponent_val = exponent >> significand_size;

  const unsigned is_inf_or_nan = exponent == exponent_mask;
  const unsigned is_denorm = !exponent;

  if (is_inf_or_nan) {
    return uf;
  }

  if (is_denorm) {
    // Scaling normalised values by 2 is a simple left bit shift
    return sign | (significand << 1);
  }

  // normalised value
  // add 1 to exponent to scale by 2 (increases power of 2 in (M * 2^E) by 1)
  exponent = (exponent_val + 1) << significand_size;

  // if exponent + 1 == All 1's (Infinity)
  // set significand bits to 0 (any 1 bit would mean NaN)
  if (exponent == exponent_mask) {
    return sign | exponent;
  }

  return sign | exponent | significand;
}

/*
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {
  const unsigned sign_mask = 0x80000000;
  const unsigned sign = uf & sign_mask;

  const unsigned significand_size = 23;
  const unsigned significand_mask = 0x007FFFFF;
  unsigned significand = uf & significand_mask;

  const unsigned exponent_mask = 0x7F800000;
  const unsigned exponent = (uf & exponent_mask) >> significand_size;
  const int exponent_val = exponent - 127;

  // Value will be larger than largest possible int
  if (exponent_val > 31) {
    return 0x80000000;
  }

  // Value is less than 1, so will be truncated to 0
  if (exponent_val < 0) {
    return 0;
  }

  // Normalised values have implicit 1 added to significand
  // Add a leading one (1 to the left of the binary point)
  // 0.110011...1 -> 1.110011...1
  significand = significand | (1 << 23);

  // Now the significand represents the int but  shifted 23 places to the
  // right.
  // (1, followed by the binary point, followed by 23 0s/1s)

  // The integer (before introducing the binary point) will have been
  // shifted between 0-31 places to the right of the binary point.

  // Need to correct to the right amount of shifts
  if (exponent_val < 23) {
    significand >>= (23 - exponent_val);
  } else {
    significand <<= (exponent_val - 23);
  }

  // Convert to negative, if sign bit set
  if (sign) {
    significand = -significand;
  }

  return significand;
}

/*
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 *
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatPower2(int x) {
  int bias = 127;
  unsigned required_exponent = x + bias;
  unsigned significand_size = 23;

  // Denormalised exponent is all zeros
  // E = 1 - Bias
  // E = 1 - 127
  // E = -126
  // Can't get smaller precision than this, round to 0
  if (x < (1 - bias)) {
    return 0;
  }

  // Normalised exponent ranges from [1, 254]
  // E = exponent + Bias = x
  // E = 1 - 127 = -126
  // E = 254 - 127 = 127
  // E = 255 - 127 = 128 (exponent all 1's, Infinity/NaN)
  if (x >= (0xFF - bias)) {
    return 0x7F800000; // +Infinity
  }

  return required_exponent << significand_size;
}
