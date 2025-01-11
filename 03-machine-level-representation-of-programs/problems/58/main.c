/*
decode2:
    subq    %rdx, %rsi  # y -= z
    imulq   %rsi, %rdi  # x *= y
    movq    %rsi, %rax  # rax = y
    salq    $63, %rax   # rax <<= 63
    sarq    $63, %rax   # rax >>= 63
    xorq    %rdi, %rax  # return x ^ rax
    ret
*/

long decode2(long x, long y, long z) {
  y -= z;
  x *= y;
  y = (y << 63) >> 63;
  return x ^ y;
}

int main(void) {
  long x = 37, y = 42, z = 99;

  decode2(x, y, z);

  return 0;
}
