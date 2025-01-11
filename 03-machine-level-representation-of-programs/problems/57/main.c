/*
# ap in %rdi, b in %xmm0, c in %rsi, dp in %rdx
funct3:
    vmovss      (%rdx), %xmm1          # xmm1 = *dp
    vcvtsi2sd   (%rdi), %xmm2, %xmm2   # xmm2 = (double)(*ap)
    vucomisd    %xmm2, %xmm0           # compare b to xmm2
    jbe         .L8                    # Jump to L8 if b <= xmm2
    vcvtsi2ssq  %rsi, %xmm0, %xmm0     # b = (double)c
    vmulss      %xmm1, %xmm0, %xmm1    # xmm1 = b * xmm1
    vunpcklps   %xmm1, %xmm1, %xmm1    #
    vcvtps2pd   %xmm1, %xmm0           #
    ret                                # return double(xmm1)
.L8:
    vaddss      %xmm1, %xmm1, %xmm1    # xmm1 = xmm1 + xmm1
    vcvtsi2ssq  %rsi, %xmm0, %xmm0     # b = (double)c
    vaddss      %xmm1, %xmm0, %xmm0    # b = (float)(b + xmm1)
    vunpcklps   %xmm0, %xmm0, %xmm0    #
    vcvtps2pd   %xmm0, %xmm0           #
    ret                                # return (double)b
*/
double funct3(int *ap, double b, long c, float *dp) {
  if (b <= *ap) {
    return (*dp * 2) + c;
  }
  return *dp * c;
}

int main(void) {
  int ap = 37;
  double b = 123.45;
  long c = 42;
  float dp = 999.99;

  funct3(&ap, b, c, &dp);

  return 0;
}
