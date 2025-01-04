/*
mycc -Og -S main.c
mycc -Og -o main main.c
*/

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

struct ELE {
  long v;
  struct ELE *p;
};

/*
# ptr in %rdi
fun:
    movl    $0, %eax        # eax = 0
    jmp     .L2             # jump to .L2
.L3:
    addq    (%rdi), %rax    # rax += ptr->v
    movq    8(%rdi), %rdi   # ptr = ptr->p
.L2:
    testq   %rdi, %rdi      #
    jne     .L3             # if (ptr != NULL): jump .L3
    rep; ret                # return
*/
long fun(struct ELE *ptr) {
  long sum = 0;
  while (ptr) {
    sum += ptr->v;
    ptr = ptr->p;
  }
  return sum;
}

int main(void) {
  struct ELE a = {.v = 37, .p = NULL};
  struct ELE b = {.v = 42, .p = NULL};
  a.p = &b;

  long result = fun(&a);

  printf("fun(a): %ld\n", result);
  assert(result == a.v + b.v);

  return 0;
}
