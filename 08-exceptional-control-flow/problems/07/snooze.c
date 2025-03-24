/*
🐳 > gcc -o snooze snooze.c
*/

#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void sigint_handler(int sig) {
  printf("\n");
}

unsigned int snooze(unsigned int secs) {
  unsigned int secs_remaining = sleep(secs);

  printf("Slept for %u of %u secs.\n", secs - secs_remaining, secs);

  return secs_remaining;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "usage: ./snooze <seconds>\n");
    exit(EXIT_FAILURE);
  }

  char *end;
  errno = 0;
  long secs_long = strtol(argv[1], &end, 10);

  if (errno != 0) {
    fprintf(stderr, "strtol failed for <seconds> [%s]: %s", argv[1],
            strerror(errno));
    exit(EXIT_FAILURE);
  }

  if (*end != '\0') {
    fprintf(stderr, "seconds must be a valid integer\n");
    exit(EXIT_FAILURE);
  }

  if (secs_long > INT_MAX || secs_long < 0) {
    fprintf(stderr, "seconds must be in range [0, %d]\n", INT_MAX);
    exit(EXIT_FAILURE);
  }

  int secs = (int)secs_long;

  if (signal(SIGINT, sigint_handler) == SIG_ERR) {
    fprintf(stderr, "signal failed: %s\n", strerror(errno));
    exit((EXIT_FAILURE));
  }

  snooze(secs);

  return 0;
}
