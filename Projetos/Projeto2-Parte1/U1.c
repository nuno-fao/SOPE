#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include "funcs.h"
#include "registers.h"

struct timeval startTime;
int pubFIFO;
char* fifoname;
int totalTime;
int i;

void *threadFunction(void *arg){

  int privFIFO, wcTime;
  struct timeval execTime;
  char privFIFOPath[256];
  char request[256];
  struct Reply reply;

  wcTime=(rand()%(10 - 1 + 1))+1;

  sprintf(privFIFOPath, "/tmp/%d.%ld", getpid(), pthread_self());
  if(mkfifo(privFIFOPath,0660)==-1){
  	perror("Error creating public fifo for client");
  	exit(1);
  }
  privFIFO=open(privFIFOPath,O_RDONLY|O_NONBLOCK);
  if(privFIFO==-1){
  	perror("Error opening public fifo for client");
  	exit(1);
  }

  sprintf(request,"[ %d, %d, %lu, %d, -1 ]", i, getpid(), pthread_self(), wcTime);

  op_reg_message(elapsedTime(&startTime,&execTime), i, getpid(), pthread_self(), wcTime, -1, "IWANT");
  write(pubFIFO,request,sizeof(request));

  signal(SIGPIPE, SIG_IGN);
  if (access(fifoname, F_OK) != -1) {
	int ct = 0;
	while (read(privFIFO, &reply, sizeof(reply)) <= 0 && ct < 5) {
	    usleep(10000);
	    ct++;
	}
	op_reg_message(elapsedTime(&startTime,&execTime), reply.i, getpid(), pthread_self(), reply.dur, reply.pl, ((reply.pl != -1)&& (reply.pl != 0))? "IAMIN" : "CLOSD");
	if(reply.pl==0){
		op_reg_message(elapsedTime(&startTime,&execTime), i, getpid(), pthread_self(), wcTime, -1, "FAILD");
	}
	
   } 
   else {
   	op_reg_message(elapsedTime(&startTime,&execTime), i, getpid(), pthread_self(), wcTime, -1, "FAILD");
    }


  close(privFIFO);
  unlink(privFIFOPath);


  return NULL;
}

int main(int argc, char **argv, char **envp)
{
  struct FLAGS flags;
  struct timeval execTime;

  i=0;

  srand(time(NULL));

  if(readFlags(argv,argc, &flags)){
  	write(STDOUT_FILENO,"Bad arguments. Usage: Un <-t nsecs> fifoname\n", strlen("Bad arguments. Usage: Un <-t nsecs> fifoname\n"));
    exit(1);
  }

  totalTime = flags.nsecs;
  fifoname = flags.fifoname;

  do {
        pubFIFO = open(flags.fifoname, O_WRONLY);
        if (pubFIFO == -1) {
            
            usleep(500000);
        }
    } while(pubFIFO == -1);

  gettimeofday(&startTime,NULL);

  while ( elapsedTime(&startTime,&execTime) < (double)flags.nsecs ) {
  	i++;
  	pthread_t tid;
  	pthread_create(&tid,NULL,threadFunction,NULL);
  	pthread_detach(tid);
    usleep(5000);
  }
  
  return 0;
  
}