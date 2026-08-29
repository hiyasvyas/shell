#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

const int MAX = 13;

static void doFib (int n, int doPrint);

/*
 * unix_error - unix-style error routine.
 */
inline static void unix_error (char *msg)
{
  fprintf (stdout, "%s: %s\n", msg, strerror (errno));
  exit (1);
}

int main (int argc, char **argv)
{
  int arg;
  int print = 1;

  if (argc != 2)
    {
      fprintf (stderr, "Usage: fib <num>\n");
      exit (-1);
    }

  arg = atoi (argv[1]);
  if (arg < 0 || arg > MAX)
    {
      fprintf (stderr, "number must be between 0 and %d\n", MAX);
      exit (-1);
    }

  doFib (arg, print);

  return 0;
}

/*
 * Recursively compute the specified number. If print is
 * true, print it. Otherwise, provide it to my parent process.
 *
 * NOTE: The solution must be recursive and it must fork
 * a new child for each call. Each process should call
 * doFib() exactly once.
 */
static void doFib (int n, int doPrint) {
  //Base case if n is 0 or 1, print the number and return
  if(n <= 1 && doPrint == 1) {
    printf("%d\n", n);
    return;
  } else if(n <= 1 && doPrint == 0){ //
    exit(n); // send the value to the parent process
  }

  //Fork new child process to compute the left side
  int leftpid = fork();
  if (leftpid < 0){
    unix_error("fork failed"); 
  }
  if (leftpid == 0){ //if the child process is created, compute the left child
    doFib(n-1, 0); 
  }

  //Fork a new child process to compute the right side
  int rightpid = fork();
  if (rightpid < 0){
    unix_error("fork failed"); 
  }
  if (rightpid == 0){ //if the child process is created, compute the right child
    doFib(n-2, 0); 
  }

  int stat1;
  int stat2;

  // waitpid looks at the leftpid and waits for it to finish then stores the child's exit status in stat1
  if(waitpid(leftpid, &stat1, 0) < 0){ // waitpid returns the pid of the child that finished
    unix_error("waitpid failed");
  }
  if(waitpid(rightpid, &stat2, 0) < 0){ 
    unix_error("waitpid failed");
  }
  int result = WEXITSTATUS (stat1) + WEXITSTATUS (stat2);

  if(doPrint == 1){
    printf("%d\n", result);
  } else {
    exit(result);
  } 
    
}