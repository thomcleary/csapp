/*
$ mycc -S -O1 main.c
$ mycc -o main main.c
*/

#include <stdio.h>

#define N 16

typedef int fix_matrix[N][N];

void fix_set_diag_void(fix_matrix A, int val) {
  int *Aptr = &A[0][0];
  int next = N + 1;   // 1row + 1column
  int end = N * next; // A[N][N] (out of bounds)

  int offset = 0; // movl $0, %eax
  do {
    Aptr[offset] = val;    // movl %esi, (%rdi, %rax)
    offset += next;        // addq $68, %rax
  } while (offset != end); // jne .L13
}

int main(void) {
  fix_matrix matrix = {0};

  fix_set_diag_void(matrix, 1);

  for (size_t r = 0; r < N; r++) {
    for (size_t c = 0; c < N; c++) {
      printf("%d ", matrix[r][c]);
    }
    printf("\n");
  }

  return 0;
}
