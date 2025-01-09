/*
$ gcc -S main.c
$ gcc main.c -o main
*/

/*
# double funct2(double w, int x, float y, long z)
# w in %xmm0, x in %edi, y in %xmm1, z in %rsi
funct2:
    vcvtsi2ss       %edi, %xmm2, %xmm2      # x = (float)x
    vmulss          %xmm1, %xmm2, %xmm1     # y *= (float)x
    vunpcklps       %xmm1, %xmm1, %xmm1     #
    vcvtps2ps       %xmm1, %xmm2            # %xmm2 = (double)y
    vcvtsi2sdq      %rsi, %xmm1, %xmm1      # y = (double)z
    vdivsd          %xmm1, %xmm0, %xmm0     # w = w / (double)z
    vsubsd          %xmm0, %xmm2, %xmm0     # w = y - w;
    ret
*/
double funct2(double w, int x, float y, long z) {
  return (y * x) - (w / z);
}

int main(void) {
  double w = 37.37;
  int x = 42;
  float y = 123.456;
  long z = 999;

  funct2(w, x, y, z);

  return 0;
}
