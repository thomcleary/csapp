/*
$ gcc -Og -S main.c
*/

#include <stdbool.h>
#include <stddef.h>

long cread(long *xp) {
  return (xp ? *xp : 0);
}

long cread_alt(long *xp) {
  long result = 0;
  long else_expr = xp ? *xp : result;
  bool test_expr = else_expr > result;

  if (test_expr) {
    result = else_expr;
  }

  return result;
}

int main(void) {
  long xp = 37;

  cread(&xp);
  cread(NULL);

  cread_alt(&xp);
  cread_alt(NULL);

  return 0;
}
