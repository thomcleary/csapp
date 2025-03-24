/*
🐳 > gcc -o snooze snooze.c
*/

#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void signal_handler(int sig) {
  printf("Caught signal %d, waking up!\n", sig);
}

unsigned int snooze(unsigned int secs) {
  unsigned int secs_remaining = sleep(secs);

  printf("Slept for %u of %u secs.\n", secs - secs_remaining, secs);

  return secs_remaining;
}

int main(void) {
  signal(SIGINT, signal_handler);

  snooze(10);

  return 0;
}
