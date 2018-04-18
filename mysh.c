/**  smsh1.c  small-shell version 1
 **		first really useful version after prompting shell
 **		this one parses the command line into strings
 **		uses fork, exec, wait, and ignores signals
 **/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "mysh.h"

char *prompt;

int main()
{
  char	*cmdline, *prompt, **arglist;
  int	result;
  void	setup();

  setup();

  while ( (cmdline = next_cmd((char*)getenv("myshPS"), stdin)) != NULL ){
    if ( (arglist = splitline(cmdline)) != NULL  ){
      result = execute(arglist);
      freelist(arglist);
    }
    free(cmdline);
  }
  return 0;
}

void setup()
/*
 * purpose: initialize shell
 * returns: nothing. calls fatal() if trouble
 */
{
  setenv("myshPS", "> ", 1);
  signal(SIGINT,  SIG_IGN);
  signal(SIGQUIT, SIG_IGN);
}

void fatal(char *s1, char *s2, int n)
{
  fprintf(stderr,"Error: %s,%s\n", s1, s2);
  exit(n);
}
