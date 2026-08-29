#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include "util.h"
#include "sig_util.h"
#include <string.h>

void sigint_handler(int signum) {
  write(1, "Nice try.\n", 10);
  return;
}

void sigusr1_handler(int signum) {
  write(1, "exiting\n", 8);
  exit(1);
}

void sleep_action(int seconds) {
  struct timespec req;
  struct timespec rem;
  req.tv_sec = seconds;
  req.tv_nsec = 0;
  while(1) {
  int ret = nanosleep(&req,&rem);
  if(ret == 0) {
    return;
  } else if(EINTR == errno && ret == -1) {
    req = rem;
    continue;
  } else {
    fprintf(stderr, "nanosleep failed: %s\n", strerror(errno));
    exit(1);
  }
  }
  return;
}


/*
 * First, print out the process ID of this process.
 *
 * Then, set up the signal handler so that ^C causes
 * the program to print "Nice try.\n" and continue looping.
 *
 * Finally, loop forever, printing "Still here\n" once every
 * three seconds.
 */
int main(int argc, char **argv)
{
  printf("%d\n", getpid());
  signal_action(SIGINT, sigint_handler);
  signal_action(SIGUSR1, sigusr1_handler);
  while (1) {
    printf("Still here\n");
    sleep_action(3);
  }
  return 0;
}

