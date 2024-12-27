/*
$ mycc -Og -c xor.c
$ objdump -d xor.o
*/
int main(int argc, char **argv) {
  (void)argv;

  long long x = 123456789L;
  long long y = (long long)argc;

  // Conditionally perform XOR so compiler doesn't know this is a constant value
  // of 0
  if (argc > 0) {
    x = x ^ x;
  }

  // XOR with unknown value so XOR doesn't get optimized away
  y = y ^ x;

  return (int)(x + y);
}
