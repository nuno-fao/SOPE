//PROGRAM p3.c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

int intvar = 0;
bool cres = true;

void siguser_handler(int signo){
	if(signo==SIGUSR1){
		cres=true;
	}
	if(signo==SIGUSR2){
		cres=false;
	}

}

int main(void){
	pid_t pid;
	pid=fork();

	if(pid>0){
		//fazer um execl aleatóriamente para mardar sigusr1 e 2 ao filho
	}
	else if(pid==0){
		struct sigaction action1;
		struct sigaction action2;

		action1.sa_handler = siguser_handler;
		sigemptyset(&action1.sa_mask);
		action1.sa_flags = 0;

		action2 = action1;

		if (sigaction(SIGUSR1,&action1,NULL) < 0){
			fprintf(stderr,"Unable to install SIGINT handler\n");
			exit(1);
		}

		if (sigaction(SIGUSR2,&action2,NULL) < 0){
			fprintf(stderr,"Unable to install SIGTERM handler\n");
			exit(1);
		}
		while(true) {
			if(cres) intvar++;
			else intvar--;
			printf("%i\n",intvar);
			sleep(1);
		}
	}
	
}