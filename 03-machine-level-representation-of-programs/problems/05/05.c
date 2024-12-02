/*
You are given the following information. A function with prototype

void decode1(long *xp, long *yp, long *zp);

is compiled to assembly code, yielding the following:

```asm
; void decode1(long *xp, long *yp, long *zp)
; xp in %rdi, yp in %rsi, zp in %%rdx
decode1:
  movq    (%rdi), %r8
  movq    (%rsi), %rcx
  movq    (%rdx), %rax
  movq    %r8, (%rsi)
  movq    %rcx, (%rdx)
  movq    %rax, (%rdi)
  ret
```

Parameters xp, yp and zp are stored in registers, %rdi, %rsi, and %rdx
respectively.

Write C code for decode1 that will have an effect equivalent to the assembly
code shown.
*/

#include <stdio.h>

void decode1(long *xp, long *yp, long *zp) {
  long r8 = *xp;  // movq    (%rdi), %r8
  long rcx = *yp; // movq    (%rsi), %rcx
  long rax = *zp; // movq    (%rdx), %rax
  *yp = r8;       // movq    %r8, (%rsi)
  *zp = rcx;      // movq    %rcx, (%rdx)
  *xp = rax;      // movq    %rax, (%rdi)
  return;         // ret
}

/*
Compile
🐳 > mycc 05.c -o 05

Produce assembly file without optimisations to compare answer
🐳 > mycc -Og -S 05.c
🐳 > grep -A 10 decode1: 05.s | bat -l asm
*/
int main(void) {
  long xp = 0x1;
  long yp = 0x2;
  long zp = 0x3;

  decode1(&xp, &yp, &zp);

  return 0;
}
