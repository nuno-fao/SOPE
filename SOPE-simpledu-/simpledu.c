#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "funcs.h"
#include "registers.h"


int main(int argc, char **argv, char **envp)
{
  struct FLAGS flags;
  
  struct sigaction action;
  action.sa_handler = sigint_handler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;
  sigaction(SIGINT,&action,NULL);

  initRegisters();
  writeCreateEvent(argc, argv);

  if(argc<3){
  	write(STDOUT_FILENO,"Insufficient arguments.\nUsage: simpledu -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]\nOrder isn't important\n",strlen("Insufficient arguments.\nUsage: simpledu -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]\nOrder isn't important\n"));
	  writeExitEvent(1);
    exit(1);  
  }

  if(readFlags(argv,argc, &flags)){
  	write(STDOUT_FILENO,"Bad arguments.\nUsage: simpledu -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]\nOrder isn't important\n",strlen("Bad arguments.\nUsage: simpledu -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]\nOrder isn't important\n"));
  	writeExitEvent(2);
    exit(2);
  }

  if(du(&flags, flags.dir,0)<0){
    exit(3);
  }
  
  return 0;
  
}