// Generate mstore.s (assembly)
// linux> gcc -Og -S mstore.c

// Generate mstore.o (object code)
// linux> gcc -Og -c mstore.c

// Disassemble mstore.o
// linux> objdump -d mstore.o | bat -l asm

long mult2(long, long);

void multstore(long x, long y, long *dst) {
  long t = mult2(x, y);
  *dst = t;
}
