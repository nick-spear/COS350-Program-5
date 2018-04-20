/* execute.c - code used by small shell to execute commands */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

int execute(char *argv[])
/*
 * purpose: run a program passing it arguments
 * returns: status returned via wait, or -1 on error
 *  errors: -1 on fork() or wait() errors
 */
{
  int	pid ;
  int	child_info = -1;

  // Continues iterating through zombie processes until none are left over
  int prev;
  int bg_processes_ended = 0;
  do {
    prev = -1;
    waitpid(-1, &prev, WNOHANG);
    if ( prev == 0 ) bg_processes_ended++;
  } while ( prev == 0 );

  // Prints appropriate 'finished' message
  if (bg_processes_ended == 1) {
    printf("Background process ended.\n");
  } else if (bg_processes_ended > 1) {
    printf("%d background processes ended.\n", bg_processes_ended);
  }

  // Looks for childless commands of exit/cd
  if ( argv[0] == NULL )		/* nothing succeeds	*/
    return 0;
  else if ( strcmp(argv[0], "exit") == 0 )
    exit(atoi(argv[1]));
  else if ( strcmp(argv[0], "cd") == 0 ) {
    if ( chdir(argv[1]) == -1 ) {
      perror("cannot execute command");
      return -1;
    }
    return 0;
  }

  // Looks for background flag, removes if existing
  int lastindex, bg_flag = 0;
  for (lastindex = 0; argv[lastindex+1] != NULL; lastindex++);
  if ( strcmp(argv[lastindex], "&") == 0) {
    bg_flag = 1;
    argv[lastindex] = NULL;
  }

  if ( (pid = fork())  == -1 )
    perror("fork");
  else if ( pid == 0 ) {
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    execvp(argv[0], argv);
    perror("cannot execute command");
    exit(1);
  }
  else {
    // Doesn't wait if background flag was present
    if ( !bg_flag ) {
      if (waitpid(pid, &child_info, 0) == -1)
        perror("wait");
    } else {
      printf("Process started in background.\n");
    }
  }
  return child_info;
}
