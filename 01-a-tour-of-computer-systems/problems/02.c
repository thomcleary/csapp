/*
The marketing department at your company has promised your customers that the
next software release will show a 2× performance improvement. You have been
assigned the task of delivering on that promise. You have determined that only
80% of the system can be improved. How much (i.e., what value of k) would you
need to improve this part to meet the overall performance target?
*/

#include <stdio.h>

int main(void) {
  const double speedup = 2.0;
  const double alpha = 0.8;

  // s = 1 / ((1 - a) + (a / k))
  // solve for k
  // k = sa / (1 - s + sa)

  printf("k: %g\n", (speedup * alpha) / (1 - speedup + (speedup * alpha)));

  return 0;
}
