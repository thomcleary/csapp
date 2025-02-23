/*
Problem 7
Modify the code for combine5 to unroll the loop by a factor of k=5

🐳 ❯ mycc ../code/vec.c 07.c -o 07
*/

#include "../code/vec.h"
#include <assert.h>
#include <stdio.h>

/* 2 x 1 loop unrolling */
void combine5(vec_ptr v, data_t *dest) {
  long i;
  long length = vec_length(v);
  long limit = length - 1;
  data_t *data = get_vec_start(v);
  data_t acc = IDENT;

  /* Combine 2 elements at a time */
  for (i = 0; i < limit; i += 2) {
    acc = (acc OP data[i])OP data[i + 1];
  }

  /* Finish any remaining elements */
  for (; i < length; i++) {
    acc = acc OP data[i];
  }

  *dest = acc;
}

void unroll5(vec_ptr v, data_t *dest) {
  long i;
  long length = vec_length(v);
  long limit = length - 4;
  data_t *data = get_vec_start(v);
  data_t acc = IDENT;

  /* Combine 5 elements at a time */
  for (i = 0; i < limit; i += 5) {
    acc = acc OP
        data[i] OP data[i + 1] OP data[i + 2] OP data[i + 3] OP data[i + 4];
  }

  /* Finish any remaining elements */
  for (; i < length; i++) {
    acc = acc OP data[i];
  }

  *dest = acc;
}

int main(void) {
  int vec_length = 5;
  vec_ptr vec = new_vec(vec_length);
  for (int i = 0; i < vec_length; i++) {
    set_vec_element(vec, i, i + 1);
  }

  int combineResult;
  combine5(vec, &combineResult);
  printf("[combine5]: %d\n", combineResult);

  int unrollResult;
  unroll5(vec, &unrollResult);
  printf("[unroll5]: %d\n", unrollResult);

  assert(combineResult == unrollResult && "Different behavior");
  free_vec(vec);

  return 0;
}
