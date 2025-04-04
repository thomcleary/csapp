/*
🐳> gcc -o mmapcopy mmapcopy.c
*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "usage: ./mmapcopy <filename>\n");
    exit(EXIT_FAILURE);
  }

  char *filename = argv[1];

  int fd = open(filename, O_RDONLY);
  if (fd == -1) {
    perror("open failed");
    exit(EXIT_FAILURE);
  }

  struct stat statbuf;
  if (fstat(fd, &statbuf) == -1) {
    perror("fstat failed");
    exit(EXIT_FAILURE);
  }

  void *addr = mmap(NULL, statbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (addr == MAP_FAILED) {
    perror("mmap failed");
    exit(EXIT_FAILURE);
  }

  int s = write(STDOUT_FILENO, addr, statbuf.st_size);
  if (s != statbuf.st_size) {
    perror(s == -1 ? "write failed" : "write failed (partial write)");
    exit(EXIT_FAILURE);
  }

  if (munmap(addr, statbuf.st_size) == -1) {
    perror("munmap failed");
    exit(EXIT_FAILURE);
  }

  if (close(fd) == -1) {
    perror("close failed");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}
